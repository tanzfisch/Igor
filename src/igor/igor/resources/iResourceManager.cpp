// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/iResourceManager.h>

#include <igor/resources/sound/iSoundFactory.h>
#include <igor/resources/texture/iTextureFactory.h>
#include <igor/resources/animation/iAnimationFactory.h>
#include <igor/resources/sprite/iSpriteFactory.h>
#include <igor/resources/model/iModelFactory.h>
#include <igor/resources/config/iConfigReader.h>
#include <igor/threading/iTaskManager.h>

#include <iaux/system/iaFile.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{
    iResourceManager::iResourceManager()
    {
        configure();

        registerFactory(iFactoryPtr(new iTextureFactory()));
        registerFactory(iFactoryPtr(new iModelFactory()));
        registerFactory(iFactoryPtr(new iSpriteFactory()));
        registerFactory(iFactoryPtr(new iAnimationFactory()));
        registerFactory(iFactoryPtr(new iSoundFactory()));
    }

    iResourceManager::~iResourceManager()
    {
        // first remove all resources that where not loaded until now
        _mutex.lock();
        _loadingQueue.clear();

        auto iter = _resources.begin();
        while (iter != _resources.end())
        {
            if (!(*iter).second->isValid())
            {
                if ((*iter).second.use_count() == 1)
                {
                    iter = _resources.erase(iter);
                    continue;
                }
            }

            iter++;
        }
        _mutex.unlock();

        // run flush twice so resources which hold on to other resources release them too
        flush(iResourceCacheMode::Keep);
        flush(iResourceCacheMode::Keep);

        _mutex.lock();
        // now check if it was actually released
        if (!_resources.empty())
        {
            con_err("Possible memory leak. Not all resources were released.");

            con_endl("Unreleased resources: ");
            for (auto resource : _resources)
            {
                con_endl(resource.second->getName() << " ref:" << resource.second.use_count());
            }
        }

        _resources.clear();
        _mutex.unlock();

        _factories.clear();
    }

    void iResourceManager::configure()
    {
        if (iConfigReader::getInstance().hasSetting("loadMode"))
        {
            const iaString loadMode = iConfigReader::getInstance().getValue("loadMode");

            if (loadMode == "Sync")
            {
                setLoadMode(iResourceManagerLoadMode::Synchronized);
            }
        }

        if (iConfigReader::getInstance().hasSetting("searchPaths"))
        {
            const std::vector<iaString> searchPaths = iConfigReader::getInstance().getValueAsArray("searchPaths");

            for (const auto &path : searchPaths)
            {
                addSearchPath(path);
            }
        }
    }

    int64 iResourceManager::calcHashValue(const iParameters &parameters, iFactoryPtr factory)
    {
        std::hash<std::wstring> hashFunc;

        iaString hashData = parameters.getParameter<iaString>("type");
        hashData += parameters.getParameter<iaString>("name");
        hashData += factory->getHashData(parameters);

        return hashFunc(hashData.getData());
    }

    void iResourceManager::registerFactory(iFactoryPtr factory)
    {
        auto iter = _factories.find(factory->getType());
        if (iter != _factories.end())
        {
            con_warn("factory \"" << factory->getType() << "\" already registered");
            return;
        }

        _factories[factory->getType()] = factory;
        factory->init();
    }

    void iResourceManager::unregisterFactory(iFactoryPtr factory)
    {
        auto iter = _factories.find(factory->getType());
        if (iter == _factories.end())
        {
            con_warn("factory \"" << factory->getType() << "\" is not registered");
            return;
        }

        factory->deinit();
        _factories.erase(iter);
    }

    iFactoryPtr iResourceManager::getFactory(const iParameters &parameters)
    {
        iFactoryPtr result;

        const iaString type = parameters.getParameter<iaString>("type");

        if (!type.isEmpty())
        {
            auto iter = _factories.find(type);
            if (iter != _factories.end())
            {
                result = iter->second;
            }
        }

        if (result == nullptr)
        {
            for (auto pair : _factories)
            {
                if (pair.second->matchingType(parameters))
                {
                    result = pair.second;
                    break;
                }
            }
        }

        if (result == nullptr)
        {
            const iaString name = parameters.getParameter<iaString>("name");
            con_err("No compatible factory registered for resource \"" << name << "\"");
        }

        return result;
    }

    iResourcePtr iResourceManager::getResource(const iParameters &parameters, iFactoryPtr factory)
    {
        iResourcePtr result;

        int64 hashValue = calcHashValue(parameters, factory);

        _mutex.lock();
        auto resourceIter = _resources.find(hashValue);
        if (resourceIter != _resources.end())
        {
            result = resourceIter->second;
        }
        _mutex.unlock();

        return result;
    }

    iResourcePtr iResourceManager::requestResource(const iParameters &parameters)
    {
        if (_loadMode == iResourceManagerLoadMode::Synchronized)
        {
            return iResourceManager::loadResource(parameters);
        }

        iResourcePtr result;
        iFactoryPtr factory;

        if ((factory = getFactory(parameters)) == nullptr)
        {
            return result;
        }

        const int64 hashValue = calcHashValue(parameters, factory);

        _mutex.lock();
        auto resourceIter = _resources.find(hashValue);
        if (resourceIter != _resources.end())
        {
            result = resourceIter->second;
        }
        else
        {
            result = factory->createResource(parameters);
            _resources[hashValue] = result;
            _loadingQueue.push_back(result);
        }
        _mutex.unlock();

        const iResourceCacheMode currentCacheMode = result->_parameters.getParameter<iResourceCacheMode>("cacheMode", iResourceCacheMode::Free);
        const iResourceCacheMode cacheMode = parameters.getParameter<iResourceCacheMode>("cacheMode", iResourceCacheMode::Free);

        if (currentCacheMode < cacheMode)
        {
            result->_parameters.setParameter("cacheMode", cacheMode);
        }

        return result;
    }

    iResourcePtr iResourceManager::loadResource(const iParameters &parameters)
    {
        iResourcePtr result;
        iFactoryPtr factory;

        if ((factory = getFactory(parameters)) == nullptr)
        {
            return result;
        }

        const int64 hashValue = calcHashValue(parameters, factory);

        bool loadNow = false;

        _mutex.lock();
        auto resourceIter = _resources.find(hashValue);
        if (resourceIter != _resources.end())
        {
            result = resourceIter->second;

            // remove from load queue because we will load it right away
            auto iter = std::find(_loadingQueue.begin(), _loadingQueue.end(), result);
            if (iter != _loadingQueue.end())
            {
                _loadingQueue.erase(iter);
                loadNow = true;
            }
        }
        else
        {
            result = factory->createResource(parameters);
            _resources[hashValue] = result;
            loadNow = true;
        }
        _mutex.unlock();

        if (loadNow)
        {
            result->setValid(factory->loadResource(result));
            result->setProcessed(true);
        }

        const iResourceCacheMode currentCacheMode = result->_parameters.getParameter<iResourceCacheMode>("cacheMode", iResourceCacheMode::Free);
        const iResourceCacheMode cacheMode = parameters.getParameter<iResourceCacheMode>("cacheMode", iResourceCacheMode::Free);

        if (currentCacheMode < cacheMode)
        {
            result->_parameters.setParameter("cacheMode", cacheMode);
        }

        return result;
    }

    void iResourceManager::flush(iResourceCacheMode cacheModeLevel)
    {
        std::vector<iResourcePtr> toUnload;

        _mutex.lock();
        auto iter = _resources.begin();

        while (iter != _resources.end())
        {
            iResourcePtr resource = iter->second;

            if (resource.use_count() == 2 &&
                resource->getCacheMode() <= cacheModeLevel)
            {
                toUnload.push_back(resource);
                iter = _resources.erase(iter);

                // remove from loading queue as well
                auto iterLoad = std::find(_loadingQueue.begin(), _loadingQueue.end(), resource);
                if (iterLoad != _loadingQueue.end())
                {
                    _loadingQueue.erase(iterLoad);
                }
                continue;
            }

            iter++;
        }
        _mutex.unlock();

        // telling the factories about it
        for (auto resource : toUnload)
        {
            iFactoryPtr factory;
            if ((factory = getFactory(resource->getParameters())) != nullptr)
            {
                factory->unloadResource(resource);
            }
        }

        _mutex.lock();
        std::deque<iResourcePtr> toLoad = std::move(_loadingQueue);
        _mutex.unlock();

        for (auto resource : toLoad)
        {
            if (_interruptLoading)
            {
                break;
            }

            // should never fail
            iFactoryPtr factory = getFactory(resource->getParameters());

            resource->setValid(factory->loadResource(resource));
            resource->setProcessed(true);
        }

        _interruptLoading = false;
    }

    void iResourceManager::interruptFlush()
    {
        _interruptLoading = true;
    }

    const std::vector<iaString> &iResourceManager::getSearchPaths() const
    {
        return _searchPaths;
    }

    void iResourceManager::addSearchPath(const iaString &folder)
    {
        _mutex.lock();

        bool found = false;
        auto iter = _searchPaths.begin();
        while (iter != _searchPaths.end())
        {
            if ((*iter) == folder)
            {
                con_warn("search path " << folder << " already in list");
                found = true;
                break;
            }

            iter++;
        }

        if (!found)
        {
            _searchPaths.push_back(folder);
        }

        _mutex.unlock();
    }

    void iResourceManager::removeSearchPath(const iaString &folder)
    {
        _mutex.lock();

        bool found = false;
        auto iter = _searchPaths.begin();
        while (iter != _searchPaths.end())
        {
            if ((*iter) == folder)
            {
                _searchPaths.erase(iter);
                found = true;
                break;
            }

            iter++;
        }

        if (!found)
        {
            con_warn("search path " << folder << " not found");
        }

        _mutex.unlock();
    }

    void iResourceManager::clearSearchPaths()
    {
        _mutex.lock();

        _searchPaths.clear();

        _mutex.unlock();
    }

    iaString iResourceManager::getPath(const iaString &filename)
    {
        iaFile file(filename);

        if (file.exist())
        {
            return file.getFullFileName();
        }

        iaString result = filename;

        _mutex.lock();

        for (auto path : _searchPaths)
        {
            iaFile composed(path + __IGOR_PATHSEPARATOR__ + filename);
            if (composed.exist())
            {
                result = composed.getFullFileName();
                break;
            }
        }

        _mutex.unlock();

        return result;
    }

    iaString iResourceManager::getRelativePath(const iaString &filename)
    {
        iaString result = filename;
        for (auto path : _searchPaths)
        {
            iaDirectory dir(path);

            std::vector<iaString> matches;
            iaString::searchRegex(filename, dir.getFullDirectoryName(), matches);
            if (!matches.empty())
            {
                result = iaDirectory::getRelativePath(dir.getFullDirectoryName(), filename);
                break;
            }
        }

        return result;
    }

    bool iResourceManager::fileExists(const iaString &filename)
    {
        iaFile file(filename);

        if (file.exist())
        {
            return true;
        }

        bool result = false;
        _mutex.lock();

        for (auto path : _searchPaths)
        {
            iaFile composed(path + __IGOR_PATHSEPARATOR__ + filename);
            if (composed.exist())
            {
                result = true;
            }
        }

        _mutex.unlock();

        return result;
    }

    void iResourceManager::setLoadMode(iResourceManagerLoadMode loadMode)
    {
        _loadMode = loadMode;

        con_info("Resource Manager Load Mode: " << loadMode);
    }

    iResourceManagerLoadMode iResourceManager::getLoadMode() const
    {
        return _loadMode;
    }

    std::wostream &operator<<(std::wostream &stream, iResourceManagerLoadMode mode)
    {
        const static std::wstring text[] = {
            L"Application",
            L"Synchronized"};

        stream << text[static_cast<int>(mode)];

        return stream;
    }

    std::wostream &operator<<(std::wostream &stream, iResourceCacheMode cacheMode)
    {
        const static std::wstring text[] = {
            L"Free",
            L"Cache",
            L"Keep"};

        stream << text[static_cast<int>(cacheMode)];

        return stream;
    }    

} // namespace igor
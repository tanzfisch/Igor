// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/iResourceManager.h>

#include <igor/resources/sound/iSoundFactory.h>
#include <igor/resources/texture/iTextureFactory.h>
#include <igor/resources/animation/iAnimationFactory.h>
#include <igor/resources/sprite/iSpriteFactory.h>
#include <igor/resources/model/iModelFactory.h>
#include <igor/resources/material/iMaterialFactory.h>
#include <igor/resources/config/iConfigReader.h>
#include <igor/threading/iTaskManager.h>
#include <igor/resources/iResourceDictionary.h>

#include <iaux/system/iaFile.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <iomanip>

namespace igor
{
    iResourceManager::iResourceManager()
    {
        configure();

        registerFactory(iFactoryPtr(new iTextureFactory()));
        registerFactory(iFactoryPtr(new iModelFactory()));
        registerFactory(iFactoryPtr(new iSpriteFactory()));
        registerFactory(iFactoryPtr(new iAnimationFactory()));
        registerFactory(iFactoryPtr(new iMaterialFactory()));
        registerFactory(iFactoryPtr(new iSoundFactory()));

        // read igor internal resource dictionary
        _resourceDictionary.read(resolvePath("igor/dictionaries/igor_resource_dictionary.xml"));
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
                con_endl(resource.second->getID() << " " << resource.second->getInfo() << " " << resource.second->getType() << " ref:" << resource.second.use_count());
            }
        }

        _resources.clear();
        _mutex.unlock();

        _factories.clear();
    }

    void iResourceManager::loadResourceDictionary(const iaString &filename)
    {
        // make sure igor resources are always in the dictionary
        _resourceDictionary.clear();
        _resourceDictionary.read(resolvePath("igor/dictionaries/igor_resource_dictionary.xml"));

        // now load app specifics
        _resourceDictionary.read(resolvePath(filename));
    }

    void iResourceManager::writeResourceDictionary(const iaString &filename)
    {
        _resourceDictionary.write(resolvePath(filename));
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

    template <typename ParameterType>
    void writeParam(const iParameters &parameters, const iaString &key, std::wofstream &stream)
    {
        if (parameters.hasParameter(key))
        {
            stream << "\t\t\t\t<Parameter key=\"" << key << "\" value=\"";
            stream << parameters.getParameter<ParameterType>(key) << "\"/>\n";
        }
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
        const iaString type = parameters.getParameter<iaString>("type", "");

        if (type.isEmpty())
        {
            con_err("empty type parameter");
            return nullptr;
        }

        auto iter = _factories.find(type);
        if (iter == _factories.end())
        {
            con_err("No factory registered for given type \"" << type << "\"");
            return nullptr;
        }

        if (!iter->second->matchingType(parameters))
        {
            con_err("Factory incompatible with given parameters");
            return nullptr;
        }

        return iter->second;
    }

    iResourcePtr iResourceManager::getResource(const iParameters &parameters)
    {
        iFactoryPtr factory = getFactory(parameters);
        if (factory == nullptr)
        {
            return nullptr;
        }

        const iResourceID id = parameters.getParameter<iResourceID>("id", IGOR_INVALID_ID);

        return getResource(id);
    }

    const iResourceID iResourceManager::getResourceID(const iaString &alias) const
    {
        return _resourceDictionary.getResource(alias);
    }

    iResourcePtr iResourceManager::getResource(const iResourceID &id)
    {
        iResourcePtr result;

        _mutex.lock();
        auto resourceIter = _resources.find(id);
        if (resourceIter != _resources.end())
        {
            result = resourceIter->second;
        }
        _mutex.unlock();

        return result;
    }

    iResourcePtr iResourceManager::createResource(iFactoryPtr factory, const iParameters &parameters)
    {
        return factory->createResource(parameters);
    }

    iResourcePtr iResourceManager::requestResource(const iParameters &parameters)
    {
        if (_loadMode == iResourceManagerLoadMode::Synchronized)
        {
            return loadResource(parameters);
        }

        iResourcePtr result;
        iFactoryPtr factory = getFactory(parameters);

        if (factory == nullptr)
        {
            return result;
        }

        iResourceID id;
        if (!iResource::extractID(parameters, id))
        {
            const iaString id = parameters.getParameter<iaString>("id", "");
            const iaString alias = parameters.getParameter<iaString>("alias", "");
            const iaString filename = parameters.getParameter<iaString>("filename", "");
            con_err("can't get resource for id:\"" << id << "\" alias:\"" << alias << "\" filename:\"" << filename << "\"");
            return nullptr;
        }

        _mutex.lock();
        auto resourceIter = _resources.find(id);
        if (resourceIter != _resources.end())
        {
            result = resourceIter->second;
        }
        else
        {
            result = createResource(factory, parameters);
            _resources[result->getID()] = result;
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
        iFactoryPtr factory = getFactory(parameters);
        if (factory == nullptr)
        {
            return nullptr;
        }

        iResourceID id;
        if (!iResource::extractID(parameters, id))
        {
            const iaString id = parameters.getParameter<iaString>("id", "");
            const iaString alias = parameters.getParameter<iaString>("alias", "");
            const iaString filename = parameters.getParameter<iaString>("filename", "");
            con_err("can't get resource for id:\"" << id << "\" alias:\"" << alias << "\" filename:\"" << filename << "\"");            
            return nullptr;
        }

        const iResourceCacheMode requestedCacheMode = parameters.getParameter<iResourceCacheMode>("cacheMode", iResourceCacheMode::Free);
        bool loadNow = false;
        iResourcePtr result;

        _mutex.lock();
        auto resourceIter = _resources.find(id);
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
            result = createResource(factory, parameters);
            if (requestedCacheMode > iResourceCacheMode::DontCache)
            {
                _resources[result->getID()] = result;
            }
            loadNow = true;
        }
        _mutex.unlock();

        if (loadNow)
        {
            result->setValid(factory->loadResource(result));
            result->setProcessed(true);
        }

        const iResourceCacheMode currentCacheMode = result->_parameters.getParameter<iResourceCacheMode>("cacheMode", iResourceCacheMode::Free);
        if (currentCacheMode < requestedCacheMode)
        {
            result->_parameters.setParameter("cacheMode", requestedCacheMode);
        }

        if (loadNow &&
            !result->isQuiet() &&
            result->isValid())
        {
            con_info("loaded " << result->getType() << " id:" << result->getID() << " \"" << result->getInfo() << "\"");
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
                if (!resource->isQuiet())
                {
                    con_info("released " << resource->getType() << " \"" << resource->getInfo() << "\"");
                }
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

    const iaString iResourceManager::getFilePath(const iResourceID &id)
    {
        return _resourceDictionary.getFilePath(id);
    }

    const iaString iResourceManager::resolvePath(const iaString &filepath)
    {
        iaFile file(filepath);

        if (file.exist())
        {
            return file.getFullFileName();
        }

        iaString result = filepath;

        _mutex.lock();

        for (auto path : _searchPaths)
        {
            iaFile composed(path + __IGOR_PATHSEPARATOR__ + filepath);
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

    void iResourceManager::getMaterials(std::vector<iMaterialPtr> &materials)
    {
        materials.clear();

        // TODO this is way too slow. need to cache materials

        _mutex.lock();
        for (const auto &pair : _resources)
        {
            if (pair.second->getType() != "material")
            {
                continue;
            }

            materials.push_back(std::dynamic_pointer_cast<iMaterial>(pair.second));
        }
        _mutex.unlock();

        sort(materials.begin(), materials.end(),
             [](const iMaterialPtr a, const iMaterialPtr b) -> bool
             {
                 return a->getOrder() < b->getOrder();
             });
    }

    iParameters iResourceManager::buildParam(const iaString &type, const iaString &alias, iResourceCacheMode cacheMode)
    {
        iParameters param({{"type", type},
                           {"cacheMode", cacheMode}});

        if (!alias.isEmpty())
        {
            param.setParameter("alias", alias);
        }

        const iaUUID id = _resourceDictionary.getResource(alias);
        if (id.isValid())
        {
            param.setParameter("id", id);
            return param;
        }

        if (iaUUID::isUUID(alias))
        {
            param.setParameter("id", iaUUID(alias));
            return param;
        }

        const iaString filename = resolvePath(alias);
        if (iaFile::exist(filename))
        {
            param.setParameter("filename", filename);            
            param.setParameter("id", _resourceDictionary.addResource(alias));
        }

        return param;
    }

} // namespace igor
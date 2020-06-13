// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/iResourceManager.h>

#include <igor/resources/sound/iSoundFactory.h>
#include <igor/threading/iTaskManager.h>
#include <igor/graphics/iRenderer.h>

#include <iaux/system/iaFile.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{
    iResourceManager::iResourceManager()
    {
        registerFactory(iFactoryPtr(new iSoundFactory()));
    }

    iResourceManager::~iResourceManager()
    {
        // first remove all resources that where not loaded until now
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

        // than run a flush to clear all released data
        flush(iResourceCacheMode::Keep);

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
        _factories.clear();
    }

    int64 iResourceManager::calcHashValue(const iResourceParameters &parameters, iFactoryPtr factory)
    {
        std::hash<std::wstring> hashFunc;

        iaString hashData = parameters._type;
        hashData += parameters._name;

        switch (parameters._cacheMode)
        {
        case iResourceCacheMode::Free:
            hashData += "F";
            break;
        case iResourceCacheMode::Cache:
            hashData += "C";
            break;
        case iResourceCacheMode::Keep:
            hashData += "K";
            break;
        }

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
    }

    void iResourceManager::unregisterFactory(iFactoryPtr factory)
    {
        auto iter = _factories.find(factory->getType());
        if (iter == _factories.end())
        {
            con_warn("factory \"" << factory->getType() << "\" is not registered");
            return;
        }

        _factories.erase(iter);
    }

    iFactoryPtr iResourceManager::getFactory(const iResourceParameters &parameters)
    {
        iFactoryPtr result;

        if (!parameters._type.isEmpty())
        {
            auto iter = _factories.find(parameters._type);
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
            con_err("No compatible factory registered for resource \"" << parameters._name << "\"");
        }

        return result;
    }

    iResourcePtr iResourceManager::getResource(const iResourceParameters &parameters, iFactoryPtr factory)
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

        if (result == nullptr)
        {
            result = factory->createResource(parameters);

            _mutex.lock();
            _resources[hashValue] = result;
            _mutex.unlock();
        }

        return result;
    }

    iResourcePtr iResourceManager::requestResource(const iResourceParameters &parameters)
    {
        iResourcePtr result;
        iFactoryPtr factory;

        if ((factory = getFactory(parameters)) == nullptr)
        {
            return result;
        }

        result = getResource(parameters, factory);
        return result;
    }

    iResourcePtr iResourceManager::loadResource(const iResourceParameters &parameters)
    {
        iResourcePtr result;
        iFactoryPtr factory;
        if ((factory = getFactory(parameters)) == nullptr)
        {
            return result;
        }

        result = getResource(parameters, factory);
        factory->loadResource(result);

        return result;
    }

    void iResourceManager::flush(iResourceCacheMode cacheModeLevel)
    {
        std::vector<iResourcePtr> toLoad;
        std::vector<iResourcePtr> toUnload;

        _mutex.lock();
        auto iter = _resources.begin();

        while (iter != _resources.end())
        {
            iResourcePtr resource = iter->second;
            if (resource->isProcessed())
            {
                if (resource->isValid() &&
                    iter->second.use_count() == 2 &&
                    resource->getCacheMode() <= cacheModeLevel)
                {
                    toUnload.push_back(resource);
                    iter = _resources.erase(iter);
                    continue;
                }
            }
            else
            {
                toLoad.push_back(resource);
            }

            iter++;
        }

        _mutex.unlock();

        for (auto resource : toUnload)
        {
            iFactoryPtr factory;
            if ((factory = getFactory(resource->getParameters())) != nullptr)
            {
                factory->unloadResource(resource);
            }
        }

        for (auto resource : toLoad)
        {
            iFactoryPtr factory;
            if ((factory = getFactory(resource->getParameters())) != nullptr)
            {
                if (factory->loadResource(resource))
                {
                    resource->_valid = true;
                }
                resource->_processed = true;
            }

            if (_interrupLoading)
            {
                break;
            }
        }

        _interrupLoading = false;
    }

    void iResourceManager::interruptFlush()
    {
        _interrupLoading = true;
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
            if (filename.findFirstOf(dir.getFullDirectoryName().getData()) != iaString::INVALID_POSITION)
            {
                result = iaDirectory::getRelativePath(dir.getFullDirectoryName().getData(), filename);
                break;
            }
        }

        return result;
    }

} // namespace igor
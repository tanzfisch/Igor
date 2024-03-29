// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/iResourceManager.h>

#include <igor/resources/sound/iSoundFactory.h>
#include <igor/resources/texture/iTextureFactory.h>
#include <igor/resources/animation/iAnimationFactory.h>
#include <igor/resources/sprite/iSpriteFactory.h>
#include <igor/resources/model/iModelFactory.h>
#include <igor/resources/shader_material/iShaderMaterialFactory.h>
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
    static const iaString s_igorResourceDictionaryPath = "igor_resource_dictionary.xml";

    static bool matchingFilename(iFactoryPtr factory, const iaString &filename)
    {
        iaFile file(filename);
        const iaString &fileExtension = file.getExtension();

        for (const auto &extension : factory->getSupportedExtensions())
        {
            if (fileExtension == extension)
            {
                return true;
            }
        }

        return false;
    }

    iResourceManager::iResourceManager()
    {
        configure();

        registerFactory(iFactoryPtr(new iTextureFactory()));
        registerFactory(iFactoryPtr(new iModelFactory()));
        registerFactory(iFactoryPtr(new iSpriteFactory()));
        registerFactory(iFactoryPtr(new iAnimationFactory()));
        registerFactory(iFactoryPtr(new iMaterialFactory()));
        registerFactory(iFactoryPtr(new iShaderMaterialFactory()));
        registerFactory(iFactoryPtr(new iSoundFactory()));

        // read igor internal resource dictionary
        _resourceDictionary.read(resolvePath(s_igorResourceDictionaryPath));
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

        // run flush twice so resources which hold on to other resources release them too (ie sprites holding on to textures)
        flush(iResourceCacheMode::Keep);
        flush(iResourceCacheMode::Keep);

        _mutex.lock();
        // now check if it was actually released
        if (!_resources.empty())
        {
            con_warn("Possible memory leak. Not all resources were released.");

            con_endl("Unreleased resources [" << _resources.size() << "]:");
            for (auto resource : _resources)
            {
                con_endl(resource.second->getInfo() << " " << resource.second->getType() << " ref:" << resource.second.use_count());
            }
        }

        _resources.clear();
        _mutex.unlock();

        _factories.clear();
    }

    bool iResourceManager::saveResource(iResourceID resourceID, const iaString &filename)
    {
        return saveResource(getResource(resourceID), filename);
    }

    bool iResourceManager::saveResource(iResourcePtr resource, const iaString &filename)
    {
        if (!resource->isValid())
        {
            con_err("can't save invalid resource " << resource->getInfo());
            return false;
        }

        iFactoryPtr factory = getFactory(resource->getParameters());
        if (factory == nullptr)
        {
            con_err("no factory found for given resource " << resource->getInfo());
            return false;
        }

        bool result = factory->saveResource(resource, filename);

        if (result)
        {
            con_info("saved " << resource->getType() << " " << resource->getInfo());
        }

        return result;
    }

    const iaString iResourceManager::getType(const iResourceID &resourceID) const
    {
        return getType(_resourceDictionary.getFilename(resourceID));
    }

    const iaString iResourceManager::getType(const iaString &filename) const
    {
        if (filename.isEmpty())
        {
            return iaString();
        }

        for (auto factory : _factories)
        {
            if (matchingFilename(factory.second, filename))
            {
                return factory.second->getType();
            }
        }

        return iaString();
    }

    void iResourceManager::loadResourceDictionary(const iaString &filename)
    {
        clearResourceDictionary();

        // now load app specifics
        _resourceDictionary.read(resolvePath(filename));
    }

    void iResourceManager::saveResourceDictionary(const iaString &filename)
    {
        _resourceDictionary.write(resolvePath(filename));
    }

    void iResourceManager::clearResourceDictionary()
    {
        // make sure igor resources are always in the dictionary
        _resourceDictionary.clear();
        _resourceDictionary.read(resolvePath(s_igorResourceDictionaryPath));
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

    static bool matchingType(iFactoryPtr factory, const iParameters &parameters)
    {
        if (parameters.getParameter<iaString>("type") == factory->getType())
        {
            return true;
        }

        if (matchingFilename(factory, parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_SOURCE)) ||
            matchingFilename(factory, parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_ALIAS)))
        {
            return true;
        }

        return false;
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

        if (!matchingType(iter->second, parameters))
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

        const iResourceID id = parameters.getParameter<iResourceID>(IGOR_RESOURCE_PARAM_ID, IGOR_INVALID_ID);

        return getResource(id);
    }

    const iResourceID iResourceManager::getResourceID(const iaString &aliasOrFilename) const
    {
        return _resourceDictionary.getResource(aliasOrFilename);
    }

    iResourcePtr iResourceManager::getResource(const iResourceID &id)
    {
        iResourcePtr result;

        _mutex.lock();
        auto resourceIter = _resources.find(id);
        if (resourceIter != _resources.end())
        {
            result = resourceIter->second;

            con_trace("cache hit " << result->getType() << " " << result->getInfo());
        }
        _mutex.unlock();

        return result;
    }

    iResourcePtr iResourceManager::createResource(iFactoryPtr factory, const iParameters &parameters)
    {
        iResourcePtr result = factory->createResource(parameters);

        con_assert(result != nullptr, "failed to create resource");
        con_trace("created resource " << result->getType() << " " << result->getInfo());
        return result;
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
            const iaString id = parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_ID, "");
            const iaString alias = parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_ALIAS, "");
            const iaString filename = parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_SOURCE, "");
            con_err("can't get resource for id:\"" << id << "\" alias:\"" << alias << "\" filename:\"" << filename << "\"");
            return nullptr;
        }

        _mutex.lock();
        auto resourceIter = _resources.find(id);
        if (resourceIter != _resources.end())
        {
            result = resourceIter->second;

            con_trace("cache hit " << result->getType() << " " << result->getInfo());
        }
        else
        {
            result = createResource(factory, parameters);
            _resources[result->getID()] = result;
            _loadingQueue.push_back(result);
        }

        const iResourceCacheMode currentCacheMode = result->_parameters.getParameter<iResourceCacheMode>(IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Free);
        const iResourceCacheMode cacheMode = parameters.getParameter<iResourceCacheMode>(IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Free);

        if (currentCacheMode < cacheMode)
        {
            result->_parameters.setParameter(IGOR_RESOURCE_PARAM_CACHE_MODE, cacheMode);
        }
        _mutex.unlock();

        return result;
    }

    iResourcePtr iResourceManager::createResource(const iParameters &parameters)
    {
        iFactoryPtr factory = getFactory(parameters);
        if (factory == nullptr)
        {
            return nullptr;
        }

        // ignoring other parameters and just create an empty resource (of what ever the factory decides this will be)
        iResourcePtr result = factory->createResource();
        if (result == nullptr)
        {
            return nullptr;
        }

        const iResourceCacheMode requestedCacheMode = parameters.getParameter<iResourceCacheMode>(IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Cache);
        if (requestedCacheMode > iResourceCacheMode::DontCache)
        {
            _resources[result->getID()] = result;
        }

        result->setProcessed(true);
        result->setValid(true);

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
            const iaString id = parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_ID, "");
            const iaString alias = parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_ALIAS, "");
            const iaString filename = parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_SOURCE, "");
            con_err("can't get resource for id:\"" << id << "\" alias:\"" << alias << "\" source:\"" << filename << "\"");
            return nullptr;
        }

        const iResourceCacheMode requestedCacheMode = parameters.getParameter<iResourceCacheMode>(IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Free);
        bool loadNow = false;
        iResourcePtr result;

        _mutex.lock();
        auto resourceIter = _resources.find(id);
        if (resourceIter != _resources.end())
        {
            result = resourceIter->second;

            con_trace("cache hit " << result->getType() << " " << result->getInfo());

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

            if (result->isValid() &&
                result->getSource().isEmpty())
            {
                result->setSource(iResourceManager::getInstance().getFilename(result->getID()));
            }

            result->setProcessed(true);
        }

        const iResourceCacheMode currentCacheMode = result->_parameters.getParameter<iResourceCacheMode>(IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Free);
        if (currentCacheMode < requestedCacheMode)
        {
            result->_parameters.setParameter(IGOR_RESOURCE_PARAM_CACHE_MODE, requestedCacheMode);
        }

        if (loadNow &&
            !result->isQuiet() &&
            result->isValid())
        {
            con_info("loaded " << result->getType() << " " << result->getInfo());
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
                    con_debug("released " << resource->getType() << " " << resource->getInfo());
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

    const iaString iResourceManager::getFilename(const iResourceID &id)
    {
        return _resourceDictionary.getFilename(id);
    }

    const iaString iResourceManager::getAlias(const iResourceID &id)
    {
        return _resourceDictionary.getAlias(id);
    }

    void iResourceManager::setAlias(const iResourceID &id, const iaString &alias)
    {
        _resourceDictionary.setAlias(id, alias);
    }

    const iaString iResourceManager::resolvePath(const iaString &filepath)
    {
        iaString result = filepath;

        const iaString currentDir = iaDirectory::getCurrentDirectory();

        _mutex.lock();

        for (auto path : _searchPaths)
        {
            iaString build;

            iaDirectory searchDir(path);
            if (searchDir.exists())
            {
                build = path + IGOR_PATHSEPARATOR + filepath;
            }
            else
            {
                // TODO assuming it's a relative path to executable. Maybe here we better refer to project path
                build = currentDir + IGOR_PATHSEPARATOR + path + IGOR_PATHSEPARATOR + filepath;
            }

            iaFile file(build);
            if (file.exists())
            {
                result = file.getFullFileName();
                break;
            }

            iaDirectory dir(build);
            if (dir.exists())
            {
                result = dir.getFullDirectoryName();
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

        if (file.exists())
        {
            return true;
        }

        bool result = false;
        _mutex.lock();

        for (auto path : _searchPaths)
        {
            iaFile composed(path + IGOR_PATHSEPARATOR + filename);
            if (composed.exists())
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

    void iResourceManager::getMaterials(std::vector<iShaderMaterialPtr> &materials)
    {
        materials.clear();

        // TODO this is way too slow. need to cache materials

        _mutex.lock();
        for (const auto &pair : _resources)
        {
            if (pair.second->getType() != IGOR_RESOURCE_SHADER_MATERIAL)
            {
                continue;
            }

            materials.push_back(std::dynamic_pointer_cast<iShaderMaterial>(pair.second));
        }
        _mutex.unlock();

        sort(materials.begin(), materials.end(),
             [](const iShaderMaterialPtr a, const iShaderMaterialPtr b) -> bool
             {
                 return a->getOrder() < b->getOrder();
             });
    }

    iParameters iResourceManager::buildParam(const iaString &type, const iaString &alias, iResourceCacheMode cacheMode)
    {
        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, type},
                           {IGOR_RESOURCE_PARAM_CACHE_MODE, cacheMode}});

        if (!alias.isEmpty())
        {
            param.setParameter(IGOR_RESOURCE_PARAM_ALIAS, alias);
        }

        const iaUUID id = _resourceDictionary.getResource(alias);
        if (id.isValid())
        {
            param.setParameter(IGOR_RESOURCE_PARAM_ID, id);
            return param;
        }

        if (iaUUID::isUUID(alias))
        {
            param.setParameter(IGOR_RESOURCE_PARAM_ID, iaUUID(alias));
            return param;
        }

        return param;
    }

    iParameters iResourceManager::buildParam(const iaString &type, const iResourceID &id, iResourceCacheMode cacheMode)
    {
        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, type},
                           {IGOR_RESOURCE_PARAM_CACHE_MODE, cacheMode}});

        if (id.isValid())
        {
            param.setParameter(IGOR_RESOURCE_PARAM_ID, id);
            return param;
        }

        return param;
    }

    void iResourceManager::removeResource(const iResourceID &resourceID)
    {
        _resourceDictionary.removeResource(resourceID);
    }

    void iResourceManager::addResource(const iaString &filename, const iaString &alias)
    {
        _resourceDictionary.addResource(filename, alias);
    }

} // namespace igor
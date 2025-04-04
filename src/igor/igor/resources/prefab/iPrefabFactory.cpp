// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/prefab/iPrefabFactory.h>

#include <igor/resources/prefab/iPrefab.h>
#include <igor/resources/prefab/loader/iPrefabIO.h>
#include <igor/resources/iResourceManager.h>
#include <igor/entities/iEntitySystemModule.h>

namespace igor
{
    iPrefabFactory::iPrefabFactory()
        : iFactory(IGOR_RESOURCE_PREFAB, IGOR_SUPPORTED_PREFAB_EXTENSIONS)
    {
    }

    iResourcePtr iPrefabFactory::createResource()
    {
        auto scene = iEntitySystemModule::getInstance().createScene();

        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_PREFAB},
                           {IGOR_RESOURCE_PARAM_ID, iaUUID()},
                           {IGOR_RESOURCE_PARAM_ENTITY_SCENE_ID, scene->getID()}});

        return iPrefabPtr(new iPrefab(param));
    }

    bool iPrefabFactory::saveResource(iResourcePtr resource, const iaString &filename)
    {
        iaString filepath;

        if (filename.isEmpty())
        {
            filepath = iResourceManager::getInstance().getFilename(resource->getID());
            if (filepath.isEmpty())
            {
                filepath = resource->getSource();
            }

            if (filepath.isEmpty())
            {
                con_err("not a valid source path \"" << filepath << "\" for ID " << resource->getID());
                return false;
            }
        }
        else
        {
            filepath = filename;
        }

        const iaString fullFilepath = iResourceManager::getInstance().resolvePath(filepath);
        iPrefabPtr prefab = std::dynamic_pointer_cast<iPrefab>(resource);
        return iPrefabIO::write(fullFilepath, prefab);
    }

    iResourcePtr iPrefabFactory::createResource(const iParameters &parameters)
    {
        return iPrefabPtr(new iPrefab(parameters));
    }

    bool iPrefabFactory::loadResource(iResourcePtr resource)
    {
        iaString filepath = iResourceManager::getInstance().getFilename(resource->getID());
        if (filepath.isEmpty())
        {
            filepath = resource->getSource();
        }

        if (filepath.isEmpty())
        {
            con_err("not a valid source path \"" << filepath << "\" for ID " << resource->getID());
            return false;
        }

        const iaString fullFilepath = iResourceManager::getInstance().resolvePath(filepath);
        iPrefabPtr prefab = std::dynamic_pointer_cast<iPrefab>(resource);
        return iPrefabIO::read(fullFilepath, prefab);
    }

    void iPrefabFactory::unloadResource(iResourcePtr resource)
    {
        auto prefab = std::dynamic_pointer_cast<iPrefab>(resource);
        con_assert(prefab != nullptr, "wrong type");

        iEntitySystemModule::getInstance().destroyScene(prefab->getSceneID());
    }

    iaString iPrefabFactory::getHashData(const iParameters &parameters) const
    {
        iaString hashData;

        // TODO if anything

        return hashData;
    }

}; // namespace igor
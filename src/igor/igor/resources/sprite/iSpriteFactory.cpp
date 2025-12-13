// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/sprite/iSpriteFactory.h>

#include <igor/resources/sprite/loader/iSpriteIO.h>
#include <igor/resources/iResourceManager.h>

namespace igor
{

    iSpriteFactory::iSpriteFactory()
        : iFactory(IGOR_RESOURCE_SPRITE, IGOR_SUPPORTED_SPRITE_EXTENSIONS)
    {
    }

    iResourcePtr iSpriteFactory::createResource()
    {
        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_SPRITE},
                           {IGOR_RESOURCE_PARAM_ID, iaUUID()}});

        return createResource(param);
    }

    iResourcePtr iSpriteFactory::createResource(const iParameters &parameters)
    {
        return iResourcePtr(new iSprite(parameters));
    }

    bool iSpriteFactory::loadResource(iResourcePtr resource)
    {
        iaString filepath = iResourceManager::getInstance().getFilename(resource->getID());
        if (filepath.isEmpty())
        {
            filepath = resource->getSource();
        }

        const iaString fullFilepath = iResourceManager::getInstance().resolvePath(filepath);
        iSpritePtr sprite = std::dynamic_pointer_cast<iSprite>(resource);
        return iSpriteIO::read(fullFilepath, sprite);
    }

    void iSpriteFactory::unloadResource(iResourcePtr resource)
    {
        // nothing else to do here
    }

    bool iSpriteFactory::saveResource(iResourcePtr resource, const iaString &filename)
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
        iSpritePtr sprite = std::dynamic_pointer_cast<iSprite>(resource);
        return iSpriteIO::write(fullFilepath, sprite);
    }

}; // namespace igor
// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/iMaterialFactory.h>

#include <igor/resources/iResourceManager.h>
#include <igor/resources/material/loader/iMaterialIO.h>

namespace igor
{

    iMaterialFactory::iMaterialFactory()
        : iFactory(IGOR_RESOURCE_MATERIAL, IGOR_SUPPORTED_MATERIAL_EXTENSIONS)
    {
    }

    iResourcePtr iMaterialFactory::createResource()
    {
        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                           {IGOR_RESOURCE_PARAM_ID, iaUUID()}});

        return iMaterialPtr(new iMaterial(param));
    }

    bool iMaterialFactory::saveResource(iResourcePtr resource, const iaString &filename)
    {
        // TODO
        return false;
    }

    iResourcePtr iMaterialFactory::createResource(const iParameters &parameters)
    {
        return iMaterialPtr(new iMaterial(parameters));
    }

    bool iMaterialFactory::loadResource(iResourcePtr resource)
    {
        iaString filepath = iResourceManager::getInstance().getFilename(resource->getID());
        if (filepath.isEmpty())
        {
            filepath = resource->getSource();
        }

        if (filepath.isEmpty())
        {
            con_err("not a valid source path " << resource->getID());
            return false;
        }

        const iaString fullFilepath = iResourceManager::getInstance().resolvePath(filepath);
        iMaterialPtr material = std::dynamic_pointer_cast<iMaterial>(resource);
        return iMaterialIO::read(fullFilepath, material);
    }

    void iMaterialFactory::unloadResource(iResourcePtr resource)
    {
        // nothing else to do here
    }

    iaString iMaterialFactory::getHashData(const iParameters &parameters) const
    {
        iaString hashData;

        // TODO if anything

        return hashData;
    }

}; // namespace igor
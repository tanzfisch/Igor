// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/iMaterialFactory.h>

#include <igor/resources/iResourceManager.h>

namespace igor
{

    iMaterialFactory::iMaterialFactory()
        : iFactory(IGOR_RESOURCE_MATERIAL, IGOR_SUPPORTED_MATERIAL_EXTENSIONS)
    {
    }

    iResourcePtr iMaterialFactory::createResource()
    {
        // TODO
        return nullptr;
    }

    bool iMaterialFactory::saveResource(iResourcePtr resource, const iaString &filename)
    {
        // TODO
        return false;
    }

    iResourcePtr iMaterialFactory::createResource(const iParameters &parameters)
    {
        // TODO
        return nullptr;
    }

    bool iMaterialFactory::loadResource(iResourcePtr resource)
    {
        // TODO
        return false;
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
// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/material/iMaterialResourceFactory.h>

namespace igor
{
    iMaterialResourceFactory::iMaterialResourceFactory()
    {
    }
    iMaterialResourceFactory::~iMaterialResourceFactory()
    {
    }

    iMaterialPtr iMaterialResourceFactory::loadMaterial(const iaString &filename, iResourceCacheMode cacheMode)
    {
        return iMaterialPtr();
    }

    iMaterialPtr iMaterialResourceFactory::createMaterial(const iaString &name)
    {
        return iMaterialPtr();
    }

    const iMaterialPtr &iMaterialResourceFactory::getDefaultMaterial() const
    {
        return _defaultMaterial;
    }

    void iMaterialResourceFactory::flush()
    {
    }
} // namespace igor
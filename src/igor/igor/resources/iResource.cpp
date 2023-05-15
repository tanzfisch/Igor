// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/iResource.h>

namespace igor
{
    bool iResource::isValid() const
    {
        return _valid;
    }

    bool iResource::isProcessed() const
    {
        return _processed;
    }

    void iResource::setProcessed(bool processed)
    {
        _processed = processed;
    }

    void iResource::setValid(bool valid)
    {
        _valid = valid;
    }

    const iaString &iResource::getName() const
    {
        return _parameters._name;
    }

    iResourceCacheMode iResource::getCacheMode() const
    {
        return _parameters._cacheMode;
    }

    const iResourceParameters &iResource::getParameters() const
    {
        return _parameters;
    }

    const iaString &iResource::getType() const
    {
        return _parameters._type;
    }
}
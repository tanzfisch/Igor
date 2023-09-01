// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/iResource.h>

namespace igor
{
    iResource::iResource(const iaString &type, const iParameters &parameters)
        : _parameters(parameters)
    {
        _type = type;
        _name = parameters.getParameter<iaString>("name", "");
        _cacheMode = parameters.getParameter<iResourceCacheMode>("cacheMode", iResourceCacheMode::Cache);
        _quiet = parameters.getParameter<bool>("quiet", false);
    }

    bool iResource::isQuiet() const
    {
        return _quiet;
    }

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
        return _name;
    }

    iResourceCacheMode iResource::getCacheMode() const
    {
        return _cacheMode;
    }

    const iParameters &iResource::getParameters() const
    {
        return _parameters;
    }

    const iaString &iResource::getType() const
    {
        return _type;
    }
}

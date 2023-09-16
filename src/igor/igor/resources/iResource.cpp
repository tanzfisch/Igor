// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/iResource.h>

#include <igor/resources/iResourceManager.h>

namespace igor
{
    iResource::iResource(const iParameters &parameters)
        : _parameters(parameters)
    {
        extractID(parameters, _id, true);

        // if there is no id specified we need a new one
        if (!_id.isValid())
        {
            _id = iaUUID();
        }

        // TODO get rid of alias
        _alias = parameters.getParameter<iaString>("alias", "");

        _type = parameters.getParameter<iaString>("type");
        _cacheMode = parameters.getParameter<iResourceCacheMode>("cacheMode", iResourceCacheMode::Cache);
        _quiet = parameters.getParameter<bool>("quiet", false);
    }

    bool iResource::extractID(const iParameters &parameters, iResourceID &id, bool quiet)
    {
        id = parameters.getParameter<iResourceID>("id", IGOR_INVALID_ID);
        if (id.isValid())
        {
            return true;
        }

        const iaString alias = parameters.getParameter<iaString>("alias", "");
        id = iResourceManager::getInstance().getResourceID(alias);
        if (id.isValid())
        {
            return true;
        }

        const bool generate = parameters.getParameter<bool>("generate", false);
        if (generate)
        {
            return true;
        }

        const iaString filename = parameters.getParameter<iaString>("filename", "");
        if (!filename.isEmpty())
        {
            return true;
        }

        if (!quiet)
        {
            con_err("iResourceManager::getInstance().resolvePath \"" << alias << "\" failed");
        }
        return false;
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

    const iaString &iResource::getAlias() const
    {
        return _alias;
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

    const iResourceID &iResource::getID() const
    {
        return _id;
    }
}

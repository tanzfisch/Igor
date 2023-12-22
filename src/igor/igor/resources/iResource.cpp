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

        _alias = parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_ALIAS, "");
        _source = parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_SOURCE, "");

        _type = parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE);
        _cacheMode = parameters.getParameter<iResourceCacheMode>(IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Cache);
        _quiet = parameters.getParameter<bool>(IGOR_RESOURCE_PARAM_QUIET, false);
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

        const bool generate = parameters.getParameter<bool>(IGOR_RESOURCE_PARAM_GENERATE, false);
        if (generate)
        {
            // no id expected
            return true;
        }

        const iaString filename = parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_SOURCE, "");
        if (!filename.isEmpty())
        {
            // if there is no id but a file name make sure the id is based on the filename
            id = static_cast<uint64>(filename.getHashValue());
            return true;
        }

        return false;
    }

    bool iResource::isQuiet() const
    {
#ifdef IGOR_DEBUG
        if (iaConsole::getInstance().getLogLevel() >= iaLogLevel::Debug)
        {
            return false;
        }
        else
        {
            return _quiet;
        }
#else
        return _quiet;
#endif
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

    const iaString &iResource::getSource() const
    {
        return _source;
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

    const iaString iResource::getInfo() const
    {
        iaString result;

        result += _id.toString();

        if (!_alias.isEmpty())
        {
            result += ", \"";
            result += _alias;
            result += "\"";
        }

        else if (!_source.isEmpty())
        {
            result += ", \"";
            result += _source;
            result += "\"";
        }

        return result;
    }

    void iResource::setName(const iaString &name)
    {
        _name = name;
    }

    const iaString &iResource::getName() const
    {
        return _name;
    }    

    const iaString &iResource::getAlias() const
    {
        return _alias;
    }
}

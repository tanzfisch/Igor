// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/iResource.h>

#include <igor/resources/iResourceManager.h>

namespace igor
{
    iResource::iResource(const iParameters &parameters)
        : _parameters(parameters)
    {
        extractID(parameters, _id);

        // if there is no id specified we need a new one
        if (!_id.isValid())
        {
            const bool generate = parameters.getParameterValue<bool>(IGOR_RESOURCE_PARAM_GENERATE, false);
            if (generate)
            {
                _id = iaUUID::getMarked();
            }
            else
            {
                _id = iaUUID();
            }
        }

        _alias = parameters.getParameterValue<iaString>(IGOR_RESOURCE_PARAM_ALIAS, "");
        _source = parameters.getParameterValue<iaString>(IGOR_RESOURCE_PARAM_SOURCE, "");

        _type = parameters.getParameterValue<iaString>(IGOR_RESOURCE_PARAM_TYPE);
        _cacheMode = parameters.getParameterValue<iResourceCacheMode>(IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Cache);
        _quiet = parameters.getParameterValue<bool>(IGOR_RESOURCE_PARAM_QUIET, false);
    }

    bool iResource::extractID(const iParameters &parameters, iResourceID &id)
    {
        id = parameters.getParameterValue<iResourceID>("id", IGOR_INVALID_ID);
        if (id.isValid())
        {
            return true;
        }

        const iaString alias = parameters.getParameterValue<iaString>("alias", "");
        id = iResourceManager::getInstance().getResourceID(alias);
        if (id.isValid())
        {
            return true;
        }

        const iaString filename = parameters.getParameterValue<iaString>(IGOR_RESOURCE_PARAM_SOURCE, "");
        if (!filename.isEmpty())
        {
            // if there is no id but a file name make sure the id is based on the filename
            id = static_cast<uint64>(filename.getHashValue());
            return true;
        }

        const bool generate = parameters.getParameterValue<bool>(IGOR_RESOURCE_PARAM_GENERATE, false);
        if (generate)
        {
            // no id expected
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

    void iResource::setSource(const iaString &source)
    {
        _source = source;
    }

    const iaString &iResource::getSource() const
    {
        return _source;
    }

    bool iResource::hasSource() const
    {
        const auto path = iResourceManager::getInstance().resolvePath(getSource());
        return iaPath::exists(path);
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

    const iaString &iResource::getAlias() const
    {
        return _alias;
    }
}

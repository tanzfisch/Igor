// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/script/iScript.h>

#include <igor/resources/script/iScriptEngine.h>

#include <igor/resources/iResourceManager.h>

namespace igor
{

    iScript::iScript(const iParameters &parameters)
        : iResource(parameters)
    {
    }

    void iScript::execute()
    {
        iScriptEngine::getInstance().execute(_script);
    }

    void iScript::setScript(const std::string &script)
    {
        _script = script;
    }

    const std::string &iScript::getScript() const
    {
        return _script;
    }

}; // namespace igor

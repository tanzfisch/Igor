// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/script/iScript.h>

#include <igor/resources/script/iScriptEngine.h>

#include <igor/resources/iResourceManager.h>

namespace igor
{

    iScript::iScript(const iParameters &parameters)
        : iResource(parameters)
    {
        const iaString script = parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_SCRIPT, "");

        int len = script.getLength() + 1;
        _script.resize(len);
        script.getData(&_script[0], len);
        _script[len] = 0;
    }

    void iScript::execute()
    {
        iScriptEngine::getInstance().execute(getScript());
    }

    void iScript::setScript(const iaString &script)
    {
        int len = script.getLength() + 1;
        _script.resize(len);
        script.getData(&_script[0], len);
        _script[len] = 0;
    }

    const char* iScript::getScript() const
    {
        return _script.c_str();
    }

}; // namespace igor

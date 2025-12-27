// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iScriptComponent.h>

#include <igor/resources/script/iScriptEngine.h>

namespace igor
{
    iScriptComponent::iScriptComponent()
    {
    }

    iEntityComponent *iScriptComponent::createInstance()
    {
        return new iScriptComponent();
    }

    const iaString &iScriptComponent::getTypeName()
    {
        static const iaString name("igor_script_component");
        return name;
    }

    iEntityComponentPtr iScriptComponent::getCopy()
    {
        iScriptComponent *component = new iScriptComponent();
        component->_scripts = _scripts;
        return component;
    }

    std::vector<iaString> iScriptComponent::getInfo() const
    {
        std::vector<iaString> result = iEntityComponent::getInfo();

        for (const auto &script : _scripts)
        {
            result.push_back(script._script->getID().toString());
        }

        return result;
    }

    const std::vector<iScriptData> &iScriptComponent::getScripts() const
    {
        return _scripts;
    }

    void iScriptComponent::addScript(const iScriptPtr script)
    {
        _scripts.push_back({nullptr, script});
    }

    void iScriptComponent::addScript(const iScriptDelegate &delegate)
    {
        _scripts.push_back({delegate, nullptr});
    }

    void iScriptComponent::removeScript(const iScriptDelegate &delegate)
    {
        auto iter = std::find_if(_scripts.begin(), _scripts.end(), [delegate](const iScriptData &scriptData)
                                 { return scriptData._delegate == delegate; });

        if (iter == _scripts.end())
        {
            con_err("can't remove script");
            return;
        }

        _scripts.erase(iter);
    }

    void iScriptComponent::execute()
    {
        for (auto &scriptData : _scripts)
        {
            if (scriptData._script != nullptr)
            {
                switch (scriptData._scriptState)
                {
                case iEntityScriptState::Init:
                    con_assert_sticky(scriptData._envRef == IGOR_LUA_NOREF, "already initialized environment");

                    if (iScriptEngine::getInstance().initEntityScript(getEntity(), scriptData))
                    {
                        iScriptEngine::getInstance().callEntityInit(getEntity(), scriptData);                        
                    }
                    break;
                case iEntityScriptState::Update:
                    iScriptEngine::getInstance().callEntityUpdate(getEntity(), scriptData);
                    break;
                case iEntityScriptState::Final:
                    iScriptEngine::getInstance().callEntityFinal(getEntity(), scriptData);
                    break;
                case iEntityScriptState::Stop:
                    break;
                };
            }
            else if (scriptData._delegate.isValid())
            {
                scriptData._delegate(getEntity());
            }
        }
    }
}
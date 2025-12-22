// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iScriptComponent.h>

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
            result.push_back(script._name);
        }

        return result;
    }

    const std::vector<iScriptData> &iScriptComponent::getScripts() const
    {
        return _scripts;
    }

    void iScriptComponent::addScript(const iScriptPtr script, const iaString &name)
    {
        _scripts.push_back({nullptr, script, name});
    }

    void iScriptComponent::addScript(const iScriptDelegate &delegate, const iaString &name)
    {
        _scripts.push_back({delegate, nullptr, name});
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
            if(scriptData._script != nullptr)
            {
                scriptData._script->execute();
            }
            else if (scriptData._delegate.isValid())
            {
                scriptData._delegate(getEntity());
            }
        }
    }
}
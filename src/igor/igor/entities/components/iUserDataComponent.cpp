// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iUserDataComponent.h>

#include <igor/resources/script/iScriptEngine.h>

namespace igor
{
    iUserDataComponent::iUserDataComponent()
    {
    }

    iEntityComponentPtr iUserDataComponent::createInstance()
    {
        return new iUserDataComponent();
    }

    const iaString &iUserDataComponent::getTypeName()
    {
        static const iaString name("igor_user_data_component");
        return name;
    }

    iEntityComponentPtr iUserDataComponent::getCopy()
    {
        iUserDataComponent *component = new iUserDataComponent();
        component->_data = _data;
        return component;
    }

    std::vector<iaString> iUserDataComponent::getInfo() const
    {
        std::vector<iaString> result = iEntityComponent::getInfo();
        
        const auto types = _data.getTypes();
        result.insert(result.end(), types.begin(), types.end());

        return result;
    }

    iMimeData& iUserDataComponent::getData()
    {
        return _data;
    }

    bool iUserDataComponent::hasValue(const iaString &key) const
    {
        return _data.hasType(key);
    }

}
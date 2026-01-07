// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iMimeDataComponent.h>

#include <igor/resources/script/iScriptEngine.h>

namespace igor
{
    iMimeDataComponent::iMimeDataComponent()
    {
    }

    iEntityComponentPtr iMimeDataComponent::createInstance()
    {
        return new iMimeDataComponent();
    }

    const iaString &iMimeDataComponent::getTypeName()
    {
        static const iaString name("igor_mime_data_component");
        return name;
    }

    iEntityComponentPtr iMimeDataComponent::getCopy()
    {
        iMimeDataComponent *component = new iMimeDataComponent();
        component->_data = _data;
        return component;
    }

    std::vector<iaString> iMimeDataComponent::getInfo() const
    {
        std::vector<iaString> result = iEntityComponent::getInfo();
        
        const auto types = _data.getTypes();
        result.insert(result.end(), types.begin(), types.end());

        return result;
    }

    iMimeData& iMimeDataComponent::getMimeData()
    {
        return _data;
    }
}
// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iUserDataComponent.h>

#include <igor/resources/script/iScriptEngine.h>
#include <igor/data/iAny.h>

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
    
        for(const auto &pair : _data.getData())
        {
            std::wstringstream stream;
            stream << std::right << std::setw(40) << pair.first << " | " << pair.second.toString() << " (" << toString(pair.second.getType()) << ")";
            result.push_back(stream.str().c_str());
        }
    
        return result;
    }

    iParameters& iUserDataComponent::getData()
    {
        return _data;
    }

    bool iUserDataComponent::hasValue(const iaString &key) const
    {
        return _data.hasParameter(key);
    }

}
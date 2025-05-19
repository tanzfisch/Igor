// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iGlobalBoundaryComponent.h>

namespace igor
{
    iGlobalBoundaryComponent::iGlobalBoundaryComponent(iGlobalBoundaryType type)
        : _type(type)
    {
    }

    iEntityComponent *iGlobalBoundaryComponent::createInstance()
    {
        return new iGlobalBoundaryComponent();
    }

    const iaString &iGlobalBoundaryComponent::getTypeName()
    {
        static const iaString name("igor_global_boundary_component");
        return name;
    }

    iEntityComponentPtr iGlobalBoundaryComponent::getCopy()
    {
        iGlobalBoundaryComponent *component = new iGlobalBoundaryComponent();
        component->_type = _type;
        return component;
    }

    iGlobalBoundaryType iGlobalBoundaryComponent::getType() const
    {
        return _type;
    }

    void iGlobalBoundaryComponent::setType(iGlobalBoundaryType type)
    {
        _type = type;
        setDirty();
    }

    std::vector<iaString> iGlobalBoundaryComponent::getInfo() const
    {
        std::vector<iaString> result = iEntityComponent::getInfo();

        iaString type("Type: ");
        switch (_type)
        {
        case iGlobalBoundaryType::None:
            type += "None";
            break;
        case iGlobalBoundaryType::Normal:
            type += "Normal";
            break;
        case iGlobalBoundaryType::Repeat:
            type += "Repeat";
            break;
        }
        result.push_back(type);

        return result;
    }
}
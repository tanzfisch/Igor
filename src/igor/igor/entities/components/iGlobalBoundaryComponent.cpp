// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
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
    }
}
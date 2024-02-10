// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntityComponent.h>

namespace igor
{

    iEntityComponent::iEntityComponent(const iaString &name)
    {
        setName(name);
    }

    void iEntityComponent::setName(const iaString &name)
    {
        _name = name;
    }

    const iEntityComponentID &iEntityComponent::getID() const
    {
        return _id;
    }

    const iaString &iEntityComponent::getName() const
    {
        return _name;
    }

} // igor
// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntityComponent.h>

namespace igor
{

    iEntityComponent::iEntityComponent()
    {
    }

    const iEntityComponentID &iEntityComponent::getID() const
    {
        return _id;
    }

    bool iEntityComponent::onLoad(iEntityPtr entity, bool &asyncLoad)
    {
        return true;
    }

    void iEntityComponent::onActivate(iEntityPtr entity)
    {
    }

    void iEntityComponent::onDeactivate(iEntityPtr entity)
    {
    }    

    void iEntityComponent::onUnLoad(iEntityPtr entity)
    {
    }

} // igor
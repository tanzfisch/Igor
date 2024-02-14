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

    bool iEntityComponent::onLoad()
    {
        return true;
    }

    void iEntityComponent::onUnload()
    {
    }

    void iEntityComponent::onActivate()
    {
    }

    void iEntityComponent::onDeactivate()
    {
    }

    bool iEntityComponent::isUnloaded() const
    {
        return _state == iEntityComponentState::Unloaded;
    }

    bool iEntityComponent::isLoaded() const
    {
        return _state == iEntityComponentState::Loaded;
    }

    bool iEntityComponent::isLoadFailed() const
    {
        return _state == iEntityComponentState::LoadFailed;
    }

    bool iEntityComponent::isActive() const
    {
        return _state == iEntityComponentState::Active;
    }

} // igor
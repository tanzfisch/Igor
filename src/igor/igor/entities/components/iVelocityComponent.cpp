// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iVelocityComponent.h>

namespace igor
{

    iVelocityComponent::iVelocityComponent(const iaVector3d &velocity, const iaVector3d &angularVelocity)
        : _velocity(velocity), _angularVelocity(angularVelocity)
    {
    }

    iEntityComponent *iVelocityComponent::createInstance()
    {
        return new iVelocityComponent();
    }

    const iaString &iVelocityComponent::getTypeName()
    {
        static const iaString name("igor_velocity_component");
        return name;
    }

    const iaVector3d &iVelocityComponent::getVelocity() const
    {
        return _velocity;
    }

    void iVelocityComponent::setVelocity(const iaVector3d &velocity)
    {
        _velocity = velocity;
    }

    const iaVector3d &iVelocityComponent::getAngularVelocity() const
    {
        return _angularVelocity;
    }

    void iVelocityComponent::setAngularVelocity(const iaVector3d &angularVelocity)
    {
        _angularVelocity = angularVelocity;
    }

}
// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iSphereComponent.h>

namespace igor
{
    iEntityComponentPtr iSphereComponent::getCopy()
    {
        iSphereComponent *component = new iSphereComponent();
        component->_radius = _radius;
        component->_offset = _offset;
        return component;
    }

    iEntityComponent *iSphereComponent::createInstance()
    {
        return new iSphereComponent();
    }      

    float64 iSphereComponent::getRadius() const
    {
        return _radius;
    }    

    const iaVector3d &iSphereComponent::getOffset() const
    {
        return _offset;
    }

    void iSphereComponent::setRadius(float64 radius)
    {
        _radius = radius;
    }

    void iSphereComponent::setOffset(const iaVector3d &offset)
    {
        _offset = offset;
    }

}
// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iCircleComponent.h>

namespace igor
{

    iCircleComponent::iCircleComponent()
    {
    }

    iEntityComponent *iCircleComponent::createInstance()
    {
        return new iCircleComponent();
    }    

    iCircleComponent::iCircleComponent(float64 radius, const iaVector2d &offset)
        : _radius(radius), _offset(offset)
    {
    }

    float64 iCircleComponent::getRadius() const
    {
        return _radius;
    }

    const iaVector2d &iCircleComponent::getOffset() const
    {
        return _offset;
    }

    void iCircleComponent::setOffset(const iaVector2d &offset)
    {
        _offset = offset;
    }

    void iCircleComponent::setRadius(float64 radius)
    {
        _radius = radius;
    }

    iEntityComponentPtr iCircleComponent::getCopy()
    {
        iCircleComponent *component = new iCircleComponent();
        component->_radius = _radius;
        component->_offset = _offset;
        return component;
    }
}
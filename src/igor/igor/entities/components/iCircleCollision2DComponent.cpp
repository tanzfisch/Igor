// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iCircleCollision2DComponent.h>

namespace igor
{

    iCircleCollision2DComponent::iCircleCollision2DComponent()
    {

    }

    iCircleCollision2DComponent::iCircleCollision2DComponent(float64 radius, const iaVector2d &offset)
        : _radius(radius), _offset(offset)
    {
    }

    float64 iCircleCollision2DComponent::getRadius() const
    {
        return _radius;
    }

    const iaVector2d &iCircleCollision2DComponent::getOffset() const
    {
        return _offset;
    }

    iEntityComponentPtr iCircleCollision2DComponent::getCopy()
    {
        iCircleCollision2DComponent *component = new iCircleCollision2DComponent();
        component->_radius = _radius;
        component->_offset = _offset;
        return component;
    }
}
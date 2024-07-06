// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iCircleCollision2DComponent.h>

namespace igor
{

    float64 iCircleCollision2DComponent::getRadius() const
    {
        return _radius;
    }

    const iaVector2d &iCircleCollision2DComponent::getOffset() const
    {
        return _offset;
    }

}
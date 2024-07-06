// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iSphereCollision3DComponent.h>

namespace igor
{
    float64 iSphereCollision3DComponent::getRadius() const
    {
        return _radius;
    }

    const iaVector3d &iSphereCollision3DComponent::getOffset() const
    {
        return _offset;
    }
}
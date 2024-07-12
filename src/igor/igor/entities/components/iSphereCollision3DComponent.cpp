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

    void iSphereCollision3DComponent::setRadius(float64 radius)
    {
        _radius = radius;
    }

    void iSphereCollision3DComponent::setOffset(const iaVector3d &offset)
    {
        _offset = offset;
    }

    void to_json(json &j, const iSphereCollision3DComponent &component)
    {
        j["radius"] = component.getRadius();
        j["offset"] = component.getOffset();
    }

    void from_json(const json &j, iSphereCollision3DComponent &component)
    {
        component.setRadius(j["radius"].get<float64>());
        component.setOffset(j["offset"].get<iaVector3d>());
    }
}
// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iTransformComponent.h>

namespace igor
{
    iTransformComponent::iTransformComponent(const iaVector3d &position, const iaVector3d &orientation, const iaVector3d &scale, const iaString &name)
        : iEntityComponent(name), _position(position), _orientation(orientation), _scale(scale)
    {
    }

    void iTransformComponent::updateWorldMatrix(iaMatrixd &worldMatrix)
    {
        worldMatrix.translate(_position);
        worldMatrix.rotate(_orientation);
        worldMatrix.scale(_scale);
        _worldMatrix = worldMatrix;
    }

    const iaMatrixd& iTransformComponent::getWorldMatrix() const
    {
        return _worldMatrix;
    }
}
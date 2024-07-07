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

    bool iTransformComponent::updateWorldMatrix(iaMatrixd &worldMatrix)
    {
        worldMatrix.translate(_position);
        worldMatrix.rotate(_orientation);
        worldMatrix.scale(_scale);
        bool result = _worldMatrix != worldMatrix;
        _worldMatrix = worldMatrix;
        return result;
    }

    const iaMatrixd &iTransformComponent::getWorldMatrix() const
    {
        return _worldMatrix;
    }

    void iTransformComponent::setPosition(const iaVector3d &position)
    {
        _position = position;
        _entity->setDirtyHierarchy(true);
    }

    void iTransformComponent::translate(const iaVector3d &translate)
    {
        _position += translate;
        _entity->setDirtyHierarchy(true);
    }

    const iaVector3d &iTransformComponent::getPosition() const
    {
        return _position;
    }

    void iTransformComponent::setOrientation(const iaVector3d &orientation)
    {
        _orientation = orientation;
        _entity->setDirtyHierarchy(true);
    }

    void iTransformComponent::rotate(const iaVector3d &rotate)
    {
        _orientation += rotate;
        _entity->setDirtyHierarchy(true);
    }

    const iaVector3d &iTransformComponent::getOrientation() const
    {
        return _orientation;
    }

    void iTransformComponent::setScale(const iaVector3d &scale)
    {
        _scale = scale;
        _entity->setDirtyHierarchy(true);
    }

    const iaVector3d &iTransformComponent::getScale() const
    {
        return _scale;
    }

}
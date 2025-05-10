// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iTransformComponent.h>

namespace igor
{
    iTransformComponent::iTransformComponent(const iaVector3d &position, const iaVector3d &orientation, const iaVector3d &scale)
        : _position(position), _orientation(orientation), _scale(scale)
    {
    }

    iEntityComponent *iTransformComponent::createInstance()
    {
        return new iTransformComponent();
    }

    const iaString &iTransformComponent::getTypeName()
    {
        static const iaString name("igor_transform_component");
        return name;
    }

    iEntityComponentPtr iTransformComponent::getCopy()
    {
        iTransformComponent *component = new iTransformComponent();
        component->_position = _position;
        component->_orientation = _orientation;
        component->_scale = _scale;
        return component;
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
        if (_position != position)
        {
            _position = position;
            _entity->setDirtyHierarchy(true);
        }
    }

    void iTransformComponent::translate(const iaVector3d &translate)
    {
        setPosition(_position + translate);
    }

    const iaVector3d &iTransformComponent::getPosition() const
    {
        return _position;
    }

    void iTransformComponent::setOrientation(const iaVector3d &orientation)
    {
        if (_orientation != orientation)
        {
            _orientation = orientation;
            _entity->setDirtyHierarchy(true);
        }
    }

    void iTransformComponent::rotate(const iaVector3d &rotate)
    {
        setOrientation(_orientation + rotate);
    }

    const iaVector3d &iTransformComponent::getOrientation() const
    {
        return _orientation;
    }

    void iTransformComponent::setScale(const iaVector3d &scale)
    {
        if (_scale != scale)
        {
            _scale = scale;
            _entity->setDirtyHierarchy(true);
        }
    }

    const iaVector3d &iTransformComponent::getScale() const
    {
        return _scale;
    }

    std::vector<iaString> iTransformComponent::getInfo() const
    {
        std::vector<iaString> result = iEntityComponent::getInfo();

        result.push_back(iaString("Pos: ") + iaString::toString(_position));
        result.push_back(iaString("Ori: ") + iaString::toString(_orientation));
        result.push_back(iaString("Sca: ") + iaString::toString(_scale));

        return result;
    }

    void iTransformComponent::onActivate(iEntityPtr entity)
    {
        if(entity->isHierarchyDirty())
        {
            return;
        }

        entity->setDirtyHierarchyDown();
    }    
}
// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iTransformComponent.h>

namespace igor
{
    iTransformComponent::iTransformComponent(const iaVector3d &position, const iaQuaterniond &orientation, const iaVector3d &scale)
    {
        _transform._position = position;
        _transform._orientation = orientation;
        _transform._scale = scale;
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
        component->_transform = _transform;
        return component;
    }

    const iaMatrixd &iTransformComponent::getWorldMatrix() const
    {
        return _worldMatrix;
    }

    void iTransformComponent::setPosition(const iaVector3d &position)
    {
        if (_transform._position != position)
        {
            _transform._position = position;
            setDirty();
            _entity->setDirtyHierarchy();
        }
    }

    void iTransformComponent::translate(const iaVector3d &translate)
    {
        setPosition(_transform._position + translate);
    }

    const iaVector3d &iTransformComponent::getPosition() const
    {
        return _transform._position;
    }

    const iaVector3d &iTransformComponent::getWorldPosition() const
    {
        return _worldTransform._position;
    }

    void iTransformComponent::setOrientation(const iaQuaterniond &orientation)
    {
        if (_transform._orientation != orientation)
        {
            _transform._orientation = orientation;
            setDirty();
            _entity->setDirtyHierarchy();
        }
    }

    void iTransformComponent::rotate(const iaQuaterniond &rotate)
    {
        _transform._orientation *= rotate;
        setDirty();
        _entity->setDirtyHierarchy();
    }

    const iaQuaterniond iTransformComponent::getOrientation() const
    {
        return _transform._orientation;
    }

    const iaQuaterniond iTransformComponent::getWorldOrientation() const
    {
        return _worldTransform._orientation;
    }

    void iTransformComponent::setScale(const iaVector3d &scale)
    {
        if (_transform._scale != scale)
        {
            _transform._scale = scale;
            setDirty();
            _entity->setDirtyHierarchy();
        }
    }

    const iaVector3d &iTransformComponent::getScale() const
    {
        return _transform._scale;
    }

    const iaVector3d &iTransformComponent::getWorldScale() const
    {
        return _worldTransform._scale;
    }

    std::vector<iaString> iTransformComponent::getInfo() const
    {
        std::vector<iaString> result = iEntityComponent::getInfo();

        result.push_back(iaString("Pos: ") + iaString::toString(_worldTransform._position) + " local:" + iaString::toString(_transform._position));
        result.push_back(iaString("Ori: ") + iaString::toString(_worldTransform._orientation) + " local:" + iaString::toString(_transform._orientation));
        result.push_back(iaString("Sca: ") + iaString::toString(_worldTransform._scale) + " local:" + iaString::toString(_transform._scale));

        return result;
    }

    void iTransformComponent::onActivate(iEntityPtr entity)
    {
        entity->setDirtyHierarchyDown();
    }
}
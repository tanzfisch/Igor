// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/traversal/iEntityTransformTraverser.h>

#include <igor/entities/components/iQuadtreeComponent.h>
#include <igor/entities/components/iOctreeComponent.h>
#include <igor/entities/components/iCircleComponent.h>
#include <igor/entities/components/iSphereComponent.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    void iEntityTransformTraverser::preTraverse()
    {
        _hasQuadtree = getScene()->hasQuadtree();
        _hasOctree = getScene()->hasOctree();
        _currentTransform.identity();
    }

    void iEntityTransformTraverser::updateQuadtree(iEntityPtr entity)
    {
        iQuadtreeComponent *body = entity->getComponent<iQuadtreeComponent>();
        if (body == nullptr ||
            body->_object == nullptr ||
            body->_object->_parent.expired())
        {
            return;
        }

        iaVector2d position(_currentTransform._position._x, _currentTransform._position._y);

        iCircleComponent *component = entity->getComponent<iCircleComponent>();
        if (component != nullptr)
        {
            const auto &offset = component->getOffset();
            const iaCircled circle(position._x + offset._x,
                                   position._y + offset._y,
                                   component->getRadius());
            getScene()->getQuadtree().update(body->_object, circle);
        }
        else
        {
            getScene()->getQuadtree().update(body->_object, position);
        }
    }

    void iEntityTransformTraverser::updateOctree(iEntityPtr entity)
    {
        iOctreeComponent *body = entity->getComponent<iOctreeComponent>();
        if (body == nullptr ||
            body->_object == nullptr ||
            body->_object->_parent.expired())
        {
            return;
        }

        iSphereComponent *sphereComponent = entity->getComponent<iSphereComponent>();
        if (sphereComponent != nullptr)
        {
            const iaSphered sphere(_currentTransform._position + sphereComponent->getOffset(), sphereComponent->getRadius());
            getScene()->getOctree().update(body->_object, sphere);
        }
        else
        {
            getScene()->getOctree().update(body->_object, _currentTransform._position);
        }
    }

    bool iEntityTransformTraverser::updateTransformComponent(iTransformComponentPtr transformComponent, iaTransformd &transform)
    {
        const iaVector3d scaledLocalPos = transform._scale * transformComponent->_transform._position;
        const iaVector3d rotatedPos = transform._orientation.rotate(scaledLocalPos);
        const auto worldPosition = transform._position + rotatedPos;
        iaQuaterniond worldOrientation = transform._orientation * transformComponent->_transform._orientation;
        worldOrientation.normalize();
        const auto worldScale = transform._scale * transformComponent->_transform._scale;

        transform._position = worldPosition;
        transform._orientation = worldOrientation;
        transform._scale = worldScale;

        if (transformComponent->_worldTransform._position == worldPosition &&
            transformComponent->_worldTransform._orientation == worldOrientation &&
            transformComponent->_worldTransform._scale == worldScale)
        {
            return false;
        }

        transformComponent->_worldTransform._position = worldPosition;
        transformComponent->_worldTransform._orientation = worldOrientation;
        transformComponent->_worldTransform._scale = worldScale;

        transformComponent->_worldMatrix = transformComponent->_worldTransform.getMatrix();

        return true;
    }

    bool iEntityTransformTraverser::preOrderVisit(iEntityPtr entity)
    {
        auto transformComponent = entity->getComponent<iTransformComponent>();
        if (transformComponent != nullptr)
        {
            _transformStack.push_back(_currentTransform);
        }

        if (!entity->isHierarchyDirty())
        {
            return false;
        }

        if (transformComponent != nullptr &&
            updateTransformComponent(transformComponent, _currentTransform))
        {
            if (_hasQuadtree)
            {
                updateQuadtree(entity);
            }

            if (_hasOctree)
            {
                updateOctree(entity);
            }
        }
        entity->resetDirtyHierarchy();
        return true;
    }

    void iEntityTransformTraverser::postOrderVisit(iEntityPtr entity)
    {
        auto transform = entity->getComponent<iTransformComponent>();

        // skip entities without transform
        if (transform == nullptr)
        {
            return;
        }

        con_assert(_transformStack.size() != 0, "stack underflow");
        _currentTransform = _transformStack.back();
        _transformStack.pop_back();
    }

    void iEntityTransformTraverser::postTraverse()
    {
        con_assert(_transformStack.size() == 0, "matrix stack should be empty");
    }

} // namespace igor
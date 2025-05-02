// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/traversal/iEntityTransformTraverser.h>

#include <igor/entities/components/iQuadtreeComponent.h>
#include <igor/entities/components/iOctreeComponent.h>
#include <igor/entities/components/iTransformComponent.h>
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
        _currentMatrix.identity();
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

        iaVector2d position(_currentMatrix._pos._x, _currentMatrix._pos._y);

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
            const iaSphered sphere(_currentMatrix._pos + sphereComponent->getOffset(), sphereComponent->getRadius());
            getScene()->getOctree().update(body->_object, sphere);
        }
        else
        {
            getScene()->getOctree().update(body->_object, _currentMatrix._pos);
        }
    }

    bool iEntityTransformTraverser::preOrderVisit(iEntityPtr entity)
    {
        auto transformComponent = entity->getComponent<iTransformComponent>();

        if (transformComponent != nullptr)
        {
            _matrixStack.push_back(_currentMatrix);
        }

        if (!entity->isHierarchyDirty())
        {
            return false;
        }

        if (transformComponent != nullptr &&
            transformComponent->updateWorldMatrix(_currentMatrix))
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
        entity->setDirtyHierarchy(false);
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

        con_assert(_matrixStack.size() != 0, "stack underflow");
        _currentMatrix = _matrixStack.back();
        _matrixStack.pop_back();
    }

    void iEntityTransformTraverser::postTraverse()
    {
        con_assert(_matrixStack.size() == 0, "matrix stack should be empty");
    }

} // namespace igor
// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/traversal/iEntityTransformTraverser.h>

#include <igor/entities/components/iQuadtreeComponent.h>
#include <igor/entities/components/iOctreeComponent.h>
#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iCircleCollision2DComponent.h>
#include <igor/entities/components/iSphereCollision3DComponent.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    void iEntityTransformTraverser::preTraverse()
    {
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

        iCircleCollision2DComponent *collision = entity->getComponent<iCircleCollision2DComponent>();
        if (collision != nullptr)
        {
            const iaCircled circle(position._x + collision->_offset._x,
                                   position._y + collision->_offset._y,
                                   collision->_radius);
            _scene->getQuadtree().update(body->_object, circle);
        }
        else
        {
            _scene->getQuadtree().update(body->_object, position);
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

        iSphereCollision3DComponent *collision = entity->getComponent<iSphereCollision3DComponent>();
        if (collision != nullptr)
        {
            const iaSphered sphere(_currentMatrix._pos + collision->_offset, collision->_radius);
            _scene->getOctree().update(body->_object, sphere);
        }
        else
        {
            _scene->getOctree().update(body->_object, _currentMatrix._pos);
        }
    }

    bool iEntityTransformTraverser::preOrderVisit(iEntityPtr entity)
    {
        auto transformComponent = entity->getComponent<iTransformComponent>();

        if (transformComponent != nullptr)
        {
            _matrixStack.push_back(_currentMatrix);
        }

        if (entity->isHierarchyDirty())
        {
            if (transformComponent != nullptr)
            {
                if (transformComponent->updateWorldMatrix(_currentMatrix))
                {
                    if (_scene->hasQuadtree())
                    {
                        updateQuadtree(entity);
                    }

                    if (_scene->hasOctree())
                    {
                        updateOctree(entity);
                    }
                }
            }
            entity->setDirtyHierarchy(false);
            return true;
        }

        return false;
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
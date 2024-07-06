// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/traversal/iEntityTransformTraverser.h>

#include <igor/entities/components/iTransformComponent.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    void iEntityTransformTraverser::preTraverse()
    {
        _currentMatrix.identity();
    }

    bool iEntityTransformTraverser::preOrderVisit(iEntityPtr entity)
    {
        auto transform = entity->getComponent<iTransformComponent>();

        if (transform != nullptr)
        {
            _matrixStack.push_back(_currentMatrix);
        }

        if (entity->isHierarchyDirty())
        {
            if (transform != nullptr)
            {
                transform->updateWorldMatrix(_currentMatrix);
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
// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/traversal/iEntityTraverser.h>

#include <igor/scene/nodes/iNode.h>
#include <iaux/system/iaConsole.h>

namespace igor
{

    void iEntityTraverser::traverseInternal(iEntityPtr entity, bool useInactive)
    {
        if(entity->isRoot() || preOrderVisit(entity))
        {
            const auto children = entity->getChildren(); // making copy on purpose here

            for (const auto child : children)
            {
                traverseInternal(child, useInactive);
            }

            if (useInactive)
            {
                const auto inactiveChildren = entity->getInactiveChildren(); // making copy on purpose here

                for (const auto child : inactiveChildren)
                {
                    traverseInternal(child, useInactive);
                }
            }
        }

        if(!entity->isRoot())
        {
            postOrderVisit(entity);
        }
    }

    iEntityScenePtr iEntityTraverser::getScene() const
    {
        return _scene;
    }

    void iEntityTraverser::traverse(iEntityPtr entity)
    {
        if(entity == nullptr)
        {
            return;
        }

        _scene = entity->getScene();

        preTraverse();
        traverseInternal(entity, !_ignoreInactive);
        postTraverse();
    }

    void iEntityTraverser::traverse(iEntityScenePtr scene)
    {
        if(scene == nullptr)
        {
            return;
        }

        _scene = scene;

        preTraverse();
        traverseInternal(_scene->_root, !_ignoreInactive);
        postTraverse();
    }

    void iEntityTraverser::setIgnoreInactive(bool ignoreInactive)
    {
        _ignoreInactive = ignoreInactive;
    }

    bool iEntityTraverser::isIgnoringInactive() const
    {
        return _ignoreInactive;
    }

} // namespace igor
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

                for (const auto child : children)
                {
                    traverseInternal(child, useInactive);
                }
            }
        }
        postOrderVisit(entity);
    }

    void iEntityTraverser::traverse(iEntityScenePtr scene)
    {
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
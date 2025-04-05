// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/traversal/iEntityCopyTraverser.h>

#include <igor/entities/iEntitySystemModule.h>

namespace igor
{

    iEntityCopyTraverser::iEntityCopyTraverser(const iEntityPtr &targetEntity, bool copyIDs)
        : _targetEntity(targetEntity), _copyIDs(copyIDs)
    {
        setIgnoreInactive(false);
    }

    void iEntityCopyTraverser::preTraverse()
    {
        _targetScene = _targetEntity->getScene();
        _currentEntity = _targetEntity;

        if(getScene()->hasOctree() && !_targetScene->hasOctree())
        {
            auto &octree = getScene()->getOctree();
            _targetScene->initializeOctree(octree.getVolume(), octree.getSplitThreshold(), octree.getMaxDepth());
        }

        if(getScene()->hasQuadtree() && !_targetScene->hasQuadtree())
        {
            auto &quadtree = getScene()->getQuadtree();
            _targetScene->initializeQuadtree(quadtree.getArea(), quadtree.getSplitThreshold(), quadtree.getMaxDepth());
        }        
    }

    bool iEntityCopyTraverser::preOrderVisit(iEntityPtr entity)
    {
        _entityStack.push_back(_currentEntity);

        iEntityPtr copy = _targetScene->createEntity(entity, _copyIDs);
        copy->setParent(_currentEntity);

        _currentEntity = copy;
        return true;
    }

    void iEntityCopyTraverser::postOrderVisit(iEntityPtr entity)
    {
        _currentEntity = _entityStack.back();
        _entityStack.pop_back();
    }

    void iEntityCopyTraverser::postTraverse()
    {
        con_assert(_entityStack.size() == 0, "item stack should be empty");

        _targetEntity->setDirtyHierarchy(true);
    }

} // namespace igor
// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/traversal/iNodeVisitorSearchName.h>
#include <igor/scene/nodes/iNode.h>

namespace igor
{

    iNodeVisitorSearchName::iNodeVisitorSearchName()
    {
        setTraverseInactiveChildren(true);
    }

    const std::vector<uint64>& iNodeVisitorSearchName::find(iNodePtr node, const iaString& name)
    {
        _name = name;
        traverseTree(node);
        return _nodeIDs;
    }

    bool iNodeVisitorSearchName::preOrderVisit(iNodePtr node, iNodePtr nextSibling)
    {
        if (node->getName() == _name)
        {
            _nodeIDs.push_back(node->getID());
        }

        return true;
    }

    void iNodeVisitorSearchName::preTraverse()
    {
        _nodeIDs.clear();
    }

    void iNodeVisitorSearchName::postOrderVisit(iNodePtr node)
    {
        // does nothing
    }

    void iNodeVisitorSearchName::postTraverse()
    {
        // does nothing
    }
}
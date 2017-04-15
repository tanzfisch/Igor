// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeVisitor.h>

#include <iNode.h>
#include <iaConsole.h>

namespace Igor
{

    void iNodeVisitor::traverseTreeInternal(iNode* node)
    {
        if (preOrderVisit(node))
        {
            vector<iNode*>& children = node->getChildren();

            auto iter = children.begin();
            while (iter != children.end())
            {
                traverseTreeInternal((*iter));
                iter++;
            }

            if (_traverseInactiveChildren)
            {
                vector<iNode*>& inactiveChildren = node->getInactiveChildren();

                auto iter = inactiveChildren.begin();
                while (iter != inactiveChildren.end())
                {
                    traverseTreeInternal((*iter));
                    iter++;
                }
            }
        }
        postOrderVisit(node);
    }

	void iNodeVisitor::traverseTree(iNode* node)
	{
        preTraverse();
        traverseTreeInternal(node);
        postTraverse();
	}

    void iNodeVisitor::setTraverseInactiveChildren(bool inactive)
    {
        _traverseInactiveChildren = inactive;
    }

}
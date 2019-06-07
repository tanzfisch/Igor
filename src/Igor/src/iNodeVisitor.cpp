// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeVisitor.h>

#include <iNode.h>
#include <iaConsole.h>

namespace Igor
{

	void iNodeVisitor::traverseTreeInternal(iNodePtr node, iNodePtr nextSibling)
	{
		if (preOrderVisit(node, nextSibling))
		{
			vector<iNodePtr>& children = node->getChildren();

			if (_traverseInactiveChildren)
			{
				vector<iNodePtr>& inactiveChildren = node->getInactiveChildren();

				children.insert(children.end(), inactiveChildren.begin(), inactiveChildren.end());
			}

			for (size_t i = 0; i < children.size(); ++i)
			{
				traverseTreeInternal(children[i], (i + 1 < children.size()) ? children[i + 1] : nullptr);
			}
		}
		postOrderVisit(node);
	}

	void iNodeVisitor::traverseTree(iNodePtr node)
	{
		preTraverse();
		traverseTreeInternal(node, nullptr);
		postTraverse();
	}

	void iNodeVisitor::setTraverseInactiveChildren(bool inactive)
	{
		_traverseInactiveChildren = inactive;
	}

}
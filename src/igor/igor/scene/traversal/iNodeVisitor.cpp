// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/traversal/iNodeVisitor.h>

#include <igor/scene/nodes/iNode.h>
#include <iaux/system/iaConsole.h>

namespace igor
{

	void iNodeVisitor::traverseTreeInternal(iNodePtr node, iNodePtr nextSibling)
	{
		if (preOrderVisit(node, nextSibling))
		{
			std::vector<iNodePtr> &children = node->getChildren();

			if (_traverseInactiveChildren)
			{
				std::vector<iNodePtr> &inactiveChildren = node->getInactiveChildren();

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

} // namespace igor
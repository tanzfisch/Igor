// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeVisitorPrintTree.h>
#include <iNode.h>
#include <iScene.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

	iNodeVisitorPrintTree::iNodeVisitorPrintTree()
	{
		setTraverseInactiveChildren();
	}

	void iNodeVisitorPrintTree::printToConsole(iNodePtr node)
	{
		traverseTree(node);
		con_endl(_stream.str() << "\ntotal nodes: " << _nodeCount << "\n" << "scene: " << node->getScene()->getName().getData());
	}

	bool iNodeVisitorPrintTree::preOrderVisit(iNodePtr node, iNodePtr nextSibling)
	{
		if (_nodeCount == 0)
		{			
			_stream << " * \"" << node->getName().getData() << "\" id:" << node->getID() << " (" << (node->isActive() ? "active" : "inactive") << ")\n";
			_stream << " | type:" << iNode::getTypeName(node->getType()).getData() << " kind:" << iNode::getKindName(node->getKind()).getData() << "\n";
			_nodeCount++;
			return true;
		}

		preLine();
		_stream << " |\n";

		// actual data
		preLine();
		_stream << " +-- \"" << node->getName().getData() << "\" id:" << node->getID() << " (" << (node->isActive() ? "active" : "inactive") << ")\n";

		// increase indentation
		if (nextSibling != nullptr)
		{
			_indenttation += L" |";
		}
		else
		{
			_indenttation += L"  ";
		}

		// second line actual data
		preLine();
		_stream << (node->hasChildren() ? " | " : "   ") << "type:" << iNode::getTypeName(node->getType()).getData() << " kind:" << iNode::getKindName(node->getKind()).getData() << "\n";

		// optional custom data

		iaString customInfo = node->getCustomInfo();

		if (!customInfo.isEmpty())
		{
			preLine();
			_stream << (node->hasChildren() ? " | " : "   ") << customInfo.getData() << "\n";
		}

		_nodeCount++;
		return true;
	}

	void iNodeVisitorPrintTree::preLine()
	{
		// extra line
		if (_indenttation.getData() != nullptr)
		{
			_stream << _indenttation.getData();
		}
	}

	void iNodeVisitorPrintTree::postOrderVisit(iNodePtr node)
	{
		// reduce indentation
		_indenttation = _indenttation.getSubString(0, _indenttation.getSize() - 2);
	}

	void iNodeVisitorPrintTree::preTraverse()
	{
		_nodeCount = 0;
		_stream << "\n\n";
	}

	void iNodeVisitorPrintTree::postTraverse()
	{
		// nothing to do
	}
}
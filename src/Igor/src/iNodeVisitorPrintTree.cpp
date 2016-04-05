// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeVisitorPrintTree.h>
#include <iNode.h>

#include <IgorAux.h>
#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

		iNodeVisitorPrintTree::iNodeVisitorPrintTree()
		{
            setTraverseInactiveChildren();
		}

		void iNodeVisitorPrintTree::printToConsole(iNode* node)
		{
			traverseTree(node);
			con_endl(_stream.str());
		}
		
		bool iNodeVisitorPrintTree::preOrderVisit(iNode* node)
		{
			iaString type(typeid(*node).name());
			type = type.getSubString(type.findLastOf(':')+1, type.getSize()-1);
            _stream << _indenttation.getData() << type.getData() << ": " << node->getName().getData();
            if (node->isActive())
            {
                _stream << " (active)";
            }
            else
            {
                _stream << " (inactive)";
            }
            _stream << "\n";
			_indenttation += L"    ";
            _nodeCount++;
			return true;
		}

		void iNodeVisitorPrintTree::postOrderVisit(iNode* node)
		{
			_indenttation = _indenttation.getSubString(0, _indenttation.getSize() - 4);
		}

		void iNodeVisitorPrintTree::preTraverse()
		{
            _nodeCount = 0;
		}

		void iNodeVisitorPrintTree::postTraverse()
		{
            _stream << "node count: " << _nodeCount << "\n";
		}
}
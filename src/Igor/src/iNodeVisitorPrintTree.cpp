// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
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

    void iNodeVisitorPrintTree::printToConsole(iNodePtr node)
    {
        traverseTree(node);
        con_endl(_stream.str());
    }

    bool iNodeVisitorPrintTree::preOrderVisit(iNodePtr node)
    {
        _stream << _indenttation.getData() << node->getInfo().getData() << "\n";
        _indenttation += L"    ";
        _nodeCount++;
        return true;
    }

    void iNodeVisitorPrintTree::postOrderVisit(iNodePtr node)
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
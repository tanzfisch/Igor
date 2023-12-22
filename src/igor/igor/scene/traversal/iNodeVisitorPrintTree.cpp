// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/traversal/iNodeVisitorPrintTree.h>
#include <igor/scene/nodes/iNode.h>
#include <igor/scene/iScene.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iNodeVisitorPrintTree::iNodeVisitorPrintTree()
    {
        setTraverseInactiveChildren();
        setTraverseSiblings();
    }

    void iNodeVisitorPrintTree::printToConsole(iNodePtr node)
    {
        if (node == nullptr)
        {
            return;
        }

        traverseTree(node);
        con_endl(_stream.str() << "\ntotal nodes: " << _nodeCount << "\n"
                               << "scene: " << node->getScene()->getName().getData());
    }

    bool iNodeVisitorPrintTree::preOrderVisit(iNodePtr node, iNodePtr nextSibling)
    {
        std::vector<iaString> info = node->getInfo();

        if (info.size() < 2)
        {
            con_err("invalid info data");
            return false;
        }

        if (_nodeCount == 0)
        {
            _stream << " * " << info[0] << "\n";
            _stream << " | " << info[1] << "\n";
            _nodeCount++;
            return true;
        }

        preLine();
        _stream << " |\n";

        // actual data
        preLine();
        _stream << " +-- " << info[0] << "\n";

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
        _stream << (node->hasChildren() ? " | " : "   ") << info[1] << "\n";

        // optional custom data
        if (info.size() > 2)
        {
            for (int i = 2; i < info.size(); ++i)
            {
                preLine();
                _stream << (node->hasChildren() ? " | " : "   ") << info[i] << "\n";
            }
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
        _indenttation = _indenttation.getSubString(0, _indenttation.getLength() - 2);
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
} // namespace igor
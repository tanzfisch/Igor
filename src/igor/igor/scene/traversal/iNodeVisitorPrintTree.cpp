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

    iNodeVisitorPrintTree::iNodeVisitorPrintTree(bool brief)
        : _brief(brief)
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
        std::vector<iaString> info = node->getInfo(_brief);

        if (info.empty())
        {
            con_err("invalid info data");
            return false;
        }

        if (_nodeCount == 0)
        {
            _stream << " * " << info[0] << "\n";
            _nodeCount++;
            return true;
        }

        if (!_brief)
        {
            preLine();
            _stream << " |\n";
        }

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

        // optional custom data
        for (int i = 1; i < info.size(); ++i)
        {
            preLine();
            _stream << (node->hasChildren() ? " | " : "   ") << info[i] << "\n";
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
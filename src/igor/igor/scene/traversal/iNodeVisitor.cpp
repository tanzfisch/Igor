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
            const auto &children = node->getChildren();

            for (size_t i = 0; i < children.size(); ++i)
            {
                traverseTreeInternal(children[i], (i + 1 < children.size()) ? children[i + 1] : nullptr);
            }
        }
        postOrderVisit(node);
    }

    void iNodeVisitor::traverseTreeInternalWithInactive(iNodePtr node, iNodePtr nextSibling)
    {
        if (preOrderVisit(node, nextSibling))
        {
            const auto &children = node->getChildren();
            const auto &inactiveChildren = node->getInactiveChildren();

            for (size_t i = 0; i < children.size(); ++i)
            {
                traverseTreeInternalWithInactive(children[i], (i + 1 < children.size()) ? children[i + 1] : nullptr);
            }

            for (size_t i = 0; i < inactiveChildren.size(); ++i)
            {
                traverseTreeInternalWithInactive(inactiveChildren[i], (i + 1 < inactiveChildren.size()) ? inactiveChildren[i + 1] : nullptr);
            }
        }
        postOrderVisit(node);
    }

    void iNodeVisitor::traverseTreeInternal(iNodePtr node)
    {
        if (preOrderVisit(node, nullptr))
        {
            for (auto child : node->getChildren())
            {
                traverseTreeInternal(child);
            }
        }
        postOrderVisit(node);
    }

    void iNodeVisitor::traverseTreeInternalWithInactive(iNodePtr node)
    {
        if (preOrderVisit(node, nullptr))
        {
            for (auto child : node->getChildren())
            {
                traverseTreeInternal(child);
            }

            for (auto child : node->getInactiveChildren())
            {
                traverseTreeInternal(child);
            }
        }
        postOrderVisit(node);
    }

    void iNodeVisitor::traverseTree(iNodePtr node)
    {
        preTraverse();

        if (_traverseInactiveChildren)
        {
            if (_traverseSiblings)
            {
                traverseTreeInternalWithInactive(node, nullptr);
            }
            else
            {
                traverseTreeInternalWithInactive(node);
            }
        }
        else
        {
            if (_traverseSiblings)
            {
                traverseTreeInternal(node, nullptr);
            }
            else
            {
                traverseTreeInternal(node);
            }
        }

        postTraverse();
    }

    void iNodeVisitor::setTraverseInactiveChildren(bool inactive)
    {
        _traverseInactiveChildren = inactive;
    }

    void iNodeVisitor::setTraverseSiblings(bool traverseSiblings)
    {
        _traverseSiblings = traverseSiblings;
    }

} // namespace igor
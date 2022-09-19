// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/data/iQuadtree.h>

#include <igor/data/iIntersection.h>

#include <algorithm>

namespace igor
{

    static const uint32 maxUserDataPerNode = 4;

    iQuadtree::iQuadtree(const iaRectangled &box)
    {
        _root = iQuadtreeNodePtr(new iQuadtreeNode());
        _root->_box = box;
    }

    iQuadtree::~iQuadtree()
    {
    }

    void iQuadtree::update(const iQuadtreeUserDataPtr userData, const iaVector2d &newPosition)
    {
        if (iIntersection::intersects(newPosition, userData->_parent->_box))
        {
            userData->_circle._center = newPosition;
        }
        else
        {
            remove(userData);
            userData->_circle._center = newPosition;
            insertInternal(_root, userData);
        }
    }

    void iQuadtree::insert(const iQuadtreeUserDataPtr userData)
    {
        if (userData->_parent != nullptr)
        {
            return;
        }

        if (!iIntersection::intersects(userData->getCircle().getCenter(), _root->_box))
        {
            con_err("position out of bounds ");
            return;
        }

        insertInternal(_root, userData);
    }

    void iQuadtree::split(const iQuadtreeNodePtr &node)
    {
        const iaRectangled &nodeBox = node->_box;
        const iaVector2d center = nodeBox.getCenter();

        node->_children[0] = iQuadtreeNodePtr(new iQuadtreeNode());
        node->_children[0]->_box = iaRectangled(nodeBox._x, nodeBox._y, nodeBox._width * 0.5, nodeBox._height * 0.5);
        node->_children[0]->_parent = node;

        node->_children[1] = iQuadtreeNodePtr(new iQuadtreeNode());
        node->_children[1]->_box = iaRectangled(nodeBox._x + nodeBox._width * 0.5, nodeBox._y, nodeBox._width * 0.5, nodeBox._height * 0.5);
        node->_children[1]->_parent = node;

        node->_children[2] = iQuadtreeNodePtr(new iQuadtreeNode());
        node->_children[2]->_box = iaRectangled(nodeBox._x, nodeBox._y + nodeBox._height * 0.5, nodeBox._width * 0.5, nodeBox._height * 0.5);
        node->_children[2]->_parent = node;

        node->_children[3] = iQuadtreeNodePtr(new iQuadtreeNode());
        node->_children[3]->_box = iaRectangled(nodeBox._x + nodeBox._width * 0.5, nodeBox._y + nodeBox._height * 0.5, nodeBox._width * 0.5, nodeBox._height * 0.5);
        node->_children[3]->_parent = node;

        for (const iQuadtreeUserDataPtr userData : node->_userData)
        {
            uint32 childIndex = 0;

            if (userData->getCircle().getX() > center._x)
            {
                childIndex |= 1;
            }

            if (userData->getCircle().getY() > center._y)
            {
                childIndex |= 2;
            }

            node->_children[childIndex]->_userData.push_back(userData);
            userData->_parent = node->_children[childIndex];
        }

        node->_userData.clear();
    }

    void iQuadtree::insertInternal(const iQuadtreeNodePtr &node, const iQuadtreeUserDataPtr userData)
    {
        // check if node has children and follow that branch
        if (!isLeaf(node))
        {
            const iaRectangled &nodeBox = node->_box;
            const iaVector2d center = nodeBox.getCenter();
            uint32 childIndex = 0;

            const iaVector2d &pos = userData->getCircle().getCenter();

            if (pos._x > center._x)
            {
                childIndex |= 1;
            }

            if (pos._y > center._y)
            {
                childIndex |= 2;
            }

            insertInternal(node->_children[childIndex], userData);
            return;
        }

        // we reached a leaf node. insert data
        if (node->_userData.size() < maxUserDataPerNode)
        {
            node->_userData.push_back(userData);
            userData->_parent = std::move(node);
            return;
        }

        // leaf node was too full. Split node and try again
        split(node);
        insertInternal(node, userData);
    }

    const iQuadtreeNodePtr &iQuadtree::getRoot() const
    {
        return _root;
    }

    void iQuadtree::remove(const iQuadtreeUserDataPtr userData)
    {
        iQuadtreeNodePtr parent = userData->_parent;

        auto iter = std::find(parent->_userData.begin(), parent->_userData.end(), userData);
        if (iter != parent->_userData.end())
        {
            (*iter)->_parent = nullptr;
            parent->_userData.erase(iter);
        }

        bool merged = true;
        while(merged && parent)
        {
            merged = tryMerge(parent);
            parent = parent->_parent;
        }
    }

    bool iQuadtree::isLeaf(const iQuadtreeNodePtr &node) const
    {
        return node->_children[0] == nullptr;
    }

    bool iQuadtree::tryMerge(const iQuadtreeNodePtr &node)
    {
        if(isLeaf(node))
        {
            return;
        }

        for (int i = 0; i < 4; ++i)
        {
            if (!isLeaf(node->_children[i]))
            {
                return false;
            }

            if (!node->_children[i]->_userData.empty())
            {
                return false;
            }
        }

        for (int i = 0; i < 4; ++i)
        {
            node->_children[i]->_parent = nullptr;
            node->_children[i] = nullptr;
        }

        return true;
    }

    void iQuadtree::clear()
    {
        const iaRectangled box = _root->_box;
        _root = iQuadtreeNodePtr(new iQuadtreeNode());
        _root->_box = box;
    }

} // namespace igor

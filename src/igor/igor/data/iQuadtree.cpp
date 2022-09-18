// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/data/iQuadtree.h>

#include <igor/data/iIntersection.h>

namespace igor
{

    iQuadtree::iQuadtree(const iRectangled &box)
    {
        _root = std::unique_ptr<iQuadtreeNode>(new iQuadtreeNode());
        _root->_box = box;
    }

    iQuadtree::~iQuadtree()
    {
    }

    void iQuadtree::insert(iUserData userData, const iaVector2d &pos)
    {
        if (!iIntersection::intersects(pos, _root->_box))
        {
            con_err("position out of bounds " << pos);
            return;
        }

        insertInternal(_root, userData, pos);
    }

    void iQuadtree::split(const std::unique_ptr<iQuadtreeNode> &node)
    {
        const iRectangled &nodeBox = node->_box;
        const iaVector2d center = nodeBox.getCenter();

        node->_children[0] = std::unique_ptr<iQuadtreeNode>(new iQuadtreeNode());
        node->_children[0]->_box = iRectangled(nodeBox._x, nodeBox._y, nodeBox._width * 0.5, nodeBox._height * 0.5);

        node->_children[1] = std::unique_ptr<iQuadtreeNode>(new iQuadtreeNode());
        node->_children[1]->_box = iRectangled(nodeBox._x + nodeBox._width * 0.5, nodeBox._y, nodeBox._width * 0.5, nodeBox._height * 0.5);

        node->_children[2] = std::unique_ptr<iQuadtreeNode>(new iQuadtreeNode());
        node->_children[2]->_box = iRectangled(nodeBox._x, nodeBox._y + nodeBox._height * 0.5, nodeBox._width * 0.5, nodeBox._height * 0.5);

        node->_children[3] = std::unique_ptr<iQuadtreeNode>(new iQuadtreeNode());
        node->_children[3]->_box = iRectangled(nodeBox._x + nodeBox._width * 0.5, nodeBox._y + nodeBox._height * 0.5, nodeBox._width * 0.5, nodeBox._height * 0.5);

        for (const auto &userDataSet : node->_userData)
        {
            uint32 childIndex = 0;

            if (userDataSet._pos._x > center._x)
            {
                childIndex |= 1;
            }

            if (userDataSet._pos._y > center._y)
            {
                childIndex |= 2;
            }

            node->_children[childIndex]->_userData.push_back(userDataSet);
        }

        node->_userData.clear();
    }

    void iQuadtree::insertInternal(const std::unique_ptr<iQuadtreeNode> &node, iUserData userData, const iaVector2d &pos)
    {
        // check if node has children and follow that branch
        if (node->_children[0] != nullptr)
        {
            const iRectangled &nodeBox = node->_box;
            const iaVector2d center = nodeBox.getCenter();
            uint32 childIndex = 0;

            if (pos._x > center._x)
            {
                childIndex |= 1;
            }

            if (pos._y > center._y)
            {
                childIndex |= 2;
            }

            insertInternal(node->_children[childIndex], userData, pos);
        }

        // we reached a leaf node. insert data
        if (node->_userData.size() < 4)
        {
            node->_userData.push_back(iQuadtreeUserDataSet(pos, userData));
            return;
        }

        // leaf node was too full. Split node and try again
        split(node);
        insertInternal(node, userData, pos);
    }

    const std::unique_ptr<iQuadtreeNode>& iQuadtree::getRoot() const
    {
        return _root;
    }

} // namespace igor

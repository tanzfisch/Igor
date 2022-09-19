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
        _root = std::shared_ptr<iQuadtreeNode>(new iQuadtreeNode());
        _root->_box = box;
    }

    iQuadtree::~iQuadtree()
    {
    }

    void iQuadtree::update(const iQuadtreeUserDataPtr userData, const iaVector2d &newPosition)
    {
        std::vector<std::shared_ptr<iQuadtreeNode>> srcPath;
        srcPath.push_back(_root);
        getNodePath(srcPath, userData->_circle.getCenter());

        if (iIntersection::intersects(newPosition, srcPath.back()->_box))
        {
            userData->_circle._center = newPosition;
        }
        else
        {
            removeInternal(_root, userData);
            userData->_circle._center = newPosition;
            insertInternal(_root, userData);
        }
    }

    void iQuadtree::getNodePath(std::vector<std::shared_ptr<iQuadtreeNode>> &path, const iaVector2d &pos)
    {
        const std::shared_ptr<iQuadtreeNode> &node = path.back();

        if (!isLeaf(node))
        {
            uint32 childIndex = 0;
            const iaRectangled &nodeBox = node->_box;
            const iaVector2d center = nodeBox.getCenter();

            if (pos._x > center._x)
            {
                childIndex |= 1;
            }

            if (pos._y > center._y)
            {
                childIndex |= 2;
            }

            path.push_back(node->_children[childIndex]);
            getNodePath(path, pos);
        }
    }

    void iQuadtree::insert(const iQuadtreeUserDataPtr userData)
    {
        if (!iIntersection::intersects(userData->getCircle().getCenter(), _root->_box))
        {
            con_err("position out of bounds ");
            return;
        }

        insertInternal(_root, userData);
    }

    void iQuadtree::split(const std::shared_ptr<iQuadtreeNode> &node)
    {
        const iaRectangled &nodeBox = node->_box;
        const iaVector2d center = nodeBox.getCenter();

        node->_children[0] = std::shared_ptr<iQuadtreeNode>(new iQuadtreeNode());
        node->_children[0]->_box = iaRectangled(nodeBox._x, nodeBox._y, nodeBox._width * 0.5, nodeBox._height * 0.5);

        node->_children[1] = std::shared_ptr<iQuadtreeNode>(new iQuadtreeNode());
        node->_children[1]->_box = iaRectangled(nodeBox._x + nodeBox._width * 0.5, nodeBox._y, nodeBox._width * 0.5, nodeBox._height * 0.5);

        node->_children[2] = std::shared_ptr<iQuadtreeNode>(new iQuadtreeNode());
        node->_children[2]->_box = iaRectangled(nodeBox._x, nodeBox._y + nodeBox._height * 0.5, nodeBox._width * 0.5, nodeBox._height * 0.5);

        node->_children[3] = std::shared_ptr<iQuadtreeNode>(new iQuadtreeNode());
        node->_children[3]->_box = iaRectangled(nodeBox._x + nodeBox._width * 0.5, nodeBox._y + nodeBox._height * 0.5, nodeBox._width * 0.5, nodeBox._height * 0.5);

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
        }

        node->_userData.clear();
    }

    void iQuadtree::insertInternal(const std::shared_ptr<iQuadtreeNode> &node, const iQuadtreeUserDataPtr userData)
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
            return;
        }

        // leaf node was too full. Split node and try again
        split(node);
        insertInternal(node, userData);
    }

    const std::shared_ptr<iQuadtreeNode> &iQuadtree::getRoot() const
    {
        return _root;
    }

    void iQuadtree::remove(const iQuadtreeUserDataPtr userData)
    {
        removeInternal(_root, userData);
    }

    bool iQuadtree::isLeaf(const std::shared_ptr<iQuadtreeNode> &node) const
    {
        return node->_children[0] == nullptr;
    }

    bool iQuadtree::removeInternal(const std::shared_ptr<iQuadtreeNode> &node, const iQuadtreeUserDataPtr userData)
    {
        if (isLeaf(node))
        {
            auto iter = std::find(node->_userData.begin(), node->_userData.end(), userData);
            if (iter != node->_userData.end())
            {
                node->_userData.erase(iter);
            }
            return true;
        }
        else
        {
            uint32 childIndex = 0;
            const iaRectangled &nodeBox = node->_box;
            const iaVector2d center = nodeBox.getCenter();
            const iaVector2d &pos = userData->getCircle().getCenter();

            if (pos._x > center._x)
            {
                childIndex |= 1;
            }

            if (pos._y > center._y)
            {
                childIndex |= 2;
            }

            if (removeInternal(node->_children[childIndex], userData))
            {
                return tryMerge(node);
            }

            return false;
        }
    }

    bool iQuadtree::tryMerge(const std::shared_ptr<iQuadtreeNode> &node)
    {
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
            node->_children[i] = nullptr;
        }

        return true;
    }

    void iQuadtree::clear()
    {
        const iaRectangled box = _root->_box;
        _root = std::shared_ptr<iQuadtreeNode>(new iQuadtreeNode());
        _root->_box = box;
    }

} // namespace igor

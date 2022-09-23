// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/data/iQuadtree.h>

#include <igor/data/iIntersection.h>

#include <algorithm>

namespace igor
{

    iQuadtree::iQuadtree(const iaRectangled &box, const uint32 splitThreshold, const uint32 maxDepth)
        : _splitThreshold(splitThreshold), _maxDepth(maxDepth)
    {
        _root = iQuadtreeNodePtr(new iQuadtreeNode());
        _root->_box = box;
    }

    iQuadtree::~iQuadtree()
    {
        clear();
    }

    void iQuadtree::query(const iaCircled &circle, iQuadtreeObjects &objects)
    {
        if (!iIntersection::intersects(circle, _root->_box))
        {
            return;
        }

        queryInternal(_root, circle, objects);
    }

    void iQuadtree::queryInternal(const iQuadtreeNodePtr &node, const iaCircled &circle, iQuadtreeObjects &objects)
    {
        if (isLeaf(node))
        {
            for (auto ud : node->_objects)
            {
                if (iIntersection::intersects(ud->_circle, circle))
                {
                    objects.push_back(ud);
                }
            }
        }
        else
        {
            for (int i = 0; i < 4; ++i)
            {
                const iQuadtreeNodePtr &child = node->_children[i];
                if (iIntersection::intersects(circle, child->_box))
                {
                    queryInternal(child, circle, objects);
                }
            }
        }
    }

    void iQuadtree::update(const iQuadtreeObjectPtr object, const iaVector2d &newPosition)
    {
        if (iIntersection::intersects(newPosition, object->_parent->_box))
        {
            object->_circle._center = newPosition;
        }
        else
        {
            remove(object);
            object->_circle._center = newPosition;
            uint32 depth = 0;
            insertInternal(_root, object, depth);
        }
    }

    void iQuadtree::insert(const iQuadtreeObjectPtr object)
    {
        if (object->_parent != nullptr)
        {
            return;
        }

        if (!iIntersection::intersects(object->_circle._center, _root->_box))
        {
            con_err("position out of bounds ");
            return;
        }

        uint32 depth = 0;
        insertInternal(_root, object, depth);
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

        for (const iQuadtreeObjectPtr object : node->_objects)
        {
            uint32 childIndex = 0;

            if (object->_circle.getX() > center._x)
            {
                childIndex |= 1;
            }

            if (object->_circle.getY() > center._y)
            {
                childIndex |= 2;
            }

            node->_children[childIndex]->_objects.push_back(object);
            object->_parent = node->_children[childIndex];
        }

        node->_objects.clear();
    }

    void iQuadtree::insertInternal(const iQuadtreeNodePtr &node, const iQuadtreeObjectPtr object, uint32 &depth)
    {
        // check if node has children and follow that branch
        if (!isLeaf(node))
        {
            const iaRectangled &nodeBox = node->_box;
            const iaVector2d center = nodeBox.getCenter();
            uint32 childIndex = 0;

            const iaVector2d &pos = object->_circle._center;

            if (pos._x > center._x)
            {
                childIndex |= 1;
            }

            if (pos._y > center._y)
            {
                childIndex |= 2;
            }

            insertInternal(node->_children[childIndex], object, ++depth);
            return;
        }

        // we reached a leaf node. insert data if not full yet
        if (depth >= _maxDepth || node->_objects.size() < _splitThreshold)
        {
            node->_objects.push_back(object);
            object->_parent = std::move(node);
            return;
        }

        // leaf node was too full. Split node and try again
        split(node);
        insertInternal(node, object, ++depth);
    }

    const iQuadtreeNodePtr &iQuadtree::getRoot() const
    {
        return _root;
    }

    void iQuadtree::remove(const iQuadtreeObjectPtr object)
    {
        iQuadtreeNodePtr parent = object->_parent;

        auto iter = std::find(parent->_objects.begin(), parent->_objects.end(), object);
        if (iter != parent->_objects.end())
        {
            (*iter)->_parent = nullptr;
            parent->_objects.erase(iter);
        }

        bool merged = true;
        while (merged && parent)
        {
            parent = parent->_parent;
            merged = tryMerge(parent);
        }
    }

    bool iQuadtree::isLeaf(const iQuadtreeNodePtr &node) const
    {
        return node->_children[0] == nullptr;
    }

    bool iQuadtree::tryMerge(const iQuadtreeNodePtr &node)
    {
        if (isLeaf(node))
        {
            return false;
        }

        for (int i = 0; i < 4; ++i)
        {
            if (!isLeaf(node->_children[i]))
            {
                return false;
            }

            if (!node->_children[i]->_objects.empty())
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

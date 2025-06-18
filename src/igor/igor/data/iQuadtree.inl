// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

template <typename F>
iQuadtree<F>::iQuadtree(const iaRectangle<F> &box, const uint32 splitThreshold, const uint32 maxDepth)
    : _splitThreshold(splitThreshold), _maxDepth(maxDepth)
{
    _root = std::make_shared<iQuadtreeNode>();
    _root->_box = box;
}

template <typename F>
void iQuadtree<F>::query(const iaCircle<F> &circle, std::vector<std::shared_ptr<iQuadtreeObject>> &objects)
{
    if (!iIntersection::intersects(circle, _root->_box))
    {
        return;
    }

    queryInternal(_root, circle, objects);
}

template <typename F>
void iQuadtree<F>::query(const iaRectangle<F> &rectangle, std::vector<std::shared_ptr<iQuadtreeObject>> &objects)
{
    if (!iIntersection::intersects(rectangle, _root->_box))
    {
        return;
    }

    queryInternal(_root, rectangle, objects);
}

template <typename F>
void iQuadtree<F>::queryInternal(const std::shared_ptr<iQuadtreeNode> &node, const iaRectangle<F> &rectangle, std::vector<std::shared_ptr<iQuadtreeObject>> &objects)
{
    if (isLeaf(node))
    {
        for (auto ud : node->_objects)
        {
            if (iIntersection::intersects(ud->_circle, rectangle))
            {
                objects.push_back(ud);
            }
        }
    }
    else
    {
        for (int i = 0; i < 4; ++i)
        {
            const std::shared_ptr<iQuadtreeNode> &child = node->_children[i];
            if (iIntersection::intersects(rectangle, child->_box))
            {
                queryInternal(child, rectangle, objects);
            }
        }
    }
}

template <typename F>
void iQuadtree<F>::queryInternal(const std::shared_ptr<iQuadtreeNode> &node, const iaCircle<F> &circle, std::vector<std::shared_ptr<iQuadtreeObject>> &objects)
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
            const std::shared_ptr<iQuadtreeNode> &child = node->_children[i];
            if (iIntersection::intersects(circle, child->_box))
            {
                queryInternal(child, circle, objects);
            }
        }
    }
}

template <typename F>
void iQuadtree<F>::update(const std::shared_ptr<iQuadtreeObject> object, const iaVector2<F> &position)
{
    if (object->_circle._center == position)
    {
        return;
    }

    if (iIntersection::intersects(position, object->_parent.lock()->_box))
    {
        object->_circle._center = position;
    }
    else
    {
        remove(object);
        object->_circle._center = position;
        uint32 depth = 0;
        insertInternal(_root, object, depth);
    }
}

template <typename F>
void iQuadtree<F>::update(const std::shared_ptr<iQuadtreeObject> object, const iaCircle<F> &circle)
{
    if (object->_circle == circle)
    {
        return;
    }

    if (iIntersection::intersects(circle._center, object->_parent.lock()->_box))
    {
        object->_circle = circle;
    }
    else
    {
        remove(object);
        object->_circle = circle;
        uint32 depth = 0;
        insertInternal(_root, object, depth);
    }
}

template <typename F>
void iQuadtree<F>::insert(const std::shared_ptr<iQuadtreeObject> object)
{
    if (object->_parent.lock() != nullptr)
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

template <typename F>
void iQuadtree<F>::split(const std::shared_ptr<iQuadtreeNode> &node)
{
    /*  +----+----+-> X
        | C0 | C1 |
        +----+----+
        | C2 | C3 |
        +----+----+
        |
        v
        y
    */
    const iaRectangle<F> &nodeBox = node->_box;
    const iaVector2<F> center = nodeBox.getCenter();

    for (int i = 0; i < 4; ++i)
    {
        node->_children[i] = std::make_shared<iQuadtreeNode>();
        node->_children[i]->_parent = node;
    }

    node->_children[0]->_box = iaRectangle<F>(nodeBox._x, nodeBox._y, nodeBox._width * 0.5, nodeBox._height * 0.5);
    node->_children[1]->_box = iaRectangle<F>(nodeBox._x + nodeBox._width * 0.5, nodeBox._y, nodeBox._width * 0.5, nodeBox._height * 0.5);
    node->_children[2]->_box = iaRectangle<F>(nodeBox._x, nodeBox._y + nodeBox._height * 0.5, nodeBox._width * 0.5, nodeBox._height * 0.5);
    node->_children[3]->_box = iaRectangle<F>(nodeBox._x + nodeBox._width * 0.5, nodeBox._y + nodeBox._height * 0.5, nodeBox._width * 0.5, nodeBox._height * 0.5);

    for (const std::shared_ptr<iQuadtreeObject> object : node->_objects)
    {
        uint32 childIndex = 0;

        if (object->_circle._center._x > center._x)
        {
            childIndex |= 1;
        }

        if (object->_circle._center._y > center._y)
        {
            childIndex |= 2;
        }

        node->_children[childIndex]->_objects.push_back(object);
        object->_parent = node->_children[childIndex];
    }

    node->_objects.clear();
}

template <typename F>
void iQuadtree<F>::insertInternal(const std::shared_ptr<iQuadtreeNode> &node, const std::shared_ptr<iQuadtreeObject> object, uint32 &depth)
{
    // check if node has children and follow that branch
    if (!isLeaf(node))
    {
        const iaRectangle<F> &nodeBox = node->_box;
        const iaVector2<F> center = nodeBox.getCenter();
        uint32 childIndex = 0;

        const iaVector2<F> &pos = object->_circle._center;

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
        object->_parent = node;
        return;
    }

    // leaf node was too full. Split node and try again
    split(node);
    insertInternal(node, object, ++depth);
}

template <typename F>
const std::shared_ptr<typename iQuadtree<F>::iQuadtreeNode> &iQuadtree<F>::getRoot() const
{
    return _root;
}

template <typename F>
void iQuadtree<F>::remove(const std::shared_ptr<iQuadtreeObject> object)
{
    std::shared_ptr<iQuadtreeNode> parent = object->_parent.lock();

    auto iter = std::find(parent->_objects.begin(), parent->_objects.end(), object);
    if (iter != parent->_objects.end())
    {
        (*iter)->_parent.lock() = nullptr;
        parent->_objects.erase(iter);
    }

    bool merged = true;
    parent = parent->_parent.lock();
    while (merged && parent)
    {
        merged = tryMerge(parent);
        parent = parent->_parent.lock();
    }
}

template <typename F>
bool iQuadtree<F>::isLeaf(const std::shared_ptr<iQuadtreeNode> &node) const
{
    return node->_children[0] == nullptr;
}

template <typename F>
bool iQuadtree<F>::tryMerge(const std::shared_ptr<iQuadtreeNode> &node)
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
        node->_children[i]->_parent.reset();
        node->_children[i] = nullptr;
    }

    return true;
}

template <typename F>
void iQuadtree<F>::clear()
{
    const iaRectangle<F> box = _root->_box;
    _root = std::make_shared<iQuadtreeNode>();
    _root->_box = box;
}

template <typename F>
const iaRectangle<F> &iQuadtree<F>::getArea() const
{
    return _root->_box;
}

template <typename F>
uint32 iQuadtree<F>::getSplitThreshold() const
{
    return _splitThreshold;
}

template <typename F>
uint32 iQuadtree<F>::getMaxDepth() const
{
    return _maxDepth;
}

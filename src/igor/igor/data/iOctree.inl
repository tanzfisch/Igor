// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

template <typename F>
iOctree<F>::iOctree(const iAACube<F> &cube, const uint32 splitThreshold, const uint32 maxDepth)
    : _splitThreshold(splitThreshold), _maxDepth(maxDepth)
{
    _root = std::make_shared<iOctreeNode>();
    _root->_cube = cube;
}

template <typename F>
void iOctree<F>::query(const iaSphere<F> &sphere, std::vector<std::shared_ptr<iOctreeObject>> &objects)
{
    if (!iIntersection::intersects(_root->_cube, sphere))
    {
        return;
    }

    queryInternal(_root, sphere, objects);
}

template <typename F>
void iOctree<F>::query(const iAACube<F> &cube, std::vector<std::shared_ptr<iOctreeObject>> &objects)
{
    if (!iIntersection::intersects(cube, _root->_cube))
    {
        return;
    }

    queryInternal(_root, cube, objects);
}

template <typename F>
void iOctree<F>::query(const iFrustumd &frustum, std::vector<std::shared_ptr<iOctreeObject>> &objects)
{
    if (!iIntersection::intersects(_root->_cube, frustum))
    {
        return;
    }

    queryInternal(_root, frustum, objects);
}

template <typename F>
void iOctree<F>::queryInternal(const std::shared_ptr<iOctreeNode> &node, const iAACube<F> &cube, std::vector<std::shared_ptr<iOctreeObject>> &objects)
{
    if (isLeaf(node))
    {
        for (auto ud : node->_objects)
        {
            if (iIntersection::intersects(cube, ud->_sphere))
            {
                objects.push_back(ud);
            }
        }
    }
    else
    {
        for (int i = 0; i < 8; ++i)
        {
            const std::shared_ptr<iOctreeNode> &child = node->_children[i];
            if (iIntersection::intersects(cube, child->_cube))
            {
                queryInternal(child, cube, objects);
            }
        }
    }
}

template <typename F>
void iOctree<F>::queryInternal(const std::shared_ptr<iOctreeNode> &node, const iaSphere<F> &sphere, std::vector<std::shared_ptr<iOctreeObject>> &objects)
{
    if (isLeaf(node))
    {
        for (auto ud : node->_objects)
        {
            if (iIntersection::intersects(ud->_sphere, sphere))
            {
                objects.push_back(ud);
            }
        }
    }
    else
    {
        for (int i = 0; i < 8; ++i)
        {
            const std::shared_ptr<iOctreeNode> &child = node->_children[i];
            if (iIntersection::intersects(child->_cube, sphere))
            {
                queryInternal(child, sphere, objects);
            }
        }
    }
}

template <typename F>
void iOctree<F>::queryInternal(const std::shared_ptr<iOctreeNode> &node, const iFrustumd &frustum, std::vector<std::shared_ptr<iOctreeObject>> &objects)
{
    if (isLeaf(node))
    {
        for (auto ud : node->_objects)
        {
            if (iIntersection::intersects(ud->_sphere, frustum))
            {
                objects.push_back(ud);
            }
        }
    }
    else
    {
        for (int i = 0; i < 8; ++i)
        {
            const std::shared_ptr<iOctreeNode> &child = node->_children[i];
            if (iIntersection::intersects(child->_cube, frustum))
            {
                queryInternal(child, frustum, objects);
            }
        }
    }
}

template <typename F>
void iOctree<F>::update(const std::shared_ptr<iOctreeObject> object, const iaVector3<F> &position)
{
    if(object->_sphere._center == position)
    {
        return;
    }

    if (iIntersection::intersects(position, object->_parent.lock()->_cube))
    {
        object->_sphere._center = position;
    }
    else
    {
        remove(object);
        object->_sphere._center = position;
        uint32 depth = 0;
        insertInternal(_root, object, depth);
    }
}

template <typename F>
void iOctree<F>::update(const std::shared_ptr<iOctreeObject> object, const iaSphere<F> &sphere)
{
    if(object->_sphere == sphere)
    {
        return;
    }

    if (iIntersection::intersects(sphere._center, object->_parent.lock()->_cube))
    {
        object->_sphere = sphere;
    }
    else
    {
        remove(object);
        object->_sphere = sphere;
        uint32 depth = 0;
        insertInternal(_root, object, depth);
    }
}

template <typename F>
void iOctree<F>::insert(const std::shared_ptr<iOctreeObject> object)
{
    if (object->_parent.lock() != nullptr)
    {
        return;
    }

    if (!iIntersection::intersects(object->_sphere._center, _root->_cube))
    {
        con_err("position out of bounds ");
        return;
    }

    uint32 depth = 0;
    insertInternal(_root, object, depth);
}

template <typename F>
void iOctree<F>::split(const std::shared_ptr<iOctreeNode> &node)
{
    /*
               Z
              +----+----+ -> X
             /| C4 | C5/|
            / +----+--/-+
           /  | C6 | C7 |
          /   +----+/---+
         /   /     /   /
        +----+----+   /
        | C0 | C1 |  /
        +----+----+ /
        | C2 | C3 |/
        +----+----+
        |
        v Y

    */

    const iAACube<F> &nodeCube = node->_cube;
    const F halfHalfLenght = nodeCube._halfEdgeLength * static_cast<F>(0.5);
    const iaVector3<F> center = nodeCube.getCenter();

    for (int i = 0; i < 8; ++i)
    {
        node->_children[i] = std::make_shared<iOctreeNode>();
        node->_children[i]->_parent = node;
    }

    node->_children[0]->_cube = iAACube<F>(iaVector3<F>(center._x - halfHalfLenght, center._y - halfHalfLenght, center._z - halfHalfLenght), halfHalfLenght);
    node->_children[1]->_cube = iAACube<F>(iaVector3<F>(center._x + halfHalfLenght, center._y - halfHalfLenght, center._z - halfHalfLenght), halfHalfLenght);
    node->_children[2]->_cube = iAACube<F>(iaVector3<F>(center._x - halfHalfLenght, center._y + halfHalfLenght, center._z - halfHalfLenght), halfHalfLenght);
    node->_children[3]->_cube = iAACube<F>(iaVector3<F>(center._x + halfHalfLenght, center._y + halfHalfLenght, center._z - halfHalfLenght), halfHalfLenght);
    node->_children[4]->_cube = iAACube<F>(iaVector3<F>(center._x - halfHalfLenght, center._y - halfHalfLenght, center._z + halfHalfLenght), halfHalfLenght);
    node->_children[5]->_cube = iAACube<F>(iaVector3<F>(center._x + halfHalfLenght, center._y - halfHalfLenght, center._z + halfHalfLenght), halfHalfLenght);
    node->_children[6]->_cube = iAACube<F>(iaVector3<F>(center._x - halfHalfLenght, center._y + halfHalfLenght, center._z + halfHalfLenght), halfHalfLenght);
    node->_children[7]->_cube = iAACube<F>(iaVector3<F>(center._x + halfHalfLenght, center._y + halfHalfLenght, center._z + halfHalfLenght), halfHalfLenght);

    for (const std::shared_ptr<iOctreeObject> object : node->_objects)
    {
        uint32 childIndex = 0;

        if (object->_sphere._center._x > center._x)
        {
            childIndex |= 1;
        }

        if (object->_sphere._center._y > center._y)
        {
            childIndex |= 2;
        }

        if (object->_sphere._center._z > center._z)
        {
            childIndex |= 4;
        }

        node->_children[childIndex]->_objects.push_back(object);
        object->_parent = node->_children[childIndex];
    }

    node->_objects.clear();
}

template <typename F>
void iOctree<F>::insertInternal(const std::shared_ptr<iOctreeNode> &node, const std::shared_ptr<iOctreeObject> object, uint32 &depth)
{
    // check if node has children and follow that branch
    if (!isLeaf(node))
    {
        const iAACube<F> &nodeCube = node->_cube;
        const iaVector3<F> center = nodeCube.getCenter();
        uint32 childIndex = 0;

        const iaVector3<F> &pos = object->_sphere._center;

        if (pos._x > center._x)
        {
            childIndex |= 1;
        }

        if (pos._y > center._y)
        {
            childIndex |= 2;
        }

        if (pos._y > center._y)
        {
            childIndex |= 4;
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
const std::shared_ptr<typename iOctree<F>::iOctreeNode> &iOctree<F>::getRoot() const
{
    return _root;
}

template <typename F>
void iOctree<F>::remove(const std::shared_ptr<iOctreeObject> object)
{
    std::shared_ptr<iOctreeNode> parent = object->_parent.lock();

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
bool iOctree<F>::isLeaf(const std::shared_ptr<iOctreeNode> &node) const
{
    return node->_children[0] == nullptr;
}

template <typename F>
bool iOctree<F>::tryMerge(const std::shared_ptr<iOctreeNode> &node)
{
    if (isLeaf(node))
    {
        return false;
    }

    for (int i = 0; i < 8; ++i)
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

    for (int i = 0; i < 8; ++i)
    {
        node->_children[i]->_parent.reset();
        node->_children[i] = nullptr;
    }

    return true;
}

template <typename F>
void iOctree<F>::clear()
{
    const iAACube<F> cube = _root->_cube;
    _root = std::make_shared<iOctreeNode>();
    _root->_cube = cube;
}

template <typename F>
const iAACube<F> &iOctree<F>::getVolume() const
{
    return _root->_cube;
}

template <typename F>
uint32 iOctree<F>::getSplitThreshold() const
{
    return _splitThreshold;
}

template <typename F>
uint32 iOctree<F>::getMaxDepth() const
{
    return _maxDepth;
}

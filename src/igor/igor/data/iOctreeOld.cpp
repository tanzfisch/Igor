// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/data/iOctreeOld.h>

#include <igor/data/iAACube.h>
#include <igor/renderer/iRenderer.h>
#include <igor/data/iIntersection.h>
#include <igor/resources/profiler/iProfiler.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    const iaVector3d iOctreeOld::_splitTable[8] = {iaVector3d(-1.0, -1.0, -1.0),
                                                iaVector3d(1.0, -1.0, -1.0),
                                                iaVector3d(-1.0, 1.0, -1.0),
                                                iaVector3d(1.0, 1.0, -1.0),
                                                iaVector3d(-1.0, -1.0, 1.0),
                                                iaVector3d(1.0, -1.0, 1.0),
                                                iaVector3d(-1.0, 1.0, 1.0),
                                                iaVector3d(1.0, 1.0, 1.0)};

    iOctreeOld::iOctreeOld(const iAACubed &box, float64 halfMinResolution, uint64 objectCountMaxThreashold, uint64 objectCountMinThreashold)
        : _halfMinResolution(halfMinResolution), _objectCountMaxThreashold(objectCountMaxThreashold), _objectCountMinThreashold(objectCountMinThreashold)
    {
        con_assert(box._halfEdgeLength > 0.0 &&
                       box._halfEdgeLength > halfMinResolution &&
                       halfMinResolution > 0.0 &&
                       objectCountMaxThreashold > objectCountMinThreashold,
                   "invalid configuration");

        _rootNode = createNode();
        _nodes[_rootNode]->_box = box;
    }

    iOctreeOld::~iOctreeOld()
    {
        clearFilter();

        for(auto object : _objects)
        {
            if(object.second != nullptr)
            {
                delete object.second;
            }
        }
        _objects.clear();

        for(auto node : _nodes)
        {
            if(node.second != nullptr)
            {
                delete node.second;
            }
        }
        _nodes.clear();
    }

    void iOctreeOld::clearFilter()
    {
        _spheresFilter.clear();
        _planesFilter.clear();
        _frustumFilter.clear();
    }

    void iOctreeOld::addFilter(const iFrustumd &frustum)
    {
        _frustumFilter.push_back(frustum);
    }

    void iOctreeOld::addFilter(const iPlaned &plane)
    {
        _planesFilter.push_back(plane);
    }

    void iOctreeOld::addFilter(const iaSphered &sphere)
    {
        _spheresFilter.push_back(sphere);
    }

    iOctreeOld::OctreeObject *iOctreeOld::createObject(void *userData, const iaSphered &sphere)
    {
        OctreeObject *object = new OctreeObject();
        object->_octreeNode = 0;
        object->_sphere = sphere;
        _objects[userData] = object;
        return object;
    }

    void iOctreeOld::deleteObject(void *userData)
    {
        auto iter = _objects.find(userData);
        if (_objects.end() != iter)
        {
            delete (*iter).second;
            _objects.erase(iter);
        }
    }

    void iOctreeOld::deleteNode(uint64 nodeID)
    {
        auto nodeIter = _nodes.find(nodeID);

        con_assert(_nodes.end() != nodeIter, "node does not exist");
        con_assert(nullptr == (*nodeIter).second->_children, "node still has children");
        con_assert(0 == (*nodeIter).second->_objects.size(), "inconsistend data");

        if (_nodes.end() != nodeIter)
        {
            _nodes.erase(nodeIter);
        }
    }

    uint64 iOctreeOld::createNode()
    {
        OctreeNode *node = new OctreeNode();
        _nodes[_nextNodeID] = node;
        return _nextNodeID++;
    }

    void iOctreeOld::insert(uint64 nodeID, void *userData, const iaSphered &sphere)
    {
        OctreeNode *node = _nodes[nodeID];
        con_assert(node != nullptr, "zero pointer");

        if (nullptr != node->_children)
        {
            int index = 0;

            if (sphere._center._x > node->_box._center._x)
            {
                index |= 1;
            }

            if (sphere._center._y > node->_box._center._y)
            {
                index |= 2;
            }

            if (sphere._center._z > node->_box._center._z)
            {
                index |= 4;
            }

            insert(node->_children[index], userData, sphere);
        }
        else
        {
            OctreeObject *object = createObject(userData, sphere);
            object->_octreeNode = nodeID;
            node->_objects.push_back(userData);

            trySplit(nodeID);
        }
    }

    void iOctreeOld::trySplit(uint64 nodeID)
    {
        OctreeNode *node = _nodes[nodeID];
        con_assert(node != nullptr, "zero pointer");

        if (node->_box._halfEdgeLength > _halfMinResolution &&
            node->_objects.size() >= _objectCountMaxThreashold)
        {
            split(nodeID);
        }
    }

    void iOctreeOld::insert(void *userData, const iaSphered &sphere)
    {
        OctreeNode *rootNode = _nodes[_rootNode];

        if (!iIntersection::intersects(sphere._center, rootNode->_box))
        {
            con_err(" out of bounds " << sphere._center);
            return;
        }

        insert(_rootNode, userData, sphere);
    }

    void iOctreeOld::split(uint64 nodeID)
    {
        con_assert(nullptr == _nodes[nodeID]->_children, "can't split because has no children");

        OctreeNode *node = _nodes[nodeID];
        float64 halfSize = node->_box._halfEdgeLength * 0.5;

        node->_children = new uint64[8];

        for (int i = 0; i < 8; ++i)
        {
            iaVector3d position = _splitTable[i];
            position *= halfSize;
            position += node->_box._center;

            uint64 newNodeID = createNode();
            OctreeNode *newNode = _nodes[newNodeID];
            newNode->_box._center = position;
            newNode->_box._halfEdgeLength = halfSize;
            newNode->_parent = nodeID;
            node->_children[i] = newNodeID;
        }

        for (auto userData : node->_objects)
        {
            iaVector3d position = _objects[userData]->_sphere._center;

            int index = 0;
            if (position._x > node->_box._center._x)
            {
                index |= 1;
            }

            if (position._y > node->_box._center._y)
            {
                index |= 2;
            }

            if (position._z > node->_box._center._z)
            {
                index |= 4;
            }

            uint64 destinationNodeID = node->_children[index];
            OctreeNode *destination = _nodes[destinationNodeID];
            destination->_objects.push_back(userData);

            OctreeObject *object = _objects[userData];
            object->_octreeNode = destinationNodeID;
        }

        node->_objects.clear();

        for (int i = 0; i < 8; ++i)
        {
            trySplit(node->_children[i]);
        }
    }

    void iOctreeOld::remove(void *userData)
    {
        con_assert(_objects.end() != _objects.find(userData), "object to remove is not registered");

        if (_objects.end() != _objects.find(userData))
        {
            uint64 nodeID = _objects[userData]->_octreeNode;
            OctreeNode *node = _nodes[nodeID];

            auto iterObjectID = find(node->_objects.begin(), node->_objects.end(), userData);
            if (node->_objects.end() != iterObjectID)
            {
                node->_objects.erase(iterObjectID);
                deleteObject(userData);
            }

            if (node->_parent != 0)
            {
                tryMerge(node->_parent);
            }
        }
    }

    void iOctreeOld::tryMerge(uint64 nodeID)
    {
        auto iter = _nodes.find(nodeID);
        con_assert(_nodes.end() != iter, "inconsistend data");

        OctreeNode *node = (*iter).second;
        con_assert(nullptr != node->_children, "inconsistend data");

        uint64 objectCount = 0;
        bool nested = false;

        for (int i = 0; i < 8; ++i)
        {
            auto child = _nodes[node->_children[i]];
            objectCount += static_cast<uint64>(child->_objects.size());

            if (nullptr != child->_children)
            {
                nested = true;
            }
        }

        if (objectCount <= _objectCountMinThreashold &&
            !nested)
        {
            merge(nodeID);
        }
    }

    void iOctreeOld::merge(uint64 nodeID)
    {
        con_assert(nullptr != _nodes[nodeID]->_children, "does not have children to merge");

        OctreeNode *node = _nodes[nodeID];

        for (int i = 0; i < 8; ++i)
        {
            auto childID = node->_children[i];
            auto child = _nodes[childID];
            auto objects = child->_objects;
            auto object = objects.begin();

            while (objects.end() != object)
            {
                node->_objects.push_back((*object));
                _objects[(*object)]->_octreeNode = nodeID;
                object++;
            }

            child->_parent = 0;
            child->_objects.clear();
            deleteNode(childID);
        }

        delete[] node->_children;
        node->_children = 0;
    }

    void iOctreeOld::update(void *userData, const iaSphered &sphere)
    {
        if (_objects.find(userData) != _objects.end())
        {
            auto object = _objects[userData];
            con_assert_sticky(object != nullptr, "corrupt data");

            if (object->_sphere != sphere)
            {
                object->_sphere = sphere;

                auto node = _nodes[object->_octreeNode];
                if (!iIntersection::intersects(sphere._center, node->_box))
                {
                    remove(userData);
                    insert(userData, sphere);
                }
            }
        }
        else
        {
            insert(userData, sphere);
        }
    }

    void iOctreeOld::filter(const iFrustumd &frustum)
    {
        _queryResult.clear();
        filter(_rootNode, frustum);
    }

    void iOctreeOld::filter(uint64 nodeID, const iFrustumd &frustum)
    {
        OctreeNode *node = _nodes[nodeID];

        if (iIntersection::intersects(node->_box, frustum))
        {
            for (const auto userData : node->_objects)
            {
                if (iIntersection::intersects(_objects[userData]->_sphere, frustum))
                {
                    _queryResult.push_back(userData);
                }
            }

            if (nullptr != node->_children)
            {
                for (uint64 i = 0; i < 8; ++i)
                {
                    filter(node->_children[i], frustum);
                }
            }
        }
    }

    void iOctreeOld::filter()
    {
        _queryResult.clear();
        filter(_rootNode);
    }

    void iOctreeOld::filter(uint64 nodeID)
    {
        OctreeNode *node = _nodes[nodeID];

        if (testFilter(node->_box))
        {
            for (auto objectID : node->_objects)
            {
                if (testFilter(_objects[objectID]->_sphere))
                {
                    _queryResult.push_back(objectID);
                }
            }

            if (nullptr != node->_children)
            {
                for (uint64 i = 0; i < 8; ++i)
                {
                    filter(node->_children[i]);
                }
            }
        }
    }

    bool iOctreeOld::testFilter(const iAACubed &cube)
    {
        for (auto plane : _planesFilter)
        {
            if (!iIntersection::inFrontOf(cube, plane))
            {
                return false;
            }
        }

        for (auto frustum : _frustumFilter)
        {
            if (!iIntersection::intersects(cube, frustum))
            {
                return false;
            }
        }

        for (auto sphere : _spheresFilter)
        {
            if (!iIntersection::intersects(cube, sphere))
            {
                return false;
            }
        }

        return true;
    }

    bool iOctreeOld::testFilter(const iaSphered &sphere)
    {
        for (auto plane : _planesFilter)
        {
            if (!iIntersection::inFrontOf(sphere, plane))
            {
                return false;
            }
        }

        for (auto frustum : _frustumFilter)
        {
            if (!iIntersection::intersects(sphere, frustum))
            {
                return false;
            }
        }

        for (auto sphere : _spheresFilter)
        {
            if (!iIntersection::intersects(sphere, sphere))
            {
                return false;
            }
        }

        return true;
    }

    const std::vector<void *> &iOctreeOld::getResult() const
    {
        return _queryResult;
    }

    void iOctreeOld::draw()
    {
        iaMatrixd matrix;
        iRenderer::getInstance().setModelMatrix(matrix);
        draw(_rootNode);
    }

    float32 iOctreeOld::draw(uint64 nodeID)
    {
        OctreeNode *node = _nodes[nodeID];
        float32 alpha = 0.4;

        if (nullptr != node->_children)
        {
            for (int i = 0; i < 8; ++i)
            {
                alpha = std::min(alpha, draw(node->_children[i]));
            }

            iRenderer::getInstance().drawBox(node->_box, iaColor4f(0, 0, 1, alpha));
        }

        alpha *= 0.7;

        return alpha;
    }

} // namespace igor

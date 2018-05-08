// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

#include <iOctree.h>

#include <iAACube.h>
#include <iRenderer.h>
#include <iNodeFactory.h>
#include <iIntersection.h>
#include <iMaterialResourceFactory.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    const iaVector3d iOctree::_splitTable[8] =
    {
        iaVector3d(-1.0, -1.0, -1.0),
        iaVector3d(1.0, -1.0, -1.0),
        iaVector3d(-1.0, 1.0, -1.0),
        iaVector3d(1.0, 1.0, -1.0),
        iaVector3d(-1.0, -1.0, 1.0),
        iaVector3d(1.0, -1.0, 1.0),
        iaVector3d(-1.0, 1.0, 1.0),
        iaVector3d(1.0, 1.0, 1.0)
    };

    iOctree::iOctree(const iAACubed& box, float64 halfMinResolution, uint64 objectCountMaxThreashold, uint64 objectCountMinThreashold)
        : _halfMinResolution(halfMinResolution)
        , _objectCountMaxThreashold(objectCountMaxThreashold)
        , _objectCountMinThreashold(objectCountMinThreashold)
    {
        con_assert(box._halfEdgeLength > 0.0 && \
            box._halfEdgeLength > halfMinResolution && \
            halfMinResolution > 0.0 && \
            objectCountMaxThreashold > objectCountMinThreashold, "invalid configuration");

        _rootNode = createNode();
        _nodes[_rootNode]->_box = box;
    }

    iOctree::~iOctree()
    {
        clearFilter();
    }

    void iOctree::clearFilter()
    {
        _spheresFilter.clear();
        _planesFilter.clear();
        _frustumFilter.clear();
        _queryResult.clear();
    }

    void iOctree::addFilter(const iFrustumd& frustum)
    {
        _frustumFilter.push_back(frustum);
    }

    void iOctree::addFilter(const iPlaned& plane)
    {
        _planesFilter.push_back(plane);
    }

    void iOctree::addFilter(const iSphered& sphere)
    {
        _spheresFilter.push_back(sphere);
    }

    iOctree::OctreeObject* iOctree::createObject(uint64 userDataID, const iSphered& sphere)
    {
        con_assert_sticky(_objects.find(userDataID) == _objects.end(), "object id already in use");

        OctreeObject* object = new OctreeObject();
        object->_octreeNode = 0;
        object->_sphere = sphere;
        _objects[userDataID] = object;
        return object;
    }

    void iOctree::deleteObject(uint64 userDataID)
    {
        con_assert(_objects.find(userDataID) != _objects.end(), "object not registered");

        auto iter = _objects.find(userDataID);
        if (_objects.end() != iter)
        {
            delete (*iter).second;
            _objects.erase(iter);
        }
    }

    void iOctree::deleteNode(uint64 nodeID)
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

    uint64 iOctree::createNode()
    {
        OctreeNode* node = new OctreeNode();
        _nodes[_nextNodeID] = node;
        return _nextNodeID++;
    }

    void iOctree::insert(uint64 nodeID, uint64 userDataID, const iSphered& sphere)
    {
        OctreeNode* node = _nodes[nodeID];
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

            insert(node->_children[index], userDataID, sphere);
        }
        else
        {
            OctreeObject* object = createObject(userDataID, sphere);
            object->_octreeNode = nodeID;
            node->_objects.push_back(userDataID);

            trySplit(nodeID);
        }
    }

    void iOctree::trySplit(uint64 nodeID)
    {
        OctreeNode* node = _nodes[nodeID];
        con_assert(node != nullptr, "zero pointer");

        if (node->_box._halfEdgeLength > _halfMinResolution &&
            node->_objects.size() >= _objectCountMaxThreashold)
        {
            split(nodeID);
        }
    }

    void iOctree::insert(uint64 userDataID, const iSphered& sphere)
    {
        OctreeNode* rootNode = _nodes[_rootNode];

        bool intersects = iIntersection::intersects(sphere._center, rootNode->_box);
        con_assert(intersects, userDataID << " out of bounds " << sphere._center);
        if (intersects)
        {
            insert(_rootNode, userDataID, sphere);
        }
        else
        {
            con_err(userDataID << " out of bounds " << sphere._center);
        }        
    }

    void iOctree::split(uint64 nodeID)
    {
        con_assert(nullptr == _nodes[nodeID]->_children, "can't split because has no children");

        OctreeNode* node = _nodes[nodeID];
        float64 halfSize = node->_box._halfEdgeLength * 0.5;

        node->_children = new uint64[8];

        for (int i = 0; i < 8; ++i)
        {
            iaVector3d position = _splitTable[i];
            position *= halfSize;
            position += node->_box._center;

            uint64 newNodeID = createNode();
            OctreeNode* newNode = _nodes[newNodeID];
            newNode->_box._center = position;
            newNode->_box._halfEdgeLength = halfSize;
            newNode->_parent = nodeID;
            node->_children[i] = newNodeID;
        }

        for (auto userDataID : node->_objects)
        {
            iaVector3d position = _objects[userDataID]->_sphere._center;

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
            OctreeNode* destination = _nodes[destinationNodeID];
            destination->_objects.push_back(userDataID);

            OctreeObject* object = _objects[userDataID];
            object->_octreeNode = destinationNodeID;
        }

        node->_objects.clear();

        for (int i = 0; i < 8; ++i)
        {
            trySplit(node->_children[i]);
        }
    }

    void iOctree::remove(uint64 userDataID)
    {
        con_assert(_objects.end() != _objects.find(userDataID), "object to remove is not registered");

        if (_objects.end() != _objects.find(userDataID))
        {
            uint64 nodeID = _objects[userDataID]->_octreeNode;
            OctreeNode* node = _nodes[nodeID];

            auto iterObjectID = find(node->_objects.begin(), node->_objects.end(), userDataID);
            if (node->_objects.end() != iterObjectID)
            {
                node->_objects.erase(iterObjectID);
                deleteObject(userDataID);
            }

            if (node->_parent != 0)
            {
                tryMerge(node->_parent);
            }
        }
    }

    void iOctree::tryMerge(uint64 nodeID)
    {
        auto iter = _nodes.find(nodeID);
        con_assert(_nodes.end() != iter, "inconsistend data");

        OctreeNode* node = (*iter).second;
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

    void iOctree::merge(uint64 nodeID)
    {
        con_assert(nullptr != _nodes[nodeID]->_children, "does not have children to merge");

        OctreeNode* node = _nodes[nodeID];

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

    void iOctree::update(uint64 userDataID, const iSphered& sphere)
    {
        if (_objects.find(userDataID) != _objects.end())
        {
            auto object = _objects[userDataID];
            con_assert_sticky(object != nullptr, "corrupt data");

            if (object->_sphere != sphere)
            {
                object->_sphere = sphere;

                auto node = _nodes[object->_octreeNode];
                if (!iIntersection::intersects(sphere._center, node->_box))
                {
                    remove(userDataID);
                    insert(userDataID, sphere);
                }
            }
        }
        else
        {
            insert(userDataID, sphere);
        }
    }

    void iOctree::filter()
    {
        filter(_rootNode);
    }

    void iOctree::filter(uint64 nodeID)
    {
        OctreeNode* node = _nodes[nodeID];

        if (testFilter(node->_box))
        {
            iAACubed box;
            for (auto objectID : node->_objects)
            {
                box._center = _objects[objectID]->_sphere._center;
                box._halfEdgeLength = _objects[objectID]->_sphere._radius;

                if (testFilter(box))
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

    bool iOctree::testFilter(iAACubed& box)
    {
        for (auto plane : _planesFilter)
        {
            if (!iIntersection::inFrontOf(box, plane))
            {
                return false;
            }
        }

        for (auto frustum : _frustumFilter)
        {
            if (!iIntersection::intersects(box, frustum))
            {
                return false;
            }
        }

        for (auto sphere : _spheresFilter)
        {
            if (!iIntersection::intersects(box, sphere))
            {
                return false;
            }
        }

        return true;
    }

    void iOctree::getResult(vector<uint64>& data)
    {
        data = _queryResult;
    }

    void iOctree::draw()
    {
        iaMatrixd matrix;
        iRenderer::getInstance().setModelMatrix(matrix);

        uint64 material = iMaterialResourceFactory::getInstance().createMaterial();
        iMaterialResourceFactory::getInstance().getMaterial(material)->getRenderStateSet().setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
        iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getMaterial(material));

        iRenderer::getInstance().setColor(0, 0, 1, 1);

        draw(_rootNode);
    }

    void iOctree::draw(uint64 nodeID)
    {
        OctreeNode* node = _nodes[nodeID];

        iRenderer::getInstance().drawBBox(node->_box);

        if (nullptr != node->_children)
        {
            for (int i = 0; i < 8; ++i)
            {
                draw(node->_children[i]);
            }
        }
    }

}

// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iOctree.h>

#include <iNodeFactory.h>
#include <iNodeVolume.h>
#include <iAACube.h>
#include <iRenderer.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    const iaVector3f iOctree::_splitTable[8] =
    {
        iaVector3f(-1.0, -1.0, -1.0),
        iaVector3f(1.0, -1.0, -1.0),
        iaVector3f(-1.0, 1.0, -1.0),
        iaVector3f(1.0, 1.0, -1.0),
        iaVector3f(-1.0, -1.0, 1.0),
        iaVector3f(1.0, -1.0, 1.0),
        iaVector3f(-1.0, 1.0, 1.0),
        iaVector3f(1.0, 1.0, 1.0)
    };

    iOctree::iOctree(const iAACubef& box, float32 halfMinResolution, uint32 objectCountMaxThreashold, uint32 objectCountMinThreashold)
        : _halfMinResolution(halfMinResolution)
        , _objectCountMaxThreashold(objectCountMaxThreashold)
        , _objectCountMinThreashold(objectCountMinThreashold)
    {
        con_assert(box._halfEdgeLength > 0 && \
            box._halfEdgeLength > halfMinResolution && \
            halfMinResolution > 0 && \
            objectCountMaxThreashold > objectCountMinThreashold, "invalid configuration");

        _rootNode = createNode();
        _nodes[_rootNode]->_box = box;
    }

    iOctree::~iOctree()
    {
    }

    iOctree::OctreeObject* iOctree::createObject(uint32 sceneNodeID)
    {
        con_assert(_objects.find(sceneNodeID) == _objects.end(), "object id already in use");

        OctreeObject* object = new OctreeObject();
        object->_octreeNode = 0;
        _objects[sceneNodeID] = object;
        return object;
    }

    void iOctree::deleteObject(uint32 sceneNodeID)
    {
        con_assert(_objects.find(sceneNodeID) != _objects.end(), "object not registered");

        auto iter = _objects.find(sceneNodeID);
        if (_objects.end() != iter)
        {
            delete (*iter).second;
            _objects.erase(iter);
        }
    }

    void iOctree::deleteNode(uint32 nodeID)
    {
        con_assert(_nodes.end() != _nodes.find(nodeID), "node does not exist");
        con_assert(nullptr == (*_nodes.find(nodeID)).second->_children, "node still has children");
        con_assert(0 == (*_nodes.find(nodeID)).second->_objects.size(), "inconsistend data");

        _nodes.erase(_nodes.find(nodeID));
    }

    uint32 iOctree::createNode()
    {
        OctreeNode* node = new OctreeNode();
        _nodes[_nextNodeID] = node;
        return _nextNodeID++;
    }

    void iOctree::insert(uint32 nodeID, uint32 sceneNodeID, iaVector3f& center)
    {
        OctreeNode* node = _nodes[nodeID];

        if (nullptr != node->_children)
        {
            int index = 0;

            if (center._x > node->_box._center._x)
            {
                index |= 1;
            }

            if (center._y > node->_box._center._y)
            {
                index |= 2;
            }

            if (center._z > node->_box._center._z)
            {
                index |= 4;
            }

            insert(node->_children[index], sceneNodeID, center);
        }
        else
        {
            OctreeObject* object = createObject(sceneNodeID);
            object->_octreeNode = nodeID;
            node->_objects.push_back(sceneNodeID);

            trySplit(nodeID);
        }
    }

    void iOctree::trySplit(uint32 nodeID)
    {
        OctreeNode* node = _nodes[nodeID];

        if (node->_box._halfEdgeLength > _halfMinResolution &&
            node->_objects.size() >= _objectCountMaxThreashold)
        {
            split(nodeID);
        }
    }

    void iOctree::insert(uint32 sceneNodeID)
    {
        iNodeVolume* sceneNode = static_cast<iNodeVolume*>(iNodeFactory::getInstance().getNode(sceneNodeID));
        con_assert(nullptr != sceneNode, "scene node does not exist");

        OctreeNode* rootNode = _nodes[_rootNode];
        iaVector3f center = sceneNode->getCenter();

        if (rootNode->_box.intersects(center))
        {
            insert(_rootNode, sceneNodeID, center);
        }
        else
        {
            con_err("out of bounds");
        }
    }

    void iOctree::split(uint32 nodeID)
    {
        con_assert(nullptr == _nodes[nodeID]->_children, "can't split because has no children");

        OctreeNode* node = _nodes[nodeID];
        float32 halfSize = node->_box._halfEdgeLength * 0.5f;

        node->_children = new uint32[8];

        for (int i = 0; i < 8; ++i)
        {
            iaVector3f center = _splitTable[i];
            center *= halfSize;
            center += node->_box._center;

            uint32 newNodeID = createNode();
            OctreeNode* newNode = _nodes[newNodeID];
            newNode->_box._center = center;
            newNode->_box._halfEdgeLength = halfSize;
            newNode->_parent = nodeID;
            node->_children[i] = newNodeID;
        }

        auto objectIter = node->_objects.begin();
        while (objectIter != node->_objects.end())
        {
            uint32 objectID = (*objectIter);
            iNodeVolume* sceneNode = static_cast<iNodeVolume*>(iNodeFactory::getInstance().getNode(objectID));
            iaVector3f center = sceneNode->getCenter();

            int index = 0;
            if (center._x > node->_box._center._x)
            {
                index |= 1;
            }

            if (center._y > node->_box._center._y)
            {
                index |= 2;
            }

            if (center._z > node->_box._center._z)
            {
                index |= 4;
            }

            uint32 destinationID = node->_children[index];
            OctreeNode* destination = _nodes[destinationID];
            destination->_objects.push_back(objectID);

            OctreeObject* object = _objects[objectID];
            object->_octreeNode = destinationID;

            objectIter++;
        }

        node->_objects.clear();

        for (int i = 0; i < 8; ++i)
        {
            trySplit(node->_children[i]);
        }
    }

    void iOctree::remove(uint32 sceneNodeID)
    {
        con_assert(_objects.end() != _objects.find(sceneNodeID), "object to remove is not registered");
        con_assert(_nodes.end() != _nodes.find(_objects[sceneNodeID]->_octreeNode), "node not found");

        uint32 nodeID = _objects[sceneNodeID]->_octreeNode;
        OctreeNode* node = _nodes[nodeID];

        auto iterObjectID = find(node->_objects.begin(), node->_objects.end(), sceneNodeID);
        if (node->_objects.end() != iterObjectID)
        {
            node->_objects.erase(iterObjectID);
            deleteObject(sceneNodeID);
        }

        if (node->_parent != 0)
        {
            tryMerge(node->_parent);
        }
    }

    void iOctree::tryMerge(uint32 nodeID)
    {
        auto iter = _nodes.find(nodeID);
        con_assert(_nodes.end() != iter, "inconsistend data");

        OctreeNode* node = (*iter).second;
        con_assert(nullptr != node->_children, "inconsistend data");

        uint32 objectCount = 0;
        bool nested = false;

        for (int i = 0; i < 8; ++i)
        {
            auto child = _nodes[node->_children[i]];
            objectCount += static_cast<uint32>(child->_objects.size());

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

    void iOctree::merge(uint32 nodeID)
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

    void iOctree::update(uint32 sceneNodeID)
    {
        auto sceneNode = static_cast<iNodeVolume*>(iNodeFactory::getInstance().getNode(sceneNodeID));
        auto vec = sceneNode->getCenter();

        auto object = _objects[sceneNodeID];
        auto node = _nodes[object->_octreeNode];

        if (!node->_box.intersects(vec))
        {
            remove(sceneNodeID);
            insert(sceneNodeID);
        }
    }

    void iOctree::resetFilter()
    {
        _queryResult.clear();
    }

    void iOctree::filter(const iFrustumf& frustum, uint32 nodeID)
    {
        OctreeNode* node = _nodes[nodeID];
        iAACubef box;

        if (node->_box.intersects(frustum))
        {
            if (node->_objects.size())
            {
                auto iterObjectID = node->_objects.begin();
                while (iterObjectID != node->_objects.end())
                {
                    iNodeVolume* sceneNode = static_cast<iNodeVolume*>(iNodeFactory::getInstance().getNode((*iterObjectID)));

                    box._center = sceneNode->getCenter();
                    box._halfEdgeLength = sceneNode->getBoundingSphere()._radius;

                    if (box.intersects(frustum))
                    {
                        sceneNode->_reached = true;
                        _queryResult.push_back(sceneNode);
                    }

                    iterObjectID++;
                }
            }

            if (nullptr != node->_children)
            {
                for (uint32 i = 0; i < 8; ++i)
                {
                    filter(frustum, node->_children[i]);
                }
            }
        }
    }

    void iOctree::filter(const iSpheref& sphere, uint32 nodeID)
    {
        OctreeNode* node = _nodes[nodeID];
        iAACubef box;

        if (node->_box.intersects(sphere))
        {
            if (node->_objects.size())
            {
                auto iterObjectID = node->_objects.begin();
                while (iterObjectID != node->_objects.end())
                {
                    iNodeVolume* sceneNode = static_cast<iNodeVolume*>(iNodeFactory::getInstance().getNode((*iterObjectID)));
                    box._center = sceneNode->getCenter();
                    box._halfEdgeLength = sceneNode->getBoundingSphere()._radius;

                    if (box.intersects(sphere))
                    {
                        sceneNode->_reached = true;
                        _queryResult.push_back(sceneNode);
                    }

                    iterObjectID++;
                }
            }

            if (nullptr != node->_children)
            {
                for (uint32 i = 0; i < 8; ++i)
                {
                    filter(sphere, node->_children[i]);
                }
            }
        }
    }

    void iOctree::filter(const iSpheref& sphere)
    {
        filter(sphere, _rootNode);
    }

    void iOctree::filter(const iFrustumf& frustum)
    {
        filter(frustum, _rootNode);
    }

    void iOctree::getResult(list<iNode*>& data)
    {
        data = _queryResult;
    }

    void iOctree::draw()
    {
        draw(_rootNode);
    }

    void iOctree::draw(uint32 nodeID)
    {
        OctreeNode* node = _nodes[nodeID];

        if (0 != node->_objects.size())
        {
            iRenderer::getInstance().setColor(iaColor4f(0, 1, 0, 1));
        }
        else
        {
            iRenderer::getInstance().setColor(iaColor4f(0.5, 0.5, 0.5, 0.5));
        }

        iaVector3f a = node->_box._center;
        iaVector3f b = node->_box._center;

        a._x -= node->_box._halfEdgeLength;
        a._y -= node->_box._halfEdgeLength;
        a._z -= node->_box._halfEdgeLength;

        b._x += node->_box._halfEdgeLength;
        b._y += node->_box._halfEdgeLength;
        b._z += node->_box._halfEdgeLength;

        iRenderer::getInstance().drawBox(a, b);

        if (nullptr != node->_children)
        {
            for (int i = 0; i < 8; ++i)
            {
                draw(node->_children[i]);
            }
        }
    }

}
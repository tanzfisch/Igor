// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include "iNodeFactory.h"

#include <iaConsole.h>
using namespace IgorAux;

#include <iNode.h>
#include <iNodeCamera.h>
#include <iCelestialNode.h>
#include <iNodeTransform.h>
#include <iNodeMesh.h>
#include <iNodeModel.h>
#include <iNodeRender.h>
#include <iNodeSkyBox.h>
#include <iSkyLightNode.h>
#include <iNodeLight.h>
#include <iNodeSwitch.h>
#include <iNodeLODSwitch.h>
#include <iNodeLODTrigger.h>
#include <iNodePhysics.h>
#include <iNodeParticleSystem.h>
#include <iNodeEmitter.h>
#include <iNodeWater.h>

#include <iApplication.h>

namespace Igor
{

    iNodeFactory::iNodeFactory()
    {
    }

    iNodeFactory::~iNodeFactory()
    {
        if (_nodes.size())
        {
            con_err("possible mem leak. nodes left: " << static_cast<int>(_nodes.size()));
        }
    }

    void iNodeFactory::insertNodeAsync(iNode* src, iNode* dst)
    {
        _mutexQueueAdd.lock();
        _queueAdd.push_back(pair<uint32, uint32>(src->getID(), dst->getID()));
        _mutexQueueAdd.unlock();
    }

    void iNodeFactory::removeNodeAsync(iNode* src, iNode* dst)
    {
        _mutexQueueRemove.lock();
        _queueRemove.push_back(pair<uint32, uint32>(src->getID(), dst->getID()));
        _mutexQueueRemove.unlock();
    }

    void iNodeFactory::handle()
    {
        _mutexQueueRemove.lock();
        auto removeQueue = std::move(_queueRemove);
        _mutexQueueRemove.unlock();

        for (auto entry : removeQueue)
        {
            iNode* parent = getNode(entry.first);
            iNode* child = getNode(entry.second);
            parent->removeNode(child);
        }

        _mutexQueueAdd.lock();
        auto addQueue = std::move(_queueAdd);
        _mutexQueueAdd.unlock();

        for (auto entry : addQueue)
        {
            iNode* parent = getNode(entry.first);
            iNode* child = getNode(entry.second);
            parent->insertNode(child);
        }

        _mutexQueueDelete.lock();
        auto deleteQueue = std::move(_queueDelete);
        _mutexQueueDelete.unlock();

        for (auto nodeID : deleteQueue)
        {
            iNode* node = getNode(nodeID);
            if (node != nullptr)
            {
                destroyNode(node);
            }
        }
    }

    vector<iNode*> iNodeFactory::getNodes(iNodeType nodeType)
    {
        vector<iNode*> result;

        for (auto node : _nodes)
        {
            if (node.second->getType() == nodeType)
            {
                result.push_back(node.second);
            }
        }

        return result;
    }

    iNode* iNodeFactory::getNode(uint32 id)
    {
        iNode* result = nullptr;

        _mutexNodes.lock();
        auto iter = _nodes.find(id);
        if (_nodes.end() != iter)
        {
            result = (*iter).second;
        }
        _mutexNodes.unlock();

        return result;
    }

    bool iNodeFactory::isNode(uint32 id)
    {
        bool result = false;;

        _mutexNodes.lock();
        auto iter = _nodes.find(id);
        if (_nodes.end() != iter)
        {
            result = true;
        }
        _mutexNodes.unlock();

        return result;
    }

    void iNodeFactory::destroyNode(uint32 nodeID)
    {
        iNode* node = nullptr;

        _mutexNodes.lock();
        auto iter = _nodes.find(nodeID);
        if (_nodes.end() != iter)
        {
            node = (*iter).second;
            _nodes.erase(iter);
        }
        _mutexNodes.unlock();

        if (node != nullptr)
        {
            delete node;
        }
    }

    void iNodeFactory::destroyNode(iNode* node)
    {
        con_assert(nullptr != node, "zero pointer");

        if (nullptr != node)
        {
            destroyNode(node->getID());
        }
        else
        {
            con_err("tried to delete zero pointer");
        }
    }

    void iNodeFactory::destroyNodeAsync(uint32 nodeID)
    {
        con_assert(nodeID != iNode::INVALID_NODE_ID, "invalid node id");

        _mutexQueueDelete.lock();
        _queueDelete.push_back(nodeID);
        _mutexQueueDelete.unlock();
    }

    void iNodeFactory::destroyNodeAsync(iNode* node)
    {
        con_assert(nullptr != node, "zero pointer");

        if (nullptr != node)
        {
            _mutexQueueDelete.lock();
            _queueDelete.push_back(node->getID());
            _mutexQueueDelete.unlock();
        }
        else
        {
            con_err("tried to delete zero pointer");
        }
    }

    iNode* iNodeFactory::createCopy(iNode* node)
    {
        iNode* result = nullptr;
        map<uint32, uint32> nodeIDMap;

        result = createCopyInternal(node, nodeIDMap, UINT32_MAX);

        for (auto nodePair : nodeIDMap)
        {
            iNode* node = getNode(nodePair.second);
            if (node != nullptr)
            {
                node->onPostCopyLink(nodeIDMap);
            }
        }

        return result;
    }

    iNode* iNodeFactory::createCopyInternal(iNode* node, map<uint32, uint32>& nodeIDMap, uint32 recursiveDepth)
    {
        iNode* result = nullptr;
        result = createNodeCopy(node);

        if (result != nullptr)
        {
            nodeIDMap[node->getID()] = result->getID();

            if (recursiveDepth > 0)
            {
                for (auto child : node->getChildren())
                {
                    result->insertNode(createCopyInternal(child, nodeIDMap, --recursiveDepth));
                }
            }
        }

        return result;
    }

    iNode* iNodeFactory::createNodeCopy(iNode* node)
    {
        con_assert(node != nullptr, "zero pointer");

        iNode* result = nullptr;

        if (node != nullptr)
        {
            switch (node->getType())
            {
            case iNodeType::iNode:
                result = new iNode(node);
                break;

            case iNodeType::iNodeCamera:
                result = new iNodeCamera(static_cast<iNodeCamera*>(node));
                break;

            case iNodeType::iCelestialNode:
                con_err("deprecated");
                // result = new iCelestialNode(static_cast<iCelestialNode*>(node));
                break;

            case iNodeType::iNodeLight:
                result = new iNodeLight(static_cast<iNodeLight*>(node));
                break;

            case iNodeType::iNodeMesh:
                result = new iNodeMesh(static_cast<iNodeMesh*>(node));
                break;

            case iNodeType::iNodeModel:
                result = new iNodeModel(static_cast<iNodeModel*>(node));
                break;

            case iNodeType::iNodeSkyBox:
                result = new iNodeSkyBox(static_cast<iNodeSkyBox*>(node));
                break;

            case iNodeType::iSkyLightNode:
                con_err("deprecated");
                //result = new iSkyLightNode(static_cast<iSkyLightNode*>(node));
                break;

            case iNodeType::iNodeTransform:
                result = new iNodeTransform(static_cast<iNodeTransform*>(node));
                break;

            case iNodeType::iNodeSwitch:
                result = new iNodeSwitch(static_cast<iNodeSwitch*>(node));
                break;

            case iNodeType::iNodeLODSwitch:
                result = new iNodeLODSwitch(static_cast<iNodeLODSwitch*>(node));
                break;

            case iNodeType::iNodeLODTrigger:
                result = new iNodeLODTrigger(static_cast<iNodeLODTrigger*>(node));
                break;

            case iNodeType::iNodePhysics:
                result = new iNodePhysics(static_cast<iNodePhysics*>(node));
                break;

            case iNodeType::iNodeParticleSystem:
                result = new iNodeParticleSystem(static_cast<iNodeParticleSystem*>(node));
                break;

            case iNodeType::iNodeEmitter:
                result = new iNodeEmitter(static_cast<iNodeEmitter*>(node));
                break;
                
            case iNodeType::iNodeWater:
                result = new iNodeWater(static_cast<iNodeWater*>(node));
                break;

            case iNodeType::Undefined:
            default:
                con_err("undefined node type");
            };

            if (result)
            {
                _mutexNodes.lock();
                _nodes[result->getID()] = result;
                _mutexNodes.unlock();
            }
        }

        return result;
    }

    iNode* iNodeFactory::createNode(iNodeType nodeType)
    {
        iNode* result = 0;

        switch (nodeType)
        {
        case iNodeType::iNode:
            result = new iNode();
            break;

        case iNodeType::iNodeCamera:
            result = new iNodeCamera();
            break;

        case iNodeType::iCelestialNode:
            result = new iCelestialNode();
            break;

        case iNodeType::iNodeLight:
            result = new iNodeLight();
            break;

        case iNodeType::iNodeMesh:
            result = new iNodeMesh();
            break;

        case iNodeType::iNodeModel:
            result = new iNodeModel();
            break;

        case iNodeType::iNodeSkyBox:
            result = new iNodeSkyBox();
            break;

        case iNodeType::iSkyLightNode:
            result = new iSkyLightNode();
            break;

        case iNodeType::iNodeTransform:
            result = new iNodeTransform();
            break;

        case iNodeType::iNodeSwitch:
            result = new iNodeSwitch();
            break;

        case iNodeType::iNodeLODSwitch:
            result = new iNodeLODSwitch();
            break;

        case iNodeType::iNodeLODTrigger:
            result = new iNodeLODTrigger();
            break;

        case iNodeType::iNodePhysics:
            result = new iNodePhysics();
            break;

        case iNodeType::iNodeParticleSystem:
            result = new iNodeParticleSystem();
            break;

        case iNodeType::iNodeEmitter:
            result = new iNodeEmitter();
            break;

        case iNodeType::iNodeWater:
            result = new iNodeWater();
            break;

        case iNodeType::Undefined:
        default:
            con_err("undefined node type");
        };

        if (result)
        {
            _mutexNodes.lock();
            _nodes[result->getID()] = result;
            _mutexNodes.unlock();
        }

        return result;
    }

}

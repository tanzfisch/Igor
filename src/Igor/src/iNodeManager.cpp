// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include "iNodeManager.h"

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
#include <iNodeVisitorPrintTree.h>

#include <iApplication.h>

namespace Igor
{

    iaMutex iNodeManager::_mutexNodes;

    iNodeManager::~iNodeManager()
    {
        if (_nodes.size())
        {
            con_err("possible mem leak. nodes left: " << static_cast<int>(_nodes.size()));

            for (auto pair : _nodes)
            {
                con_debug_endl(pair.second->getName() << " id:" << pair.second->getID());
            }
        }
    }

    void iNodeManager::onPreDestroyInstance()
    {
        flush();
    }

    void iNodeManager::applyActionsAsync(const std::vector<iAction>& actionQueue)
    {
        _mutexQueue.lock();
        _actionQueue.insert(_actionQueue.end(), actionQueue.begin(), actionQueue.end());
        _mutexQueue.unlock();
    }

    void iNodeManager::setActiveAsync(iNodePtr node, bool active)
    {
        iAction action;
        action._action = active ? iActionType::Activate : iActionType::Deactivate;
        action._nodeA = node->getID();

        _mutexQueue.lock();
        _actionQueue.push_back(action);
        _mutexQueue.unlock();
    }

    void iNodeManager::insertNodeAsync(iNodePtr src, iNodePtr dst)
    {
        iAction action;
        action._action = iActionType::Insert;
        action._nodeA = src->getID();
        action._nodeB = dst->getID();

        _mutexQueue.lock();
        _actionQueue.push_back(action);
        _mutexQueue.unlock();
    }

    void iNodeManager::removeNodeAsync(iNodePtr src, iNodePtr dst)
    {
        iAction action;
        action._action = iActionType::Remove;
        action._nodeA = src->getID();
        action._nodeB = dst->getID();

        _mutexQueue.lock();
        _actionQueue.push_back(action);
        _mutexQueue.unlock();
    }
    
    void iNodeManager::destroyNodeAsync(uint64 nodeID)
    {
        con_assert(nodeID != iNode::INVALID_NODE_ID, "invalid node id");

        iAction action;
        action._action = iActionType::Destroy;
        action._nodeA = nodeID;

        _mutexQueue.lock();
        _actionQueue.push_back(action);
        _mutexQueue.unlock();
    }

    void iNodeManager::destroyNodeAsync(iNodePtr node)
    {
        con_assert(nullptr != node, "zero pointer");

        if (nullptr != node)
        {
            iAction action;
            action._action = iActionType::Destroy;
            action._nodeA = node->getID();

            _mutexQueue.lock();
            _actionQueue.push_back(action);
            _mutexQueue.unlock();
        }
        else
        {
            con_err("tried to delete zero pointer");
        }
    }

    void iNodeManager::handle()
    {
        flush();
    }

    void iNodeManager::flush()
    {
        _mutexQueue.lock();
        auto queue = std::move(_actionQueue);
        _mutexQueue.unlock();

        iNodePtr nodeA = nullptr;
        iNodePtr nodeB = nullptr;

        for (auto entry : queue)
        {
            switch (entry._action)
            {
            case iActionType::Insert:
                nodeA = getNode(entry._nodeA);
                nodeB = getNode(entry._nodeB);
                if (nodeA != nullptr && nodeB != nullptr)
                {
                    nodeA->insertNode(nodeB);
                }
                break;

            case iActionType::Remove:
                nodeA = getNode(entry._nodeA);
                nodeB = getNode(entry._nodeB);
                if (nodeA != nullptr && nodeB != nullptr)
                {
                    nodeA->removeNode(nodeB);
                }
                break;

            case iActionType::Destroy:
                destroyNode(entry._nodeA);
                break;

            case iActionType::Activate:
                nodeA = getNode(entry._nodeA);
                if (nodeA != nullptr)
                {
                    nodeA->setActive(true);
                }
                break;

            case iActionType::Deactivate:
                nodeA = getNode(entry._nodeA);
                if (nodeA != nullptr)
                {
                    nodeA->setActive(false);
                }
                break;
            }
        }
    }

    std::vector<uint64> iNodeManager::getNodes(iNodeType nodeType)
    {
        std::vector<uint64> result;

        for (auto node : _nodes)
        {
            if (node.second->getType() == nodeType)
            {
                result.push_back(node.first);
            }
        }

        return result;
    }

    void iNodeManager::destroyNode(uint64 nodeID)
    {
        iNodePtr node = nullptr;

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

    void iNodeManager::destroyNode(iNodePtr node)
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


    iNodePtr iNodeManager::createCopy(iNodePtr node)
    {
        iNodePtr result = nullptr;
        std::map<uint64, uint64> nodeIDMap;

        result = createCopyInternal(node, nodeIDMap, UINT32_MAX);

        for (auto nodePair : nodeIDMap)
        {
            iNodePtr node = getNode(nodePair.second);
            if (node != nullptr)
            {
                node->onPostCopyLink(nodeIDMap);
            }
        }

        return result;
    }

    iNodePtr iNodeManager::createCopyInternal(iNodePtr node, std::map<uint64, uint64>& nodeIDMap, uint32 recursiveDepth)
    {
        iNodePtr result = nullptr;
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

    iNodePtr iNodeManager::createNodeCopy(iNodePtr node)
    {
        con_assert(node != nullptr, "zero pointer");

        iNodePtr result = nullptr;

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

}
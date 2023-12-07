// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "iNodeManager.h"

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <igor/scene/nodes/iNode.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/scene/nodes/iNodeRender.h>
#include <igor/scene/nodes/iNodeSkyBox.h>
#include <igor/scene/nodes/iNodeLight.h>
#include <igor/scene/nodes/iNodeSwitch.h>
#include <igor/scene/nodes/iNodeLODSwitch.h>
#include <igor/scene/nodes/iNodeLODTrigger.h>
#include <igor/scene/nodes/iNodePhysics.h>
#include <igor/scene/nodes/iNodeParticleSystem.h>
#include <igor/scene/nodes/iNodeEmitter.h>
#include <igor/scene/nodes/iNodeWater.h>
#include <igor/scene/traversal/iNodeVisitorPrintTree.h>

#include <igor/system/iApplication.h>

namespace igor
{

    iaMutex iNodeManager::_mutexNodes;

    iNodeManager::~iNodeManager()
    {
        flush();

        if (_nodes.size())
        {
            con_warn("possible memory leak. nodes left: " << static_cast<int>(_nodes.size()));

            for (auto pair : _nodes)
            {
                // info must return at least one line
                std::vector<iaString> info = pair.second->getInfo(true);
                con_debug(info[0]);
            }
        }
    }

    void iNodeManager::applyActionsAsync(const std::vector<iAction> &actionQueue)
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

    void iNodeManager::destroyNodeAsync(iNodeID nodeID)
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

    void iNodeManager::onUpdate()
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

    void iNodeManager::getNodes(std::vector<iNodeID> &nodes)
    {
        _mutexNodes.lock();
        for (auto node : _nodes)
        {
            nodes.push_back(node.first);
        }
        _mutexNodes.unlock();
    }

    void iNodeManager::getNodes(std::vector<iNodeID> &nodes, iNodeType nodeType)
    {
        _mutexNodes.lock();
        for (auto node : _nodes)
        {
            if (node.second->getType() == nodeType)
            {
                nodes.push_back(node.first);
            }
        }
        _mutexNodes.unlock();
    }

    void iNodeManager::destroyNode(iNodeID nodeID)
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
        std::map<iNodeID, iNodeID> nodeIDMap;

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

    iNodePtr iNodeManager::createCopyInternal(iNodePtr node, std::map<iNodeID, iNodeID> &nodeIDMap, uint32 recursiveDepth)
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
                result = new iNodeCamera(static_cast<iNodeCamera *>(node));
                break;

            case iNodeType::iCelestialNode:
                con_err("deprecated");
                // result = new iCelestialNode(static_cast<iCelestialNode*>(node));
                break;

            case iNodeType::iNodeLight:
                result = new iNodeLight(static_cast<iNodeLight *>(node));
                break;

            case iNodeType::iNodeMesh:
                result = new iNodeMesh(static_cast<iNodeMesh *>(node));
                break;

            case iNodeType::iNodeModel:
                result = new iNodeModel(static_cast<iNodeModel *>(node));
                break;

            case iNodeType::iNodeSkyBox:
                result = new iNodeSkyBox(static_cast<iNodeSkyBox *>(node));
                break;

            case iNodeType::iSkyLightNode:
                con_err("deprecated");
                // result = new iSkyLightNode(static_cast<iSkyLightNode*>(node));
                break;

            case iNodeType::iNodeTransform:
                result = new iNodeTransform(static_cast<iNodeTransform *>(node));
                break;

            case iNodeType::iNodeSwitch:
                result = new iNodeSwitch(static_cast<iNodeSwitch *>(node));
                break;

            case iNodeType::iNodeLODSwitch:
                result = new iNodeLODSwitch(static_cast<iNodeLODSwitch *>(node));
                break;

            case iNodeType::iNodeLODTrigger:
                result = new iNodeLODTrigger(static_cast<iNodeLODTrigger *>(node));
                break;

            case iNodeType::iNodePhysics:
                result = new iNodePhysics(static_cast<iNodePhysics *>(node));
                break;

            case iNodeType::iNodeParticleSystem:
                result = new iNodeParticleSystem(static_cast<iNodeParticleSystem *>(node));
                break;

            case iNodeType::iNodeEmitter:
                result = new iNodeEmitter(static_cast<iNodeEmitter *>(node));
                break;

            case iNodeType::iNodeWater:
                result = new iNodeWater(static_cast<iNodeWater *>(node));
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

} // namespace igor

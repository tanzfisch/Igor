// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include "iNodeFactory.h"

#include <iaConsole.h>
using namespace IgorAux;

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

#include <iApplication.h>

namespace Igor
{

    iNodeFactory::iNodeFactory()
    {
        iApplication::getInstance().registerApplicationHandleDelegate(iApplicationHandleDelegate(this, &iNodeFactory::flush));
    }

    iNodeFactory::~iNodeFactory()
    {
        iApplication::getInstance().unregisterApplicationHandleDelegate(iApplicationHandleDelegate(this, &iNodeFactory::flush));

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

    void iNodeFactory::flush()
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
            destroyNode(getNode(nodeID));
        }
    }

    iNode* iNodeFactory::getNode(uint32 id)
    {
        con_assert(id != iNode::INVALID_NODE_ID, "invalid id");

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
        destroyNode(getNode(nodeID));
    }

    void iNodeFactory::destroyNode(iNode* node)
    {
        con_assert(nullptr != node, "zero pointer");

        if (nullptr != node)
        {
            bool del = false;
            _mutexNodes.lock();
            auto iter = _nodes.find(node->getID());
            if (_nodes.end() != iter)
            {
                del = true;
                _nodes.erase(iter);
            }
            _mutexNodes.unlock();

            if (del)
            {
                delete node;
            }
        }
        else
        {
            con_err("tried to delete zero pointer");
        }
    }

    void iNodeFactory::destroyNodeAsync(uint32 nodeID)
    {
        destroyNodeAsync(getNode(nodeID));
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

    iNode* iNodeFactory::createCopy(iNode* node, uint32 recursiveDepth)
    {
        iNode* result = nullptr;
        result = createCopyInternal(node);

        if (recursiveDepth > 0)
        {
            for (auto child : node->getChildren())
            {
                result->insertNode(createCopy(child, --recursiveDepth));
            }
        }

        return result;
    }

    iNode* iNodeFactory::createCopyInternal(iNode* node)
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

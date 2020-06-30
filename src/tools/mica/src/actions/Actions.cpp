#include "Actions.h"

#include "ActionContext.h"

#include <igor/system/iClipboard.h>
#include <igor/scene/nodes/iNode.h>
#include <igor/ui/actions/iActionManager.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeSwitch.h>
#include <igor/scene/nodes/iNodeEmitter.h>
#include <igor/scene/nodes/iNodeParticleSystem.h>
using namespace igor;

#include <vector>
#include <any>

ActionDeleteNode::ActionDeleteNode()
    : iAction("mica:deleteNodes")
{
    setPicturePath("icons/delete.png");
    setDescription("delete selection");
}

void ActionDeleteNode::execute(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr ||
        actionContext->getNodes().empty())
    {
        return;
    }

    std::vector<iNodeID> copies;

    for (auto nodeID : actionContext->getNodes())
    {
        iNodeManager::getInstance().destroyNodeAsync(nodeID);
    }
}

ActionAddTransform::ActionAddTransform()
    : iAction("mica:addTransform")
{
    setPicturePath("icons/addTransformation.png");
    setDescription("add transformation");
}

void ActionAddTransform::execute(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return;
    }

    auto nodes = actionContext->getNodes();
    if (nodes.empty())
    {
        nodes.push_back(actionContext->getRootNode());
    }

    for (auto nodeID : nodes)
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        if (node != nullptr)
        {
            node->insertNodeAsync(iNodeManager::getInstance().createNode<iNodeTransform>());
        }
    }
}

ActionAddGroup::ActionAddGroup()
    : iAction("mica:addGroup")
{
    setPicturePath("icons/addGroup.png");
    setDescription("add group");
}

void ActionAddGroup::execute(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return;
    }

    auto nodes = actionContext->getNodes();
    if (nodes.empty())
    {
        nodes.push_back(actionContext->getRootNode());
    }

    for (auto nodeID : nodes)
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        if (node != nullptr)
        {
            node->insertNodeAsync(iNodeManager::getInstance().createNode<iNode>());
        }
    }
}

ActionAddSwitch::ActionAddSwitch()
    : iAction("mica:addSwitch")
{
    setPicturePath("icons/addSwitch.png");
    setDescription("add switch");
}

void ActionAddSwitch::execute(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return;
    }

    auto nodes = actionContext->getNodes();
    if (nodes.empty())
    {
        nodes.push_back(actionContext->getRootNode());
    }

    for (auto nodeID : nodes)
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        if (node != nullptr)
        {
            node->insertNodeAsync(iNodeManager::getInstance().createNode<iNodeSwitch>());
        }
    }
}

ActionAddEmitter::ActionAddEmitter()
    : iAction("mica:addEmitter")
{
    setPicturePath("icons/addEmitter.png");
    setDescription("add emitter");
}

void ActionAddEmitter::execute(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return;
    }

    auto nodes = actionContext->getNodes();
    if (nodes.empty())
    {
        nodes.push_back(actionContext->getRootNode());
    }

    for (auto nodeID : nodes)
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        if (node != nullptr)
        {
            node->insertNodeAsync(iNodeManager::getInstance().createNode<iNodeEmitter>());
        }
    }
}

ActionAddParticleSystem::ActionAddParticleSystem()
    : iAction("mica:addParticleSystem")
{
    setPicturePath("icons/addParticleSystem.png");
    setDescription("add particle system");
}

void ActionAddParticleSystem::execute(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return;
    }

    auto nodes = actionContext->getNodes();
    if (nodes.empty())
    {
        nodes.push_back(actionContext->getRootNode());
    }

    for (auto nodeID : nodes)
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        if (node != nullptr)
        {
            node->insertNodeAsync(iNodeManager::getInstance().createNode<iNodeParticleSystem>());
        }
    }
}

ActionAddModel::ActionAddModel()
    : iAction("mica:addModel")
{
    setPicturePath("icons/addModel.png");
    setDescription("add model");
}

void ActionAddModel::execute(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return;
    }

    actionContext->getOutliner()->addModel();
}

void registerMicaActions()
{
    iActionManager::getInstance().registerAction(new ActionDeleteNode());
    iActionManager::getInstance().registerAction(new ActionAddTransform());
    iActionManager::getInstance().registerAction(new ActionAddGroup());
    iActionManager::getInstance().registerAction(new ActionAddSwitch());
    iActionManager::getInstance().registerAction(new ActionAddEmitter());
    iActionManager::getInstance().registerAction(new ActionAddParticleSystem());
    iActionManager::getInstance().registerAction(new ActionAddModel());
}

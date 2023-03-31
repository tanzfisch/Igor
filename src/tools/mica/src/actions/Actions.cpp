#include "Actions.h"

#include <vector>
#include <any>

ActionDeleteNode::ActionDeleteNode()
    : iAction("mica:deleteNodes")
{
    setPicturePath("igor/icons/delete.png");
    setDescription("delete");
}

void ActionDeleteNode::execute(const iActionContext &context)
{
    const ActionContext *actionContext = static_cast<const ActionContext *>(&context);

    for (auto nodeID : actionContext->getWorkspace()->getSelection())
    {
        iNodeManager::getInstance().destroyNodeAsync(nodeID);
    }
}

bool ActionDeleteNode::isCompatible(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    if (actionContext->getWorkspace()->getSelection().empty())
    {
        return false;
    }

    return true;
}

ActionCopyNode::ActionCopyNode()
    : iAction("mica:copyNodes")
{
    setPicturePath("igor/icons/copy.png");
    setDescription("copy");
}

void ActionCopyNode::execute(const iActionContext &context)
{
    const ActionContext *actionContext = static_cast<const ActionContext *>(&context);
    actionContext->getWorkspace()->copySelected();
}

bool ActionCopyNode::isCompatible(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    if (actionContext->getWorkspace()->getSelection().empty())
    {
        return false;
    }

    return true;
}

ActionPasteNode::ActionPasteNode()
    : iAction("mica:pasteNodes")
{
    setPicturePath("igor/icons/paste.png");
    setDescription("paste");
}

void ActionPasteNode::execute(const iActionContext &context)
{
    const ActionContext *actionContext = static_cast<const ActionContext *>(&context);
    actionContext->getWorkspace()->pasteSelected();
}

bool ActionPasteNode::isCompatible(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    return true;
}

ActionCutNode::ActionCutNode()
    : iAction("mica:cutNodes")
{
    setPicturePath("igor/icons/cut.png");
    setDescription("cut");
}

void ActionCutNode::execute(const iActionContext &context)
{
    const ActionContext *actionContext = static_cast<const ActionContext *>(&context);
    actionContext->getWorkspace()->cutSelected();
}

bool ActionCutNode::isCompatible(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    if (actionContext->getWorkspace()->getSelection().empty())
    {
        return false;
    }

    return true;
}

ActionAddTransform::ActionAddTransform()
    : iAction("mica:addTransform")
{
    setPicturePath("igor/icons/addTransformation.png");
    setDescription("add transformation");
}

void ActionAddTransform::execute(const iActionContext &context)
{
    const ActionContext *actionContext = static_cast<const ActionContext *>(&context);
    auto selection = actionContext->getWorkspace()->getSelection();

    if (selection.empty())
    {
        selection.push_back(actionContext->getWorkspace()->getRootUser()->getID());
    }

    for (auto nodeID : selection)
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        if (node != nullptr)
        {
            node->insertNodeAsync(iNodeManager::getInstance().createNode<iNodeTransform>());
        }
    }
}

bool ActionAddTransform::isCompatible(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    return true;
}

ActionAddGroup::ActionAddGroup()
    : iAction("mica:addGroup")
{
    setPicturePath("igor/icons/addGroup.png");
    setDescription("add group");
}

void ActionAddGroup::execute(const iActionContext &context)
{
    const ActionContext *actionContext = static_cast<const ActionContext *>(&context);
    auto selection = actionContext->getWorkspace()->getSelection();

    if (selection.empty())
    {
        selection.push_back(actionContext->getWorkspace()->getRootUser()->getID());
    }

    for (auto nodeID : selection)
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        if (node != nullptr)
        {
            node->insertNodeAsync(iNodeManager::getInstance().createNode<iNode>());
        }
    }
}

bool ActionAddGroup::isCompatible(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    return true;
}

ActionAddSwitch::ActionAddSwitch()
    : iAction("mica:addSwitch")
{
    setPicturePath("igor/icons/addSwitch.png");
    setDescription("add switch");
}

void ActionAddSwitch::execute(const iActionContext &context)
{
    const ActionContext *actionContext = static_cast<const ActionContext *>(&context);
    auto selection = actionContext->getWorkspace()->getSelection();

    if (selection.empty())
    {
        selection.push_back(actionContext->getWorkspace()->getRootUser()->getID());
    }

    for (auto nodeID : selection)
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        if (node != nullptr)
        {
            node->insertNodeAsync(iNodeManager::getInstance().createNode<iNodeSwitch>());
        }
    }
}

bool ActionAddSwitch::isCompatible(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    return true;
}

ActionAddEmitter::ActionAddEmitter()
    : iAction("mica:addEmitter")
{
    setPicturePath("igor/icons/addEmitter.png");
    setDescription("add emitter");
}

void ActionAddEmitter::execute(const iActionContext &context)
{
    const ActionContext *actionContext = static_cast<const ActionContext *>(&context);
    auto selection = actionContext->getWorkspace()->getSelection();

    if (selection.empty())
    {
        selection.push_back(actionContext->getWorkspace()->getRootUser()->getID());
    }

    for (auto nodeID : selection)
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        if (node != nullptr)
        {
            node->insertNodeAsync(iNodeManager::getInstance().createNode<iNodeEmitter>());
        }
    }
}

bool ActionAddEmitter::isCompatible(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    return true;
}

ActionAddParticleSystem::ActionAddParticleSystem()
    : iAction("mica:addParticleSystem")
{
    setPicturePath("igor/icons/addParticleSystem.png");
    setDescription("add particle system");
}

void ActionAddParticleSystem::execute(const iActionContext &context)
{
    const ActionContext *actionContext = static_cast<const ActionContext *>(&context);
    auto selection = actionContext->getWorkspace()->getSelection();

    if (selection.empty())
    {
        selection.push_back(actionContext->getWorkspace()->getRootUser()->getID());
    }

    for (auto nodeID : selection)
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        if (node != nullptr)
        {
            node->insertNodeAsync(iNodeManager::getInstance().createNode<iNodeParticleSystem>());
        }
    }
}

bool ActionAddParticleSystem::isCompatible(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    return true;
}

ActionAddModel::ActionAddModel()
    : iAction("mica:addModel")
{
    setPicturePath("igor/icons/addModel.png");
    setDescription("add model");
}

void ActionAddModel::execute(const iActionContext &context)
{
    const ActionContext *actionContext = static_cast<const ActionContext *>(&context);
    actionContext->getOutliner()->addModel();
}

bool ActionAddModel::isCompatible(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    return true;
}

ActionBakeMeshToWorld::ActionBakeMeshToWorld()
    : iAction("mica:bakeMeshToWorld")
{
    // TODO setPicturePath("igor/icons/addModel.png");
    setDescription("bake mesh to world");
}

void ActionBakeMeshToWorld::execute(const iActionContext &context)
{
    const ActionContext *actionContext = static_cast<const ActionContext *>(&context);
    for (auto nodeID : actionContext->getWorkspace()->getSelection())
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        bakeToWorld(static_cast<iNodeMesh *>(node), actionContext->getWorkspace()->getRootUser());
    }
}

bool ActionBakeMeshToWorld::isCompatible(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    if (actionContext->getWorkspace()->getSelection().empty())
    {
        return false;
    }

    for (auto nodeID : actionContext->getWorkspace()->getSelection())
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        if (node == nullptr || node->getType() != iNodeType::iNodeMesh)
        {
            return false;
        }
    }

    return true;
}

void ActionBakeMeshToWorld::bakeToWorld(iNodeMeshPtr meshNode, iNodePtr root)
{
    iaMatrixd matrix;
    meshNode->calcWorldTransformation(matrix);

    iMeshBuilder meshBuilder;
    meshBuilder.setJoinVertexes(false);
    meshBuilder.setMatrix(matrix.convert<float32>());
    iMeshPtr mesh = meshNode->getMesh();
    iMeshBuilderUtils::addMesh(meshBuilder, mesh);

    iMeshPtr newMesh = meshBuilder.createMesh();
    newMesh->setKeepRawData(true);

    iNodeMesh *newMeshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
    newMeshNode->setMesh(newMesh);
    newMeshNode->setMaterial(meshNode->getMaterial());
    newMeshNode->setTargetMaterial(meshNode->getTargetMaterial());

    root->insertNode(newMeshNode);
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
    iActionManager::getInstance().registerAction(new ActionBakeMeshToWorld());
    iActionManager::getInstance().registerAction(new ActionCopyNode());
    iActionManager::getInstance().registerAction(new ActionPasteNode());
    iActionManager::getInstance().registerAction(new ActionCutNode());
}

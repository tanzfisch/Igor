#include "Actions.h"

#include <vector>
#include <any>

ActionSetEntityActive::ActionSetEntityActive()
    : iAction("mica:set_entity_active")
{
    //        setIcon("igor_icon_delete");
    setDescription("Activate entity");
}

void ActionSetEntityActive::execute(const iActionContext &context)
{
    const iEntityActionContext *actionContext = static_cast<const iEntityActionContext *>(&context);

    auto scene = iEntitySystemModule::getInstance().getScene(actionContext->getSceneID());
    for(auto entityID : actionContext->getEntities())
    {
        auto entity = scene->getEntity(entityID);
        entity->setActive(true);
    }
}

bool ActionSetEntityActive::isCompatible(const iActionContext &context)
{
    const iEntityActionContext *actionContext = dynamic_cast<const iEntityActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    if (actionContext->getEntities().empty())
    {
        return false;
    }

    auto scene = iEntitySystemModule::getInstance().getScene(actionContext->getSceneID());
    if(scene == nullptr)
    {
        return false;
    }

    return true;
}

ActionSetEntityInactive::ActionSetEntityInactive()
    : iAction("mica:set_entity_inactive")
{
    //        setIcon("igor_icon_delete");
    setDescription("Deactivate entity");
}

void ActionSetEntityInactive::execute(const iActionContext &context)
{
    const iEntityActionContext *actionContext = static_cast<const iEntityActionContext *>(&context);

    auto scene = iEntitySystemModule::getInstance().getScene(actionContext->getSceneID());
    for(auto entityID : actionContext->getEntities())
    {
        auto entity = scene->getEntity(entityID);
        entity->setActive(false);
    }    
}

bool ActionSetEntityInactive::isCompatible(const iActionContext &context)
{
    const iEntityActionContext *actionContext = dynamic_cast<const iEntityActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    if (actionContext->getEntities().empty())
    {
        return false;
    }

    auto scene = iEntitySystemModule::getInstance().getScene(actionContext->getSceneID());
    if(scene == nullptr)
    {
        return false;
    }    

    return true;    
}

ActionDeleteNode::ActionDeleteNode()
    : iAction("mica:deleteNodes")
{
    setIcon("igor_icon_delete");
    setDescription("delete");
}

void ActionDeleteNode::execute(const iActionContext &context)
{
    const MicaActionContext *actionContext = static_cast<const MicaActionContext *>(&context);

    for (auto nodeID : actionContext->getWorkspace()->getSelection())
    {
        iNodeManager::getInstance().destroyNodeAsync(nodeID);
    }
}

bool ActionDeleteNode::isCompatible(const iActionContext &context)
{
    const MicaActionContext *actionContext = dynamic_cast<const MicaActionContext *>(&context);
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
    setIcon("igor_icon_copy");
    setDescription("copy");
}

void ActionCopyNode::execute(const iActionContext &context)
{
    const MicaActionContext *actionContext = static_cast<const MicaActionContext *>(&context);
    actionContext->getWorkspace()->copySelected();
}

bool ActionCopyNode::isCompatible(const iActionContext &context)
{
    const MicaActionContext *actionContext = dynamic_cast<const MicaActionContext *>(&context);
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
    setIcon("igor_icon_paste");
    setDescription("paste");
}

void ActionPasteNode::execute(const iActionContext &context)
{
    const MicaActionContext *actionContext = static_cast<const MicaActionContext *>(&context);
    actionContext->getWorkspace()->pasteSelected();
}

bool ActionPasteNode::isCompatible(const iActionContext &context)
{
    const MicaActionContext *actionContext = dynamic_cast<const MicaActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    return true;
}

ActionCutNode::ActionCutNode()
    : iAction("mica:cutNodes")
{
    setIcon("igor_icon_cut");
    setDescription("cut");
}

void ActionCutNode::execute(const iActionContext &context)
{
    const MicaActionContext *actionContext = static_cast<const MicaActionContext *>(&context);
    actionContext->getWorkspace()->cutSelected();
}

bool ActionCutNode::isCompatible(const iActionContext &context)
{
    const MicaActionContext *actionContext = dynamic_cast<const MicaActionContext *>(&context);
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
    setIcon("igor_icon_add_transformation");
    setDescription("add transformation");
}

void ActionAddTransform::execute(const iActionContext &context)
{
    const MicaActionContext *actionContext = static_cast<const MicaActionContext *>(&context);
    auto selection = actionContext->getWorkspace()->getSelection();

    if (selection.empty())
    {
        selection.push_back(actionContext->getWorkspace()->getUserScene()->getID());
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
    const MicaActionContext *actionContext = dynamic_cast<const MicaActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    return true;
}

ActionAddGroup::ActionAddGroup()
    : iAction("mica:addGroup")
{
    setIcon("igor_icon_add_group");
    setDescription("add group");
}

void ActionAddGroup::execute(const iActionContext &context)
{
    const MicaActionContext *actionContext = static_cast<const MicaActionContext *>(&context);
    auto selection = actionContext->getWorkspace()->getSelection();

    if (selection.empty())
    {
        selection.push_back(actionContext->getWorkspace()->getUserScene()->getID());
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
    const MicaActionContext *actionContext = dynamic_cast<const MicaActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    return true;
}

ActionAddSwitch::ActionAddSwitch()
    : iAction("mica:addSwitch")
{
    setIcon("igor_icon_add_switch");
    setDescription("add switch");
}

void ActionAddSwitch::execute(const iActionContext &context)
{
    const MicaActionContext *actionContext = static_cast<const MicaActionContext *>(&context);
    auto selection = actionContext->getWorkspace()->getSelection();

    if (selection.empty())
    {
        selection.push_back(actionContext->getWorkspace()->getUserScene()->getID());
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
    const MicaActionContext *actionContext = dynamic_cast<const MicaActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    return true;
}

ActionAddEmitter::ActionAddEmitter()
    : iAction("mica:addEmitter")
{
    setIcon("igor_icon_add_emitter");
    setDescription("add emitter");
}

void ActionAddEmitter::execute(const iActionContext &context)
{
    const MicaActionContext *actionContext = static_cast<const MicaActionContext *>(&context);
    auto selection = actionContext->getWorkspace()->getSelection();

    if (selection.empty())
    {
        selection.push_back(actionContext->getWorkspace()->getUserScene()->getID());
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
    const MicaActionContext *actionContext = dynamic_cast<const MicaActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    return true;
}

ActionAddParticleSystem::ActionAddParticleSystem()
    : iAction("mica:addParticleSystem")
{
    setIcon("igor_icon_add_particle_system");
    setDescription("add particle system");
}

void ActionAddParticleSystem::execute(const iActionContext &context)
{
    const MicaActionContext *actionContext = static_cast<const MicaActionContext *>(&context);
    auto selection = actionContext->getWorkspace()->getSelection();

    if (selection.empty())
    {
        selection.push_back(actionContext->getWorkspace()->getUserScene()->getID());
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
    const MicaActionContext *actionContext = dynamic_cast<const MicaActionContext *>(&context);
    if (actionContext == nullptr)
    {
        return false;
    }

    return true;
}

ActionBakeMeshToWorld::ActionBakeMeshToWorld()
    : iAction("mica:bakeMeshToWorld")
{
    // TODO setIcon("igor_icon_add_model");
    setDescription("bake mesh to world");
}

void ActionBakeMeshToWorld::execute(const iActionContext &context)
{
    const MicaActionContext *actionContext = static_cast<const MicaActionContext *>(&context);
    for (auto nodeID : actionContext->getWorkspace()->getSelection())
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        bakeToWorld(static_cast<iNodeMesh *>(node), actionContext->getWorkspace()->getUserScene());
    }
}

bool ActionBakeMeshToWorld::isCompatible(const iActionContext &context)
{
    const MicaActionContext *actionContext = dynamic_cast<const MicaActionContext *>(&context);
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
    meshBuilder.setJoinVertices(false);
    meshBuilder.setMatrix(matrix.convert<float32>());
    iMeshPtr mesh = meshNode->getMesh();
    iMeshBuilderUtils::addMesh(meshBuilder, mesh);

    iMeshPtr newMesh = meshBuilder.createMesh();
    newMesh->setKeepRawData(true);

    iNodeMesh *newMeshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
    newMeshNode->setMesh(newMesh);
    newMeshNode->setMaterial(meshNode->getMaterial());
    newMeshNode->setMaterial(meshNode->getMaterial());

    root->insertNode(newMeshNode);
}

void registerMicaActions()
{
    iActionManager::getInstance().registerAction(new ActionSetEntityActive());
    iActionManager::getInstance().registerAction(new ActionSetEntityInactive());

    // deprecated
    iActionManager::getInstance().registerAction(new ActionDeleteNode());
    iActionManager::getInstance().registerAction(new ActionAddTransform());
    iActionManager::getInstance().registerAction(new ActionAddGroup());
    iActionManager::getInstance().registerAction(new ActionAddSwitch());
    iActionManager::getInstance().registerAction(new ActionAddEmitter());
    iActionManager::getInstance().registerAction(new ActionAddParticleSystem());
    iActionManager::getInstance().registerAction(new ActionBakeMeshToWorld());
    iActionManager::getInstance().registerAction(new ActionCopyNode());
    iActionManager::getInstance().registerAction(new ActionPasteNode());
    iActionManager::getInstance().registerAction(new ActionCutNode());
}

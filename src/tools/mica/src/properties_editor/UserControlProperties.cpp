// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlProperties.h"

#include "nodes/UserControlNode.h"
#include "nodes/UserControlTransformation.h"

UserControlProperties::UserControlProperties(iNodeID nodeID, const iWidgetPtr parent)
    : iUserControl(iWidgetType::iUserControl, parent)
{
    initUI();
    initNodeUI(nodeID);
}

UserControlProperties::UserControlProperties(const iResourceID &resourceID, const iWidgetPtr parent)
    : iUserControl(iWidgetType::iUserControl, parent)
{
    initUI();
    initResourceUI(resourceID);
}

void UserControlProperties::initUI()
{
    setHorizontalAlignment(iHorizontalAlignment::Left);
    setVerticalAlignment(iVerticalAlignment::Top);
    setMinWidth(340);
    setMinHeight(0);

    _layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    _layout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
}

void UserControlProperties::initNodeUI(iNodeID nodeID)
{
    iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
    if (node == nullptr)
    {
        return;
    }

    UserControlNode *userControl = nullptr;

    switch (node->getType())
    {
    case iNodeType::iNode:
        // nothing to do
        break;

    case iNodeType::iNodeTransform:
        userControl = new UserControlTransformation(nodeID, _layout);
        break;

    case iNodeType::iNodeLight:
        // initLightNode();
        break;

    case iNodeType::iNodeMesh:
        // initMeshNode();
        break;

    case iNodeType::iNodeModel:
        // initModel();
        break;

    case iNodeType::iNodeEmitter:
        // initEmitter();
        break;

    case iNodeType::iNodeParticleSystem:
        // initParticleSystem();
        break;

    default:
        con_warn("not implemented");
    }

    if (userControl == nullptr)
    {
        return;
    }

    userControl->init();
    userControl->update();
}

void UserControlProperties::initResourceUI(const iResourceID &resourceID)
{
}
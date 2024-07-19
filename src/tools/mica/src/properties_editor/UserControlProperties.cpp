// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlProperties.h"

#include "nodes/UserControlNode.h"
#include "nodes/UserControlTransformation.h"
#include "nodes/UserControlParticleSystem.h"
#include "nodes/UserControlLight.h"
#include "nodes/UserControlEmitter.h"
#include "nodes/UserControlMesh.h"
#include "nodes/UserControlModel.h"

#include "resources/UserControlResource.h"
#include "resources/UserControlShaderMaterial.h"
#include "resources/UserControlMaterial.h"
#include "resources/UserControlTexture.h"

#include "entities/UserControlEntity.h"

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

UserControlProperties::UserControlProperties(const iEntitySceneID &sceneID, const iEntityID &entityID, const iWidgetPtr parent)
{
    initUI();
    initEntityUI(sceneID, entityID);
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
        userControl = new UserControlLight(nodeID, _layout);
        break;

    case iNodeType::iNodeMesh:
        userControl = new UserControlMesh(nodeID, _layout);
        break;

    case iNodeType::iNodeModel:
        userControl = new UserControlModel(nodeID, _layout);
        break;

    case iNodeType::iNodeEmitter:
        userControl = new UserControlEmitter(nodeID, _layout);
        break;

    case iNodeType::iNodeParticleSystem:
        userControl = new UserControlParticleSystem(nodeID, _layout);
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

void UserControlProperties::initEntityUI(const iEntitySceneID &sceneID, const iEntityID &entityID)
{
    UserControlEntity *userControl = new UserControlEntity(sceneID, entityID, _layout);
    userControl->init();
    userControl->update();    
}

void UserControlProperties::initResourceUI(const iResourceID &resourceID)
{
    iaString resourceType = iResourceManager::getInstance().getType(resourceID);
    UserControlResource *userControl = nullptr;

    if (resourceType == IGOR_RESOURCE_SHADER)
    {
        userControl = new UserControlShaderMaterial(resourceID, _layout);
    }
    if (resourceType == IGOR_RESOURCE_MATERIAL)
    {
        userControl = new UserControlMaterial(resourceID, _layout);
    }
    else if (resourceType == "texture")
    {
        userControl = new UserControlTexture(resourceID, _layout);
    }

    if (userControl == nullptr)
    {
        userControl = new UserControlResource(resourceID, _layout);
    }

    userControl->init();
    userControl->update();
}
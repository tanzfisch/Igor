// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlProperties.h"

#include "resources/UserControlResource.h"
#include "resources/UserControlShaderMaterial.h"
#include "resources/UserControlMaterial.h"
#include "resources/UserControlTexture.h"

#include "entities/UserControlEntity.h"

UserControlProperties::UserControlProperties(PropertyType propertyType, const std::vector<iaUUID> &id, const iWidgetPtr parent)
    : iUserControl(iWidgetType::iUserControl, parent)
{
    con_assert(!id.empty(), "no ids");

    initUI();

    switch (propertyType)
    {
    case PropertyType::Resource:
        initResourceUI(id.front());
        break;
    case PropertyType::Entity:
        con_assert(id.size() == 2, "invalid ids");
        initEntityUI(id[0], id[1]);
        break;
    }
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

void UserControlProperties::initEntityUI(const iEntitySceneID &sceneID, const iEntityID &entityID)
{
    if (iEntitySystemModule::getInstance().getScene(sceneID) == nullptr)
    {
        return;
    }

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
    else if (resourceType == IGOR_RESOURCE_TEXTURE)
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
// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlResourceScript.h"

#include "../../MicaDefines.h"

UserControlResourceScript::UserControlResourceScript(iResourceID resourceID, const iWidgetPtr parent)
    : UserControlResource(resourceID, parent)
{
}

void UserControlResourceScript::onInit()
{
    UserControlResource::onInit();

    iWidgetBoxLayoutPtr mainLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, getLayout());
    mainLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    mainLayout->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGroupBox *lightGroupBox = new iWidgetGroupBox(mainLayout);
    lightGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    lightGroupBox->setText("Script");
    lightGroupBox->setHeaderOnly();
}

void UserControlResourceScript::onUpdateResource()
{
    iScriptPtr script = iResourceManager::getInstance().getResource<iScript>(getResourceID());

    if (script == nullptr)
    {
        return;
    }

    // TODO

    iResourceManager::getInstance().saveResource(getResourceID());
}

void UserControlResourceScript::onUpdateUI()
{
    UserControlResource::onUpdateUI();

    iScriptPtr script = iResourceManager::getInstance().getResource<iScript>(getResourceID());

    if (script == nullptr)
    {
        return;
    }    
}
// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponent.h"

UserControlComponent::UserControlComponent(const iEntitySceneID &scene, const iEntityID &entity, const iaString &typeName, const iWidgetPtr parent)
    : iUserControl(iWidgetType::iUserControl, parent), _sceneID(scene), _entityID(entity), _typeName(typeName)
{
    con_assert(iEntitySystemModule::getInstance().getScene(_sceneID) != nullptr, "invalid scene id");
    con_assert(iEntitySystemModule::getInstance().getScene(_sceneID)->getEntity(_entityID) != nullptr, "invalid entity id");
}

void UserControlComponent::init()
{
    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetGroupBoxPtr groupBox = new iWidgetGroupBox(this);
    groupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    groupBox->setText(_typeName);
    groupBox->setHeaderOnly();

    _layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, groupBox);
    _layout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _layout->setVerticalAlignment(iVerticalAlignment::Top);
}

void UserControlComponent::update()
{
    // nothing to do
}

void UserControlComponent::updateComponent()
{
    // nothing to do
}

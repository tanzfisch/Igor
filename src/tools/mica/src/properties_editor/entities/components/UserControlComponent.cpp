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

    _buttonlayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    _buttonlayout->setHorizontalAlignment(iHorizontalAlignment::Right);

    iWidgetButtonPtr deleteButton = new iWidgetButton();
    deleteButton->setIcon("igor_icon_delete");
    deleteButton->getClickEvent().add(iClickDelegate(this, &UserControlComponent::onClickDelete));
    _buttonlayout->addWidget(deleteButton);
}

void UserControlComponent::onClickDelete(iWidgetPtr source)
{
    auto scene = iEntitySystemModule::getInstance().getScene(_sceneID);
    if(scene == nullptr)
    {
        return;
    }

    auto entity = scene->getEntity(_entityID);
    if(entity == nullptr)
    {
        return;
    }

    onDestroyComponent(entity);
}

void UserControlComponent::update()
{
    // nothing to do
}

void UserControlComponent::updateComponent()
{
    // nothing to do
}

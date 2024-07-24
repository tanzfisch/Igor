// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
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
    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(_sceneID);
    if(scene == nullptr)
    {
        return;
    }

    iEntityPtr entity = scene->getEntity(_entityID);
    if(entity == nullptr)
    {
        return;
    }

    _ignoreUpdate = true;

    // TODO

    _ignoreUpdate = false;
}

void UserControlComponent::updateComponent()
{
    if(_ignoreUpdate)
    {
        return;
    }    

    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(_sceneID);
    if(scene == nullptr)
    {
        return;
    }

    iEntityPtr entity = scene->getEntity(_entityID);
    if(entity == nullptr)
    {
        return;
    }    

    // TODO
}

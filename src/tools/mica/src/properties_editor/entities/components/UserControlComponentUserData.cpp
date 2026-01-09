// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentUserData.h"

#include <igor/entities/components/iUserDataComponent.h>

UserControlComponentUserData::UserControlComponentUserData(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "User Data", parent)
{
}

void UserControlComponentUserData::onInit()
{
    UserControlComponent::onInit();

    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    // TODO
}

void UserControlComponentUserData::onUpdateUI()
{
    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(_sceneID);
    if (scene == nullptr)
    {
        return;
    }

    iEntityPtr entity = scene->getEntity(_entityID);
    if (entity == nullptr)
    {
        return;
    }

    iUserDataComponentPtr component = entity->getComponent<iUserDataComponent>();
    if (component == nullptr)
    {
        return;
    }

    _ignoreUpdate = true;

    // TODO

    _ignoreUpdate = false;
}

void UserControlComponentUserData::onUpdateComponent()
{
    if (_ignoreUpdate)
    {
        return;
    }

    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(_sceneID);
    if (scene == nullptr)
    {
        return;
    }

    iEntityPtr entity = scene->getEntity(_entityID);
    if (entity == nullptr)
    {
        return;
    }

    iUserDataComponentPtr component = entity->getComponent<iUserDataComponent>();
    if (component == nullptr)
    {
        return;
    }

    // TODO
}

void UserControlComponentUserData::onDestroyComponent(iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");
    
    entity->destroyComponent<iUserDataComponent>();
}
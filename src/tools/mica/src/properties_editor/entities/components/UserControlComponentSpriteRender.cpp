// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentSpriteRender.h"

#include <igor/entities/components/iSpriteRenderComponent.h>

UserControlComponentSpriteRender::UserControlComponentSpriteRender(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Sprite Renderer", parent)
{
}

void UserControlComponentSpriteRender::init()
{
    UserControlComponent::init();

    setHorizontalAlignment(iHorizontalAlignment::Stretch);

 /*   iWidgetBoxLayoutPtr meshCountLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    meshCountLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    meshCountLayout->setStretchIndex(1);
    iWidgetLabelPtr labelMesh = new iWidgetLabel(meshCountLayout);
    labelMesh->setText("Meshes");
    labelMesh->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelMesh->setHorizontalAlignment(iHorizontalAlignment::Left);

    _labelMeshCount = new iWidgetLabel(meshCountLayout);
    _labelMeshCount->setHorizontalAlignment(iHorizontalAlignment::Left);    */
}

void UserControlComponentSpriteRender::onValueChanged(iWidgetPtr source)
{
    updateComponent();
}

void UserControlComponentSpriteRender::update()
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

    iSpriteRenderComponent *component = entity->getComponent<iSpriteRenderComponent>();
    if (component == nullptr)
    {
        return;
    }

    _ignoreUpdate = true;

    // TODO

    _ignoreUpdate = false;
}

void UserControlComponentSpriteRender::updateComponent()
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

    iSpriteRenderComponent *component = entity->getComponent<iSpriteRenderComponent>();
    if (component == nullptr)
    {
        return;
    }

    // TODO 
}

void UserControlComponentSpriteRender::onDestroyComponent(iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");
    
    entity->destroyComponent<iSpriteRenderComponent>();
}
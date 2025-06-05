// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentSpriteRender.h"

#include <igor/entities/components/iSpriteRenderComponent.h>

UserControlComponentSpriteRender::UserControlComponentSpriteRender(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Sprite Renderer", parent)
{
}

void UserControlComponentSpriteRender::onInit()
{
    UserControlComponent::onInit();

    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetBoxLayoutPtr spriteLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    spriteLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    spriteLayout->setStretchIndex(1);
    iWidgetLabelPtr labelSprite = new iWidgetLabel(spriteLayout);
    labelSprite->setText("Sprite");
    labelSprite->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelSprite->setHorizontalAlignment(iHorizontalAlignment::Left);

    _sprite = new iUserControlSprite(spriteLayout);

    // size
    // color
    // zindex
    // render mode
    // frame index
}

void UserControlComponentSpriteRender::onValueChanged(iWidgetPtr source)
{
    onUpdateComponent();
}

void UserControlComponentSpriteRender::onUpdateUI()
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

    if (component->getSprite() != nullptr)
    {
        _sprite->setID(component->getSprite()->getID());
    }
    else
    {
        _sprite->setID(iResourceID::getInvalid());
    }

    _ignoreUpdate = false;
}

void UserControlComponentSpriteRender::onUpdateComponent()
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

    component->setSprite(iResourceManager::getInstance().getResource<iSprite>(_sprite->getID()));
}

void UserControlComponentSpriteRender::onDestroyComponent(iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");

    entity->destroyComponent<iSpriteRenderComponent>();
}
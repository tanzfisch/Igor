// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "LightOverlay.h"

LightOverlay::LightOverlay(iViewPtr view)
    : EntityOverlay(view)
{
    onInit();
}

LightOverlay::~LightOverlay()
{
    onDeinit();
}

bool LightOverlay::accepts(OverlayMode mode, iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");
    return entity->getComponent<iLightComponent>() != nullptr;
}

void LightOverlay::onDeinit()
{
}

void LightOverlay::onInit()
{
    getView()->getRenderEvent().add(iPreRenderDelegate(this, &LightOverlay::onPreRender));

    const auto &entitySceneID = getView()->getSceneID();
    auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
    con_assert(entityScene != nullptr, "no scene");

    _lightRoot = entityScene->createEntity("overlay.light.root");
    _lightRoot->addComponent(new iTransformComponent());
    _lightRoot->addComponent(new iSphereComponent(1.0));
    _lightRoot->addComponent(new iOctreeComponent());

    iSpritePtr wagiu = iResourceManager::getInstance().createResource<iSprite>();
    wagiu->setTexture(iResourceManager::getInstance().requestResource<iTexture>("example_texture_supremacy_wagiu_a5"));
    // _lightRoot->addComponent(new iSpriteRenderComponent(wagiu, iaVector2d(3, 3), iaColor4f::white, 0, iSpriteRenderComponent::iRenderMode::Billboard));

    _lightRoot->setActive(false);
}

void LightOverlay::setActive(bool active)
{
    EntityOverlay::setActive(active);
    _lightRoot->setActive(active);
}

void LightOverlay::onUpdate()
{
    if (!isActive())
    {
        return;
    }

    auto entityScene = iEntitySystemModule::getInstance().getScene(getSceneID());
    if (entityScene == nullptr)
    {
        return;
    }

    auto entity = entityScene->getEntity(getEntityID());
    if (entity == nullptr)
    {
        setActive(false);
        return;
    }

    auto transformComp = entity->getComponent<iTransformComponent>();
    if (transformComp == nullptr)
    {
        return;
    }    

    auto lightTransformComp = _lightRoot->getComponent<iTransformComponent>();
    lightTransformComp->setPosition(transformComp->getWorldPosition());
}

void LightOverlay::onPreRender()
{
    onUpdate();
}
// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentAnimation.h"

UserControlComponentAnimation::UserControlComponentAnimation(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Animation", parent)
{
}

void UserControlComponentAnimation::onInit()
{
    UserControlComponent::onInit();

    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetBoxLayoutPtr clipsLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    clipsLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    clipsLayout->setStretchIndex(1);
    iWidgetLabelPtr labelClips = new iWidgetLabel(clipsLayout);
    labelClips->setText("Clips");
    labelClips->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    labelClips->setHorizontalAlignment(iHorizontalAlignment::Left);

    // TODO for now just add one clip

    // TODO add a list of clips
    // define start clip
    // add list of clip-to-clip transitions
}

void UserControlComponentAnimation::onValueChanged(iWidgetPtr source)
{
    onUpdateComponent();
}

void UserControlComponentAnimation::onUpdateUI()
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

    iAnimationComponent *component = entity->getComponent<iAnimationComponent>();
    if (component == nullptr)
    {
        return;
    }

    _ignoreUpdate = true;

    // TODO update ui

    _ignoreUpdate = false;
}

void UserControlComponentAnimation::onUpdateComponent()
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

    iAnimationComponent *component = entity->getComponent<iAnimationComponent>();
    if (component == nullptr)
    {
        return;
    }

    // TODO
}

void UserControlComponentAnimation::onDestroyComponent(iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");
    
    entity->destroyComponent<iCircleComponent>();
}
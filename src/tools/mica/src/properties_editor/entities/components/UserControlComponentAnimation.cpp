// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
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

    iWidgetBoxLayoutPtr radiusLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    radiusLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    radiusLayout->setStretchIndex(1);
    iWidgetLabelPtr labelRadius = new iWidgetLabel(radiusLayout);
    labelRadius->setText("Radius");
    labelRadius->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelRadius->setHorizontalAlignment(iHorizontalAlignment::Left);

    /*_radius = new iWidgetLineTextEdit(radiusLayout);
    _radius->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _radius->getChangeEvent().add(iChangeDelegate(this, &UserControlComponentAnimation::onValueChanged));

    iWidgetBoxLayoutPtr offsetLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    offsetLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    offsetLayout->setStretchIndex(1);
    iWidgetLabelPtr labelOffset = new iWidgetLabel(offsetLayout);
    labelOffset->setText("Offset");
    labelOffset->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelOffset->setHorizontalAlignment(iHorizontalAlignment::Left);

    _offset = new iUserControlVector(2, offsetLayout);
    _offset->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _offset->getChangeEvent().add(iChangeDelegate(this, &UserControlComponentAnimation::onValueChanged));*/
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

    /*_radius->setText(iaString::toString(component->getRadius(), 4));
    _offset->setValue(0, component->getOffset()._x);
    _offset->setValue(1, component->getOffset()._y);*/

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

    /*component->setRadius(iaString::toFloat(_radius->getText()));
    iaVector2d offset(_offset->getValue(0), _offset->getValue(1));
    component->setOffset(offset);*/
}

void UserControlComponentAnimation::onDestroyComponent(iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");
    
    entity->destroyComponent<iCircleComponent>();
}
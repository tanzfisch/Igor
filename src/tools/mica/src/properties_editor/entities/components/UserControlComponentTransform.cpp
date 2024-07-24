// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlComponentTransform.h"

#include <igor/entities/components/iTransformComponent.h>

UserControlComponentTransform::UserControlComponentTransform(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Transform", parent)
{
}

void UserControlComponentTransform::init()
{
    UserControlComponent::init();

    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    // use icons instead of labels if it get's too tight

    iWidgetBoxLayoutPtr positionLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    positionLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    positionLayout->setStretchIndex(1);
    iWidgetLabelPtr labelPosition = new iWidgetLabel(positionLayout);
    labelPosition->setText("Pos");
    labelPosition->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelPosition->setHorizontalAlignment(iHorizontalAlignment::Left);

    _position = new iUserControlVector(3, positionLayout);
    _position->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _position->setEnabled(true);
    _position->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentTransform::onValueChanged));

    iWidgetBoxLayoutPtr orientationLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    orientationLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    orientationLayout->setStretchIndex(1);
    iWidgetLabelPtr labelOrientation = new iWidgetLabel(orientationLayout);
    labelOrientation->setText("Ori");
    labelOrientation->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelOrientation->setHorizontalAlignment(iHorizontalAlignment::Left);

    _orientation = new iUserControlVector(3, orientationLayout);
    _orientation->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _orientation->setEnabled(true);
    _orientation->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentTransform::onValueChanged));    

    iWidgetBoxLayoutPtr scaleLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    scaleLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    scaleLayout->setStretchIndex(1);
    iWidgetLabelPtr labelScale = new iWidgetLabel(scaleLayout);
    labelScale->setText("Ori");
    labelScale->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelScale->setHorizontalAlignment(iHorizontalAlignment::Left);

    _scale = new iUserControlVector(3, scaleLayout);
    _scale->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _scale->setEnabled(true);
    _scale->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentTransform::onValueChanged));       
}

void UserControlComponentTransform::onValueChanged(iWidgetPtr source)
{
    updateComponent();
}

void UserControlComponentTransform::update()
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

    iTransformComponent* transform = entity->getComponent<iTransformComponent>();
    if(transform == nullptr)
    {
        return;
    }

    _ignoreUpdate = true;

    _position->setValue(0, transform->getPosition()._x);
    _position->setValue(1, transform->getPosition()._y);
    _position->setValue(2, transform->getPosition()._z);
    _orientation->setValue(0, transform->getOrientation()._x);
    _orientation->setValue(1, transform->getOrientation()._y);
    _orientation->setValue(2, transform->getOrientation()._z);
    _scale->setValue(0, transform->getScale()._x);
    _scale->setValue(1, transform->getScale()._y);
    _scale->setValue(2, transform->getScale()._z);

    _ignoreUpdate = false;
}

void UserControlComponentTransform::updateComponent()
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

    // TODO
}

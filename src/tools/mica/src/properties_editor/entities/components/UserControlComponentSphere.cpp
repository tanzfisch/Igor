// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentSphere.h"

#include <igor/entities/components/iSphereComponent.h>

UserControlComponentSphere::UserControlComponentSphere(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Sphere", parent)
{
}

void UserControlComponentSphere::init()
{
    UserControlComponent::init();

    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetBoxLayoutPtr radiusLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    radiusLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    radiusLayout->setStretchIndex(1);
    iWidgetLabelPtr labelRadius = new iWidgetLabel(radiusLayout);
    labelRadius->setText("Radius");
    labelRadius->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelRadius->setHorizontalAlignment(iHorizontalAlignment::Left);

    _radius = new iWidgetLineTextEdit(radiusLayout);
    _radius->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _radius->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentSphere::onValueChanged));

    iWidgetBoxLayoutPtr offsetLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    offsetLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    offsetLayout->setStretchIndex(1);
    iWidgetLabelPtr labelOffset = new iWidgetLabel(offsetLayout);
    labelOffset->setText("Offset");
    labelOffset->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelOffset->setHorizontalAlignment(iHorizontalAlignment::Left);

    _offset = new iUserControlVector(3, offsetLayout);
    _offset->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _offset->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentSphere::onValueChanged));
}

void UserControlComponentSphere::onValueChanged(iWidgetPtr source)
{
    updateComponent();
}

void UserControlComponentSphere::update()
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

    iSphereComponent *component = entity->getComponent<iSphereComponent>();
    if (component == nullptr)
    {
        return;
    }

    _ignoreUpdate = true;

    _radius->setText(iaString::toString(component->getRadius(), 4));
    _offset->setValue(0, component->getOffset()._x);
    _offset->setValue(1, component->getOffset()._y);
    _offset->setValue(2, component->getOffset()._z);

    _ignoreUpdate = false;
}

void UserControlComponentSphere::updateComponent()
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

    iSphereComponent *component = entity->getComponent<iSphereComponent>();
    if (component == nullptr)
    {
        return;
    }

    component->setRadius(iaString::toFloat(_radius->getText()));
    const iaVector3d offset(_offset->getValue(0), _offset->getValue(1), _offset->getValue(2));
    component->setOffset(offset);
}

void UserControlComponentSphere::onDestroyComponent(iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");
    
    entity->destroyComponent<iSphereComponent>();
}
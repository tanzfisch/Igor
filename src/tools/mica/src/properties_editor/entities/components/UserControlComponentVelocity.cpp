// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentVelocity.h"

#include <igor/entities/components/iCircleComponent.h>

UserControlComponentVelocity::UserControlComponentVelocity(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Velocity", parent)
{
}

void UserControlComponentVelocity::onInit()
{
    UserControlComponent::onInit();

    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetBoxLayoutPtr velocityLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    velocityLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    velocityLayout->setStretchIndex(1);
    iWidgetLabelPtr labelVelocity = new iWidgetLabel(velocityLayout);
    labelVelocity->setText("Velocity");
    labelVelocity->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    labelVelocity->setHorizontalAlignment(iHorizontalAlignment::Left);

    _velocity = new iUserControlVector(3, velocityLayout);
    _velocity->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _velocity->getChangeEvent().add(iChangeDelegate(this, &UserControlComponentVelocity::onValueChanged));

    iWidgetBoxLayoutPtr angularVelocityLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    angularVelocityLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    angularVelocityLayout->setStretchIndex(1);
    iWidgetLabelPtr labelAngularVelocity = new iWidgetLabel(angularVelocityLayout);
    labelAngularVelocity->setText("Angular Velocity");
    labelAngularVelocity->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    labelAngularVelocity->setHorizontalAlignment(iHorizontalAlignment::Left);

    _angularVelocity = new iUserControlVector(3, angularVelocityLayout);
    _angularVelocity->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _angularVelocity->getChangeEvent().add(iChangeDelegate(this, &UserControlComponentVelocity::onValueChanged));    
}

void UserControlComponentVelocity::onValueChanged(iWidgetPtr source)
{
    onUpdateComponent();
}

void UserControlComponentVelocity::onUpdateUI()
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

    iVelocityComponentPtr component = entity->getComponent<iVelocityComponent>();
    if (component == nullptr)
    {
        return;
    }

    _ignoreUpdate = true;

    const auto &velocity = component->getVelocity();
    const auto &angularVelocity = component->getAngularVelocity();

    _velocity->setValue(0, velocity._x);
    _velocity->setValue(1, velocity._y);
    _velocity->setValue(2, velocity._z);

    _angularVelocity->setValue(0, angularVelocity._x);
    _angularVelocity->setValue(1, angularVelocity._y);
    _angularVelocity->setValue(2, angularVelocity._z);

    _ignoreUpdate = false;
}

void UserControlComponentVelocity::onUpdateComponent()
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

    iVelocityComponentPtr component = entity->getComponent<iVelocityComponent>();
    if (component == nullptr)
    {
        return;
    }

    iaVector3d velocity(_velocity->getValue(0), _velocity->getValue(1), _velocity->getValue(2));
    iaVector3d angularVelocity(_angularVelocity->getValue(0), _angularVelocity->getValue(1), _angularVelocity->getValue(2));
    component->setVelocity(velocity);
    component->setAngularVelocity(angularVelocity);
}

void UserControlComponentVelocity::onDestroyComponent(iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");
    
    entity->destroyComponent<iVelocityComponent>();
}
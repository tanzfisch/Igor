// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentMeshReference.h"

#include <igor/entities/components/iMeshReferenceComponent.h>
#include <igor/entities/components/iMeshRenderComponent.h>

UserControlComponentMeshReference::UserControlComponentMeshReference(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Mesh Reference", parent)
{
}

void UserControlComponentMeshReference::init()
{
    UserControlComponent::init();

    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetBoxLayoutPtr meshRefLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    meshRefLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    meshRefLayout->setStretchIndex(1);
    iWidgetLabelPtr labelMeshRef = new iWidgetLabel(meshRefLayout);
    labelMeshRef->setText("Mesh Ref.");
    labelMeshRef->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelMeshRef->setVerticalAlignment(iVerticalAlignment::Top);
    labelMeshRef->setHorizontalAlignment(iHorizontalAlignment::Left);

    _meshReference = new iUserControlMeshReference(meshRefLayout);
    _meshReference->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _meshReference->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentMeshReference::onValueChanged));
}

void UserControlComponentMeshReference::onValueChanged(iWidgetPtr source)
{
    updateComponent();
}

void UserControlComponentMeshReference::update()
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

    iMeshReferenceComponent *component = entity->getComponent<iMeshReferenceComponent>();
    if (component == nullptr)
    {
        return;
    }

    _ignoreUpdate = true;

    if (component->getModel() != nullptr)
    {
        _meshReference->setReference(component->getModel()->getID(), component->getMeshPaths());
    }
    else
    {
        _meshReference->setReference(iResourceID::getInvalid(), {});
    }

    _ignoreUpdate = false;
}

void UserControlComponentMeshReference::updateComponent()
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

    iMeshReferenceComponentPtr component = entity->getComponent<iMeshReferenceComponent>();
    if (component == nullptr)
    {
        return;
    }

    iModelPtr model = iResourceManager::getInstance().requestResource<iModel>(_meshReference->getModelID());
    component->setModel(model);
    component->setMeshPaths(_meshReference->getMeshPaths());

    iMeshRenderComponentPtr meshRenderComponent = entity->getComponent<iMeshRenderComponent>();
    if (meshRenderComponent != nullptr)
    {
        entity->reloadComponent<iMeshRenderComponent>();
    }
}

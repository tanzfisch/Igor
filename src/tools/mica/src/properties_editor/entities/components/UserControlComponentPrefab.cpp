// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentPrefab.h"

#include <igor/entities/components/iPrefabComponent.h>

UserControlComponentPrefab::UserControlComponentPrefab(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Prefab", parent)
{
    setAcceptDrop(true);
}

void UserControlComponentPrefab::init()
{
    UserControlComponent::init();

    iWidgetBoxLayoutPtr prefabRefLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    prefabRefLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    prefabRefLayout->setStretchIndex(1);
    iWidgetLabelPtr labelPrefab = new iWidgetLabel(prefabRefLayout);
    labelPrefab->setText("Prefab");
    labelPrefab->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelPrefab->setVerticalAlignment(iVerticalAlignment::Top);
    labelPrefab->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetBoxLayoutPtr labelLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, prefabRefLayout);
    _labelID = new iWidgetLabel(labelLayout);
    _labelID->setHorizontalAlignment(iHorizontalAlignment::Left);
    _labelID->setVerticalAlignment(iVerticalAlignment::Top);

    _labelAlias = new iWidgetLabel(labelLayout);
    _labelAlias->setHorizontalAlignment(iHorizontalAlignment::Left);
    _labelAlias->setVerticalAlignment(iVerticalAlignment::Top);

    _labelSource = new iWidgetLabel(labelLayout);
    _labelSource->setHorizontalAlignment(iHorizontalAlignment::Left);
    _labelSource->setVerticalAlignment(iVerticalAlignment::Top);
}

void UserControlComponentPrefab::update()
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

    iPrefabComponent *component = entity->getComponent<iPrefabComponent>();
    if (component == nullptr)
    {
        return;
    }

    _ignoreUpdate = true;

    if (component->getPrefab() == nullptr)
    {
        _labelID->setText("no prefab defined");
        _labelAlias->setText("");
        _labelSource->setText("");
    }
    else
    {
        _labelID->setText(component->getPrefab()->getID().toString());
        _labelAlias->setText(component->getPrefab()->getAlias());
        _labelSource->setText(component->getPrefab()->getSource());
    }

    _ignoreUpdate = false;
}

void UserControlComponentPrefab::updateComponent()
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

    iPrefabComponent *component = entity->getComponent<iPrefabComponent>();
    if (component == nullptr)
    {
        return;
    }

    // TODO
}

void UserControlComponentPrefab::onDestroyComponent(iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");

    entity->destroyComponent<iPrefabComponent>();
}

void UserControlComponentPrefab::onDragMove(iDrag &drag, const iaVector2f &mousePos)
{
    const iMimeData &mimeData = drag.getMimeData();
    if (!mimeData.hasResourceID())
    {
        drag.reject();
        return;
    }

    iResourceID id = mimeData.getResourceID();

    const iaString resourceType = iResourceManager::getInstance().getType(id);
    if (resourceType == IGOR_RESOURCE_PREFAB)
    {
        drag.accept();
        return;
    }

    drag.reject();
}

void UserControlComponentPrefab::onDrop(const iDrag &drag, const iaVector2f &mousePos)
{
    const iMimeData &mimeData = drag.getMimeData();
    if (!mimeData.hasResourceID())
    {
        return;
    }

    iResourceID resourceID = mimeData.getResourceID();
    const iaString resourceType = iResourceManager::getInstance().getType(resourceID);
    if (resourceType != IGOR_RESOURCE_PREFAB)
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

    iPrefabComponent *component = entity->getComponent<iPrefabComponent>();
    if (component == nullptr)
    {
        return;
    }

    component->setPrefab(iResourceManager::getInstance().getResource<iPrefab>(resourceID));
    refresh();
}
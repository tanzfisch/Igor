// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlEntity.h"

#include "components/UserControlComponentTransform.h"
#include "components/UserControlComponentSpriteRender.h"
#include "components/UserControlComponentAnimation.h"
#include "components/UserControlComponentCamera.h"
#include "components/UserControlComponentScript.h"
#include "components/UserControlComponentCircle.h"
#include "components/UserControlComponentSphere.h"
#include "components/UserControlComponentLight.h"
#include "components/UserControlComponentMeshReference.h"
#include "components/UserControlComponentMeshRender.h"
#include "components/UserControlComponentOctree.h"
#include "components/UserControlComponentQuadtree.h"
#include "components/UserControlComponentPrefab.h"
#include "components/UserControlComponentVelocity.h"
#include "components/UserControlComponentUserData.h"

#include "../../MicaDefines.h"

UserControlEntity::UserControlEntity(iEntitySceneID sceneID, iEntityID entityID, const iWidgetPtr parent)
    : iUserControl(iWidgetType::iUserControl, parent), _sceneID(sceneID), _entityID(entityID)
{
    con_assert(iEntitySystemModule::getInstance().getScene(getSceneID()) != nullptr, "invalid scene id");
    con_assert(iEntitySystemModule::getInstance().getScene(getSceneID())->getEntity(entityID) != nullptr, "invalid entity id");

    _componentSelectionDialog = std::make_unique<DialogComponentTypeSelection>();
}

void UserControlEntity::init()
{
    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetBoxLayoutPtr mainLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    mainLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetGroupBoxPtr entityGroupBox = new iWidgetGroupBox(mainLayout);
    entityGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    entityGroupBox->setText("Entity");
    entityGroupBox->setHeaderOnly();

    iWidgetBoxLayoutPtr entityLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, entityGroupBox);
    entityLayout->setSpacing(5);
    entityLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    entityLayout->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetBoxLayoutPtr nameLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, entityLayout);
    nameLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    nameLayout->setStretchIndex(1);
    iWidgetLabelPtr labelName = new iWidgetLabel(nameLayout);
    labelName->setText("Name");
    labelName->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    labelName->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textName = new iWidgetLineTextEdit(nameLayout);
    _textName->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textName->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textName->setEnabled(true);
    _textName->getChangeEvent().add(iChangeDelegate(this, &UserControlEntity::onNameChanged));

    iWidgetBoxLayoutPtr idLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, entityLayout);
    idLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    idLayout->setStretchIndex(1);
    iWidgetLabelPtr labelID = new iWidgetLabel(idLayout);
    labelID->setText("ID");
    labelID->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    labelID->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textID = new iWidgetLineTextEdit(idLayout);
    _textID->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textID->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textID->setEnabled(false);

    iWidgetBoxLayoutPtr activeLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, entityLayout);
    activeLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    activeLayout->setStretchIndex(1);
    iWidgetLabelPtr labelActive = new iWidgetLabel(activeLayout);
    labelActive->setText("Active");
    labelActive->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    labelActive->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxActive = new iWidgetCheckBox(activeLayout);
    _checkBoxActive->setHorizontalAlignment(iHorizontalAlignment::Left);
    _checkBoxActive->getChangeEvent().add(iChangeDelegate(this, &UserControlEntity::onActiveChanged));

    _addComponent = new iWidgetButton(entityLayout);
    _addComponent->setHorizontalAlignment(iHorizontalAlignment::Left);
    _addComponent->setText("Add Component");
    _addComponent->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    _addComponent->getClickEvent().add(iClickDelegate(this, &UserControlEntity::onAddComponentClicked));

    iWidgetGroupBoxPtr componentsGroupBox = new iWidgetGroupBox(mainLayout);
    componentsGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    componentsGroupBox->setText("Components");
    componentsGroupBox->setHeaderOnly();

    _componentsLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, componentsGroupBox);
    _componentsLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _componentsLayout->setVerticalAlignment(iVerticalAlignment::Top);

    iEntitySystemModule::getInstance().getEntityChangedEvent().add(iEntityChangedDelegate(this, &UserControlEntity::onEntityChanged));
}

void UserControlEntity::onEntityChanged(iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");

    if (_entityID != entity->getID())
    {
        return;
    }

    update();
}

void UserControlEntity::onDialogClosed(iDialogPtr source)
{
    if (_componentSelectionDialog->getReturnState() != iDialogReturnState::Ok)
    {
        return;
    }

    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(getSceneID());
    if (scene == nullptr)
    {
        return;
    }

    iEntityPtr entity = scene->getEntity(getEntityID());
    if (entity == nullptr)
    {
        return;
    }

    const auto &componentTypes = iEntitySystemModule::getInstance().getRegisteredComponentTypes();

    auto iter = componentTypes.find(_componentSelectionDialog->getSelectedTypeIndex());
    if (iter == componentTypes.end())
    {
        return;
    }

    entity->addComponent(iter->first, iter->second._factory());

    update();
}

void UserControlEntity::onAddComponentClicked(iWidgetPtr source)
{
    _componentSelectionDialog->open(iDialogCloseDelegate(this, &UserControlEntity::onDialogClosed));
}

void UserControlEntity::update()
{
    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(getSceneID());
    if (scene == nullptr)
    {
        return;
    }

    iEntityPtr entity = scene->getEntity(getEntityID());
    if (entity == nullptr)
    {
        return;
    }

    _ignoreUpdate = true;

    _componentsLayout->clear();

    _textName->setText(entity->getName());
    _textID->setText(entity->getID().toString());
    _checkBoxActive->setChecked(entity->isActive());

    auto transform = entity->getComponent<iTransformComponent>();
    if (transform != nullptr)
    {
        UserControlComponentTransform *userControl = new UserControlComponentTransform(_sceneID, _entityID, _componentsLayout);
        userControl->onInit();
        userControl->onUpdateUI();
    }

    auto spriteRender = entity->getComponent<iSpriteRenderComponent>();
    if (spriteRender != nullptr)
    {
        UserControlComponentSpriteRender *userControl = new UserControlComponentSpriteRender(_sceneID, _entityID, _componentsLayout);
        userControl->onInit();
        userControl->onUpdateUI();
    }

    auto animation = entity->getComponent<iAnimationComponent>();
    if (animation != nullptr)
    {
        UserControlComponentAnimation *userControl = new UserControlComponentAnimation(_sceneID, _entityID, _componentsLayout);
        userControl->onInit();
        userControl->onUpdateUI();
    }

    auto camera = entity->getComponent<iCameraComponent>();
    if (camera != nullptr)
    {
        UserControlComponentCamera *userControl = new UserControlComponentCamera(_sceneID, _entityID, _componentsLayout);
        userControl->onInit();
        userControl->onUpdateUI();
    }

    auto script = entity->getComponent<iScriptComponent>();
    if (script != nullptr)
    {
        UserControlComponentScript *userControl = new UserControlComponentScript(_sceneID, _entityID, _componentsLayout);
        userControl->onInit();
        userControl->onUpdateUI();
    }

    auto circle = entity->getComponent<iCircleComponent>();
    if (circle != nullptr)
    {
        UserControlComponentCircle *userControl = new UserControlComponentCircle(_sceneID, _entityID, _componentsLayout);
        userControl->onInit();
        userControl->onUpdateUI();
    }

    auto velocity = entity->getComponent<iVelocityComponent>();
    if (velocity != nullptr)
    {
        UserControlComponentVelocity *userControl = new UserControlComponentVelocity(_sceneID, _entityID, _componentsLayout);
        userControl->onInit();
        userControl->onUpdateUI();
    }

    auto userData = entity->getComponent<iUserDataComponent>();
    if (userData != nullptr)
    {
        UserControlComponentUserData *userControl = new UserControlComponentUserData(_sceneID, _entityID, _componentsLayout);
        userControl->onInit();
        userControl->onUpdateUI();
    }

    auto sphere = entity->getComponent<iSphereComponent>();
    if (sphere != nullptr)
    {
        UserControlComponentSphere *userControl = new UserControlComponentSphere(_sceneID, _entityID, _componentsLayout);
        userControl->onInit();
        userControl->onUpdateUI();
    }

    auto light = entity->getComponent<iLightComponent>();
    if (light != nullptr)
    {
        UserControlComponentLight *userControl = new UserControlComponentLight(_sceneID, _entityID, _componentsLayout);
        userControl->onInit();
        userControl->onUpdateUI();
    }

    auto meshReference = entity->getComponent<iMeshReferenceComponent>();
    if (meshReference != nullptr)
    {
        UserControlComponentMeshReference *userControl = new UserControlComponentMeshReference(_sceneID, _entityID, _componentsLayout);
        userControl->onInit();
        userControl->onUpdateUI();
    }

    auto meshRender = entity->getComponent<iMeshRenderComponent>();
    if (meshRender != nullptr)
    {
        UserControlComponentMeshRender *userControl = new UserControlComponentMeshRender(_sceneID, _entityID, _componentsLayout);
        userControl->onInit();
        userControl->onUpdateUI();
    }

    auto prefab = entity->getComponent<iPrefabComponent>();
    if (prefab != nullptr)
    {
        UserControlComponentPrefab *userControl = new UserControlComponentPrefab(_sceneID, _entityID, _componentsLayout);
        userControl->onInit();
        userControl->onUpdateUI();
    }

    auto quadtree = entity->getComponent<iQuadtreeComponent>();
    if (quadtree != nullptr)
    {
        UserControlComponentQuadtree *userControl = new UserControlComponentQuadtree(_sceneID, _entityID, _componentsLayout);
        userControl->onInit();
        userControl->onUpdateUI();
    }

    auto octree = entity->getComponent<iOctreeComponent>();
    if (octree != nullptr)
    {
        UserControlComponentOctree *userControl = new UserControlComponentOctree(_sceneID, _entityID, _componentsLayout);
        userControl->onInit();
        userControl->onUpdateUI();
    }

    _ignoreUpdate = false;
}

void UserControlEntity::updateEntity()
{
    if (_ignoreUpdate)
    {
        return;
    }

    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(getSceneID());
    if (scene == nullptr)
    {
        return;
    }

    iEntityPtr entity = scene->getEntity(getEntityID());
    if (entity == nullptr)
    {
        return;
    }

    entity->setName(_textName->getText());
    entity->setActive(_checkBoxActive->isChecked());

    // components get updated in UserControlComponent*
}

iEntitySceneID UserControlEntity::getSceneID() const
{
    return _sceneID;
}

iEntityID UserControlEntity::getEntityID() const
{
    return _entityID;
}

void UserControlEntity::onActiveChanged(iWidgetPtr source)
{
    updateEntity();
}

void UserControlEntity::onNameChanged(iWidgetPtr source)
{
    updateEntity();
}

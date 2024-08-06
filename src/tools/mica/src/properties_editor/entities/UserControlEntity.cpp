// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlEntity.h"

#include "components/UserControlComponentTransform.h"
#include "components/UserControlComponentCamera.h"
#include "components/UserControlComponentCircle.h"
#include "components/UserControlComponentSphere.h"
#include "components/UserControlComponentLight.h"
#include "components/UserControlComponentMeshReference.h"
#include "components/UserControlComponentMeshRender.h"
#include "components/UserControlComponentOctree.h"
#include "components/UserControlComponentQuadtree.h"
#include "components/UserControlComponentPrefab.h"

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
    entityLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    entityLayout->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetBoxLayoutPtr nameLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, entityLayout);
    nameLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    nameLayout->setStretchIndex(1);
    iWidgetLabelPtr labelName = new iWidgetLabel(nameLayout);
    labelName->setText("Name");
    labelName->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelName->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textName = new iWidgetLineTextEdit(nameLayout);
    _textName->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textName->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textName->setEnabled(true);
    _textName->registerOnChangeEvent(iChangeDelegate(this, &UserControlEntity::onNameChanged));

    iWidgetBoxLayoutPtr idLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, entityLayout);
    idLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    idLayout->setStretchIndex(1);
    iWidgetLabelPtr labelID = new iWidgetLabel(idLayout);
    labelID->setText("ID");
    labelID->setMinWidth(MICA_REGULARBUTTON_SIZE);
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
    labelActive->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelActive->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxActive = new iWidgetCheckBox(activeLayout);
    _checkBoxActive->setHorizontalAlignment(iHorizontalAlignment::Left);
    _checkBoxActive->registerOnChangeEvent(iChangeDelegate(this, &UserControlEntity::onActiveChanged));

    _addComponent = new iWidgetButton(entityLayout);
    _addComponent->setHorizontalAlignment(iHorizontalAlignment::Center);
    _addComponent->setText("Add Component");
    _addComponent->setMinWidth(MICA_REGULARBUTTON_SIZE);
    _addComponent->registerOnClickEvent(iClickDelegate(this, &UserControlEntity::onAddComponentClicked));

    iWidgetGroupBoxPtr componentsGroupBox = new iWidgetGroupBox(mainLayout);
    componentsGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    componentsGroupBox->setText("Components");
    componentsGroupBox->setHeaderOnly();

    _componentsLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, componentsGroupBox);
    _componentsLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _componentsLayout->setVerticalAlignment(iVerticalAlignment::Top);    
}

void UserControlEntity::onDialogClosed(iDialogPtr source)
{
    if(_componentSelectionDialog->getReturnState() != iDialogReturnState::Ok)
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
    if(iter == componentTypes.end())
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
        userControl->init();
        userControl->update();
    }

    auto camera = entity->getComponent<iCameraComponent>();
    if (camera != nullptr)
    {
        UserControlComponentCamera *userControl = new UserControlComponentCamera(_sceneID, _entityID, _componentsLayout);
        userControl->init();
        userControl->update();
    }

    auto circle = entity->getComponent<iCircleComponent>();
    if (circle != nullptr)
    {
        UserControlComponentCircle *userControl = new UserControlComponentCircle(_sceneID, _entityID, _componentsLayout);
        userControl->init();
        userControl->update();
    }

    auto sphere = entity->getComponent<iSphereComponent>();
    if (sphere != nullptr)
    {
        UserControlComponentSphere *userControl = new UserControlComponentSphere(_sceneID, _entityID, _componentsLayout);
        userControl->init();
        userControl->update();
    }

    auto light = entity->getComponent<iLightComponent>();
    if (light != nullptr)
    {
        UserControlComponentLight *userControl = new UserControlComponentLight(_sceneID, _entityID, _componentsLayout);
        userControl->init();
        userControl->update();
    }    

    auto meshReference = entity->getComponent<iMeshReferenceComponent>();
    if (meshReference != nullptr)
    {
        UserControlComponentMeshReference *userControl = new UserControlComponentMeshReference(_sceneID, _entityID, _componentsLayout);
        userControl->init();
        userControl->update();
    }

    auto meshRender = entity->getComponent<iMeshRenderComponent>();
    if (meshRender != nullptr)
    {
        UserControlComponentMeshRender *userControl = new UserControlComponentMeshRender(_sceneID, _entityID, _componentsLayout);
        userControl->init();
        userControl->update();
    }

    auto octree = entity->getComponent<iOctreeComponent>();
    if (octree != nullptr)
    {
        UserControlComponentOctree *userControl = new UserControlComponentOctree(_sceneID, _entityID, _componentsLayout);
        userControl->init();
        userControl->update();
    }

    auto prefab = entity->getComponent<iPrefabComponent>();
    if (prefab != nullptr)
    {
        UserControlComponentPrefab *userControl = new UserControlComponentPrefab(_sceneID, _entityID, _componentsLayout);
        userControl->init();
        userControl->update();
    }    

    auto quadtree = entity->getComponent<iQuadtreeComponent>();
    if (quadtree != nullptr)
    {
        UserControlComponentQuadtree *userControl = new UserControlComponentQuadtree(_sceneID, _entityID, _componentsLayout);
        userControl->init();
        userControl->update();
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

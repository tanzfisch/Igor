// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlEntity.h"

#include "components/UserControlComponentTransform.h"

#include "../../MicaDefines.h"

UserControlEntity::UserControlEntity(iEntitySceneID sceneID, iEntityID entityID, const iWidgetPtr parent)
    : iUserControl(iWidgetType::iUserControl, parent), _sceneID(sceneID), _entityID(entityID)
{
    con_assert(iEntitySystemModule::getInstance().getScene(getSceneID()) != nullptr, "invalid scene id");
    con_assert(iEntitySystemModule::getInstance().getScene(getSceneID())->getEntity(entityID) != nullptr, "invalid entity id");
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
    _checkBoxActive->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _checkBoxActive->setEnabled(true);
    _checkBoxActive->registerOnChangeEvent(iChangeDelegate(this, &UserControlEntity::onActiveChanged));

    iWidgetGroupBoxPtr componentsGroupBox = new iWidgetGroupBox(mainLayout);
    componentsGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    componentsGroupBox->setText("Components");
    componentsGroupBox->setHeaderOnly();

    _componentsLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, componentsGroupBox);
    _componentsLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _componentsLayout->setVerticalAlignment(iVerticalAlignment::Top);
}

void UserControlEntity::update()
{
    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(getSceneID());
    if(scene == nullptr)
    {
        return;
    }

    iEntityPtr entity = scene->getEntity(getEntityID());
    if(entity == nullptr)
    {
        return;
    }

    _ignoreUpdate = true;

    _componentsLayout->clear();

    _textName->setText(entity->getName());
    _textID->setText(entity->getID().toString());
    _checkBoxActive->setChecked(entity->isActive());

    auto transform = entity->getComponent<iTransformComponent>();
    if(transform != nullptr)
    {
        UserControlComponentTransform* userControl = new UserControlComponentTransform(_sceneID, _entityID, _componentsLayout);
        userControl->init();
        userControl->update();
    }

    _ignoreUpdate = false;
}

void UserControlEntity::updateEntity()
{
    if(_ignoreUpdate)
    {
        return;
    }    

    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(getSceneID());
    if(scene == nullptr)
    {
        return;
    }

    iEntityPtr entity = scene->getEntity(getEntityID());
    if(entity == nullptr)
    {
        return;
    }    

    entity->setName(_textName->getText());
    entity->setActive(_checkBoxActive->isChecked());
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

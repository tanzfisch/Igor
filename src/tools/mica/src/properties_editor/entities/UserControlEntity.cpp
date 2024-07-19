// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlEntity.h"

#include "../../MicaDefines.h"

UserControlEntity::UserControlEntity(iEntitySceneID sceneID, iEntityID entityID, const iWidgetPtr parent)
    : iUserControl(iWidgetType::iUserControl, parent), _sceneID(sceneID), _entityID(entityID)
{
    con_assert(_sceneID.isValid(), "invalid scene id");
}

void UserControlEntity::init()
{
    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetGroupBox *mainGroupBox = new iWidgetGroupBox(this);
    mainGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    if (_entityID.isValid())
    {
        mainGroupBox->setText("Entity");
    }
    else
    {
        mainGroupBox->setText("Scene");
    }
    mainGroupBox->setHeaderOnly();

    _layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, mainGroupBox);
    _layout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _layout->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetBoxLayoutPtr nameLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    nameLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    nameLayout->setStretchIndex(1);
    iWidgetLabelPtr labelName = new iWidgetLabel(nameLayout);
    labelName->setText("Name");
    labelName->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelName->setHorizontalAlignment(iHorizontalAlignment::Left);    

    _textName = new iWidgetLineTextEdit(nameLayout);
    _textName->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textName->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textName->setEnabled(false);    
}

iWidgetBoxLayoutPtr UserControlEntity::getLayout()
{
    return _layout;
}

void UserControlEntity::update()
{
    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(getSceneID());
    iEntityPtr entity = scene->getEntity(getEntityID());

    if (entity != nullptr)
    {
        _textName->setText(entity->getName());
    }
    else
    {
        _textName->setText(scene->getName());
    }
}

void UserControlEntity::updateEntity()
{
}

iEntitySceneID UserControlEntity::getSceneID() const
{
    return _sceneID;
}

iEntityID UserControlEntity::getEntityID() const
{
    return _entityID;
}
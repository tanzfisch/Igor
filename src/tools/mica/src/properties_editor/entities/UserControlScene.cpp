// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlScene.h"

#include "../../MicaDefines.h"

UserControlScene::UserControlScene(iEntitySceneID sceneID, const iWidgetPtr parent)
    : iUserControl(iWidgetType::iUserControl, parent), _sceneID(sceneID)
{
    con_assert(iEntitySystemModule::getInstance().getScene(getSceneID()) != nullptr, "invalid scene id");
}

void UserControlScene::init()
{
    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetGroupBox *mainGroupBox = new iWidgetGroupBox(this);
    mainGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    mainGroupBox->setText("Scene");
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

void UserControlScene::update()
{
    _ignoreUpdate = true;

    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(getSceneID());
    _textName->setText(scene->getName());

    _ignoreUpdate = false;
}

void UserControlScene::updateScene()
{
    if (_ignoreUpdate)
    {
        return;
    }
}

iEntitySceneID UserControlScene::getSceneID() const
{
    return _sceneID;
}

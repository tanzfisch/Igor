// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlNode.h"

#include <iWidgetManager.h>
#include <iWidgetGrid.h>
#include <iWidgetTextEdit.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetCheckBox.h>
#include <iNodeMesh.h>
#include <iMesh.h>
#include <iNodeFactory.h>
using namespace Igor;

#include "ModelViewerDefines.h"

UserControlNode::UserControlNode()
{
    initGUI();
}

UserControlNode::~UserControlNode()
{
    deinitGUI();
}

void UserControlNode::setNode(uint32 id)
{
    _nodeId = id;
    updateGUI();
}

uint32 UserControlNode::getNode()
{
    return _nodeId;
}

void UserControlNode::updateGUI()
{
    iNodeMesh* node = static_cast<iNodeMesh*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        _textName->setWriteProtected(!node->isChild());
        _textName->setText(node->getName());
        _checkBoxActive->setChecked(node->isActive());
    }
}

void UserControlNode::deinitGUI()
{
    _textName->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlNode::onNameChanged));

    for(auto widget : _allWidgets)
    {
        iWidgetManager::getInstance().destroyWidget(widget);
    }

    _allWidgets.clear();

    _grid = nullptr;
    _labelName = nullptr;
    _labelActive = nullptr;
    _checkBoxActive = nullptr;
    _textName = nullptr;
}

void UserControlNode::initGUI()
{
    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(_grid);
    _grid->appendCollumns(1);
    _grid->appendRows(1);
    _grid->setCellSpacing(2);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);
    _grid->setStrechColumn(1);

    _labelName = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(_labelName);
    _labelName->setText("Name");
    _labelName->setWidth(MV_REGULARBUTTON_SIZE);
    _labelName->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textName = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget("TextEdit"));
    _allWidgets.push_back(_textName);
    _textName->setMaxTextLength(256);
    _textName->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textName->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _textName->setText("");
    _textName->registerOnChangeEvent(iChangeDelegate(this, &UserControlNode::onNameChanged));

    _labelActive = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(_labelActive);
    _labelActive->setText("Active");
    _labelActive->setWidth(MV_REGULARBUTTON_SIZE);
    _labelActive->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxActive = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
    _allWidgets.push_back(_checkBoxActive);
    _checkBoxActive->setText("");
    _checkBoxActive->setActive(false);
    _checkBoxActive->setHorizontalAlignment(iHorizontalAlignment::Left);

    _grid->addWidget(_labelName, 0, 0);
    _grid->addWidget(_textName, 1, 0);
    _grid->addWidget(_labelActive, 0, 1);
    _grid->addWidget(_checkBoxActive, 1, 1);
}

void UserControlNode::updateNode()
{
    iNodeMesh* node = static_cast<iNodeMesh*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        node->setName(_textName->getText());
    }
}

void UserControlNode::onNameChanged(iWidget* source)
{
    updateNode();
    _nameChangedEvent();
}

void UserControlNode::registerNameChangeDelegate(NameChangedDelegate nameChangedDelegate)
{
    _nameChangedEvent.append(nameChangedDelegate);
}

void UserControlNode::unregisterNameChangeDelegate(NameChangedDelegate nameChangedDelegate)
{
    _nameChangedEvent.remove(nameChangedDelegate);
}

iWidget* UserControlNode::getWidget()
{
    return _grid;
}
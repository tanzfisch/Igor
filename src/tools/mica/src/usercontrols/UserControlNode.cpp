// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlNode.h"

#include "../MicaDefines.h"

UserControlNode::UserControlNode()
{
    initGUI();
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
    iNodeMesh *node = static_cast<iNodeMesh *>(iNodeManager::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        _textName->setWriteProtected(!node->isChild());
        _textName->setText(node->getName());
        _checkBoxActive->setChecked(node->isActive());
    }
}

void UserControlNode::initGUI()
{
    _grid = new iWidgetGrid();
    _grid->appendColumns(1);
    _grid->appendRows(1);
    _grid->setCellSpacing(2);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);
    _grid->setStretchColumn(1);

    _labelName = new iWidgetLabel();
    _labelName->setText("Name");
    _labelName->setWidth(MICA_REGULARBUTTON_SIZE);
    _labelName->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textName = new iWidgetLineTextEdit();
    _textName->setMaxTextLength(256);
    _textName->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textName->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textName->setText("");
    _textName->registerOnChangeEvent(iChangeDelegate(this, &UserControlNode::onNameChanged));

    _labelActive = new iWidgetLabel();
    _labelActive->setText("Active");
    _labelActive->setWidth(MICA_REGULARBUTTON_SIZE);
    _labelActive->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxActive = new iWidgetCheckBox();
    _checkBoxActive->setText("");
    _checkBoxActive->setEnabled(false);
    _checkBoxActive->setHorizontalAlignment(iHorizontalAlignment::Left);

    _grid->addWidget(_labelName, 0, 0);
    _grid->addWidget(_textName, 1, 0);
    _grid->addWidget(_labelActive, 0, 1);
    _grid->addWidget(_checkBoxActive, 1, 1);

    addWidget(_grid);
}

void UserControlNode::updateNode()
{
    iNodeMesh *node = static_cast<iNodeMesh *>(iNodeManager::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        node->setName(_textName->getText());
    }
}

void UserControlNode::onNameChanged(const iWidgetPtr source)
{
    updateNode();
    _nameChangedEvent();
}

void UserControlNode::registerNameChangeDelegate(NameChangedDelegate nameChangedDelegate)
{
    _nameChangedEvent.add(nameChangedDelegate);
}

void UserControlNode::unregisterNameChangeDelegate(NameChangedDelegate nameChangedDelegate)
{
    _nameChangedEvent.remove(nameChangedDelegate);
}

// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlModel.h"

UserControlModel::UserControlModel()
{
    initGUI();
}

UserControlModel::~UserControlModel()
{
}

void UserControlModel::updateNode()
{
    // nothing to do
}

void UserControlModel::updateGUI()
{
    iNodeModel *node = static_cast<iNodeModel *>(iNodeManager::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        _textFilename->setText(node->getModelName());
    }
}

void UserControlModel::setNode(uint32 id)
{
    _nodeId = id;
    updateGUI();
}

uint32 UserControlModel::getNode()
{
    return _nodeId;
}

void UserControlModel::initGUI()
{
    _grid = new iWidgetGridLayout();
    _grid->appendColumns(1);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);
    _grid->setStretchColumn(1);

    _labelFilename = new iWidgetLabel();
    _labelFilename->setMinWidth(80);
    _labelFilename->setText("Filename");
    _labelFilename->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textFilename = new iWidgetLineTextEdit();
    _textFilename->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textFilename->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textFilename->setEnabled(false);

    _grid->addWidget(_labelFilename, 0, 0);
    _grid->addWidget(_textFilename, 1, 0);

    addWidget(_grid);
}

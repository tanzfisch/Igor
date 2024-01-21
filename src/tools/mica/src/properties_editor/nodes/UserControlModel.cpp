// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlModel.h"

UserControlModel::UserControlModel(iNodeID nodeID, const iWidgetPtr parent)
    : UserControlNode(nodeID, parent)
{
}

void UserControlModel::update()
{
    UserControlNode::update();

    iNodeModel *node = static_cast<iNodeModel *>(iNodeManager::getInstance().getNode(getNodeID()));
    _textFilename->setText(node->getModelName());
}

void UserControlModel::init()
{
    UserControlNode::init();
    
    _grid = new iWidgetGridLayout(getLayout());
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
}

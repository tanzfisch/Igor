// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlEmitter.h"

#include "../../MicaDefines.h"

UserControlEmitter::UserControlEmitter(iNodeID nodeID, const iWidgetPtr parent)
    : UserControlNode(nodeID, parent)
{
}

void UserControlEmitter::updateNode()
{
    iNodeEmitter *node = static_cast<iNodeEmitter *>(iNodeManager::getInstance().getNode(getNodeID()));

    float32 size = iaString::toFloat(_textSize->getText());
    node->setSize(size);
    node->setEmitterType(static_cast<iEmitterType>(_selectType->getSelectedIndex()));
}

void UserControlEmitter::update()
{
    UserControlNode::update();

    iNodeEmitter *node = static_cast<iNodeEmitter *>(iNodeManager::getInstance().getNode(getNodeID()));
    _textSize->setText(iaString::toString(node->getSize(), 4));
    _selectType->setSelection(static_cast<uint32>(node->getEmitterType()));
}

void UserControlEmitter::init()
{
    UserControlNode::init();

    _grid = new iWidgetGridLayout(getLayout());
    _grid->appendColumns(1);
    _grid->appendRows(1);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);
    _grid->setStretchColumn(1);

    iWidgetLabel *labelType = new iWidgetLabel();
    labelType->setText("Type");
    labelType->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelType->setHorizontalAlignment(iHorizontalAlignment::Left);

    _selectType = new iWidgetSelectBox();
    _selectType->addItem("Mesh");
    _selectType->addItem("Point");
    _selectType->addItem("Disc");
    _selectType->addItem("Circle");
    _selectType->addItem("Sphere");
    _selectType->addItem("Square");
    _selectType->addItem("Cube");
    _selectType->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _selectType->registerOnChangeEvent(iChangeDelegate(this, &UserControlEmitter::onTypeChanged));

    iWidgetLabel *labelSize = new iWidgetLabel();
    labelSize->setText("Size");
    labelSize->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelSize->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textSize = new iWidgetLineTextEdit();
    _textSize->setMinWidth(100);
    _textSize->setWriteProtected(false);
    _textSize->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textSize->setHorizontalTextAlignment(iHorizontalAlignment::Right);
    _textSize->registerOnChangeEvent(iChangeDelegate(this, &UserControlEmitter::onSizeChanged));

    _grid->addWidget(labelType, 0, 0);
    _grid->addWidget(_selectType, 1, 0);
    _grid->addWidget(labelSize, 0, 1);
    _grid->addWidget(_textSize, 1, 1);
}

void UserControlEmitter::onTypeChanged(const iWidgetPtr source)
{
    updateNode();
}

void UserControlEmitter::onSizeChanged(const iWidgetPtr source)
{
    updateNode();
}
// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlEmitter.h"

#include "../MicaDefines.h"

UserControlEmitter::UserControlEmitter()
{
    initGUI();
}

void UserControlEmitter::updateNode()
{
    iNodeEmitter *node = static_cast<iNodeEmitter *>(iNodeManager::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        float32 size = iaString::toFloat(_textSize->getText());
        node->setSize(size);

        node->setEmitterType(static_cast<iEmitterType>(_selectType->getSelectedIndex()));
    }
}

void UserControlEmitter::updateGUI()
{
    iNodeEmitter *node = static_cast<iNodeEmitter *>(iNodeManager::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        _textSize->setText(iaString::toString(node->getSize(), 4));
        _selectType->setSelection(static_cast<uint32>(node->getEmitterType()));
    }
}

void UserControlEmitter::setNode(uint32 id)
{
    iNodePtr node = iNodeManager::getInstance().getNode(id);
    if (node != nullptr)
    {
        if (node->getType() == iNodeType::iNodeEmitter)
        {
            _nodeId = id;
            updateGUI();
        }
        else
        {
            con_err("node type is not iNodeEmitter");
        }
    }
}

uint32 UserControlEmitter::getNode()
{
    return _nodeId;
}

void UserControlEmitter::initGUI()
{
    _grid = new iWidgetGrid();
    _grid->appendColumns(1);
    _grid->appendRows(1);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);
    _grid->setStrechColumn(1);
    addWidget(_grid);

    iWidgetLabel *labelType = new iWidgetLabel();
    labelType->setText("Type");
    labelType->setWidth(MICA_REGULARBUTTON_SIZE);
    labelType->setHorizontalAlignment(iHorizontalAlignment::Left);

    _selectType = new iWidgetSelectBox();
    _selectType->addSelectionEntry("Mesh");
    _selectType->addSelectionEntry("Point");
    _selectType->addSelectionEntry("Disc");
    _selectType->addSelectionEntry("Circle");
    _selectType->addSelectionEntry("Sphere");
    _selectType->addSelectionEntry("Square");
    _selectType->addSelectionEntry("Cube");
    _selectType->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _selectType->registerOnChangeEvent(iChangeDelegate(this, &UserControlEmitter::onTypeChanged));

    iWidgetLabel *labelSize = new iWidgetLabel();
    labelSize->setText("Size");
    labelSize->setWidth(MICA_REGULARBUTTON_SIZE);
    labelSize->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textSize = new iWidgetTextEdit();
    _textSize->setWidth(100);
    _textSize->setWriteProtected(false);
    _textSize->setHorizontalAlignment(iHorizontalAlignment::Strech);
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
// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlEmitter.h"

#include <iWidgetManager.h>
#include <iWidgetGrid.h>
#include <iWidgetTextEdit.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetSlider.h>
#include <iWidgetNumberChooser.h>
#include <iNodeEmitter.h>
#include <iWidgetSelectBox.h>
#include <iMesh.h>
#include <iNodeFactory.h>
#include <iTargetMaterial.h>
using namespace Igor;

#include "ModelViewerDefines.h"

UserControlEmitter::UserControlEmitter()
{
    initGUI();
}

iWidget* UserControlEmitter::createInstance()
{
	return new UserControlEmitter();
}

void UserControlEmitter::updateNode()
{
    iNodeEmitter* node = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        float32 size = iaString::toFloat(_textSize->getText());
        node->setSize(size);

        node->setEmitterType(static_cast<iEmitterType>(_selectType->getSelectedIndex()));
    }
}

void UserControlEmitter::updateGUI()
{
    iNodeEmitter* node = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        _textSize->setText(iaString::toString(node->getSize(), 4));
        _selectType->setSelection(static_cast<uint32>(node->getEmitterType()));
    }
}

void UserControlEmitter::setNode(uint32 id)
{
    iNodePtr node = iNodeFactory::getInstance().getNode(id);
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
    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _grid->appendCollumns(1);
    _grid->appendRows(1);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);
    _grid->setStrechColumn(1);
	addWidget(_grid);

    iWidgetLabel* labelType = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    labelType->setText("Type");
    labelType->setWidth(MV_REGULARBUTTON_SIZE);
    labelType->setHorizontalAlignment(iHorizontalAlignment::Left);

    _selectType = static_cast<iWidgetSelectBox*>(iWidgetManager::getInstance().createWidget("SelectBox"));
    _selectType->addSelectionEntry("Mesh");
    _selectType->addSelectionEntry("Point");
    _selectType->addSelectionEntry("Disc");
    _selectType->addSelectionEntry("Circle");
    _selectType->addSelectionEntry("Sphere");
    _selectType->addSelectionEntry("Square");
    _selectType->addSelectionEntry("Cube");
    _selectType->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _selectType->registerOnChangeEvent(iChangeDelegate(this, &UserControlEmitter::onTypeChanged));

    iWidgetLabel* labelSize = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    labelSize->setText("Size");
    labelSize->setWidth(MV_REGULARBUTTON_SIZE);
    labelSize->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textSize = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget("TextEdit"));
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

void UserControlEmitter::onTypeChanged(iWidget* source)
{
    updateNode();
}

void UserControlEmitter::onSizeChanged(iWidget* source)
{
    updateNode();
}
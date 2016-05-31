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

UserControlEmitter::~UserControlEmitter()
{
    deinitGUI();
}

void UserControlEmitter::updateNode()
{
    iNodeEmitter* node = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        float32 size = iaString::atof(_textSize->getText());
        node->setSize(size);

        node->setType(static_cast<iEmitterType>(_selectType->getSelectedIndex()));
    }
}

void UserControlEmitter::updateGUI()
{
    iNodeEmitter* node = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        _textSize->setText(iaString::ftoa(node->getSize(), 4));
        _selectType->setSelection(static_cast<uint32>(node->getType()));
    }
}

void UserControlEmitter::setNode(uint32 id)
{
    _nodeId = id;
    updateGUI();
}

uint32 UserControlEmitter::getNode()
{
    return _nodeId;
}

void UserControlEmitter::initGUI()
{
    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(_grid);
    _grid->appendCollumns(1);
    _grid->appendRows(1);
    _grid->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);
    _grid->setStrechColumn(1);

    _labelType = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_labelType);
    _labelType->setText("Type");
    _labelType->setWidth(MV_REGULARBUTTON_SIZE);
    _labelType->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _selectType = static_cast<iWidgetSelectBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::SelectBox));
    _allWidgets.push_back(_selectType);
    // see iEmitterTypes
    _selectType->addSelectionEntry("Mesh");
    _selectType->addSelectionEntry("Point");
    _selectType->addSelectionEntry("Disc");
    _selectType->addSelectionEntry("Circle");
    _selectType->addSelectionEntry("Sphere");
    _selectType->addSelectionEntry("Square");
    _selectType->addSelectionEntry("Cube");
    _selectType->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    _selectType->registerOnChangeEvent(iChangeDelegate(this, &UserControlEmitter::onTypeChanged));

    _labelSize = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_labelSize);
    _labelSize->setText("Size");
    _labelSize->setWidth(MV_REGULARBUTTON_SIZE);
    _labelSize->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _textSize = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget(iWidgetType::TextEdit));
    _allWidgets.push_back(_textSize);
    _textSize->setWidth(100);
    _textSize->setWriteProtected(false);
    _textSize->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    _textSize->setHorrizontalTextAlignment(iHorrizontalAlignment::Right);
    _textSize->registerOnChangeEvent(iChangeDelegate(this, &UserControlEmitter::onSizeChanged));

    _grid->addWidget(_labelType, 0, 0);
    _grid->addWidget(_selectType, 1, 0);
    _grid->addWidget(_labelSize, 0, 1);
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

void UserControlEmitter::deinitGUI()
{
    _selectType->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlEmitter::onTypeChanged));
    _textSize->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlEmitter::onSizeChanged));

    auto iter = _allWidgets.begin();
    while (iter != _allWidgets.end())
    {
        iWidgetManager::getInstance().destroyWidget((*iter));
        iter++;
    }

    _grid = nullptr;
    _labelType = nullptr;
    _labelSize = nullptr;
    _textSize = nullptr;
}

iWidget* UserControlEmitter::getWidget()
{
    return _grid;
}
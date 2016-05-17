// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlModel.h"

#include <iWidgetManager.h>
#include <iWidgetGrid.h>
#include <iWidgetTextEdit.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetSlider.h>
#include <iWidgetNumberChooser.h>
#include <iNodeMesh.h>
#include <iMesh.h>
#include <iNodeFactory.h>
#include <iTargetMaterial.h>
using namespace Igor;

UserControlModel::UserControlModel()
{
    initGUI();
}

UserControlModel::~UserControlModel()
{
    deinitGUI();
}

void UserControlModel::updateNode()
{
    // nothing to do
}

void UserControlModel::updateGUI()
{
    iNodeModel* node = static_cast<iNodeModel*>(iNodeFactory::getInstance().getNode(_nodeId));

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
    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(_grid);
    _grid->appendCollumns(1);
    _grid->setBorder(2);
    _grid->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);

    _labelFilename = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_labelFilename);
    _labelFilename->setText("Filename");
    _labelFilename->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _textFilename = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget(iWidgetType::TextEdit));
    _allWidgets.push_back(_textFilename);
    _textFilename->setWidth(290);
    _textFilename->setHorrizontalAlignment(iHorrizontalAlignment::Right);
    _textFilename->setHorrizontalTextAlignment(iHorrizontalAlignment::Left);
    _textFilename->setActive(false);
    _textFilename->setText("...");

    _grid->addWidget(_labelFilename, 0, 0);
    _grid->addWidget(_textFilename, 1, 0);
}

void UserControlModel::deinitGUI()
{
    auto iter = _allWidgets.begin();
    while(iter != _allWidgets.end())
    {
        iWidgetManager::getInstance().destroyWidget((*iter));
        iter++;
    }

    _grid = nullptr;
    _labelFilename = nullptr;
    _textFilename = nullptr;
}

iWidget* UserControlModel::getWidget()
{
    return _grid;
}
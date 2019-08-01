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
}

iWidget* UserControlModel::createInstance()
{
	return new UserControlModel();
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
    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _grid->appendCollumns(1);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);
    _grid->setStrechColumn(1);

    _labelFilename = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _labelFilename->setWidth(80);
    _labelFilename->setText("Filename");
    _labelFilename->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textFilename = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget("TextEdit"));
    _textFilename->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _textFilename->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textFilename->setActive(false);

    _grid->addWidget(_labelFilename, 0, 0);
    _grid->addWidget(_textFilename, 1, 0);

	addWidget(_grid);
}

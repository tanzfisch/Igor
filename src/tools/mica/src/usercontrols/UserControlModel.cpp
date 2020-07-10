// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlModel.h"

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/widgets/iWidgetGrid.h>
#include <igor/ui/widgets/iWidgetTextEdit.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/widgets/iWidgetSlider.h>
#include <igor/ui/widgets/iWidgetNumberChooser.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/resources/material/iTargetMaterial.h>
using namespace igor;

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
    _grid = new iWidgetGrid();
    _grid->appendColumns(1);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);
    _grid->setStrechColumn(1);

    _labelFilename = new iWidgetLabel();
    _labelFilename->setWidth(80);
    _labelFilename->setText("Filename");
    _labelFilename->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textFilename = new iWidgetTextEdit();
    _textFilename->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _textFilename->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textFilename->setActive(false);

    _grid->addWidget(_labelFilename, 0, 0);
    _grid->addWidget(_textFilename, 1, 0);

    addWidget(_grid);
}

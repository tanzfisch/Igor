// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlParticleSystem.h"

#include <iWidgetManager.h>
#include <iWidgetGrid.h>
#include <iWidgetTextEdit.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetSlider.h>
#include <iWidgetNumberChooser.h>
#include <iNodeParticleSystem.h>
#include <iMesh.h>
#include <iNodeFactory.h>
#include <iTargetMaterial.h>
using namespace Igor;

UserControlParticleSystem::UserControlParticleSystem()
{
    initGUI();
}

UserControlParticleSystem::~UserControlParticleSystem()
{
    deinitGUI();
}

void UserControlParticleSystem::updateNode()
{
    // nothing to do
}

void UserControlParticleSystem::updateGUI()
{
    iNodeParticleSystem* node = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        //_textSize->setText(iaString::ftoa(node->getSize(), 4));
    }
}

void UserControlParticleSystem::setNode(uint32 id)
{
    _nodeId = id;
    updateGUI();
}

uint32 UserControlParticleSystem::getNode()
{
    return _nodeId;
}

void UserControlParticleSystem::initGUI()
{
    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(_grid);
    _grid->appendCollumns(1);
    _grid->appendRows(1);
    _grid->setBorder(2);
    _grid->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);

    _labelType = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_labelType);
    _labelType->setText("Type");
    _labelType->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _labelSize = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_labelSize);
    _labelSize->setText("Size");
    _labelSize->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _textSize = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget(iWidgetType::TextEdit));
    _allWidgets.push_back(_textSize);
    _textSize->setWidth(50);
    _textSize->setHorrizontalAlignment(iHorrizontalAlignment::Right);
    _textSize->setHorrizontalTextAlignment(iHorrizontalAlignment::Left);
    _textSize->setActive(false);
    _textSize->setText("...");

    _grid->addWidget(_labelType, 0, 0);
    _grid->addWidget(_labelSize, 0, 1);
    _grid->addWidget(_textSize, 1, 1);
}

void UserControlParticleSystem::deinitGUI()
{
    auto iter = _allWidgets.begin();
    while(iter != _allWidgets.end())
    {
        iWidgetManager::getInstance().destroyWidget((*iter));
        iter++;
    }

    _grid = nullptr;
    _labelType = nullptr;
    _labelSize = nullptr;
    _textSize = nullptr;
}

iWidget* UserControlParticleSystem::getWidget()
{
    return _grid;
}
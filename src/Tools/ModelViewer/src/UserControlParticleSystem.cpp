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
#include <iWidgetSelectBox.h>
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
    iNodeParticleSystem* node = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        uint32 emitterID = _emitters[_emitterSelection->getSelectedIndex()]->getID();
        if (emitterID != iNode::INVALID_NODE_ID)
        {
            node->setEmitter(emitterID);
        }
        else
        {
            con_err("invalid emitter id");
        }
    }
}

void UserControlParticleSystem::updateGUI()
{
    _emitterSelection->clear();
    _emitters = iNodeFactory::getInstance().getNodes(iNodeType::iNodeEmitter);
    for (auto emitter : _emitters)
    {
        _emitterSelection->addSelectionEntry(emitter->getName());
    }

    iNodeParticleSystem* node = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        int i = 0;
        for (auto emitter : _emitters)
        {
            if (emitter->getID() == node->getEmitter())
            {
                _emitterSelection->setSelection(i);
                break;
            }
            i++;
        }
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

    _gridProperties = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(_gridProperties);
    _gridProperties->appendCollumns(1);
    _gridProperties->appendRows(1);
    _gridProperties->setBorder(2);
    _gridProperties->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    _gridProperties->setVerticalAlignment(iVerticalAlignment::Strech);

    _labelEmitter = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_labelEmitter);
    _labelEmitter->setText("Emitter");
    _labelEmitter->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _gridButtons = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(_gridButtons);
    _gridButtons->appendCollumns(2);
    _gridButtons->setBorder(2);
    _gridButtons->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _gridButtons->setVerticalAlignment(iVerticalAlignment::Top);

    _buttonStart = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allWidgets.push_back(_buttonStart);
    _buttonStart->setText("Start");
    _buttonStart->setWidth(85);
    _buttonStart->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onStart));

    _buttonStop = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allWidgets.push_back(_buttonStop);
    _buttonStop->setText("Stop");
    _buttonStop->setWidth(85);
    _buttonStop->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onStop));

    _buttonReset = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allWidgets.push_back(_buttonReset);
    _buttonReset->setText("Reset");
    _buttonReset->setWidth(85);
    _buttonReset->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onReset));

    _emitterSelection = static_cast<iWidgetSelectBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::SelectBox));
    _allWidgets.push_back(_emitterSelection);
    _emitterSelection->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onEmitterChanged));

    _gridButtons->addWidget(_buttonStart, 0, 0);
    _gridButtons->addWidget(_buttonStop, 1, 0);
    _gridButtons->addWidget(_buttonReset, 2, 0);

    _grid->addWidget(_gridButtons, 0, 0);
    _grid->addWidget(_gridProperties, 0, 1);

    _gridProperties->addWidget(_labelEmitter, 0, 0);
    _gridProperties->addWidget(_emitterSelection, 1, 0);
}

void UserControlParticleSystem::onEmitterChanged(iWidget* source)
{
    updateNode();
}

void UserControlParticleSystem::onStart(iWidget* source)
{
    iNodeParticleSystem* node = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        node->start();
    }
}

void UserControlParticleSystem::onStop(iWidget* source)
{
    iNodeParticleSystem* node = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        node->stop();
    }
}

void UserControlParticleSystem::onReset(iWidget* source)
{
    iNodeParticleSystem* node = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        node->reset();
    }
}

void UserControlParticleSystem::deinitGUI()
{
    _buttonReset->unregisterOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onReset));
    _buttonStop->unregisterOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onStop));
    _buttonStart->unregisterOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onStart));
    _emitterSelection->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onEmitterChanged));

    auto iter = _allWidgets.begin();
    while(iter != _allWidgets.end())
    {
        iWidgetManager::getInstance().destroyWidget((*iter));
        iter++;
    }

    _grid = nullptr;
    _gridButtons = nullptr;
    _buttonStart = nullptr;
    _buttonStop = nullptr;
    _buttonReset = nullptr;
}

iWidget* UserControlParticleSystem::getWidget()
{
    return _grid;
}


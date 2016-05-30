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
#include <iMaterialResourceFactory.h>
#include <iMaterial.h>
#include <iMaterialGroup.h>
#include <iUserControlFileChooser.h>
#include <iResourceManager.h>
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
    if (!_ignoreNodeUpdate)
    {
        iNodeParticleSystem* node = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(_nodeId));

        if (node != nullptr)
        {
            if (_emitterSelection->getSelectedIndex() != -1)
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

            if (_materialSelection->getSelectedUserData() != nullptr)
            {
                uint32 materialID = *(static_cast<uint32*>(_materialSelection->getSelectedUserData()));
                node->setMaterial(materialID);
            }

            node->setTextureA(_textureChooser0->getFileName());
            node->setTextureB(_textureChooser1->getFileName());
            node->setTextureC(_textureChooser2->getFileName());
        }
    }
}

void UserControlParticleSystem::updateGUI()
{
    _ignoreNodeUpdate = true;

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

    for (auto entry : _userDataMaterialID)
    {
        delete entry;
    }
    _userDataMaterialID.clear();
    _materialSelection->clear();

    auto materialGroups = iMaterialResourceFactory::getInstance().getMaterialGroups();
    auto materialGroupIter = materialGroups->begin();
    while (materialGroupIter != materialGroups->end())
    {
        uint32 materialID = (*materialGroupIter)->getID();
        iaString materialName = (*materialGroupIter)->getMaterial()->getName();

        uint32* ptrmaterialID = new uint32(materialID);
        _materialSelection->addSelectionEntry(materialName, ptrmaterialID);
        _userDataMaterialID.push_back(ptrmaterialID);

        if (materialID == node->getMaterial())
        {
            _materialSelection->setSelection(_materialSelection->getSelectionEntryCount() - 1);
        }

        materialGroupIter++;
    }

    _textureChooser0->setFileName(node->getTextureA());
    _textureChooser1->setFileName(node->getTextureB());
    _textureChooser2->setFileName(node->getTextureC());

    _ignoreNodeUpdate = false;
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

    iWidgetGrid* gridProperties = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(gridProperties);
    gridProperties->appendCollumns(1);
    gridProperties->appendRows(5);
    gridProperties->setBorder(2);
    gridProperties->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    gridProperties->setVerticalAlignment(iVerticalAlignment::Strech);

    iWidgetGrid* gridButtons = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(gridButtons);
    gridButtons->appendCollumns(2);
    gridButtons->setBorder(2);
    gridButtons->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    gridButtons->setVerticalAlignment(iVerticalAlignment::Top);

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

    iWidgetLabel* labelEmitter = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelEmitter);
    labelEmitter->setText("Emitter");
    labelEmitter->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _emitterSelection = static_cast<iWidgetSelectBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::SelectBox));
    _allWidgets.push_back(_emitterSelection);
    _emitterSelection->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel* labelMaterial = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelMaterial);
    labelMaterial->setText("Material");
    labelMaterial->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _materialSelection = static_cast<iWidgetSelectBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::SelectBox));
    _allWidgets.push_back(_materialSelection);
    _materialSelection->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetGrid* gridTextures = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(gridTextures);
    gridTextures->appendCollumns(1);
    gridTextures->appendRows(3);
    gridTextures->setBorder(2);
    gridTextures->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    gridTextures->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel* labelTextureUnit0 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelTextureUnit0);
    labelTextureUnit0->setText("Texture 0");
    labelTextureUnit0->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    iWidgetLabel* labelTextureUnit1 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelTextureUnit1);
    labelTextureUnit1->setText("Texture 1");
    labelTextureUnit1->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    iWidgetLabel* labelTextureUnit2 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelTextureUnit2);
    labelTextureUnit2->setText("Texture 2");
    labelTextureUnit2->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    iWidgetLabel* labelTextureUnit3 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelTextureUnit3);
    labelTextureUnit3->setText("Texture 3");
    labelTextureUnit3->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _textureChooser0 = new iUserControlFileChooser();
    _textureChooser0->setPreselectedPath("..\\data\\textures");
    _textureChooser0->registerOnChangedDelegate(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    _textureChooser1 = new iUserControlFileChooser();
    _textureChooser1->setPreselectedPath("..\\data\\textures");
    _textureChooser1->registerOnChangedDelegate(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    _textureChooser2 = new iUserControlFileChooser();
    _textureChooser2->setPreselectedPath("..\\data\\textures");
    _textureChooser2->registerOnChangedDelegate(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    _textureChooser3 = new iUserControlFileChooser();
    _textureChooser3->setPreselectedPath("..\\data\\textures");
    _textureChooser3->registerOnChangedDelegate(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    gridButtons->addWidget(_buttonStart, 0, 0);
    gridButtons->addWidget(_buttonStop, 1, 0);
    gridButtons->addWidget(_buttonReset, 2, 0);

    _grid->addWidget(gridButtons, 0, 0);
    _grid->addWidget(gridProperties, 0, 1);

    gridProperties->addWidget(labelEmitter, 0, 0);
    gridProperties->addWidget(_emitterSelection, 1, 0);

    gridProperties->addWidget(labelMaterial, 0, 1);
    gridProperties->addWidget(_materialSelection, 1, 1);

    gridProperties->addWidget(labelTextureUnit0, 0, 2);
    gridProperties->addWidget(_textureChooser0->getWidget(), 1, 2);

    gridProperties->addWidget(labelTextureUnit1, 0, 3);
    gridProperties->addWidget(_textureChooser1->getWidget(), 1, 3);

    gridProperties->addWidget(labelTextureUnit2, 0, 4);
    gridProperties->addWidget(_textureChooser2->getWidget(), 1, 4);

    gridProperties->addWidget(labelTextureUnit3, 0, 5);
    gridProperties->addWidget(_textureChooser3->getWidget(), 1, 5);

    updateNode();
}

void UserControlParticleSystem::onDoUpdateNode(iWidget* source)
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
    _emitterSelection->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));
    _materialSelection->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    for (auto entry : _userDataMaterialID)
    {
        delete entry;
    }
    _userDataMaterialID.clear();

    auto iter = _allWidgets.begin();
    while(iter != _allWidgets.end())
    {
        iWidgetManager::getInstance().destroyWidget((*iter));
        iter++;
    }

    _grid = nullptr;
    _buttonStart = nullptr;
    _buttonStop = nullptr;
    _buttonReset = nullptr;
}

iWidget* UserControlParticleSystem::getWidget()
{
    return _grid;
}

void UserControlParticleSystem::onTexture0Button(iWidget* source)
{
}

void UserControlParticleSystem::onTexture1Button(iWidget* source)
{
}

void UserControlParticleSystem::onTexture2Button(iWidget* source)
{
}

void UserControlParticleSystem::onTexture3Button(iWidget* source)
{
}

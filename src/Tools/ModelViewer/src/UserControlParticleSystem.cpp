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
#include <iWidgetCheckBox.h>
#include <iMaterialResourceFactory.h>
#include <iMaterial.h>
#include <iMaterialGroup.h>
#include <iUserControlFileChooser.h>
#include <iResourceManager.h>
using namespace Igor;

#include "ModelViewerDefines.h"

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

            node->setLoop(_loopCheckBox->isChecked());
            node->setPeriodTime(_periodChooser->getValue());
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

    _periodChooser->setValue(node->getPeriodTime());
    _loopCheckBox->setChecked(node->getLoop());
    _periodChooser->setActive(!_loopCheckBox->isChecked());

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
    _grid->appendRows(1);
    _grid->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGrid* gridProperties = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(gridProperties);
    gridProperties->appendCollumns(0);
    gridProperties->appendRows(5);
    gridProperties->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    gridProperties->setStrechColumn(0);
    gridProperties->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGrid* gridSimulationProperties = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(gridSimulationProperties);
    gridSimulationProperties->appendCollumns(1);
    gridSimulationProperties->appendRows(5);
    gridSimulationProperties->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    gridSimulationProperties->setStrechColumn(1);
    gridSimulationProperties->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel* labelLoop = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelLoop);
    labelLoop->setText("Loop");
    labelLoop->setWidth(80);
    labelLoop->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _loopCheckBox = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::CheckBox));
    _loopCheckBox->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _loopCheckBox->setVerticalAlignment(iVerticalAlignment::Top);
    _loopCheckBox->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onLoopChanged));

    iWidgetLabel* labelPeriod = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelPeriod);
    labelPeriod->setText("Period");
    labelPeriod->setWidth(80);
    labelPeriod->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _periodChooser = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget(iWidgetType::NumberChooser));
    _periodChooser->setMinMaxNumber(0.0f, 120.0f);
    _periodChooser->setStepping(0.1f, 0.1f);
    _periodChooser->setSteppingWheel(1.0f, 1.0f);
    _periodChooser->setPostFix("s");
    _periodChooser->setAfterPoint(1);
    _periodChooser->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _periodChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _periodChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetGrid* gridAppearanceProperties = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(gridAppearanceProperties);
    gridAppearanceProperties->appendCollumns(1);
    gridAppearanceProperties->appendRows(5);
    gridAppearanceProperties->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    gridAppearanceProperties->setStrechColumn(1);
    gridAppearanceProperties->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGrid* gridButtons = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(gridButtons);
    gridButtons->appendCollumns(2);
    gridButtons->setBorder(2);
    gridButtons->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    gridButtons->setVerticalAlignment(iVerticalAlignment::Top);

    _buttonStart = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allWidgets.push_back(_buttonStart);
    _buttonStart->setText("Start");
    _buttonStart->setWidth(MV_REGULARBUTTON_SIZE);
    _buttonStart->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onStart));

    _buttonStop = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allWidgets.push_back(_buttonStop);
    _buttonStop->setText("Stop");
    _buttonStop->setWidth(MV_REGULARBUTTON_SIZE);
    _buttonStop->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onStop));

    _buttonReset = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allWidgets.push_back(_buttonReset);
    _buttonReset->setText("Reset");
    _buttonReset->setWidth(MV_REGULARBUTTON_SIZE);
    _buttonReset->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onReset));

    iWidgetLabel* labelEmitter = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelEmitter);
    labelEmitter->setText("Emitter");
    labelEmitter->setWidth(MV_REGULARBUTTON_SIZE);
    labelEmitter->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _emitterSelection = static_cast<iWidgetSelectBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::SelectBox));
    _allWidgets.push_back(_emitterSelection);
    _emitterSelection->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _emitterSelection->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));
    _emitterSelection->setWidth(200);

    iWidgetLabel* labelMaterial = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelMaterial);
    labelMaterial->setText("Material");
    labelMaterial->setWidth(MV_REGULARBUTTON_SIZE);
    labelMaterial->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _materialSelection = static_cast<iWidgetSelectBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::SelectBox));
    _allWidgets.push_back(_materialSelection);
    _materialSelection->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _materialSelection->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));
    _materialSelection->setGrowingByContent();
    _materialSelection->setWidth(200);

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
    labelTextureUnit0->setWidth(MV_REGULARBUTTON_SIZE);
    labelTextureUnit0->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    iWidgetLabel* labelTextureUnit1 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelTextureUnit1);
    labelTextureUnit1->setText("Texture 1");
    labelTextureUnit1->setWidth(MV_REGULARBUTTON_SIZE);
    labelTextureUnit1->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    iWidgetLabel* labelTextureUnit2 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelTextureUnit2);
    labelTextureUnit2->setText("Texture 2");
    labelTextureUnit2->setWidth(MV_REGULARBUTTON_SIZE);
    labelTextureUnit2->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    iWidgetLabel* labelTextureUnit3 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelTextureUnit3);
    labelTextureUnit3->setText("Texture 3");
    labelTextureUnit3->setWidth(MV_REGULARBUTTON_SIZE);
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

    gridProperties->addWidget(gridSimulationProperties, 0, 0);

    gridSimulationProperties->addWidget(labelLoop, 0, 0);
    gridSimulationProperties->addWidget(_loopCheckBox, 1, 0);

    gridSimulationProperties->addWidget(labelPeriod, 0, 1);
    gridSimulationProperties->addWidget(_periodChooser, 1, 1);

    gridProperties->addWidget(gridAppearanceProperties, 0, 1);

    gridAppearanceProperties->addWidget(labelEmitter, 0, 0);
    gridAppearanceProperties->addWidget(_emitterSelection, 1, 0);

    gridAppearanceProperties->addWidget(labelMaterial, 0, 1);
    gridAppearanceProperties->addWidget(_materialSelection, 1, 1);

    gridAppearanceProperties->addWidget(labelTextureUnit0, 0, 2);
    gridAppearanceProperties->addWidget(_textureChooser0->getWidget(), 1, 2);

    gridAppearanceProperties->addWidget(labelTextureUnit1, 0, 3);
    gridAppearanceProperties->addWidget(_textureChooser1->getWidget(), 1, 3);

    gridAppearanceProperties->addWidget(labelTextureUnit2, 0, 4);
    gridAppearanceProperties->addWidget(_textureChooser2->getWidget(), 1, 4);

    gridAppearanceProperties->addWidget(labelTextureUnit3, 0, 5);
    gridAppearanceProperties->addWidget(_textureChooser3->getWidget(), 1, 5);

    updateNode();
}

void UserControlParticleSystem::onDoUpdateNode(iWidget* source)
{
    updateNode();
}

void UserControlParticleSystem::onLoopChanged(iWidget* source)
{
    _periodChooser->setActive(!_loopCheckBox->isChecked());
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
    _periodChooser->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    _textureChooser0->unregisterOnChangedDelegate(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));
    _textureChooser1->unregisterOnChangedDelegate(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));
    _textureChooser2->unregisterOnChangedDelegate(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));
    _textureChooser3->unregisterOnChangedDelegate(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

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

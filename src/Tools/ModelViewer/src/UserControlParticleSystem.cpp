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
#include <iWidgetSpacer.h>
#include <iWidgetGroupBox.h>
#include <iWidgetColorGradient.h>
#include <iDialogColorGradient.h>
#include <iWidgetGraph.h>
#include <iDialogGraph.h>
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
            node->setAirDrag(_airDragChooser->getValue());
            node->setVelocityOriented(_velocityOrientedCheckBox->isChecked());
            node->setVorticityConfinement(_vorticityConfinementChooser->getValue());
            node->setVortexApperanceRate(_vorticityAppearanceRateChooser->getValue());
            node->setFirstTextureTiling(_tilingHorizontalChooser->getValue(), _tilingVerticalChooser->getValue());
            node->setColorGradient(_colorGradient->getGradient());

            //node->setStartSizeGradient(_startSizeGraph->get);
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

        _loopCheckBox->setChecked(node->getLoop());
        _periodChooser->setValue(node->getPeriodTime());
        _periodChooser->setActive(!_loopCheckBox->isChecked());
        _airDragChooser->setValue(node->getAirDrag());
        _velocityOrientedCheckBox->setChecked(node->getVelocityOriented());
        _vorticityConfinementChooser->setValue(node->getVorticityConfinement());
        _vorticityAppearanceRateChooser->setValue(node->getVortexApperanceRate());
        _tilingHorizontalChooser->setValue(node->getFirstTextureColumns());
        _tilingVerticalChooser->setValue(node->getFirstTextureRows());

        iGradientColor4f gradient;
        node->getColorGradient(gradient);
        _colorGradient->setGradient(gradient);

        convertGradientsToUI(node);
    }

    _ignoreNodeUpdate = false;
}

void UserControlParticleSystem::convertGradientsToUI(iNodeParticleSystem* node)
{
    // start size
    iGradientVector2f startSizeGradient;
    node->getStartSizeGradient(startSizeGradient);

    vector<iaVector2f> minStartSize;
    vector<iaVector2f> maxStartSize;

    vector<pair<float, iaVector2f>> startSizeValues = startSizeGradient.getValues();
    for (auto value : startSizeValues)
    {
        minStartSize.push_back(iaVector2f(value.first, value.second._x));
        maxStartSize.push_back(iaVector2f(value.first, value.second._y));
    }

    _startSizeGraph->setPoints(0, minStartSize);
    _startSizeGraph->setPoints(1, maxStartSize);

    // scale size
    iGradientf sizeScaleGradient;
    node->getSizeScaleGradient(sizeScaleGradient);

    vector<iaVector2f> scaleSize;

    vector<pair<float, float32>> scaleSizeValues = sizeScaleGradient.getValues();
    for (auto value : scaleSizeValues)
    {
        scaleSize.push_back(iaVector2f(value.first, value.second));
    }

    _scaleSizeGraph->setPoints(0, scaleSize);

    // visibility
    iGradientVector2f visibilityGradient;
    node->getStartVisibleTimeGradient(visibilityGradient);

    vector<iaVector2f> minVisibility;
    vector<iaVector2f> maxVisibility;

    vector<pair<float, iaVector2f>> visibilityValues = visibilityGradient.getValues();
    for (auto value : visibilityValues)
    {
        minVisibility.push_back(iaVector2f(value.first, value.second._x));
        maxVisibility.push_back(iaVector2f(value.first, value.second._y));
    }

    _visibilityGraph->setPoints(0, minVisibility);
    _visibilityGraph->setPoints(1, maxVisibility);
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
    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(_grid);
    _grid->appendRows(1);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGrid* gridProperties = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(gridProperties);
    gridProperties->appendCollumns(0);
    gridProperties->appendRows(2);
    gridProperties->setHorizontalAlignment(iHorizontalAlignment::Strech);
    gridProperties->setStrechColumn(0);
    gridProperties->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGrid* gridButtons = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(gridButtons);
    gridButtons->appendCollumns(2);
    gridButtons->setBorder(2);
    gridButtons->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridButtons->setVerticalAlignment(iVerticalAlignment::Top);

    _buttonStart = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _allWidgets.push_back(_buttonStart);
    _buttonStart->setText("Start");
    _buttonStart->setWidth(MV_REGULARBUTTON_SIZE);
    _buttonStart->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onStart));

    _buttonStop = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _allWidgets.push_back(_buttonStop);
    _buttonStop->setText("Stop");
    _buttonStop->setWidth(MV_REGULARBUTTON_SIZE);
    _buttonStop->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onStop));

    _buttonReset = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _allWidgets.push_back(_buttonReset);
    _buttonReset->setText("Reset");
    _buttonReset->setWidth(MV_REGULARBUTTON_SIZE);
    _buttonReset->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onReset));

    ///////////////
    iWidgetGroupBox* simulationGroupBox = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget("GroupBox"));
    _allWidgets.push_back(simulationGroupBox);
    simulationGroupBox->setText("Simulation");
    simulationGroupBox->setHeaderOnly();
    simulationGroupBox->setHorizontalAlignment(iHorizontalAlignment::Strech);
    simulationGroupBox->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGrid* gridSimulationProperties = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(gridSimulationProperties);
    gridSimulationProperties->appendCollumns(1);
    gridSimulationProperties->appendRows(5);
    gridSimulationProperties->setHorizontalAlignment(iHorizontalAlignment::Strech);
    gridSimulationProperties->setStrechColumn(1);
    gridSimulationProperties->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel* labelLoop = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelLoop);
    labelLoop->setText("Loop");
    labelLoop->setWidth(80);
    labelLoop->setHorizontalAlignment(iHorizontalAlignment::Left);

    _loopCheckBox = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
    _allWidgets.push_back(_loopCheckBox);
    _loopCheckBox->setHorizontalAlignment(iHorizontalAlignment::Left);
    _loopCheckBox->setVerticalAlignment(iVerticalAlignment::Top);
    _loopCheckBox->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onLoopChanged));

    iWidgetLabel* labelPeriod = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelPeriod);
    labelPeriod->setText("Period");
    labelPeriod->setWidth(80);
    labelPeriod->setHorizontalAlignment(iHorizontalAlignment::Left);

    _periodChooser = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget("NumberChooser"));
    _allWidgets.push_back(_periodChooser);
    _periodChooser->setMinMaxNumber(0.0f, 120.0f);
    _periodChooser->setStepping(0.1f, 0.1f);
    _periodChooser->setSteppingWheel(1.0f, 1.0f);
    _periodChooser->setPostFix("s");
    _periodChooser->setAfterPoint(1);
    _periodChooser->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _periodChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _periodChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel* labelAirDrag = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelAirDrag);
    labelAirDrag->setText("Air Drag");
    labelAirDrag->setWidth(80);
    labelAirDrag->setHorizontalAlignment(iHorizontalAlignment::Left);

    _airDragChooser = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget("NumberChooser"));
    _allWidgets.push_back(_airDragChooser);
    _airDragChooser->setMinMaxNumber(0.0f, 1.0f);
    _airDragChooser->setStepping(0.001f, 0.001f);
    _airDragChooser->setSteppingWheel(0.01f, 0.01f);
    _airDragChooser->setPostFix("");
    _airDragChooser->setAfterPoint(3);
    _airDragChooser->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _airDragChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _airDragChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    ///////////
    iWidgetGroupBox* vortexSimulationGroupBox = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget("GroupBox"));
    _allWidgets.push_back(vortexSimulationGroupBox);
    vortexSimulationGroupBox->setText("Vortex Simulation");
    vortexSimulationGroupBox->setHeaderOnly();
    vortexSimulationGroupBox->setHorizontalAlignment(iHorizontalAlignment::Strech);
    vortexSimulationGroupBox->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGrid* gridVortexSimulationProperties = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(gridVortexSimulationProperties);
    gridVortexSimulationProperties->appendCollumns(1);
    gridVortexSimulationProperties->appendRows(5);
    gridVortexSimulationProperties->setHorizontalAlignment(iHorizontalAlignment::Strech);
    gridVortexSimulationProperties->setStrechColumn(1);
    gridVortexSimulationProperties->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel* labelVortexRate = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelVortexRate);
    labelVortexRate->setText("Crea. Rate");
    labelVortexRate->setWidth(80);
    labelVortexRate->setHorizontalAlignment(iHorizontalAlignment::Left);

    _vorticityAppearanceRateChooser = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget("NumberChooser"));
    _allWidgets.push_back(_vorticityAppearanceRateChooser);
    _vorticityAppearanceRateChooser->setMinMaxNumber(0.0f, 100.0f);
    _vorticityAppearanceRateChooser->setStepping(1.0f, 1.0f);
    _vorticityAppearanceRateChooser->setSteppingWheel(1.0f, 1.0f);
    _vorticityAppearanceRateChooser->setPostFix(" v/f");
    _vorticityAppearanceRateChooser->setAfterPoint(0);
    _vorticityAppearanceRateChooser->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _vorticityAppearanceRateChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _vorticityAppearanceRateChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel* labelVorticityConfinement = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelVorticityConfinement);
    labelVorticityConfinement->setText("Confinement");
    labelVorticityConfinement->setWidth(80);
    labelVorticityConfinement->setHorizontalAlignment(iHorizontalAlignment::Left);

    _vorticityConfinementChooser = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget("NumberChooser"));
    _allWidgets.push_back(_vorticityConfinementChooser);
    _vorticityConfinementChooser->setMinMaxNumber(0.0f, 100.0f);
    _vorticityConfinementChooser->setStepping(0.1f, 0.1f);
    _vorticityConfinementChooser->setSteppingWheel(1.0f, 1.0f);
    _vorticityConfinementChooser->setPostFix("");
    _vorticityConfinementChooser->setAfterPoint(1);
    _vorticityConfinementChooser->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _vorticityConfinementChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _vorticityConfinementChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    ///////////
    iWidgetGroupBox* appearanceGroupBox = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget("GroupBox"));
    _allWidgets.push_back(appearanceGroupBox);
    appearanceGroupBox->setText("Appearance");
    appearanceGroupBox->setHeaderOnly();
    appearanceGroupBox->setHorizontalAlignment(iHorizontalAlignment::Strech);
    appearanceGroupBox->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGrid* gridAppearanceProperties = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(gridAppearanceProperties);
    gridAppearanceProperties->appendCollumns(1);
    gridAppearanceProperties->appendRows(15);
    gridAppearanceProperties->setHorizontalAlignment(iHorizontalAlignment::Strech);
    gridAppearanceProperties->setStrechColumn(1);
    gridAppearanceProperties->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel* labelVerticalTiling = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelVerticalTiling);
    labelVerticalTiling->setText("V Tiling");
    labelVerticalTiling->setWidth(MV_REGULARBUTTON_SIZE);
    labelVerticalTiling->setHorizontalAlignment(iHorizontalAlignment::Left);

    _tilingVerticalChooser = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget("NumberChooser"));
    _allWidgets.push_back(_tilingVerticalChooser);
    _tilingVerticalChooser->setMinMaxNumber(1.0f, 32.0f);
    _tilingVerticalChooser->setStepping(1.0f, 1.0f);
    _tilingVerticalChooser->setSteppingWheel(1.0f, 1.0f);
    _tilingVerticalChooser->setPostFix("");
    _tilingVerticalChooser->setAfterPoint(0);
    _tilingVerticalChooser->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _tilingVerticalChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _tilingVerticalChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel* labelHorizontalTiling = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelHorizontalTiling);
    labelHorizontalTiling->setText("H Tiling");
    labelHorizontalTiling->setWidth(MV_REGULARBUTTON_SIZE);
    labelHorizontalTiling->setHorizontalAlignment(iHorizontalAlignment::Left);

    _tilingHorizontalChooser = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget("NumberChooser"));
    _allWidgets.push_back(_tilingHorizontalChooser);
    _tilingHorizontalChooser->setMinMaxNumber(1.0f, 32.0f);
    _tilingHorizontalChooser->setStepping(1.0f, 1.0f);
    _tilingHorizontalChooser->setSteppingWheel(1.0f, 1.0f);
    _tilingHorizontalChooser->setPostFix("");
    _tilingHorizontalChooser->setAfterPoint(0);
    _tilingHorizontalChooser->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _tilingHorizontalChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _tilingHorizontalChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel* labelVelocityOriented = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelVelocityOriented);
    labelVelocityOriented->setText("Vel. Ori.");
    labelVelocityOriented->setWidth(MV_REGULARBUTTON_SIZE);
    labelVelocityOriented->setHorizontalAlignment(iHorizontalAlignment::Left);

    _velocityOrientedCheckBox = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
    _velocityOrientedCheckBox->setHorizontalAlignment(iHorizontalAlignment::Left);
    _velocityOrientedCheckBox->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel* labelColorGradient = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelColorGradient);
    labelColorGradient->setText("Color");
    labelColorGradient->setWidth(MV_REGULARBUTTON_SIZE);
    labelColorGradient->setHorizontalAlignment(iHorizontalAlignment::Left);

    _colorGradient = static_cast<iWidgetColorGradient*>(iWidgetManager::getInstance().createWidget("DialogColorGradient"));
    _allWidgets.push_back(_colorGradient);    
    _colorGradient->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _colorGradient->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onOpenColorGradientEditor));

    iWidgetLabel* labelSizeGradient = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelSizeGradient);
    labelSizeGradient->setText("Size");
    labelSizeGradient->setWidth(MV_REGULARBUTTON_SIZE);
    labelSizeGradient->setHorizontalAlignment(iHorizontalAlignment::Left);
    labelSizeGradient->setVerticalAlignment(iVerticalAlignment::Top);
    
    _startSizeGraph = static_cast<iWidgetGraph*>(iWidgetManager::getInstance().createWidget("Graph"));
    _allWidgets.push_back(_startSizeGraph);
    _startSizeGraph->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _startSizeGraph->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onOpenStartSizeGradientEditor));
    _startSizeGraph->setViewFrame();
    _startSizeGraph->setExtrapolateData();
    _startSizeGraph->setViewGrid();
    _startSizeGraph->setLineColor(0, iaColor4f(1.0f, 0.0f, 0.0f, 1.0f));
    _startSizeGraph->setLineColor(1, iaColor4f(0.0f, 1.0f, 0.0f, 1.0f));

    iWidgetLabel* labelSizeScaleGradient = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelSizeScaleGradient);
    labelSizeScaleGradient->setText("Size Scale");
    labelSizeScaleGradient->setWidth(MV_REGULARBUTTON_SIZE);
    labelSizeScaleGradient->setHorizontalAlignment(iHorizontalAlignment::Left);
    labelSizeScaleGradient->setVerticalAlignment(iVerticalAlignment::Top);

    _scaleSizeGraph = static_cast<iWidgetGraph*>(iWidgetManager::getInstance().createWidget("Graph"));
    _allWidgets.push_back(_scaleSizeGraph);
    _scaleSizeGraph->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _scaleSizeGraph->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onOpenScaleSizeGradientEditor));
    _scaleSizeGraph->setViewFrame();
    _scaleSizeGraph->setExtrapolateData();
    _scaleSizeGraph->setViewGrid();
    _scaleSizeGraph->setBoundings(iRectanglef(0, 0, 1, 1));
    _scaleSizeGraph->setLineColor(0, iaColor4f(0.0f, 0.0f, 1.0f, 1.0f));

    iWidgetLabel* labelVisibilityGradient = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelVisibilityGradient);
    labelVisibilityGradient->setText("Visibility");
    labelVisibilityGradient->setWidth(MV_REGULARBUTTON_SIZE);
    labelVisibilityGradient->setHorizontalAlignment(iHorizontalAlignment::Left);
    labelVisibilityGradient->setVerticalAlignment(iVerticalAlignment::Top);

    _visibilityGraph = static_cast<iWidgetGraph*>(iWidgetManager::getInstance().createWidget("Graph"));
    _allWidgets.push_back(_visibilityGraph);
    _visibilityGraph->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _visibilityGraph->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onOpenVisibilityGradientEditor));
    _visibilityGraph->setViewFrame();
    _visibilityGraph->setExtrapolateData();
    _visibilityGraph->setViewGrid();
    _visibilityGraph->setLineColor(0, iaColor4f(1.0f, 0.0f, 0.0f, 1.0f));
    _visibilityGraph->setLineColor(1, iaColor4f(0.0f, 1.0f, 0.0f, 1.0f));

    iWidgetLabel* labelEmitter = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelEmitter);
    labelEmitter->setText("Emitter");
    labelEmitter->setWidth(MV_REGULARBUTTON_SIZE);
    labelEmitter->setHorizontalAlignment(iHorizontalAlignment::Left);

    _emitterSelection = static_cast<iWidgetSelectBox*>(iWidgetManager::getInstance().createWidget("SelectBox"));
    _allWidgets.push_back(_emitterSelection);
    _emitterSelection->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _emitterSelection->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));
    _emitterSelection->setWidth(200);

    iWidgetLabel* labelMaterial = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelMaterial);
    labelMaterial->setText("Material");
    labelMaterial->setWidth(MV_REGULARBUTTON_SIZE);
    labelMaterial->setHorizontalAlignment(iHorizontalAlignment::Left);

    _materialSelection = static_cast<iWidgetSelectBox*>(iWidgetManager::getInstance().createWidget("SelectBox"));
    _allWidgets.push_back(_materialSelection);
    _materialSelection->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _materialSelection->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));
    _materialSelection->setGrowingByContent();
    _materialSelection->setWidth(200);

    iWidgetGrid* gridTextures = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(gridTextures);
    gridTextures->appendCollumns(1);
    gridTextures->appendRows(3);
    gridTextures->setBorder(2);
    gridTextures->setHorizontalAlignment(iHorizontalAlignment::Strech);
    gridTextures->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel* labelTextureUnit0 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelTextureUnit0);
    labelTextureUnit0->setText("Texture");
    labelTextureUnit0->setWidth(MV_REGULARBUTTON_SIZE);
    labelTextureUnit0->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelTextureUnit1 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelTextureUnit1);
    labelTextureUnit1->setText("Noise 0");
    labelTextureUnit1->setWidth(MV_REGULARBUTTON_SIZE);
    labelTextureUnit1->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelTextureUnit2 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelTextureUnit2);
    labelTextureUnit2->setText("Noise 1");
    labelTextureUnit2->setWidth(MV_REGULARBUTTON_SIZE);
    labelTextureUnit2->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelTextureUnit3 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelTextureUnit3);
    labelTextureUnit3->setText("Noise 3");
    labelTextureUnit3->setWidth(MV_REGULARBUTTON_SIZE);
    labelTextureUnit3->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textureChooser0 = static_cast<iUserControlFileChooser*>(iWidgetManager::getInstance().createWidget("UserControlFileChooser"));
    _allWidgets.push_back(_textureChooser0);
    _textureChooser0->setPreselectedPath("..\\data\\textures");
    _textureChooser0->registerOnChangedDelegate(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    _textureChooser1 = static_cast<iUserControlFileChooser*>(iWidgetManager::getInstance().createWidget("UserControlFileChooser"));
    _allWidgets.push_back(_textureChooser1);
    _textureChooser1->setPreselectedPath("..\\data\\textures");
    _textureChooser1->registerOnChangedDelegate(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    _textureChooser2 = static_cast<iUserControlFileChooser*>(iWidgetManager::getInstance().createWidget("UserControlFileChooser"));
    _allWidgets.push_back(_textureChooser2);
    _textureChooser2->setPreselectedPath("..\\data\\textures");
    _textureChooser2->registerOnChangedDelegate(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    _textureChooser3 = static_cast<iUserControlFileChooser*>(iWidgetManager::getInstance().createWidget("UserControlFileChooser"));
    _allWidgets.push_back(_textureChooser3);
    _textureChooser3->setPreselectedPath("..\\data\\textures");
    _textureChooser3->registerOnChangedDelegate(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    gridButtons->addWidget(_buttonStart, 0, 0);
    gridButtons->addWidget(_buttonStop, 1, 0);
    gridButtons->addWidget(_buttonReset, 2, 0);

    _grid->addWidget(gridButtons, 0, 0);
    _grid->addWidget(gridProperties, 0, 1);

    ///////
    gridProperties->addWidget(simulationGroupBox, 0, 0);
    simulationGroupBox->addWidget(gridSimulationProperties);

    gridSimulationProperties->addWidget(labelEmitter, 0, 0);
    gridSimulationProperties->addWidget(_emitterSelection, 1, 0);

    gridSimulationProperties->addWidget(labelLoop, 0, 1);
    gridSimulationProperties->addWidget(_loopCheckBox, 1, 1);

    gridSimulationProperties->addWidget(labelPeriod, 0, 2);
    gridSimulationProperties->addWidget(_periodChooser, 1, 2);

    gridSimulationProperties->addWidget(labelAirDrag, 0, 3);
    gridSimulationProperties->addWidget(_airDragChooser, 1, 3);

    ///////////////
    gridProperties->addWidget(vortexSimulationGroupBox, 0, 1);
    vortexSimulationGroupBox->addWidget(gridVortexSimulationProperties);

    gridVortexSimulationProperties->addWidget(labelVortexRate, 0, 0);
    gridVortexSimulationProperties->addWidget(_vorticityAppearanceRateChooser, 1, 0);

    gridVortexSimulationProperties->addWidget(labelVorticityConfinement, 0, 1);
    gridVortexSimulationProperties->addWidget(_vorticityConfinementChooser, 1, 1);

    /////////////////
    gridProperties->addWidget(appearanceGroupBox, 0, 2);
    appearanceGroupBox->addWidget(gridAppearanceProperties);

    gridAppearanceProperties->addWidget(labelMaterial, 0, 0);
    gridAppearanceProperties->addWidget(_materialSelection, 1, 0);

    gridAppearanceProperties->addWidget(labelTextureUnit0, 0, 1);
    gridAppearanceProperties->addWidget(_textureChooser0, 1, 1);

    gridAppearanceProperties->addWidget(labelHorizontalTiling, 0, 2);
    gridAppearanceProperties->addWidget(_tilingHorizontalChooser, 1, 2);

    gridAppearanceProperties->addWidget(labelVerticalTiling, 0, 3);
    gridAppearanceProperties->addWidget(_tilingVerticalChooser, 1, 3);

    gridAppearanceProperties->addWidget(labelTextureUnit1, 0, 4);
    gridAppearanceProperties->addWidget(_textureChooser1, 1, 4);

    gridAppearanceProperties->addWidget(labelTextureUnit2, 0, 5);
    gridAppearanceProperties->addWidget(_textureChooser2, 1, 5);

    gridAppearanceProperties->addWidget(labelTextureUnit3, 0, 6);
    gridAppearanceProperties->addWidget(_textureChooser3, 1, 6);

    gridAppearanceProperties->addWidget(labelVelocityOriented, 0, 7);
    gridAppearanceProperties->addWidget(_velocityOrientedCheckBox, 1, 7);

    gridAppearanceProperties->addWidget(labelColorGradient, 0, 8);
    gridAppearanceProperties->addWidget(_colorGradient, 1, 8);

    gridAppearanceProperties->addWidget(labelSizeGradient, 0, 9);
    gridAppearanceProperties->addWidget(_startSizeGraph, 1, 9);

    gridAppearanceProperties->addWidget(labelSizeScaleGradient, 0, 10);
    gridAppearanceProperties->addWidget(_scaleSizeGraph, 1, 10);

    gridAppearanceProperties->addWidget(labelVisibilityGradient, 0, 11);
    gridAppearanceProperties->addWidget(_visibilityGraph, 1, 11);

    _colorGradientDialog = static_cast<iDialogColorGradient*>(iWidgetManager::getInstance().createDialog("DialogColorGradient"));
    _dialogGraph = static_cast<iDialogGraph*>(iWidgetManager::getInstance().createDialog("DialogGraph"));

    updateNode();
}

void UserControlParticleSystem::onOpenStartSizeGradientEditor(iWidget* source)
{
    vector<vector<iaVector2f>> graphs;
    for (int i = 0; i < _startSizeGraph->getGraphCount(); ++i)
    {
        vector<iaVector2f> temp = _startSizeGraph->getPoints(i);
        graphs.push_back(temp);
    }

    _dialogGraph->configureXAxis(0.0f, 100.0f, 0.1f); // todo max should depend on particle lifetime 
    _dialogGraph->configureYAxis(0.0f, 100.0f, 0.1f);
    _dialogGraph->setTitle("Edit Start Size Gradient");
    _dialogGraph->setAxisName(0, "Time");
    _dialogGraph->setAxisName(1, "Min");
    _dialogGraph->setAxisName(2, "Max");

    _dialogGraph->show(iDialogGraphCloseDelegate(this, &UserControlParticleSystem::onCloseStartSizeGradientEditor), graphs);
}

void UserControlParticleSystem::onCloseStartSizeGradientEditor(bool ok, const vector<vector<iaVector2f>>& graphs)
{
    if (ok)
    {
        //_colorGradient->setGradient(gradient);
        //updateNode();
    }
}

void UserControlParticleSystem::onOpenScaleSizeGradientEditor(iWidget* source)
{
    vector<vector<iaVector2f>> graphs;
    for (int i = 0; i < _scaleSizeGraph->getGraphCount(); ++i)
    {
        vector<iaVector2f> temp = _scaleSizeGraph->getPoints(i);
        graphs.push_back(temp);
    }

    _dialogGraph->show(iDialogGraphCloseDelegate(this, &UserControlParticleSystem::onCloseScaleSizeGradientEditor), graphs);
}

void UserControlParticleSystem::onCloseScaleSizeGradientEditor(bool ok, const vector<vector<iaVector2f>>& graphs)
{
    if (ok)
    {
        //_colorGradient->setGradient(gradient);
        //updateNode();
    }
}

void UserControlParticleSystem::onCloseColorGradientEditor(bool ok, const iGradientColor4f& gradient)
{
    if (ok)
    {
        _colorGradient->setGradient(gradient);
        updateNode();
    }
}

void UserControlParticleSystem::onOpenVisibilityGradientEditor(iWidget* source)
{

}




void UserControlParticleSystem::onOpenColorGradientEditor(iWidget* source)
{
    _colorGradientDialog->show(iColorGradientCloseDelegate(this, &UserControlParticleSystem::onCloseColorGradientEditor), _colorGradient->getGradient(), true);
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
    _airDragChooser->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));
    _velocityOrientedCheckBox->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));
    _vorticityConfinementChooser->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));
    _vorticityAppearanceRateChooser->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));
    _tilingHorizontalChooser->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));
    _tilingVerticalChooser->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

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

    if (_colorGradientDialog != nullptr)
    {
        iWidgetManager::getInstance().destroyDialog(_colorGradientDialog);
        _colorGradientDialog = nullptr;
    }

    if (_dialogGraph != nullptr)
    {
        iWidgetManager::getInstance().destroyDialog(_dialogGraph);
        _dialogGraph = nullptr;
    }
}

iWidget* UserControlParticleSystem::getWidget()
{
    return _grid;
}
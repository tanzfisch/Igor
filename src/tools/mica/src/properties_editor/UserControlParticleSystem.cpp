// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlParticleSystem.h"

#include "../MicaDefines.h"

UserControlParticleSystem::UserControlParticleSystem()
{
    initGUI();
}

UserControlParticleSystem::~UserControlParticleSystem()
{
}

void UserControlParticleSystem::onUpdate()
{
    iNodeParticleSystem *node = static_cast<iNodeParticleSystem *>(iNodeManager::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        _textParticleCount->setText(iaString::toString(node->getParticleCount()));
    }
}

void UserControlParticleSystem::updateNode()
{
    if (!_ignoreNodeUpdate)
    {
        iNodeParticleSystem *node = static_cast<iNodeParticleSystem *>(iNodeManager::getInstance().getNode(_nodeId));

        if (node != nullptr)
        {
            if (_emitterSelection->getSelectedIndex() != -1)
            {
                uint32 emitterID = _emitters[_emitterSelection->getSelectedIndex()];
                if (emitterID != iNode::INVALID_NODE_ID)
                {
                    node->setEmitter(emitterID);
                }
                else
                {
                    con_err("invalid emitter id");
                }
            }

            if (_materialSelection->getSelectedUserData().has_value())
            {
                std::any userData = _materialSelection->getSelectedUserData();
                iMaterialID materialID = std::any_cast<iMaterialID>(userData);
                node->setMaterial(iResourceManager::getInstance().getResource<iMaterial>(materialID));
            }

            node->setTextureA(_textureChooser0->getFileName());
            node->setTextureB(_textureChooser1->getFileName());
            node->setTextureC(_textureChooser2->getFileName());

            node->setLoop(_loopCheckBox->isChecked());
            node->setPeriodTime(_periodChooser->getValue());
            node->setMaxParticleCount(_maxParticleCount->getValue());
            node->setAirDrag(1.0f - _airDragChooser->getValue());
            node->setVelocityOriented(_velocityOrientedCheckBox->isChecked());
            node->setVorticityConfinement(_vorticityConfinementChooser->getValue());
            node->setVortexCheckRange(_vortexCheckRange->getValue());
            node->setVortexToParticleRate(_vortexToParticleRateChooser->getValue() / 100.0f);
            node->setVortexTorque(_vortexTorqueMinChooser->getValue(), _vortexTorqueMaxChooser->getValue());
            node->setVortexRange(_vortexRangeMinChooser->getValue(), _vortexRangeMaxChooser->getValue());
            node->setTextureTiling(_tilingHorizontalChooser->getValue(), _tilingVerticalChooser->getValue());
            node->setColorGradient(_colorGradient->getGradient());

            iaKeyFrameGraphVector2f startSizeGradient;
            for (int i = 0; i < _startSizeGraph->getPoints(0).size(); ++i)
            {
                startSizeGradient.setValue(_startSizeGraph->getPoints(0)[i]._x, iaVector2f(_startSizeGraph->getPoints(0)[i]._y, _startSizeGraph->getPoints(1)[i]._y));
            }
            node->setStartSizeGradient(startSizeGradient);

            iaKeyFrameGraphf sizeScaleGradient;
            auto scaleSizePoints = _scaleSizeGraph->getPoints(0);
            for (auto point : scaleSizePoints)
            {
                sizeScaleGradient.setValue(point._x, point._y);
            }
            node->setSizeScaleGradient(sizeScaleGradient);

            iaKeyFrameGraphVector2f startVisibilityGradient;
            for (int i = 0; i < _visibilityGraph->getPoints(0).size(); ++i)
            {
                startVisibilityGradient.setValue(_visibilityGraph->getPoints(0)[i]._x, iaVector2f(_visibilityGraph->getPoints(0)[i]._y, _visibilityGraph->getPoints(1)[i]._y));
            }
            node->setStartAgeGradient(startVisibilityGradient);

            iaKeyFrameGraphVector2f orientationGradient;
            for (int i = 0; i < _orientationGraph->getPoints(0).size(); ++i)
            {
                orientationGradient.setValue(_orientationGraph->getPoints(0)[i]._x,
                                             iaVector2f(_orientationGraph->getPoints(0)[i]._y / 180.0f * M_PI, _orientationGraph->getPoints(1)[i]._y / 180.0f * M_PI));
            }
            node->setStartOrientationGradient(orientationGradient);

            iaKeyFrameGraphVector2f orientationRateGradient;
            for (int i = 0; i < _orientationRateGraph->getPoints(0).size(); ++i)
            {
                orientationRateGradient.setValue(_orientationRateGraph->getPoints(0)[i]._x,
                                                 iaVector2f(_orientationRateGraph->getPoints(0)[i]._y / 180.0f * M_PI, _orientationRateGraph->getPoints(1)[i]._y / 180.0f * M_PI));
            }
            node->setStartOrientationRateGradient(orientationRateGradient);

            iaKeyFrameGraphVector2f startVelocityGradient;
            for (int i = 0; i < _startVelocityGraph->getPoints(0).size(); ++i)
            {
                startVelocityGradient.setValue(_startVelocityGraph->getPoints(0)[i]._x, iaVector2f(_startVelocityGraph->getPoints(0)[i]._y, _startVelocityGraph->getPoints(1)[i]._y));
            }
            node->setStartVelocityGradient(startVelocityGradient);

            iaKeyFrameGraphVector2f startLiftGradient;
            for (int i = 0; i < _startLiftGraph->getPoints(0).size(); ++i)
            {
                startLiftGradient.setValue(_startLiftGraph->getPoints(0)[i]._x, iaVector2f(_startLiftGraph->getPoints(0)[i]._y, _startLiftGraph->getPoints(1)[i]._y));
            }
            node->setStartLiftGradient(startLiftGradient);

            iaKeyFrameGraphf emissionGradient;
            auto emissionPoints = _emissionGraph->getPoints(0);
            for (auto point : emissionPoints)
            {
                emissionGradient.setValue(point._x, point._y);
            }
            node->setEmissionGradient(emissionGradient);
        }
    }
}

void UserControlParticleSystem::updateGUI()
{
    _ignoreNodeUpdate = true;

    _emitterSelection->clear();
    iNodeManager::getInstance().getNodes(_emitters, iNodeType::iNodeEmitter);
    for (auto emitterID : _emitters)
    {
        iNodeEmitter *emitter = static_cast<iNodeEmitter *>(iNodeManager::getInstance().getNode(emitterID));
        _emitterSelection->addItem(emitter->getName());
    }

    iNodeParticleSystem *node = static_cast<iNodeParticleSystem *>(iNodeManager::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        int i = 0;
        for (auto emitterID : _emitters)
        {
            if (emitterID == node->getEmitter())
            {
                _emitterSelection->setSelection(i);
                break;
            }
            i++;
        }

        _materialSelection->clear();

        std::vector<iMaterialPtr> materials;
        iResourceManager::getInstance().getMaterials(materials);
        for (auto material : materials)
        {
            const iMaterialID &materialID = material->getID();
            _materialSelection->addItem(material->getName(), materialID);

            if (materialID == node->getMaterial()->getID())
            {
                _materialSelection->setSelection(_materialSelection->getItemCount() - 1);
            }
        }

        _textureChooser0->setFileName(node->getTextureA());
        _textureChooser1->setFileName(node->getTextureB());
        _textureChooser2->setFileName(node->getTextureC());

        _loopCheckBox->setChecked(node->isLooped());
        _periodChooser->setValue(node->getPeriodTime());
        _maxParticleCount->setValue(node->getMaxParticleCount());
        _airDragChooser->setValue(1.0f - node->getAirDrag());
        _velocityOrientedCheckBox->setChecked(node->getVelocityOriented());
        _vorticityConfinementChooser->setValue(node->getVorticityConfinement());
        _vortexCheckRange->setValue(node->getVortexCheckRange());
        _vortexToParticleRateChooser->setValue(node->getVortexToParticleRate() * 100.0f);
        _vortexTorqueMinChooser->setValue(node->getVortexTorqueMin());
        _vortexTorqueMaxChooser->setValue(node->getVortexTorqueMax());
        _vortexRangeMinChooser->setValue(node->getVortexRangeMin());
        _vortexRangeMaxChooser->setValue(node->getVortexRangeMax());
        _tilingHorizontalChooser->setValue(node->getTextureColumns());
        _tilingVerticalChooser->setValue(node->getTextureRows());

        iaKeyFrameGraphColor4f gradient;
        node->getColorGradient(gradient);
        _colorGradient->setGradient(gradient);

        convertGradientsToUI(node);
    }

    _ignoreNodeUpdate = false;
}

void UserControlParticleSystem::convertGradientsToUI(iNodeParticleSystem *node)
{
    // start size
    iaKeyFrameGraphVector2f startSizeGradient;
    node->getStartSizeGradient(startSizeGradient);

    std::vector<iaVector2f> minStartSize;
    std::vector<iaVector2f> maxStartSize;

    const auto &startSizeValues = startSizeGradient.getValues();
    for (auto value : startSizeValues)
    {
        minStartSize.push_back(iaVector2f(value.first, value.second._x));
        maxStartSize.push_back(iaVector2f(value.first, value.second._y));
    }

    _startSizeGraph->setPoints(0, minStartSize);
    _startSizeGraph->setPoints(1, maxStartSize);

    // scale size
    iaKeyFrameGraphf sizeScaleGradient;
    node->getSizeScaleGradient(sizeScaleGradient);

    std::vector<iaVector2f> scaleSize;
    const auto &scaleSizeValues = sizeScaleGradient.getValues();
    for (auto value : scaleSizeValues)
    {
        scaleSize.push_back(iaVector2f(value.first, value.second));
    }
    _scaleSizeGraph->setPoints(0, scaleSize);

    // visibility
    iaKeyFrameGraphVector2f visibilityGradient;
    node->getStartAgeGradient(visibilityGradient);

    std::vector<iaVector2f> minVisibility;
    std::vector<iaVector2f> maxVisibility;

    const auto &visibilityValues = visibilityGradient.getValues();
    for (auto value : visibilityValues)
    {
        minVisibility.push_back(iaVector2f(value.first, value.second._x));
        maxVisibility.push_back(iaVector2f(value.first, value.second._y));
    }

    _visibilityGraph->setPoints(0, minVisibility);
    _visibilityGraph->setPoints(1, maxVisibility);

    // start orientation
    iaKeyFrameGraphVector2f startOrientationGradient;
    node->getStartOrientationGradient(startOrientationGradient);

    std::vector<iaVector2f> minStartOrientation;
    std::vector<iaVector2f> maxStartOrientation;

    const auto &startOrientationValues = startOrientationGradient.getValues();
    for (auto value : startOrientationValues)
    {
        minStartOrientation.push_back(iaVector2f(value.first, value.second._x));
        maxStartOrientation.push_back(iaVector2f(value.first, value.second._y * 180.0f / M_PI));
    }

    _orientationGraph->setPoints(0, minStartOrientation);
    _orientationGraph->setPoints(1, maxStartOrientation);

    // start orientation rate
    iaKeyFrameGraphVector2f startOrientationRateGradient;
    node->getStartOrientationRateGradient(startOrientationRateGradient);

    std::vector<iaVector2f> minStartOrientationRate;
    std::vector<iaVector2f> maxStartOrientationRate;

    const auto &startOrientationRateValues = startOrientationRateGradient.getValues();
    for (auto value : startOrientationRateValues)
    {
        minStartOrientationRate.push_back(iaVector2f(value.first, value.second._x));
        maxStartOrientationRate.push_back(iaVector2f(value.first, value.second._y * 180.0f / M_PI));
    }

    _orientationRateGraph->setPoints(0, minStartOrientationRate);
    _orientationRateGraph->setPoints(1, maxStartOrientationRate);

    // start velocity
    iaKeyFrameGraphVector2f startVelocityGradient;
    node->getStartVelocityGradient(startVelocityGradient);

    std::vector<iaVector2f> minStartVelocity;
    std::vector<iaVector2f> maxStartVelocity;

    const auto &startVelocityValues = startVelocityGradient.getValues();
    for (auto value : startVelocityValues)
    {
        minStartVelocity.push_back(iaVector2f(value.first, value.second._x));
        maxStartVelocity.push_back(iaVector2f(value.first, value.second._y));
    }

    _startVelocityGraph->setPoints(0, minStartVelocity);
    _startVelocityGraph->setPoints(1, maxStartVelocity);

    // start Lift
    iaKeyFrameGraphVector2f startLiftGradient;
    node->getStartLiftGradient(startLiftGradient);

    std::vector<iaVector2f> minStartLift;
    std::vector<iaVector2f> maxStartLift;

    const auto &startLiftValues = startLiftGradient.getValues();
    for (auto value : startLiftValues)
    {
        minStartLift.push_back(iaVector2f(value.first, value.second._x));
        maxStartLift.push_back(iaVector2f(value.first, value.second._y));
    }

    _startLiftGraph->setPoints(0, minStartLift);
    _startLiftGraph->setPoints(1, maxStartLift);

    // emission
    iaKeyFrameGraphf emissionGradient;
    node->getEmissionGradient(emissionGradient);

    std::vector<iaVector2f> emissionGraph;
    const auto &emissionValues = emissionGradient.getValues();
    for (auto value : emissionValues)
    {
        emissionGraph.push_back(iaVector2f(value.first, value.second));
    }
    _emissionGraph->setPoints(0, emissionGraph);
}

void UserControlParticleSystem::setNode(uint64 id)
{
    _nodeId = id;
    updateGUI();
}

uint64 UserControlParticleSystem::getNode()
{
    return _nodeId;
}

void UserControlParticleSystem::initGUI()
{
    _grid = new iWidgetGridLayout();
    _grid->appendRows(2);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);
    addWidget(_grid);

    iWidgetGridLayout *gridProperties = new iWidgetGridLayout();
    gridProperties->appendColumns(0);
    gridProperties->appendRows(2);
    gridProperties->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    gridProperties->setStretchColumn(0);
    gridProperties->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGridLayout *detailsGrid = new iWidgetGridLayout();
    detailsGrid->appendColumns(1);
    detailsGrid->appendRows(2);
    detailsGrid->setStretchColumn(1);
    detailsGrid->setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetLabel *labelParticleCount = new iWidgetLabel();
    labelParticleCount->setText("Particles");
    labelParticleCount->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelParticleCount->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelParticleCount->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelFrequency = new iWidgetLabel();
    labelFrequency->setText("Frequency");
    labelFrequency->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelFrequency->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelFrequency->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textParticleCount = new iWidgetLineTextEdit();
    _textParticleCount->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textParticleCount->setHorizontalTextAlignment(iHorizontalAlignment::Right);
    _textParticleCount->setEnabled(false);
    _textParticleCount->setText("0");

    _textFrequency = new iWidgetLineTextEdit();
    _textFrequency->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textFrequency->setHorizontalTextAlignment(iHorizontalAlignment::Right);
    _textFrequency->setEnabled(false);
    iaString frequency = iaString::toString(iParticleSystem::getSimulationRate(), 2);
    frequency += " Hz";
    _textFrequency->setText(frequency);

    iWidgetGridLayout *gridButtons = new iWidgetGridLayout();
    gridButtons->appendColumns(2);
    gridButtons->setBorder(2);
    gridButtons->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridButtons->setVerticalAlignment(iVerticalAlignment::Top);

    _buttonStart = new iWidgetButton();
    _buttonStart->setText("Start");
    _buttonStart->setTooltip("Start/Continue particle system playback");
    _buttonStart->setMinWidth(MICA_REGULARBUTTON_SIZE);
    _buttonStart->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onStart));

    _buttonStop = new iWidgetButton();
    _buttonStop->setText("Stop");
    _buttonStop->setTooltip("Stop/Pause particle system playback");
    _buttonStop->setMinWidth(MICA_REGULARBUTTON_SIZE);
    _buttonStop->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onStop));

    _buttonReset = new iWidgetButton();
    _buttonReset->setText("Reset");
    _buttonReset->setTooltip("Reset/Restart particle system playback");
    _buttonReset->setMinWidth(MICA_REGULARBUTTON_SIZE);
    _buttonReset->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onReset));

    ///////////////
    iWidgetGroupBox *simulationGroupBox = new iWidgetGroupBox();
    simulationGroupBox->setText("Simulation");
    simulationGroupBox->setHeaderOnly();
    simulationGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    simulationGroupBox->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGridLayout *gridSimulationProperties = new iWidgetGridLayout();
    gridSimulationProperties->appendColumns(1);
    gridSimulationProperties->appendRows(7);
    gridSimulationProperties->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    gridSimulationProperties->setStretchColumn(1);
    gridSimulationProperties->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel *labelMaxParticles = new iWidgetLabel();
    labelMaxParticles->setText("Max Part.");
    labelMaxParticles->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelMaxParticles->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelMaxParticles->setHorizontalAlignment(iHorizontalAlignment::Left);

    _maxParticleCount = new iWidgetNumberChooser();
    _maxParticleCount->setMinMaxNumber(0.0f, 1000.0f);
    _maxParticleCount->setStepping(1.0f, 1.0f);
    _maxParticleCount->setSteppingWheel(10.0f, 10.0f);
    _maxParticleCount->setPostFix("");
    _maxParticleCount->setAfterPoint(0);
    _maxParticleCount->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _maxParticleCount->setVerticalAlignment(iVerticalAlignment::Top);
    _maxParticleCount->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel *labelLoop = new iWidgetLabel();
    labelLoop->setText("Loop");
    labelLoop->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelLoop->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelLoop->setHorizontalAlignment(iHorizontalAlignment::Left);

    _loopCheckBox = new iWidgetCheckBox();
    _loopCheckBox->setHorizontalAlignment(iHorizontalAlignment::Left);
    _loopCheckBox->setVerticalAlignment(iVerticalAlignment::Top);
    _loopCheckBox->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel *labelPeriod = new iWidgetLabel();
    labelPeriod->setText("Period");
    labelPeriod->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelPeriod->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelPeriod->setHorizontalAlignment(iHorizontalAlignment::Left);

    _periodChooser = new iWidgetNumberChooser();
    _periodChooser->setMinMaxNumber(0.0f, 120.0f);
    _periodChooser->setStepping(0.1f, 0.1f);
    _periodChooser->setSteppingWheel(1.0f, 1.0f);
    _periodChooser->setPostFix("s");
    _periodChooser->setAfterPoint(1);
    _periodChooser->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _periodChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _periodChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel *labelEmission = new iWidgetLabel();
    labelEmission->setText("Emission Rate");
    labelEmission->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelEmission->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelEmission->setHorizontalAlignment(iHorizontalAlignment::Left);

    _emissionGraph = new iWidgetGraph();
    _emissionGraph->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _emissionGraph->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onOpenEmissionGradientEditor));
    _emissionGraph->setExtrapolateData();
    _emissionGraph->setViewGrid();
    _emissionGraph->setBoundings(iaRectanglef(0, 0, 1, 1));
    _emissionGraph->setLineColor(0, iaColor4f(0.0f, 0.0f, 1.0f, 1.0f));

    iWidgetLabel *labelAirDrag = new iWidgetLabel();
    labelAirDrag->setText("Air Drag");
    labelAirDrag->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelAirDrag->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelAirDrag->setHorizontalAlignment(iHorizontalAlignment::Left);

    _airDragChooser = new iWidgetNumberChooser();
    _airDragChooser->setMinMaxNumber(0.0f, 1.0f);
    _airDragChooser->setStepping(0.001f, 0.001f);
    _airDragChooser->setSteppingWheel(0.01f, 0.01f);
    _airDragChooser->setPostFix("");
    _airDragChooser->setAfterPoint(3);
    _airDragChooser->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _airDragChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _airDragChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    ///////////
    iWidgetGroupBox *vortexSimulationGroupBox = new iWidgetGroupBox();
    vortexSimulationGroupBox->setText("Vortex Simulation");
    vortexSimulationGroupBox->setHeaderOnly();
    vortexSimulationGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    vortexSimulationGroupBox->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGridLayout *gridVortexSimulationProperties = new iWidgetGridLayout();
    gridVortexSimulationProperties->appendColumns(1);
    gridVortexSimulationProperties->appendRows(6);
    gridVortexSimulationProperties->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    gridVortexSimulationProperties->setStretchColumn(1);
    gridVortexSimulationProperties->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel *labelVortexRate = new iWidgetLabel();
    labelVortexRate->setText("Vortex Particle Rate");
    labelVortexRate->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelVortexRate->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelVortexRate->setHorizontalAlignment(iHorizontalAlignment::Left);

    _vortexToParticleRateChooser = new iWidgetNumberChooser();
    _vortexToParticleRateChooser->setMinMaxNumber(0.0f, 100.0f);
    _vortexToParticleRateChooser->setStepping(1.0f, 1.0f);
    _vortexToParticleRateChooser->setSteppingWheel(1.0f, 1.0f);
    _vortexToParticleRateChooser->setPostFix(" %");
    _vortexToParticleRateChooser->setAfterPoint(0);
    _vortexToParticleRateChooser->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _vortexToParticleRateChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _vortexToParticleRateChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel *labelVortexMinTorque = new iWidgetLabel();
    labelVortexMinTorque->setText("Min Tourque");
    labelVortexMinTorque->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelVortexMinTorque->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelVortexMinTorque->setHorizontalAlignment(iHorizontalAlignment::Left);

    _vortexTorqueMinChooser = new iWidgetNumberChooser();
    _vortexTorqueMinChooser->setMinMaxNumber(0.0f, 10.0f);
    _vortexTorqueMinChooser->setStepping(0.001f, 0.001f);
    _vortexTorqueMinChooser->setSteppingWheel(0.01f, 0.01f);
    _vortexTorqueMinChooser->setPostFix("");
    _vortexTorqueMinChooser->setAfterPoint(4);
    _vortexTorqueMinChooser->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _vortexTorqueMinChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _vortexTorqueMinChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel *labelVortexMaxTorque = new iWidgetLabel();
    labelVortexMaxTorque->setText("Max Tourque");
    labelVortexMaxTorque->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelVortexMaxTorque->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelVortexMaxTorque->setHorizontalAlignment(iHorizontalAlignment::Left);

    _vortexTorqueMaxChooser = new iWidgetNumberChooser();
    _vortexTorqueMaxChooser->setMinMaxNumber(0.0f, 10.0f);
    _vortexTorqueMaxChooser->setStepping(0.001f, 0.001f);
    _vortexTorqueMaxChooser->setSteppingWheel(0.01f, 0.01f);
    _vortexTorqueMaxChooser->setPostFix("");
    _vortexTorqueMaxChooser->setAfterPoint(4);
    _vortexTorqueMaxChooser->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _vortexTorqueMaxChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _vortexTorqueMaxChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel *labelVortexMinRange = new iWidgetLabel();
    labelVortexMinRange->setText("Min Range");
    labelVortexMinRange->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelVortexMinRange->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelVortexMinRange->setHorizontalAlignment(iHorizontalAlignment::Left);

    _vortexRangeMinChooser = new iWidgetNumberChooser();
    _vortexRangeMinChooser->setMinMaxNumber(0.0f, 100.0f);
    _vortexRangeMinChooser->setStepping(0.1f, 0.1f);
    _vortexRangeMinChooser->setSteppingWheel(1.0f, 1.0f);
    _vortexRangeMinChooser->setPostFix("");
    _vortexRangeMinChooser->setAfterPoint(2);
    _vortexRangeMinChooser->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _vortexRangeMinChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _vortexRangeMinChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel *labelVortexMaxRange = new iWidgetLabel();
    labelVortexMaxRange->setText("Max Range");
    labelVortexMaxRange->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelVortexMaxRange->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelVortexMaxRange->setHorizontalAlignment(iHorizontalAlignment::Left);

    _vortexRangeMaxChooser = new iWidgetNumberChooser();
    _vortexRangeMaxChooser->setMinMaxNumber(0.0f, 100.0f);
    _vortexRangeMaxChooser->setStepping(0.1f, 0.1f);
    _vortexRangeMaxChooser->setSteppingWheel(1.0f, 1.0f);
    _vortexRangeMaxChooser->setPostFix("");
    _vortexRangeMaxChooser->setAfterPoint(2);
    _vortexRangeMaxChooser->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _vortexRangeMaxChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _vortexRangeMaxChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel *labelVorticityConfinement = new iWidgetLabel();
    labelVorticityConfinement->setText("Confinement");
    labelVorticityConfinement->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelVorticityConfinement->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelVorticityConfinement->setHorizontalAlignment(iHorizontalAlignment::Left);

    _vorticityConfinementChooser = new iWidgetNumberChooser();
    _vorticityConfinementChooser->setMinMaxNumber(0.0f, 10.0f);
    _vorticityConfinementChooser->setStepping(0.001f, 0.001f);
    _vorticityConfinementChooser->setSteppingWheel(0.01f, 0.01f);
    _vorticityConfinementChooser->setPostFix("");
    _vorticityConfinementChooser->setAfterPoint(4);
    _vorticityConfinementChooser->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _vorticityConfinementChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _vorticityConfinementChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel *labelVortexCheckRange = new iWidgetLabel();
    labelVortexCheckRange->setText("Quality");
    labelVortexCheckRange->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelVortexCheckRange->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelVortexCheckRange->setHorizontalAlignment(iHorizontalAlignment::Left);

    _vortexCheckRange = new iWidgetNumberChooser();
    _vortexCheckRange->setMinMaxNumber(0.0f, 30.0f);
    _vortexCheckRange->setStepping(1.0f, 1.0f);
    _vortexCheckRange->setSteppingWheel(5.0f, 5.0f);
    _vortexCheckRange->setPostFix("");
    _vortexCheckRange->setAfterPoint(0);
    _vortexCheckRange->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _vortexCheckRange->setVerticalAlignment(iVerticalAlignment::Top);
    _vortexCheckRange->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel *labelVelocityGradient = new iWidgetLabel();
    labelVelocityGradient->setText("Initial Velocity");
    labelVelocityGradient->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelVelocityGradient->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelVelocityGradient->setHorizontalAlignment(iHorizontalAlignment::Left);
    labelVelocityGradient->setVerticalAlignment(iVerticalAlignment::Top);

    _startVelocityGraph = new iWidgetGraph();
    _startVelocityGraph->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _startVelocityGraph->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onOpenStartVelocityGradientEditor));
    _startVelocityGraph->setExtrapolateData();
    _startVelocityGraph->setViewGrid();
    _startVelocityGraph->setLineColor(0, iaColor4f(1.0f, 0.0f, 0.0f, 1.0f));
    _startVelocityGraph->setLineColor(1, iaColor4f(0.0f, 1.0f, 0.0f, 1.0f));

    iWidgetLabel *labelLiftGradient = new iWidgetLabel();
    labelLiftGradient->setText("Initial Lift/Weight");
    labelLiftGradient->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelLiftGradient->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelLiftGradient->setHorizontalAlignment(iHorizontalAlignment::Left);
    labelLiftGradient->setVerticalAlignment(iVerticalAlignment::Top);

    _startLiftGraph = new iWidgetGraph();
    _startLiftGraph->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _startLiftGraph->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onOpenStartLiftGradientEditor));
    _startLiftGraph->setExtrapolateData();
    _startLiftGraph->setViewGrid();
    _startLiftGraph->setLineColor(0, iaColor4f(1.0f, 0.0f, 0.0f, 1.0f));
    _startLiftGraph->setLineColor(1, iaColor4f(0.0f, 1.0f, 0.0f, 1.0f));

    ///////////
    iWidgetGroupBox *appearanceGroupBox = new iWidgetGroupBox();
    appearanceGroupBox->setText("Appearance");
    appearanceGroupBox->setHeaderOnly();
    appearanceGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    appearanceGroupBox->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGridLayout *gridAppearanceProperties = new iWidgetGridLayout();
    gridAppearanceProperties->appendColumns(1);
    gridAppearanceProperties->appendRows(15);
    gridAppearanceProperties->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    gridAppearanceProperties->setStretchColumn(1);
    gridAppearanceProperties->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel *labelVerticalTiling = new iWidgetLabel();
    labelVerticalTiling->setText("V Tiling");
    labelVerticalTiling->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelVerticalTiling->setHorizontalAlignment(iHorizontalAlignment::Left);

    _tilingVerticalChooser = new iWidgetNumberChooser();
    _tilingVerticalChooser->setMinMaxNumber(1.0f, 32.0f);
    _tilingVerticalChooser->setStepping(1.0f, 1.0f);
    _tilingVerticalChooser->setSteppingWheel(1.0f, 1.0f);
    _tilingVerticalChooser->setPostFix("");
    _tilingVerticalChooser->setAfterPoint(0);
    _tilingVerticalChooser->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _tilingVerticalChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _tilingVerticalChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel *labelHorizontalTiling = new iWidgetLabel();
    labelHorizontalTiling->setText("H Tiling");
    labelHorizontalTiling->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelHorizontalTiling->setHorizontalAlignment(iHorizontalAlignment::Left);

    _tilingHorizontalChooser = new iWidgetNumberChooser();
    _tilingHorizontalChooser->setMinMaxNumber(1.0f, 32.0f);
    _tilingHorizontalChooser->setStepping(1.0f, 1.0f);
    _tilingHorizontalChooser->setSteppingWheel(1.0f, 1.0f);
    _tilingHorizontalChooser->setPostFix("");
    _tilingHorizontalChooser->setAfterPoint(0);
    _tilingHorizontalChooser->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _tilingHorizontalChooser->setVerticalAlignment(iVerticalAlignment::Top);
    _tilingHorizontalChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel *labelVelocityOriented = new iWidgetLabel();
    labelVelocityOriented->setText("Vel. Ori.");
    labelVelocityOriented->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelVelocityOriented->setHorizontalAlignment(iHorizontalAlignment::Left);

    _velocityOrientedCheckBox = new iWidgetCheckBox();
    _velocityOrientedCheckBox->setHorizontalAlignment(iHorizontalAlignment::Left);
    _velocityOrientedCheckBox->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));

    iWidgetLabel *labelColorGradient = new iWidgetLabel();
    labelColorGradient->setText("Color");
    labelColorGradient->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelColorGradient->setHorizontalAlignment(iHorizontalAlignment::Left);

    _colorGradient = new iWidgetColorGradient();
    _colorGradient->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _colorGradient->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onOpenColorGradientEditor));

    iWidgetLabel *labelSizeGradient = new iWidgetLabel();
    labelSizeGradient->setText("Size");
    labelSizeGradient->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelSizeGradient->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelSizeGradient->setHorizontalAlignment(iHorizontalAlignment::Left);
    labelSizeGradient->setVerticalAlignment(iVerticalAlignment::Top);

    _startSizeGraph = new iWidgetGraph();
    _startSizeGraph->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _startSizeGraph->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onOpenStartSizeGradientEditor));
    _startSizeGraph->setExtrapolateData();
    _startSizeGraph->setViewGrid();
    _startSizeGraph->setLineColor(0, iaColor4f(1.0f, 0.0f, 0.0f, 1.0f));
    _startSizeGraph->setLineColor(1, iaColor4f(0.0f, 1.0f, 0.0f, 1.0f));

    iWidgetLabel *labelSizeScaleGradient = new iWidgetLabel();
    labelSizeScaleGradient->setText("Size Scale over Time");
    labelSizeScaleGradient->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelSizeScaleGradient->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelSizeScaleGradient->setHorizontalAlignment(iHorizontalAlignment::Left);
    labelSizeScaleGradient->setVerticalAlignment(iVerticalAlignment::Top);

    _scaleSizeGraph = new iWidgetGraph();
    _scaleSizeGraph->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _scaleSizeGraph->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onOpenScaleSizeGradientEditor));
    _scaleSizeGraph->setExtrapolateData();
    _scaleSizeGraph->setViewGrid();
    _scaleSizeGraph->setBoundings(iaRectanglef(0, 0, 1, 1));
    _scaleSizeGraph->setLineColor(0, iaColor4f(0.0f, 0.0f, 1.0f, 1.0f));

    iWidgetLabel *labelVisibilityGradient = new iWidgetLabel();
    labelVisibilityGradient->setText("Visibility");
    labelVisibilityGradient->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelVisibilityGradient->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelVisibilityGradient->setHorizontalAlignment(iHorizontalAlignment::Left);
    labelVisibilityGradient->setVerticalAlignment(iVerticalAlignment::Top);

    _visibilityGraph = new iWidgetGraph();
    _visibilityGraph->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _visibilityGraph->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onOpenVisibilityGradientEditor));
    _visibilityGraph->setExtrapolateData();
    _visibilityGraph->setViewGrid();
    _visibilityGraph->setLineColor(0, iaColor4f(1.0f, 0.0f, 0.0f, 1.0f));
    _visibilityGraph->setLineColor(1, iaColor4f(0.0f, 1.0f, 0.0f, 1.0f));

    iWidgetLabel *labelOrientationGradient = new iWidgetLabel();
    labelOrientationGradient->setText("Orientation");
    labelOrientationGradient->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelOrientationGradient->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelOrientationGradient->setHorizontalAlignment(iHorizontalAlignment::Left);
    labelOrientationGradient->setVerticalAlignment(iVerticalAlignment::Top);

    _orientationGraph = new iWidgetGraph();
    _orientationGraph->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _orientationGraph->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onOpenStartOrientationGradientEditor));
    _orientationGraph->setExtrapolateData();
    _orientationGraph->setViewGrid();
    _orientationGraph->setLineColor(0, iaColor4f(1.0f, 0.0f, 0.0f, 1.0f));
    _orientationGraph->setLineColor(1, iaColor4f(0.0f, 1.0f, 0.0f, 1.0f));

    iWidgetLabel *labelOrientationRateGradient = new iWidgetLabel();
    labelOrientationRateGradient->setText("Orientation Rate");
    labelOrientationRateGradient->setMaxTextWidth(MICA_REGULARBUTTON_SIZE);
    labelOrientationRateGradient->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelOrientationRateGradient->setHorizontalAlignment(iHorizontalAlignment::Left);
    labelOrientationRateGradient->setVerticalAlignment(iVerticalAlignment::Top);

    _orientationRateGraph = new iWidgetGraph();
    _orientationRateGraph->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _orientationRateGraph->registerOnClickEvent(iClickDelegate(this, &UserControlParticleSystem::onOpenStartOrientationRateGradientEditor));
    _orientationRateGraph->setExtrapolateData();
    _orientationRateGraph->setViewGrid();
    _orientationRateGraph->setLineColor(0, iaColor4f(1.0f, 0.0f, 0.0f, 1.0f));
    _orientationRateGraph->setLineColor(1, iaColor4f(0.0f, 1.0f, 0.0f, 1.0f));

    iWidgetLabel *labelEmitter = new iWidgetLabel();
    labelEmitter->setText("Emitter");
    labelEmitter->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelEmitter->setHorizontalAlignment(iHorizontalAlignment::Left);

    _emitterSelection = new iWidgetSelectBox();
    _emitterSelection->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _emitterSelection->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));
    _emitterSelection->setMinWidth(200);

    iWidgetLabel *labelMaterial = new iWidgetLabel();
    labelMaterial->setText("Material");
    labelMaterial->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelMaterial->setHorizontalAlignment(iHorizontalAlignment::Left);

    _materialSelection = new iWidgetSelectBox();
    _materialSelection->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _materialSelection->registerOnChangeEvent(iChangeDelegate(this, &UserControlParticleSystem::onDoUpdateNode));
    _materialSelection->setGrowingByContent();
    _materialSelection->setMinWidth(200);

    iWidgetLabel *labelTextureUnit0 = new iWidgetLabel();
    labelTextureUnit0->setText("Texture");
    labelTextureUnit0->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelTextureUnit0->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelTextureUnit1 = new iWidgetLabel();
    labelTextureUnit1->setText("Noise 0");
    labelTextureUnit1->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelTextureUnit1->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelTextureUnit2 = new iWidgetLabel();
    labelTextureUnit2->setText("Noise 1");
    labelTextureUnit2->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelTextureUnit2->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelTextureUnit3 = new iWidgetLabel();
    labelTextureUnit3->setText("Noise 3");
    labelTextureUnit3->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelTextureUnit3->setHorizontalAlignment(iHorizontalAlignment::Left);

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

    _grid->addWidget(detailsGrid, 0, 0);
    _grid->addWidget(gridButtons, 0, 1);
    _grid->addWidget(gridProperties, 0, 2);

    /////// details

    detailsGrid->addWidget(labelFrequency, 0, 0);
    detailsGrid->addWidget(_textFrequency, 1, 0);

    detailsGrid->addWidget(labelParticleCount, 0, 1);
    detailsGrid->addWidget(_textParticleCount, 1, 1);

    ///////
    gridProperties->addWidget(simulationGroupBox, 0, 0);
    simulationGroupBox->addWidget(gridSimulationProperties);

    gridSimulationProperties->addWidget(labelEmitter, 0, 0);
    gridSimulationProperties->addWidget(_emitterSelection, 1, 0);

    gridSimulationProperties->addWidget(labelMaxParticles, 0, 1);
    gridSimulationProperties->addWidget(_maxParticleCount, 1, 1);

    gridSimulationProperties->addWidget(labelLoop, 0, 2);
    gridSimulationProperties->addWidget(_loopCheckBox, 1, 2);

    gridSimulationProperties->addWidget(labelPeriod, 0, 3);
    gridSimulationProperties->addWidget(_periodChooser, 1, 3);

    gridSimulationProperties->addWidget(labelEmission, 0, 4);
    gridSimulationProperties->addWidget(_emissionGraph, 1, 4);

    gridSimulationProperties->addWidget(labelAirDrag, 0, 5);
    gridSimulationProperties->addWidget(_airDragChooser, 1, 5);

    gridSimulationProperties->addWidget(labelVelocityGradient, 0, 6);
    gridSimulationProperties->addWidget(_startVelocityGraph, 1, 6);

    gridSimulationProperties->addWidget(labelLiftGradient, 0, 7);
    gridSimulationProperties->addWidget(_startLiftGraph, 1, 7);

    ///////////////
    gridProperties->addWidget(vortexSimulationGroupBox, 0, 1);
    vortexSimulationGroupBox->addWidget(gridVortexSimulationProperties);

    gridVortexSimulationProperties->addWidget(labelVortexRate, 0, 0);
    gridVortexSimulationProperties->addWidget(_vortexToParticleRateChooser, 1, 0);

    gridVortexSimulationProperties->addWidget(labelVortexMinTorque, 0, 1);
    gridVortexSimulationProperties->addWidget(_vortexTorqueMinChooser, 1, 1);

    gridVortexSimulationProperties->addWidget(labelVortexMaxTorque, 0, 2);
    gridVortexSimulationProperties->addWidget(_vortexTorqueMaxChooser, 1, 2);

    gridVortexSimulationProperties->addWidget(labelVortexMinRange, 0, 3);
    gridVortexSimulationProperties->addWidget(_vortexRangeMinChooser, 1, 3);

    gridVortexSimulationProperties->addWidget(labelVortexMaxRange, 0, 4);
    gridVortexSimulationProperties->addWidget(_vortexRangeMaxChooser, 1, 4);

    gridVortexSimulationProperties->addWidget(labelVorticityConfinement, 0, 5);
    gridVortexSimulationProperties->addWidget(_vorticityConfinementChooser, 1, 5);

    gridVortexSimulationProperties->addWidget(labelVortexCheckRange, 0, 6);
    gridVortexSimulationProperties->addWidget(_vortexCheckRange, 1, 6);

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

    gridAppearanceProperties->addWidget(labelOrientationGradient, 0, 12);
    gridAppearanceProperties->addWidget(_orientationGraph, 1, 12);

    gridAppearanceProperties->addWidget(labelOrientationRateGradient, 0, 13);
    gridAppearanceProperties->addWidget(_orientationRateGraph, 1, 13);

    updateNode();
}

void UserControlParticleSystem::onOpenStartLiftGradientEditor(const iWidgetPtr source)
{
    std::vector<std::vector<iaVector2f>> graphs;
    for (int i = 0; i < _startLiftGraph->getGraphCount(); ++i)
    {
        std::vector<iaVector2f> temp = _startLiftGraph->getPoints(i);
        graphs.push_back(temp);
    }

    if (_dialogGraph == nullptr)
    {
        _dialogGraph = new iDialogGraph();

        _dialogGraph->configureXAxis(0.0f, 100.0f, 0.001f); // todo max should depend on particle lifetime
        _dialogGraph->configureYAxis(-5.0f, 5.0f, 0.0001f);
        _dialogGraph->setTitle("Edit Start Lift/Weight Gradient");
        _dialogGraph->setAxisName(0, "Time");
        _dialogGraph->setAxisName(1, "Min");
        _dialogGraph->setAxisName(2, "Max");
        _dialogGraph->setAfterPoint(4);

        _dialogGraph->open(iDialogCloseDelegate(this, &UserControlParticleSystem::onCloseStartLiftGradientEditor), graphs);
    }
}

void UserControlParticleSystem::onCloseStartLiftGradientEditor(iDialogPtr dialog)
{
    if (_dialogGraph != dialog)
    {
        return;
    }

    if (_dialogGraph->getReturnState() == iDialogReturnState::Ok)
    {
        _startLiftGraph->clearPoints();
        int i = 0;
        for (auto points : _dialogGraph->getGraphs())
        {
            _startLiftGraph->setPoints(i++, points);
        }
        updateNode();
    }

    delete _dialogGraph;
    _dialogGraph = nullptr;
}

void UserControlParticleSystem::onOpenEmissionGradientEditor(const iWidgetPtr source)
{
    std::vector<std::vector<iaVector2f>> graphs;
    for (int i = 0; i < _emissionGraph->getGraphCount(); ++i)
    {
        graphs.push_back(_emissionGraph->getPoints(i));
    }

    if (_dialogGraph == nullptr)
    {
        _dialogGraph = new iDialogGraph();

        _dialogGraph->configureXAxis(0.0f, 100.0f, 0.01f); // todo max should depend on particle lifetime
        _dialogGraph->configureYAxis(0.0f, 100.0f, 0.01f);
        _dialogGraph->setTitle("Edit Emission Gradient");
        _dialogGraph->setAxisName(0, "Time");
        _dialogGraph->setAxisName(1, "Rate");
        _dialogGraph->setAfterPoint(2);

        _dialogGraph->open(iDialogCloseDelegate(this, &UserControlParticleSystem::onCloseEmissionGradientEditor), graphs);
    }
}

void UserControlParticleSystem::onCloseEmissionGradientEditor(iDialogPtr dialog)
{
    if (_dialogGraph != dialog)
    {
        return;
    }

    if (_dialogGraph->getReturnState() == iDialogReturnState::Ok)
    {
        _emissionGraph->clearPoints();
        int i = 0;
        for (auto points : _dialogGraph->getGraphs())
        {
            _emissionGraph->setPoints(i++, points);
        }
        updateNode();
    }

    delete _dialogGraph;
    _dialogGraph = nullptr;
}

void UserControlParticleSystem::onOpenStartVelocityGradientEditor(const iWidgetPtr source)
{
    std::vector<std::vector<iaVector2f>> graphs;
    for (int i = 0; i < _startVelocityGraph->getGraphCount(); ++i)
    {
        std::vector<iaVector2f> temp = _startVelocityGraph->getPoints(i);
        graphs.push_back(temp);
    }

    if (_dialogGraph == nullptr)
    {
        _dialogGraph = new iDialogGraph();

        _dialogGraph->configureXAxis(0.0f, 100.0f, 0.01f); // todo max should depend on particle lifetime
        _dialogGraph->configureYAxis(0.0f, 5.0f, 0.001f);
        _dialogGraph->setTitle("Edit Initial Velocity Gradient");
        _dialogGraph->setAxisName(0, "Time");
        _dialogGraph->setAxisName(1, "Min");
        _dialogGraph->setAxisName(2, "Max");
        _dialogGraph->setAfterPoint(4);

        _dialogGraph->open(iDialogCloseDelegate(this, &UserControlParticleSystem::onCloseStartVelocityGradientEditor), graphs);
    }
}

void UserControlParticleSystem::onCloseStartVelocityGradientEditor(iDialogPtr dialog)
{
    if (_dialogGraph != dialog)
    {
        return;
    }

    if (_dialogGraph->getReturnState() == iDialogReturnState::Ok)
    {
        _startVelocityGraph->clearPoints();
        int i = 0;
        for (auto points : _dialogGraph->getGraphs())
        {
            _startVelocityGraph->setPoints(i++, points);
        }
        updateNode();
    }

    delete _dialogGraph;
    _dialogGraph = nullptr;
}

void UserControlParticleSystem::onOpenStartSizeGradientEditor(const iWidgetPtr source)
{
    std::vector<std::vector<iaVector2f>> graphs;
    for (int i = 0; i < _startSizeGraph->getGraphCount(); ++i)
    {
        std::vector<iaVector2f> temp = _startSizeGraph->getPoints(i);
        graphs.push_back(temp);
    }

    if (_dialogGraph == nullptr)
    {
        _dialogGraph = new iDialogGraph();

        _dialogGraph->configureXAxis(0.0f, 100.0f, 0.01f); // todo max should depend on particle lifetime
        _dialogGraph->configureYAxis(0.0f, 100.0f, 0.01f);
        _dialogGraph->setTitle("Edit Start Size Gradient");
        _dialogGraph->setAxisName(0, "Time");
        _dialogGraph->setAxisName(1, "Min");
        _dialogGraph->setAxisName(2, "Max");
        _dialogGraph->setAfterPoint(2);

        _dialogGraph->open(iDialogCloseDelegate(this, &UserControlParticleSystem::onCloseStartSizeGradientEditor), graphs);
    }
}

void UserControlParticleSystem::onCloseStartSizeGradientEditor(iDialogPtr dialog)
{
    if (_dialogGraph != dialog)
    {
        return;
    }

    if (_dialogGraph->getReturnState() == iDialogReturnState::Ok)
    {
        _startSizeGraph->clearPoints();
        int i = 0;
        for (auto points : _dialogGraph->getGraphs())
        {
            _startSizeGraph->setPoints(i++, points);
        }
        updateNode();
    }

    delete _dialogGraph;
    _dialogGraph = nullptr;
}

void UserControlParticleSystem::onOpenStartOrientationRateGradientEditor(const iWidgetPtr source)
{
    std::vector<std::vector<iaVector2f>> graphs;
    for (int i = 0; i < _orientationRateGraph->getGraphCount(); ++i)
    {
        graphs.push_back(_orientationRateGraph->getPoints(i));
    }

    if (_dialogGraph == nullptr)
    {
        _dialogGraph = new iDialogGraph();

        _dialogGraph->configureXAxis(0.0f, 100.0f, 0.01f); // todo max should depend on particle lifetime
        _dialogGraph->configureYAxis(-360.0f, 360.0f, 0.1f);
        _dialogGraph->setTitle("Edit Start Orientation Rate Gradient");
        _dialogGraph->setAxisName(0, "Time");
        _dialogGraph->setAxisName(1, "Min");
        _dialogGraph->setAxisName(2, "Max");
        _dialogGraph->setAfterPoint(2);

        _dialogGraph->open(iDialogCloseDelegate(this, &UserControlParticleSystem::onCloseStartOrientationRateGradientEditor), graphs);
    }
}

void UserControlParticleSystem::onCloseStartOrientationRateGradientEditor(iDialogPtr dialog)
{
    if (_dialogGraph != dialog)
    {
        return;
    }

    if (_dialogGraph->getReturnState() == iDialogReturnState::Ok)
    {
        _orientationRateGraph->clearPoints();
        int i = 0;
        for (auto points : _dialogGraph->getGraphs())
        {
            _orientationRateGraph->setPoints(i++, points);
        }
        updateNode();
    }

    delete _dialogGraph;
    _dialogGraph = nullptr;
}

void UserControlParticleSystem::onOpenStartOrientationGradientEditor(const iWidgetPtr source)
{
    std::vector<std::vector<iaVector2f>> graphs;
    for (int i = 0; i < _orientationGraph->getGraphCount(); ++i)
    {
        graphs.push_back(_orientationGraph->getPoints(i));
    }

    if (_dialogGraph == nullptr)
    {
        _dialogGraph = new iDialogGraph();

        _dialogGraph->configureXAxis(0.0f, 100.0f, 0.01f); // todo max should depend on particle lifetime
        _dialogGraph->configureYAxis(-360.0f, 360.0f, 1.0f);
        _dialogGraph->setTitle("Edit Start Orientation Gradient");
        _dialogGraph->setAxisName(0, "Time");
        _dialogGraph->setAxisName(1, "Min");
        _dialogGraph->setAxisName(2, "Max");
        _dialogGraph->setAfterPoint(2);

        _dialogGraph->open(iDialogCloseDelegate(this, &UserControlParticleSystem::onCloseStartOrientationGradientEditor), graphs);
    }
}

void UserControlParticleSystem::onCloseStartOrientationGradientEditor(iDialogPtr dialog)
{
    if (_dialogGraph != dialog)
    {
        return;
    }

    if (_dialogGraph->getReturnState() == iDialogReturnState::Ok)
    {
        _orientationGraph->clearPoints();
        int i = 0;
        for (auto points : _dialogGraph->getGraphs())
        {
            _orientationGraph->setPoints(i++, points);
        }
        updateNode();
    }

    delete _dialogGraph;
    _dialogGraph = nullptr;
}

void UserControlParticleSystem::onOpenScaleSizeGradientEditor(const iWidgetPtr source)
{
    std::vector<std::vector<iaVector2f>> graphs;
    for (int i = 0; i < _scaleSizeGraph->getGraphCount(); ++i)
    {
        graphs.push_back(_scaleSizeGraph->getPoints(i));
    }

    if (_dialogGraph == nullptr)
    {
        _dialogGraph = new iDialogGraph();

        _dialogGraph->configureXAxis(0.0f, 100.0f, 0.01f); // todo max should depend on particle lifetime
        _dialogGraph->configureYAxis(0.0f, 100.0f, 0.01f);
        _dialogGraph->setTitle("Edit Size Scale Gradient");
        _dialogGraph->setAxisName(0, "Time");
        _dialogGraph->setAxisName(1, "Factor");
        _dialogGraph->setAfterPoint(2);

        _dialogGraph->open(iDialogCloseDelegate(this, &UserControlParticleSystem::onCloseScaleSizeGradientEditor), graphs);
    }
}

void UserControlParticleSystem::onCloseScaleSizeGradientEditor(iDialogPtr dialog)
{
    if (_dialogGraph != dialog)
    {
        return;
    }

    if (_dialogGraph->getReturnState() == iDialogReturnState::Ok)
    {
        _scaleSizeGraph->clearPoints();
        int i = 0;
        for (auto points : _dialogGraph->getGraphs())
        {
            _scaleSizeGraph->setPoints(i++, points);
        }
        updateNode();
    }

    delete _dialogGraph;
    _dialogGraph = nullptr;
}

void UserControlParticleSystem::onOpenVisibilityGradientEditor(const iWidgetPtr source)
{
    std::vector<std::vector<iaVector2f>> graphs;
    for (int i = 0; i < _visibilityGraph->getGraphCount(); ++i)
    {
        std::vector<iaVector2f> temp = _visibilityGraph->getPoints(i);
        graphs.push_back(temp);
    }

    if (_dialogGraph == nullptr)
    {
        _dialogGraph = new iDialogGraph();

        _dialogGraph->configureXAxis(0.0f, 100.0f, 0.01f); // todo max should depend on particle lifetime
        _dialogGraph->configureYAxis(0.0f, 100.0f, 0.01f);
        _dialogGraph->setTitle("Edit Visibility Gradient");
        _dialogGraph->setAxisName(0, "Time");
        _dialogGraph->setAxisName(1, "Min");
        _dialogGraph->setAxisName(2, "Max");
        _dialogGraph->setAfterPoint(2);

        _dialogGraph->open(iDialogCloseDelegate(this, &UserControlParticleSystem::onCloseVisibilityGradientEditor), graphs);
    }
}

void UserControlParticleSystem::onCloseVisibilityGradientEditor(iDialogPtr dialog)
{
    if (_dialogGraph != dialog)
    {
        return;
    }

    if (_dialogGraph->getReturnState() == iDialogReturnState::Ok)
    {
        _visibilityGraph->clearPoints();
        int i = 0;
        for (auto points : _dialogGraph->getGraphs())
        {
            _visibilityGraph->setPoints(i++, points);
        }
        updateNode();
    }

    delete _dialogGraph;
    _dialogGraph = nullptr;
}

void UserControlParticleSystem::onOpenColorGradientEditor(const iWidgetPtr source)
{
    if (_colorGradientDialog == nullptr)
    {
        _colorGradientDialog = new iDialogColorGradient();
        _colorGradientDialog->setColorGradient(_colorGradient->getGradient());
        _colorGradientDialog->open(iDialogCloseDelegate(this, &UserControlParticleSystem::onCloseColorGradientEditor));
    }
}

void UserControlParticleSystem::onCloseColorGradientEditor(iDialogPtr dialog)
{
    if (_colorGradientDialog != dialog)
    {
        return;
    }

    if (_colorGradientDialog->getReturnState() == iDialogReturnState::Ok)
    {
        _colorGradient->setGradient(_colorGradientDialog->getColorGradient());
        updateNode();
    }

    delete _colorGradientDialog;
    _colorGradientDialog = nullptr;
}

void UserControlParticleSystem::onDoUpdateNode(const iWidgetPtr source)
{
    updateNode();
}

void UserControlParticleSystem::onStart(const iWidgetPtr source)
{
    iNodeParticleSystem *node = static_cast<iNodeParticleSystem *>(iNodeManager::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        node->start();
    }
}

void UserControlParticleSystem::onStop(const iWidgetPtr source)
{
    iNodeParticleSystem *node = static_cast<iNodeParticleSystem *>(iNodeManager::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        node->stop();
    }
}

void UserControlParticleSystem::onReset(const iWidgetPtr source)
{
    iNodeParticleSystem *node = static_cast<iNodeParticleSystem *>(iNodeManager::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        node->reset();
    }
}

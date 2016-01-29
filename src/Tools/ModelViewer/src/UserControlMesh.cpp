// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlMesh.h"

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

UserControlMesh::UserControlMesh()
{
    initGUI();
}

UserControlMesh::~UserControlMesh()
{
    deinitGUI();
}

void UserControlMesh::updateNode()
{
    iNodeMesh* node = static_cast<iNodeMesh*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        iaColor3f ambient(_ambient._r, _ambient._g, _ambient._b);
        iaColor3f diffuse(_diffuse._r, _diffuse._g, _diffuse._b);
        iaColor3f specular(_specular._r, _specular._g, _specular._b);
        iaColor3f emissive(_emissive._r, _emissive._g, _emissive._b);

        node->getTargetMaterial()->setAmbient(ambient);
        node->getTargetMaterial()->setDiffuse(diffuse);
        node->getTargetMaterial()->setSpecular(specular);
        node->getTargetMaterial()->setEmissive(emissive);
        node->getTargetMaterial()->setShininess(_shininess);
        // TODO node->getTargetMaterial()->setTexture();
    }
}

void UserControlMesh::onAmbientChange(const iaColor4f& color)
{
    _ambient = color;
    updateNode();
}

void UserControlMesh::onDiffuseChange(const iaColor4f& color)
{
    _diffuse = color;
    updateNode();
}

void UserControlMesh::onSpecularChange(const iaColor4f& color)
{
    _specular = color;
    updateNode();
}

void UserControlMesh::onEmissiveChange(const iaColor4f& color)
{
    _emissive = color;
    updateNode();
}

void UserControlMesh::onTextChangedShininess(iWidget* source)
{
    _shininess = _textShininess->getValue();
    _sliderShininess->setValue(_shininess);
    updateNode();
}

void UserControlMesh::onSliderChangedShininess(iWidget* source)
{
    _shininess = _sliderShininess->getValue();
    _textShininess->setValue(_shininess);
    updateNode();
}

void UserControlMesh::updateGUI()
{
    iNodeMesh* node = static_cast<iNodeMesh*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        iaColor3f ambient = node->getAmbient();
        _ambient.set(ambient._r, ambient._g, ambient._b, 1.0f);

        iaColor3f diffuse = node->getDiffuse();
        _diffuse.set(diffuse._r, diffuse._g, diffuse._b, 1.0f);

        iaColor3f specular = node->getSpecular();
        _specular.set(specular._r, specular._g, specular._b, 1.0f);

        iaColor3f emissive = node->getEmissive();
        _emissive.set(emissive._r, emissive._g, emissive._b, 1.0f);

        _shininess = node->getShininess();

        _ambientColorChooser->setColor(_ambient);
        _diffuseColorChooser->setColor(_diffuse);
        _specularColorChooser->setColor(_specular);
        _emissiveColorChooser->setColor(_emissive);
        _sliderShininess->setValue(_shininess);
        _textShininess->setValue(_shininess);

        _textVertices->setText(iaString::itoa(node->getVertexCount()));
        _textTriangles->setText(iaString::itoa(node->getTrianglesCount()));
        _textIndexes->setText(iaString::itoa(node->getIndexesCount()));
    }
}

void UserControlMesh::setNode(uint32 id)
{
    _nodeId = id;
    updateGUI();
}

uint32 UserControlMesh::getNode()
{
    return _nodeId;
}

void UserControlMesh::initGUI()
{
    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(_grid);
    _grid->appendRows(5);
    _grid->setBorder(2);
    _grid->setHorrizontalAlignment(iHorrizontalAlignment::Right);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);

    _ambientColorChooser = new iUserControlColorChooser(iColorChooserMode::RGB);
    _ambientColorChooser->setText("Ambient");
    _ambientColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlMesh::onAmbientChange));

    _diffuseColorChooser = new iUserControlColorChooser(iColorChooserMode::RGB);
    _diffuseColorChooser->setText("Diffuse");
    _diffuseColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlMesh::onDiffuseChange));

    _specularColorChooser = new iUserControlColorChooser(iColorChooserMode::RGB);
    _specularColorChooser->setText("Specular");
    _specularColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlMesh::onSpecularChange));

    _emissiveColorChooser = new iUserControlColorChooser(iColorChooserMode::RGB);
    _emissiveColorChooser->setText("Emissive");
    _emissiveColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlMesh::onEmissiveChange));

    _detailsGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(_detailsGrid);
    _detailsGrid->appendCollumns(1);
    _detailsGrid->appendRows(2);
    _detailsGrid->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _labelVertexes = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_labelVertexes);
    _labelVertexes->setText("Vertexes");
    _labelVertexes->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _labelTriangles = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_labelTriangles);
    _labelTriangles->setText("Triangles");
    _labelTriangles->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _labelIndexes = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_labelIndexes);
    _labelIndexes->setText("Indexes");
    _labelIndexes->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _textVertices = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget(iWidgetType::TextEdit));
    _allWidgets.push_back(_textVertices);
    _textVertices->setWidth(100);
    _textVertices->setHorrizontalAlignment(iHorrizontalAlignment::Right);
    _textVertices->setHorrizontalTextAlignment(iHorrizontalAlignment::Right);
    _textVertices->setActive(false);
    _textVertices->setText("0");

    _textTriangles = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget(iWidgetType::TextEdit));
    _allWidgets.push_back(_textTriangles);
    _textTriangles->setWidth(100);
    _textTriangles->setHorrizontalAlignment(iHorrizontalAlignment::Right);
    _textTriangles->setHorrizontalTextAlignment(iHorrizontalAlignment::Right);
    _textTriangles->setActive(false);
    _textTriangles->setText("0");

    _textIndexes = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget(iWidgetType::TextEdit));
    _allWidgets.push_back(_textIndexes);
    _textIndexes->setWidth(100);
    _textIndexes->setHorrizontalAlignment(iHorrizontalAlignment::Right);
    _textIndexes->setHorrizontalTextAlignment(iHorrizontalAlignment::Right);
    _textIndexes->setActive(false);
    _textIndexes->setText("0");

    _gridShininess = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(_gridShininess);
    _gridShininess->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _gridShininess->appendCollumns(2);
    _gridShininess->appendRows(1);

    _labelShininess = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_labelShininess);
    _labelShininess->setText("Shininess");
    _labelShininess->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _labelShininessShort = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_labelShininessShort);
    _labelShininessShort->setText("S");
    _labelShininessShort->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _sliderShininess = static_cast<iWidgetSlider*>(iWidgetManager::getInstance().createWidget(iWidgetType::Slider));
    _allWidgets.push_back(_sliderShininess);
    _sliderShininess->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _sliderShininess->setMinValue(0.0f);
    _sliderShininess->setMaxValue(1000.0f);
    _sliderShininess->setSteppingWheel(0.1f, 0.1f);
    _sliderShininess->setValue(0.0f);
    _sliderShininess->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _sliderShininess->setWidth(235);
    _sliderShininess->registerOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onSliderChangedShininess));

    _textShininess = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget(iWidgetType::NumberChooser));
    _allWidgets.push_back(_textShininess);
    _textShininess->setMinMaxNumber(0.0f, 1000.0f);
    _textShininess->setAfterPoint(2);
    _textShininess->setValue(0.0f);
    _textShininess->setWidth(80);
    _textShininess->setSteppingWheel(0.1f, 0.1f);
    _textShininess->setStepping(0.01f, 0.01f);
    _textShininess->registerOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onTextChangedShininess));

    _gridShininess->addWidget(_labelShininess, 1, 0);
    _gridShininess->addWidget(_sliderShininess, 1, 1);
    _gridShininess->addWidget(_labelShininessShort, 0, 1);
    _gridShininess->addWidget(_textShininess, 2, 1);

    _detailsGrid->addWidget(_labelVertexes, 0, 0);
    _detailsGrid->addWidget(_labelTriangles, 0, 1);
    _detailsGrid->addWidget(_labelIndexes, 0, 2);

    _detailsGrid->addWidget(_textVertices, 1, 0);
    _detailsGrid->addWidget(_textTriangles, 1, 1);
    _detailsGrid->addWidget(_textIndexes, 1, 2);

    _grid->addWidget(_detailsGrid, 0, 0);
    _grid->addWidget(_ambientColorChooser->getWidget(), 0, 1);
    _grid->addWidget(_diffuseColorChooser->getWidget(), 0, 2);
    _grid->addWidget(_specularColorChooser->getWidget(), 0, 3);
    _grid->addWidget(_emissiveColorChooser->getWidget(), 0, 4);
    _grid->addWidget(_gridShininess, 0, 5);
}

void UserControlMesh::deinitGUI()
{
    _sliderShininess->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onSliderChangedShininess));
    _textShininess->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onTextChangedShininess));

    auto iter = _allWidgets.begin();
    while(iter != _allWidgets.end())
    {
        iWidgetManager::getInstance().destroyWidget((*iter));
        iter++;
    }

    _grid = nullptr;
    _detailsGrid = nullptr;
    _labelVertexes = nullptr;
    _labelTriangles = nullptr;
    _labelIndexes = nullptr;
    _textVertices = nullptr;
    _textTriangles = nullptr;
    _textIndexes = nullptr;

    _gridShininess = nullptr;
    _textShininess = nullptr;
    _labelShininess = nullptr;
    _labelShininessShort = nullptr;
    _sliderShininess = nullptr;

    if (_ambientColorChooser != nullptr)
    {
        delete _ambientColorChooser;
        _ambientColorChooser = nullptr;
    }

    if (_emissiveColorChooser != nullptr)
    {
        delete _emissiveColorChooser;
        _emissiveColorChooser = nullptr;
    }

    if (_diffuseColorChooser != nullptr)
    {
        delete _diffuseColorChooser;
        _diffuseColorChooser = nullptr;
    }

    if (_specularColorChooser != nullptr)
    {
        delete _specularColorChooser;
        _specularColorChooser = nullptr;
    }
}

iWidget* UserControlMesh::getWidget()
{
    return _grid;
}
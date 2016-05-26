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
#include <iTextureResourceFactory.h>
using namespace Igor;

UserControlMesh::UserControlMesh()
{
    initGUI();
}

UserControlMesh::~UserControlMesh()
{
    deinitGUI();
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

void UserControlMesh::updateNode()
{
    if (!_ignoreNodeUpdate)
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

            if (!_textTexture0->getText().isEmpty())
            {
                node->getTargetMaterial()->setTexture(iTextureResourceFactory::getInstance().loadFile(_textTexture0->getText()), 0);
            }
            else
            {
                if (node->getTargetMaterial()->hasTextureUnit(0))
                {
                    node->getTargetMaterial()->setTexture(nullptr, 0);
                }
            }

            if (!_textTexture1->getText().isEmpty())
            {
                node->getTargetMaterial()->setTexture(iTextureResourceFactory::getInstance().loadFile(_textTexture1->getText()), 1);
            }
            else
            {
                if (node->getTargetMaterial()->hasTextureUnit(1))
                {
                    node->getTargetMaterial()->setTexture(nullptr, 1);
                }
            }

            if (!_textTexture2->getText().isEmpty())
            {
                node->getTargetMaterial()->setTexture(iTextureResourceFactory::getInstance().loadFile(_textTexture2->getText()), 2);
            }
            else
            {
                if (node->getTargetMaterial()->hasTextureUnit(2))
                {
                    node->getTargetMaterial()->setTexture(nullptr, 2);
                }
            }

            if (!_textTexture3->getText().isEmpty())
            {
                node->getTargetMaterial()->setTexture(iTextureResourceFactory::getInstance().loadFile(_textTexture3->getText()), 3);
            }
            else
            {
                if (node->getTargetMaterial()->hasTextureUnit(3))
                {
                    node->getTargetMaterial()->setTexture(nullptr, 3);
                }
            }
        }
    }
}

void UserControlMesh::updateGUI()
{
    iNodeMesh* node = static_cast<iNodeMesh*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        _ignoreNodeUpdate = true;

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

        if (node->getTargetMaterial()->hasTextureUnit(0))
        {
            _textTexture0->setText(node->getTargetMaterial()->getTexture(0)->getFilename());
        }

        if (node->getTargetMaterial()->hasTextureUnit(1))
        {
            _textTexture1->setText(node->getTargetMaterial()->getTexture(1)->getFilename());
        }

        if (node->getTargetMaterial()->hasTextureUnit(2))
        {
            _textTexture2->setText(node->getTargetMaterial()->getTexture(2)->getFilename());
        }

        if (node->getTargetMaterial()->hasTextureUnit(3))
        {
            _textTexture3->setText(node->getTargetMaterial()->getTexture(3)->getFilename());
        }

        _ignoreNodeUpdate = false;
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
    _grid->appendRows(6);
    _grid->setBorder(2);
    _grid->setHorrizontalAlignment(iHorrizontalAlignment::Left);
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

    iWidgetGrid* detailsGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(detailsGrid);
    detailsGrid->appendCollumns(1);
    detailsGrid->appendRows(2);
    detailsGrid->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    iWidgetLabel* labelVertexes = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelVertexes);
    labelVertexes->setText("Vertexes");
    labelVertexes->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    iWidgetLabel* labelTriangles = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelTriangles);
    labelTriangles->setText("Triangles");
    labelTriangles->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    iWidgetLabel* labelIndexes = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelIndexes);
    labelIndexes->setText("Indexes");
    labelIndexes->setHorrizontalAlignment(iHorrizontalAlignment::Left);

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

    iWidgetGrid* gridShininess = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(gridShininess);
    gridShininess->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    gridShininess->appendCollumns(2);
    gridShininess->appendRows(1);

    iWidgetLabel* labelShininess = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelShininess);
    labelShininess->setText("Shininess");
    labelShininess->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    iWidgetLabel* labelShininessShort = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelShininessShort);
    labelShininessShort->setText("S");
    labelShininessShort->setHorrizontalAlignment(iHorrizontalAlignment::Left);

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

    iWidgetGrid* gridTextures = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(gridTextures);
    gridTextures->appendRows(3);
    gridTextures->appendCollumns(1);
    gridTextures->setBorder(2);
    gridTextures->setHorrizontalAlignment(iHorrizontalAlignment::Left);
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

    _textTexture0 = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget(iWidgetType::TextEdit));
    _allWidgets.push_back(_textTexture0);
    _textTexture0->setWidth(200);
    _textTexture0->setHorrizontalAlignment(iHorrizontalAlignment::Right);
    _textTexture0->setHorrizontalTextAlignment(iHorrizontalAlignment::Left);
    _textTexture0->setText("");
    _textTexture0->registerOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onTexture0Change));

    _textTexture1 = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget(iWidgetType::TextEdit));
    _allWidgets.push_back(_textTexture1);
    _textTexture1->setWidth(200);
    _textTexture1->setHorrizontalAlignment(iHorrizontalAlignment::Right);
    _textTexture1->setHorrizontalTextAlignment(iHorrizontalAlignment::Left);
    _textTexture1->setText("");
    _textTexture1->registerOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onTexture1Change));

    _textTexture2 = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget(iWidgetType::TextEdit));
    _allWidgets.push_back(_textTexture2);
    _textTexture2->setWidth(200);
    _textTexture2->setHorrizontalAlignment(iHorrizontalAlignment::Right);
    _textTexture2->setHorrizontalTextAlignment(iHorrizontalAlignment::Left);
    _textTexture2->setText("");
    _textTexture2->registerOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onTexture2Change));

    _textTexture3 = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget(iWidgetType::TextEdit));
    _allWidgets.push_back(_textTexture3);
    _textTexture3->setWidth(200);
    _textTexture3->setHorrizontalAlignment(iHorrizontalAlignment::Right);
    _textTexture3->setHorrizontalTextAlignment(iHorrizontalAlignment::Left);
    _textTexture3->setText("");
    _textTexture3->registerOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onTexture3Change));

    gridShininess->addWidget(labelShininess, 1, 0);
    gridShininess->addWidget(_sliderShininess, 1, 1);
    gridShininess->addWidget(labelShininessShort, 0, 1);
    gridShininess->addWidget(_textShininess, 2, 1);

    detailsGrid->addWidget(labelVertexes, 0, 0);
    detailsGrid->addWidget(labelTriangles, 0, 1);
    detailsGrid->addWidget(labelIndexes, 0, 2);

    detailsGrid->addWidget(_textVertices, 1, 0);
    detailsGrid->addWidget(_textTriangles, 1, 1);
    detailsGrid->addWidget(_textIndexes, 1, 2);

    gridTextures->addWidget(labelTextureUnit0, 0, 0);
    gridTextures->addWidget(labelTextureUnit1, 0, 1);
    gridTextures->addWidget(labelTextureUnit2, 0, 2);
    gridTextures->addWidget(labelTextureUnit3, 0, 3);
    gridTextures->addWidget(_textTexture0, 1, 0);
    gridTextures->addWidget(_textTexture1, 1, 1);
    gridTextures->addWidget(_textTexture2, 1, 2);
    gridTextures->addWidget(_textTexture3, 1, 3);

    _grid->addWidget(detailsGrid, 0, 0);
    _grid->addWidget(_ambientColorChooser->getWidget(), 0, 1);
    _grid->addWidget(_diffuseColorChooser->getWidget(), 0, 2);
    _grid->addWidget(_specularColorChooser->getWidget(), 0, 3);
    _grid->addWidget(_emissiveColorChooser->getWidget(), 0, 4);
    _grid->addWidget(gridShininess, 0, 5);
    _grid->addWidget(gridTextures, 0, 6);
}

void UserControlMesh::deinitGUI()
{
    _sliderShininess->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onSliderChangedShininess));
    _textShininess->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onTextChangedShininess));

    auto iter = _allWidgets.begin();
    while (iter != _allWidgets.end())
    {
        iWidgetManager::getInstance().destroyWidget((*iter));
        iter++;
    }

    _grid = nullptr;
    _textVertices = nullptr;
    _textTriangles = nullptr;
    _textIndexes = nullptr;
    _textShininess = nullptr;
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

void UserControlMesh::onTexture0Change(iWidget* source)
{
    updateNode();
}

void UserControlMesh::onTexture1Change(iWidget* source)
{
    updateNode();
}

void UserControlMesh::onTexture2Change(iWidget* source)
{
    updateNode();
}

void UserControlMesh::onTexture3Change(iWidget* source)
{
    updateNode();
}
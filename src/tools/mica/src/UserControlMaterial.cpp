// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlMaterial.h"

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/widgets/iWidgetGrid.h>
#include <igor/ui/widgets/iWidgetTextEdit.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/widgets/iWidgetSlider.h>
#include <igor/ui/widgets/iWidgetNumberChooser.h>
#include <igor/resources/material/iTargetMaterial.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/ui/widgets/iWidgetCheckBox.h>
#include <igor/ui/widgets/iWidgetSelectBox.h>
#include <igor/resources/iResourceManager.h>
#include <igor/ui/widgets/iWidgetGroupBox.h>
#include <igor/graphics/iRenderer.h>
using namespace igor;

UserControlMaterial::UserControlMaterial()
{
    initGUI();
}

UserControlMaterial::~UserControlMaterial()
{
	deinitGUI();
}

void UserControlMaterial::updateMaterial()
{
    iMaterialPtr material = iMaterialResourceFactory::getInstance().getMaterial(_materialID);

    if (!_ignoreMaterialUpdate &&
        material != nullptr &&
        material->isValid())
    {
        material->setName(_textName->getText());
        material->setOrder(static_cast<int32>(_renderingOrder->getValue()));
        material->setRenderState(iRenderState::CullFace, _checkBoxCullFace->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->setRenderState(iRenderState::DepthTest, _checkBoxDepthTest->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->setRenderState(iRenderState::DepthMask, _checkBoxDepthMask->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->setRenderState(iRenderState::Blend, _checkBoxBlend->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->setRenderState(iRenderState::Texture2D0, _checkBoxTexture2D0->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->setRenderState(iRenderState::Texture2D1, _checkBoxTexture2D1->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->setRenderState(iRenderState::Texture2D2, _checkBoxTexture2D2->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->setRenderState(iRenderState::Texture2D3, _checkBoxTexture2D3->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->setRenderState(iRenderState::Wireframe, _checkBoxWireframe->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        //material->setRenderState(iRenderState::Instanced, _checkBoxInstanced->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->setRenderState(iRenderState::DepthFunc, static_cast<iRenderStateValue>(_selectBoxDepthFunc->getSelectedIndex() + static_cast<int>(iRenderStateValue::Never)));
        material->setRenderState(iRenderState::CullFaceFunc, static_cast<iRenderStateValue>(_selectBoxCullFaceFunc->getSelectedIndex() + static_cast<int>(iRenderStateValue::Front)));

        // TODO optimize!
        iRenderStateValue value;
        if (_selectBoxBlendFuncSource->getSelectedIndex() != -1)
        {
            switch (_selectBoxBlendFuncSource->getSelectedIndex())
            {
            case 0: value = iRenderStateValue::Zero; break;
            case 1: value = iRenderStateValue::One; break;
            case 2: value = iRenderStateValue::SourceColor; break;
            case 3: value = iRenderStateValue::OneMinusSourceColor; break;
            case 4: value = iRenderStateValue::DestinationColor; break;
            case 5: value = iRenderStateValue::OneMinusDestinationColor; break;
            case 6: value = iRenderStateValue::SourceAlpha; break;
            case 7: value = iRenderStateValue::OneMinusSourceAlpha; break;
            case 8: value = iRenderStateValue::DestinationAlpha; break;
            case 9: value = iRenderStateValue::OneMinusDestinationAlpha; break;
            }

            material->setRenderState(iRenderState::BlendFuncSource, value);
        }

        if (_selectBoxBlendFuncDestination->getSelectedIndex() != -1)
        {
            switch (_selectBoxBlendFuncDestination->getSelectedIndex())
            {
            case 0: value = iRenderStateValue::Zero; break;
            case 1: value = iRenderStateValue::One; break;
            case 2: value = iRenderStateValue::SourceColor; break;
            case 3: value = iRenderStateValue::OneMinusSourceColor; break;
            case 4: value = iRenderStateValue::DestinationColor; break;
            case 5: value = iRenderStateValue::OneMinusDestinationColor; break;
            case 6: value = iRenderStateValue::SourceAlpha; break;
            case 7: value = iRenderStateValue::OneMinusSourceAlpha; break;
            case 8: value = iRenderStateValue::DestinationAlpha; break;
            case 9: value = iRenderStateValue::OneMinusDestinationAlpha; break;
            }
            material->setRenderState(iRenderState::BlendFuncDestination, value);
        }

        reloadShader(material);

        // TODO		_selectBoxInstancedFunc
    }
}

void UserControlMaterial::reloadShader(iMaterialPtr material)
{
    material->clearShader();
    if (_textShaderGeometry->getText() != "")
    {
        material->addShaderSource(_textShaderGeometry->getText(), iShaderObjectType::Geometry);
    }

    if (_textShaderVertex->getText() != "")
    {
        material->addShaderSource(_textShaderVertex->getText(), iShaderObjectType::Vertex);
    }

    if (_textShaderFragment->getText() != "")
    {
        material->addShaderSource(_textShaderFragment->getText(), iShaderObjectType::Fragment);
    }
    material->compileShader();
}

void UserControlMaterial::updateGUI()
{
    auto material = iMaterialResourceFactory::getInstance().getMaterial(_materialID);

    if (material != nullptr &&
        material->isValid())
    {
        _ignoreMaterialUpdate = true;

        _textName->setText(material->getName());
        _checkBoxCullFace->setChecked(material->getRenderState(iRenderState::CullFace) == iRenderStateValue::On ? true : false);
        _checkBoxDepthTest->setChecked(material->getRenderState(iRenderState::DepthTest) == iRenderStateValue::On ? true : false);
        _checkBoxDepthMask->setChecked(material->getRenderState(iRenderState::DepthMask) == iRenderStateValue::On ? true : false);
        _checkBoxBlend->setChecked(material->getRenderState(iRenderState::Blend) == iRenderStateValue::On ? true : false);
        _checkBoxTexture2D0->setChecked(material->getRenderState(iRenderState::Texture2D0) == iRenderStateValue::On ? true : false);
        _checkBoxTexture2D1->setChecked(material->getRenderState(iRenderState::Texture2D1) == iRenderStateValue::On ? true : false);
        _checkBoxTexture2D2->setChecked(material->getRenderState(iRenderState::Texture2D2) == iRenderStateValue::On ? true : false);
        _checkBoxTexture2D3->setChecked(material->getRenderState(iRenderState::Texture2D3) == iRenderStateValue::On ? true : false);
        _checkBoxWireframe->setChecked(material->getRenderState(iRenderState::Wireframe) == iRenderStateValue::On ? true : false);
        //_checkBoxInstanced->setChecked(material->getRenderState(iRenderState::Instanced) == iRenderStateValue::On ? true : false);

        _selectBoxDepthFunc->setSelection(static_cast<int>(material->getRenderState(iRenderState::DepthFunc)) - static_cast<int>(iRenderStateValue::Never));
        _selectBoxCullFaceFunc->setSelection(static_cast<int>(material->getRenderState(iRenderState::CullFaceFunc)) - static_cast<int>(iRenderStateValue::Front));

        // TODO optimize!
        int selection = 0;
        switch (material->getRenderState(iRenderState::BlendFuncSource))
        {
        case iRenderStateValue::Zero: selection = 0; break;
        case iRenderStateValue::One: selection = 1; break;
        case iRenderStateValue::SourceColor: selection = 2; break;
        case iRenderStateValue::OneMinusSourceColor: selection = 3; break;
        case iRenderStateValue::DestinationColor: selection = 4; break;
        case iRenderStateValue::OneMinusDestinationColor: selection = 5; break;
        case iRenderStateValue::SourceAlpha: selection = 6; break;
        case iRenderStateValue::OneMinusSourceAlpha: selection = 7; break;
        case iRenderStateValue::DestinationAlpha: selection = 8; break;
        case iRenderStateValue::OneMinusDestinationAlpha: selection = 9; break;
        }

        _selectBoxBlendFuncSource->setSelection(selection);

        switch (material->getRenderState(iRenderState::BlendFuncDestination))
        {
        case iRenderStateValue::Zero: selection = 0; break;
        case iRenderStateValue::One: selection = 1; break;
        case iRenderStateValue::SourceColor: selection = 2; break;
        case iRenderStateValue::OneMinusSourceColor: selection = 3; break;
        case iRenderStateValue::DestinationColor: selection = 4; break;
        case iRenderStateValue::OneMinusDestinationColor: selection = 5; break;
        case iRenderStateValue::SourceAlpha: selection = 6; break;
        case iRenderStateValue::OneMinusSourceAlpha: selection = 7; break;
        case iRenderStateValue::DestinationAlpha: selection = 8; break;
        case iRenderStateValue::OneMinusDestinationAlpha: selection = 9; break;
        }

        _selectBoxBlendFuncDestination->setSelection(selection);
        //_selectBoxInstancedFunc = nullptr;

        auto shaderSources = material->getShaderSources();

        if(!shaderSources.empty())
        {
            for (auto shader : shaderSources)
            {
                iaString filename = shader._filename;
                // todo need contains or begins with function for iaString iaString shortName = iResourceManager::getInstance().getRelativePath(filename);
                //   if (!shortName.isEmpty())
       //            {
         //              filename = shortName;
           //        }

                switch (shader._type)
                {
                case iShaderObjectType::Geometry:
                    _textShaderGeometry->setText(filename);
                    break;

                case iShaderObjectType::Vertex:
                    _textShaderVertex->setText(filename);
                    break;

                case iShaderObjectType::Fragment:
                    _textShaderFragment->setText(filename);
                    break;
                }
            }
        }
        else
        {
            _textShaderGeometry->setText("");
            _textShaderVertex->setText("");
            _textShaderFragment->setText("");
        }

        _ignoreMaterialUpdate = false;
    }
}

void UserControlMaterial::setMaterial(uint64 id)
{
    _materialID = id;
    updateGUI();
}

uint64 UserControlMaterial::getMaterialID() const
{
    return _materialID;
}

void UserControlMaterial::initGUI()
{
    iWidgetGridPtr grid = new iWidgetGrid(this);
    grid->appendRows(1);
    grid->setBorder(2);
    grid->setStrechColumn(0);
    grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
    grid->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGroupBox* paramGroupBox = new iWidgetGroupBox();
    paramGroupBox->setHorizontalAlignment(iHorizontalAlignment::Strech);
    paramGroupBox->setText("Render States");
    paramGroupBox->setHeaderOnly();

    iWidgetGrid* gridParam = new iWidgetGrid();
    gridParam->appendRows(14);
    gridParam->appendCollumns(1);
    gridParam->setBorder(2);
    gridParam->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridParam->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel* labelName = new iWidgetLabel();
    labelName->setText("Name");
    labelName->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textName = new iWidgetTextEdit();
    _textName->setMaxTextLength(100);
    _textName->setWidth(200);
    _textName->setHorizontalAlignment(iHorizontalAlignment::Left);
    _textName->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textName->setText("...");
    _textName->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onTextChangedName));

    iWidgetLabel* labelDepthTest = new iWidgetLabel();
    labelDepthTest->setText("Depth Test");
    labelDepthTest->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxDepthTest = new iWidgetCheckBox();
    _checkBoxDepthTest->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxDepthTest->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelDepthFunction = new iWidgetLabel();
    labelDepthFunction->setText("Depth Function");
    labelDepthFunction->setHorizontalAlignment(iHorizontalAlignment::Left);

    _selectBoxDepthFunc = new iWidgetSelectBox();
    _selectBoxDepthFunc->addSelectionEntry("Never");
    _selectBoxDepthFunc->addSelectionEntry("Less");
    _selectBoxDepthFunc->addSelectionEntry("Equal");
    _selectBoxDepthFunc->addSelectionEntry("LessOrEqual");
    _selectBoxDepthFunc->addSelectionEntry("Greater");
    _selectBoxDepthFunc->addSelectionEntry("NotEqual");
    _selectBoxDepthFunc->addSelectionEntry("GreaterOrEqual");
    _selectBoxDepthFunc->addSelectionEntry("Always");
    _selectBoxDepthFunc->setHorizontalAlignment(iHorizontalAlignment::Left);
    _selectBoxDepthFunc->setWidth(200);

    iWidgetLabel* labelDepthMask = new iWidgetLabel();
    labelDepthMask->setText("Depth Mask");
    labelDepthMask->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxDepthMask = new iWidgetCheckBox();
    _checkBoxDepthMask->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxDepthMask->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelBlend = new iWidgetLabel();
    labelBlend->setText("Blending");
    labelBlend->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxBlend = new iWidgetCheckBox();
    _checkBoxBlend->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxBlend->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelBlendFuncSource = new iWidgetLabel();
    labelBlendFuncSource->setText("Blend Source");
    labelBlendFuncSource->setHorizontalAlignment(iHorizontalAlignment::Left);

    _selectBoxBlendFuncSource = new iWidgetSelectBox();
    _selectBoxBlendFuncSource->addSelectionEntry("Zero");
    _selectBoxBlendFuncSource->addSelectionEntry("One");
    _selectBoxBlendFuncSource->addSelectionEntry("SourceColor");
    _selectBoxBlendFuncSource->addSelectionEntry("OneMinusSourceColor");
    _selectBoxBlendFuncSource->addSelectionEntry("DestinationColor");
    _selectBoxBlendFuncSource->addSelectionEntry("OneMinusDestinationColor");
    _selectBoxBlendFuncSource->addSelectionEntry("SourceAlpha");
    _selectBoxBlendFuncSource->addSelectionEntry("OneMinusSourceAlpha");
    _selectBoxBlendFuncSource->addSelectionEntry("DestinationAlpha");
    _selectBoxBlendFuncSource->addSelectionEntry("OneMinusDestinationAlpha");
    _selectBoxBlendFuncSource->setHorizontalAlignment(iHorizontalAlignment::Left);
    _selectBoxBlendFuncSource->setWidth(200);

    iWidgetLabel* labelBlendFuncDestination = new iWidgetLabel();
    labelBlendFuncDestination->setText("Blend Destination");
    labelBlendFuncDestination->setHorizontalAlignment(iHorizontalAlignment::Left);

    _selectBoxBlendFuncDestination = new iWidgetSelectBox();
    _selectBoxBlendFuncDestination->addSelectionEntry("Zero");
    _selectBoxBlendFuncDestination->addSelectionEntry("One");
    _selectBoxBlendFuncDestination->addSelectionEntry("SourceColor");
    _selectBoxBlendFuncDestination->addSelectionEntry("OneMinusSourceColor");
    _selectBoxBlendFuncDestination->addSelectionEntry("DestinationColor");
    _selectBoxBlendFuncDestination->addSelectionEntry("OneMinusDestinationColor");
    _selectBoxBlendFuncDestination->addSelectionEntry("SourceAlpha");
    _selectBoxBlendFuncDestination->addSelectionEntry("OneMinusSourceAlpha");
    _selectBoxBlendFuncDestination->addSelectionEntry("DestinationAlpha");
    _selectBoxBlendFuncDestination->addSelectionEntry("OneMinusDestinationAlpha");
    _selectBoxBlendFuncDestination->setHorizontalAlignment(iHorizontalAlignment::Left);
    _selectBoxBlendFuncDestination->setWidth(200);

    iWidgetLabel* labelCullFace = new iWidgetLabel();
    labelCullFace->setText("Cull Face");
    labelCullFace->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxCullFace = new iWidgetCheckBox();
    _checkBoxCullFace->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxCullFace->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelCullFaceFunc = new iWidgetLabel();
    labelCullFaceFunc->setText("Cull Face Function");
    labelCullFaceFunc->setHorizontalAlignment(iHorizontalAlignment::Left);

    _selectBoxCullFaceFunc = new iWidgetSelectBox();
    _selectBoxCullFaceFunc->addSelectionEntry("Front");
    _selectBoxCullFaceFunc->addSelectionEntry("Back");
    _selectBoxCullFaceFunc->setHorizontalAlignment(iHorizontalAlignment::Left);
    _selectBoxCullFaceFunc->setWidth(200);

    iWidgetLabel* labelTexture2D0 = new iWidgetLabel();
    labelTexture2D0->setText("Texture 0");
    labelTexture2D0->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxTexture2D0 = new iWidgetCheckBox();
    _checkBoxTexture2D0->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxTexture2D0->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelTexture2D1 = new iWidgetLabel();
    labelTexture2D1->setText("Texture 1");
    labelTexture2D1->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxTexture2D1 = new iWidgetCheckBox();
    _checkBoxTexture2D1->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxTexture2D1->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelTexture2D2 = new iWidgetLabel();
    labelTexture2D2->setText("Texture 2");
    labelTexture2D2->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxTexture2D2 = new iWidgetCheckBox();
    _checkBoxTexture2D2->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxTexture2D2->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelTexture2D3 = new iWidgetLabel();
    labelTexture2D3->setText("Texture 3");
    labelTexture2D3->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxTexture2D3 = new iWidgetCheckBox();
    _checkBoxTexture2D3->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxTexture2D3->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelWireframe = new iWidgetLabel();
    labelWireframe->setText("Wireframe");
    labelWireframe->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxWireframe = new iWidgetCheckBox();
    _checkBoxWireframe->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxWireframe->setHorizontalAlignment(iHorizontalAlignment::Left);

    // TODO
    /*iWidgetLabel* labelInstanced = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label));
    _allWidgets.push_back(labelInstanced);
    labelInstanced->setText("Instanced");
    labelInstanced->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxInstanced = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox));
    _checkBoxInstanced->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));

    iWidgetSelectBox* _selectBoxInstancedFunc = nullptr;*/

    iWidgetLabel* labelRenderingOrder = new iWidgetLabel();
    labelRenderingOrder->setText("Rendering Order");
    labelRenderingOrder->setHorizontalAlignment(iHorizontalAlignment::Left);

    _renderingOrder = new iWidgetNumberChooser();
    _renderingOrder->setMinMaxNumber(iMaterial::RENDER_ORDER_MIN, iMaterial::RENDER_ORDER_MAX);
    _renderingOrder->setAfterPoint(0);
    _renderingOrder->setValue(iMaterial::RENDER_ORDER_DEFAULT);
    _renderingOrder->setWidth(80);
    _renderingOrder->setSteppingWheel(10.0f, 10.0f);
    _renderingOrder->setStepping(1.0f, 1.0f);
    _renderingOrder->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _renderingOrder->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetGroupBox* shaderGroupBox = new iWidgetGroupBox();
    shaderGroupBox->setHorizontalAlignment(iHorizontalAlignment::Strech);
    shaderGroupBox->setText("Shader Settings");
    shaderGroupBox->setHeaderOnly();

    iWidgetGrid* gridShadersGroup = new iWidgetGrid();
    gridShadersGroup->appendRows(1);
    gridShadersGroup->setBorder(2);
    gridShadersGroup->setHorizontalAlignment(iHorizontalAlignment::Strech);
    gridShadersGroup->setVerticalAlignment(iVerticalAlignment::Top);
    gridShadersGroup->setStrechColumn(0);

    iWidgetGrid* gridShaders = new iWidgetGrid();
    gridShaders->appendRows(2);
    gridShaders->appendCollumns(2);
    gridShaders->setBorder(2);
    gridShaders->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridShaders->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel* labelShader0 = new iWidgetLabel();
    labelShader0->setText("Geometry");
    labelShader0->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelShader1 = new iWidgetLabel();
    labelShader1->setText("Vertex");
    labelShader1->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelShader2 = new iWidgetLabel();
    labelShader2->setText("Fragment");
    labelShader2->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textShaderGeometry = new iWidgetTextEdit();
    _textShaderGeometry->setWidth(200);
    _textShaderGeometry->setMaxTextLength(200);
    _textShaderGeometry->setHorizontalAlignment(iHorizontalAlignment::Right);
    _textShaderGeometry->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textShaderGeometry->setText("");
    _textShaderGeometry->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));

    _textShaderVertex = new iWidgetTextEdit();
    _textShaderVertex->setWidth(200);
    _textShaderVertex->setMaxTextLength(200);
    _textShaderVertex->setHorizontalAlignment(iHorizontalAlignment::Right);
    _textShaderVertex->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textShaderVertex->setText("");
    _textShaderVertex->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));

    _textShaderFragment = new iWidgetTextEdit();
    _textShaderFragment->setWidth(200);
    _textShaderFragment->setMaxTextLength(200);
    _textShaderFragment->setHorizontalAlignment(iHorizontalAlignment::Right);
    _textShaderFragment->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textShaderFragment->setText("");
    _textShaderFragment->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));

    _shader0Button = new iWidgetButton();
    _shader0Button->setWidth(20);
    _shader0Button->setHeight(20);
    _shader0Button->setText("...");
	_shader0Button->setTooltip("Browse for geometry shader");
    _shader0Button->registerOnClickEvent(iClickDelegate(this, &UserControlMaterial::onShader0Button));

    _shader1Button = new iWidgetButton();
    _shader1Button->setWidth(20);
    _shader1Button->setHeight(20);
    _shader1Button->setText("...");
	_shader1Button->setTooltip("Browse for vertex shader");
    _shader1Button->registerOnClickEvent(iClickDelegate(this, &UserControlMaterial::onShader1Button));

    _shader2Button = new iWidgetButton();
    _shader2Button->setWidth(20);
    _shader2Button->setHeight(20);
    _shader2Button->setText("...");
	_shader2Button->setTooltip("Browse for fragment shader");
    _shader2Button->registerOnClickEvent(iClickDelegate(this, &UserControlMaterial::onShader2Button));

    _shaderReload = new iWidgetButton();
    _shaderReload->setWidth(20);
    _shaderReload->setHeight(20);
    _shaderReload->setText("Reload");
	_shaderReload->setTooltip("Reload shaders");
    _shaderReload->setHorizontalAlignment(iHorizontalAlignment::Right);
    _shaderReload->registerOnClickEvent(iClickDelegate(this, &UserControlMaterial::onReloadShader));

    shaderGroupBox->addWidget(gridShadersGroup);

    gridShadersGroup->addWidget(gridShaders, 0, 0);
    gridShadersGroup->addWidget(_shaderReload, 0, 1);

    gridShaders->addWidget(labelShader0, 0, 0);
    gridShaders->addWidget(labelShader1, 0, 1);
    gridShaders->addWidget(labelShader2, 0, 2);

    gridShaders->addWidget(_textShaderGeometry, 1, 0);
    gridShaders->addWidget(_textShaderVertex, 1, 1);
    gridShaders->addWidget(_textShaderFragment, 1, 2);

    gridShaders->addWidget(_shader0Button, 2, 0);
    gridShaders->addWidget(_shader1Button, 2, 1);
    gridShaders->addWidget(_shader2Button, 2, 2);

    gridParam->addWidget(labelName, 0, 0);
    gridParam->addWidget(labelDepthTest, 0, 1);
    gridParam->addWidget(labelDepthFunction, 0, 2);
    gridParam->addWidget(labelDepthMask, 0, 3);
    gridParam->addWidget(labelBlend, 0, 4);
    gridParam->addWidget(labelBlendFuncSource, 0, 5);
    gridParam->addWidget(labelBlendFuncDestination, 0, 6);
    gridParam->addWidget(labelCullFace, 0, 7);
    gridParam->addWidget(labelCullFaceFunc, 0, 8);
    gridParam->addWidget(labelTexture2D0, 0, 9);
    gridParam->addWidget(labelTexture2D1, 0, 10);
    gridParam->addWidget(labelTexture2D2, 0, 11);
    gridParam->addWidget(labelTexture2D3, 0, 12);
    gridParam->addWidget(labelWireframe, 0, 13);
    gridParam->addWidget(labelRenderingOrder, 0, 14);

    gridParam->addWidget(_textName, 1, 0);
    gridParam->addWidget(_checkBoxDepthTest, 1, 1);
    gridParam->addWidget(_selectBoxDepthFunc, 1, 2);
    gridParam->addWidget(_checkBoxDepthMask, 1, 3);
    gridParam->addWidget(_checkBoxBlend, 1, 4);
    gridParam->addWidget(_selectBoxBlendFuncSource, 1, 5);
    gridParam->addWidget(_selectBoxBlendFuncDestination, 1, 6);
    gridParam->addWidget(_checkBoxCullFace, 1, 7);
    gridParam->addWidget(_selectBoxCullFaceFunc, 1, 8);
    gridParam->addWidget(_checkBoxTexture2D0, 1, 9);
    gridParam->addWidget(_checkBoxTexture2D1, 1, 10);
    gridParam->addWidget(_checkBoxTexture2D2, 1, 11);
    gridParam->addWidget(_checkBoxTexture2D3, 1, 12);
    gridParam->addWidget(_checkBoxWireframe, 1, 13);
    gridParam->addWidget(_renderingOrder, 1, 14);

    paramGroupBox->addWidget(gridParam);

    grid->addWidget(paramGroupBox, 0, 0);
    grid->addWidget(shaderGroupBox, 0, 1);
}

void UserControlMaterial::onDoUpdateMaterial(const iWidgetPtr source)
{
    updateMaterial();
}

void UserControlMaterial::onTextChangedName(const iWidgetPtr source)
{
    updateMaterial();
    _materialNameChangedEvent();
}

void UserControlMaterial::deinitGUI()
{
    if (_fileDialog != nullptr)
    {
        delete _fileDialog;
        _fileDialog = nullptr;
    }
}

void UserControlMaterial::registerNameChangeDelegate(MaterialNameChangedDelegate nameChangedDelegate)
{
    _materialNameChangedEvent.append(nameChangedDelegate);
}

void UserControlMaterial::unregisterNameChangeDelegate(MaterialNameChangedDelegate nameChangedDelegate)
{
    _materialNameChangedEvent.remove(nameChangedDelegate);
}

void UserControlMaterial::onShader0Button(const iWidgetPtr source)
{
    _loadShaderNumber = 0;

    if (_fileDialog == nullptr)
    {
        _fileDialog = new iDialogFileSelect();
    }

    _fileDialog->open(iDialogCloseDelegate(this, &UserControlMaterial::onFileLoadDialogClosed), iFileDialogPurpose::Load, "..\\data\\shaders"); // TODO hard coded path
}

void UserControlMaterial::onShader1Button(const iWidgetPtr source)
{
    _loadShaderNumber = 1;

    if (_fileDialog == nullptr)
    {
        _fileDialog = new iDialogFileSelect();
    }

    _fileDialog->open(iDialogCloseDelegate(this, &UserControlMaterial::onFileLoadDialogClosed), iFileDialogPurpose::Load, "..\\data\\shaders"); // TODO hard coded path
}

void UserControlMaterial::onShader2Button(const iWidgetPtr source)
{
    _loadShaderNumber = 2;

    if (_fileDialog == nullptr)
    {
        _fileDialog = new iDialogFileSelect();
    }

    _fileDialog->open(iDialogCloseDelegate(this, &UserControlMaterial::onFileLoadDialogClosed), iFileDialogPurpose::Load, "..\\data\\shaders"); // TODO hard coded path
}

void UserControlMaterial::onReloadShader(const iWidgetPtr source)
{
    auto material = iMaterialResourceFactory::getInstance().getMaterial(_materialID);

    if (!_ignoreMaterialUpdate &&
        material != nullptr &&
        material->isValid())
    {
        reloadShader(material);
    }
}

void UserControlMaterial::onFileLoadDialogClosed(iDialogPtr dialog)
{
    if (_fileDialog != dialog)
    {
        return;
    }

    if (_fileDialog->getReturnState() == iDialogReturnState::Ok)
    {
        iaString filename = iResourceManager::getInstance().getRelativePath(_fileDialog->getFullPath());
        switch (_loadShaderNumber)
        {
        case 0:
            _textShaderGeometry->setText(filename);
            break;

        case 1:
            _textShaderVertex->setText(filename);
            break;

        case 2:
            _textShaderFragment->setText(filename);
            break;

        default:
            con_err("out of range");
        }
        updateMaterial();
    }

    delete _fileDialog;
    _fileDialog = nullptr;
}


// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlMaterial.h"

#include <iWidgetManager.h>
#include <iWidgetGrid.h>
#include <iWidgetTextEdit.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetSlider.h>
#include <iWidgetNumberChooser.h>
#include <iTargetMaterial.h>
#include <iMaterialResourceFactory.h>
#include <iMaterial.h>
#include <iMaterialGroup.h>
#include <iWidgetCheckBox.h>
#include <iWidgetSelectBox.h>
#include <iResourceManager.h>
#include <iWidgetGroupBox.h>
using namespace Igor;

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
    iMaterial* material = iMaterialResourceFactory::getInstance().getMaterial(_materialID);

    if (!_ignoreMaterialUpdate &&
        material != nullptr)
    {
        material->setName(_textName->getText());
        material->setOrder(static_cast<int32>(_renderingOrder->getValue()));
        material->getRenderStateSet().setRenderState(iRenderState::CullFace, _checkBoxCullFace->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->getRenderStateSet().setRenderState(iRenderState::DepthTest, _checkBoxDepthTest->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->getRenderStateSet().setRenderState(iRenderState::DepthMask, _checkBoxDepthMask->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->getRenderStateSet().setRenderState(iRenderState::Blend, _checkBoxBlend->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->getRenderStateSet().setRenderState(iRenderState::Texture2D0, _checkBoxTexture2D0->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->getRenderStateSet().setRenderState(iRenderState::Texture2D1, _checkBoxTexture2D1->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->getRenderStateSet().setRenderState(iRenderState::Texture2D2, _checkBoxTexture2D2->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->getRenderStateSet().setRenderState(iRenderState::Texture2D3, _checkBoxTexture2D3->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->getRenderStateSet().setRenderState(iRenderState::Wireframe, _checkBoxWireframe->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        //material->getRenderStateSet().setRenderState(iRenderState::Instanced, _checkBoxInstanced->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
        material->getRenderStateSet().setRenderState(iRenderState::DepthFunc, static_cast<iRenderStateValue>(_selectBoxDepthFunc->getSelectedIndex() + static_cast<int>(iRenderStateValue::Never)));
        material->getRenderStateSet().setRenderState(iRenderState::CullFaceFunc, static_cast<iRenderStateValue>(_selectBoxCullFaceFunc->getSelectedIndex() + static_cast<int>(iRenderStateValue::Front)));

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

            material->getRenderStateSet().setRenderState(iRenderState::BlendFuncSource, value);
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
            material->getRenderStateSet().setRenderState(iRenderState::BlendFuncDestination, value);
        }

        reloadShader(material);

        // TODO		_selectBoxInstancedFunc
    }
}

void UserControlMaterial::reloadShader(iMaterial* material)
{
    material->clearShader();
    if (_textShader0->getText() != "")
    {
        material->addShaderSource(_textShader0->getText(), iShaderObjectType::Geometry);
    }

    if (_textShader1->getText() != "")
    {
        material->addShaderSource(_textShader1->getText(), iShaderObjectType::Vertex);
    }

    if (_textShader2->getText() != "")
    {
        material->addShaderSource(_textShader2->getText(), iShaderObjectType::Fragment);
    }
    material->compileShader();
}

void UserControlMaterial::updateGUI()
{
    iMaterial* material = iMaterialResourceFactory::getInstance().getMaterial(_materialID);

    if (material != nullptr)
    {
        _ignoreMaterialUpdate = true;

        _textName->setText(material->getName());
        _checkBoxCullFace->setChecked(material->getRenderStateSet().getRenderStateValue(iRenderState::CullFace) == iRenderStateValue::On ? true : false);
        _checkBoxDepthTest->setChecked(material->getRenderStateSet().getRenderStateValue(iRenderState::DepthTest) == iRenderStateValue::On ? true : false);
        _checkBoxDepthMask->setChecked(material->getRenderStateSet().getRenderStateValue(iRenderState::DepthMask) == iRenderStateValue::On ? true : false);
        _checkBoxBlend->setChecked(material->getRenderStateSet().getRenderStateValue(iRenderState::Blend) == iRenderStateValue::On ? true : false);
        _checkBoxTexture2D0->setChecked(material->getRenderStateSet().getRenderStateValue(iRenderState::Texture2D0) == iRenderStateValue::On ? true : false);
        _checkBoxTexture2D1->setChecked(material->getRenderStateSet().getRenderStateValue(iRenderState::Texture2D1) == iRenderStateValue::On ? true : false);
        _checkBoxTexture2D2->setChecked(material->getRenderStateSet().getRenderStateValue(iRenderState::Texture2D2) == iRenderStateValue::On ? true : false);
        _checkBoxTexture2D3->setChecked(material->getRenderStateSet().getRenderStateValue(iRenderState::Texture2D3) == iRenderStateValue::On ? true : false);
        _checkBoxWireframe->setChecked(material->getRenderStateSet().getRenderStateValue(iRenderState::Wireframe) == iRenderStateValue::On ? true : false);
        //_checkBoxInstanced->setChecked(material->getRenderStateSet().getRenderStateValue(iRenderState::Instanced) == iRenderStateValue::On ? true : false);

        _selectBoxDepthFunc->setSelection(static_cast<int>(material->getRenderStateSet().getRenderStateValue(iRenderState::DepthFunc)) - static_cast<int>(iRenderStateValue::Never));
        _selectBoxCullFaceFunc->setSelection(static_cast<int>(material->getRenderStateSet().getRenderStateValue(iRenderState::CullFaceFunc)) - static_cast<int>(iRenderStateValue::Front));

        // TODO optimize!
        int selection = 0;
        switch (material->getRenderStateSet().getRenderStateValue(iRenderState::BlendFuncSource))
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

        switch (material->getRenderStateSet().getRenderStateValue(iRenderState::BlendFuncDestination))
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

        if (material->hasShader())
        {
            for (auto shader : material->getShaderSources())
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
                    _textShader0->setText(filename);
                    break;

                case iShaderObjectType::Vertex:
                    _textShader1->setText(filename);
                    break;

                case iShaderObjectType::Fragment:
                    _textShader2->setText(filename);
                    break;
                }
            }
        }
        else
        {
            _textShader0->setText("");
            _textShader1->setText("");
            _textShader2->setText("");
        }

        _ignoreMaterialUpdate = false;
    }
}

void UserControlMaterial::setMaterial(uint32 id)
{
    _materialID = id;
    updateGUI();
}

uint32 UserControlMaterial::getMaterial()
{
    return _materialID;
}

void UserControlMaterial::initGUI()
{
    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(_grid);
    _grid->appendRows(1);
    _grid->setBorder(2);
    _grid->setStrechColumn(0);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGroupBox* paramGroupBox = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget("GroupBox"));
    _allWidgets.push_back(paramGroupBox);
    paramGroupBox->setHorizontalAlignment(iHorizontalAlignment::Strech);
    paramGroupBox->setText("Render States");
    paramGroupBox->setHeaderOnly();

    iWidgetGrid* gridParam = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(gridParam);
    gridParam->appendRows(14);
    gridParam->appendCollumns(1);
    gridParam->setBorder(2);
    gridParam->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridParam->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel* labelName = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelName);
    labelName->setText("Name");
    labelName->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textName = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget("TextEdit"));
    _allWidgets.push_back(_textName);
    _textName->setMaxTextLength(100);
    _textName->setWidth(200);
    _textName->setHorizontalAlignment(iHorizontalAlignment::Left);
    _textName->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textName->setText("...");
    _textName->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onTextChangedName));

    iWidgetLabel* labelDepthTest = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelDepthTest);
    labelDepthTest->setText("Depth Test");
    labelDepthTest->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxDepthTest = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
    _checkBoxDepthTest->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxDepthTest->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelDepthFunction = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelDepthFunction);
    labelDepthFunction->setText("Depth Function");
    labelDepthFunction->setHorizontalAlignment(iHorizontalAlignment::Left);

    _selectBoxDepthFunc = static_cast<iWidgetSelectBox*>(iWidgetManager::getInstance().createWidget("SelectBox"));
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

    iWidgetLabel* labelDepthMask = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelDepthMask);
    labelDepthMask->setText("Depth Mask");
    labelDepthMask->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxDepthMask = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
    _checkBoxDepthMask->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxDepthMask->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelBlend = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelBlend);
    labelBlend->setText("Blending");
    labelBlend->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxBlend = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
    _checkBoxBlend->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxBlend->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelBlendFuncSource = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelBlendFuncSource);
    labelBlendFuncSource->setText("Blend Source");
    labelBlendFuncSource->setHorizontalAlignment(iHorizontalAlignment::Left);

    _selectBoxBlendFuncSource = static_cast<iWidgetSelectBox*>(iWidgetManager::getInstance().createWidget("SelectBox"));
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

    iWidgetLabel* labelBlendFuncDestination = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelBlendFuncDestination);
    labelBlendFuncDestination->setText("Blend Destination");
    labelBlendFuncDestination->setHorizontalAlignment(iHorizontalAlignment::Left);

    _selectBoxBlendFuncDestination = static_cast<iWidgetSelectBox*>(iWidgetManager::getInstance().createWidget("SelectBox"));
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

    iWidgetLabel* labelCullFace = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelCullFace);
    labelCullFace->setText("Cull Face");
    labelCullFace->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxCullFace = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
    _checkBoxCullFace->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxCullFace->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelCullFaceFunc = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelCullFaceFunc);
    labelCullFaceFunc->setText("Cull Face Function");
    labelCullFaceFunc->setHorizontalAlignment(iHorizontalAlignment::Left);

    _selectBoxCullFaceFunc = static_cast<iWidgetSelectBox*>(iWidgetManager::getInstance().createWidget("SelectBox"));
    _selectBoxCullFaceFunc->addSelectionEntry("Front");
    _selectBoxCullFaceFunc->addSelectionEntry("Back");
    _selectBoxCullFaceFunc->setHorizontalAlignment(iHorizontalAlignment::Left);
    _selectBoxCullFaceFunc->setWidth(200);

    iWidgetLabel* labelTexture2D0 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelTexture2D0);
    labelTexture2D0->setText("Texture 0");
    labelTexture2D0->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxTexture2D0 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
    _checkBoxTexture2D0->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxTexture2D0->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelTexture2D1 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelTexture2D1);
    labelTexture2D1->setText("Texture 1");
    labelTexture2D1->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxTexture2D1 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
    _checkBoxTexture2D1->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxTexture2D1->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelTexture2D2 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelTexture2D2);
    labelTexture2D2->setText("Texture 2");
    labelTexture2D2->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxTexture2D2 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
    _checkBoxTexture2D2->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxTexture2D2->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelTexture2D3 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelTexture2D3);
    labelTexture2D3->setText("Texture 3");
    labelTexture2D3->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxTexture2D3 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
    _checkBoxTexture2D3->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxTexture2D3->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelWireframe = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelWireframe);
    labelWireframe->setText("Wireframe");
    labelWireframe->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxWireframe = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
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

    iWidgetLabel* labelRenderingOrder = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelRenderingOrder);
    labelRenderingOrder->setText("Rendering Order");
    labelRenderingOrder->setHorizontalAlignment(iHorizontalAlignment::Left);

    _renderingOrder = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget("NumberChooser"));
    _allWidgets.push_back(_renderingOrder);
    _renderingOrder->setMinMaxNumber(iMaterial::RENDER_ORDER_MIN, iMaterial::RENDER_ORDER_MAX);
    _renderingOrder->setAfterPoint(0);
    _renderingOrder->setValue(iMaterial::RENDER_ORDER_DEFAULT);
    _renderingOrder->setWidth(80);
    _renderingOrder->setSteppingWheel(10.0f, 10.0f);
    _renderingOrder->setStepping(1.0f, 1.0f);
    _renderingOrder->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _renderingOrder->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetGroupBox* shaderGroupBox = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget("GroupBox"));
    _allWidgets.push_back(shaderGroupBox);
    shaderGroupBox->setHorizontalAlignment(iHorizontalAlignment::Strech);
    shaderGroupBox->setText("Shader Settings");
    shaderGroupBox->setHeaderOnly();

    iWidgetGrid* gridShadersGroup = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(gridShadersGroup);
    gridShadersGroup->appendRows(1);
    gridShadersGroup->setBorder(2);
    gridShadersGroup->setHorizontalAlignment(iHorizontalAlignment::Strech);
    gridShadersGroup->setVerticalAlignment(iVerticalAlignment::Top);
    gridShadersGroup->setStrechColumn(0);

    iWidgetGrid* gridShaders = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(gridShaders);
    gridShaders->appendRows(2);
    gridShaders->appendCollumns(2);
    gridShaders->setBorder(2);
    gridShaders->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridShaders->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel* labelShader0 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelShader0);
    labelShader0->setText("Geometry");
    labelShader0->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelShader1 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelShader1);
    labelShader1->setText("Vertex");
    labelShader1->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel* labelShader2 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(labelShader2);
    labelShader2->setText("Fragment");
    labelShader2->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textShader0 = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget("TextEdit"));
    _allWidgets.push_back(_textShader0);
    _textShader0->setWidth(200);
    _textShader0->setMaxTextLength(200);
    _textShader0->setHorizontalAlignment(iHorizontalAlignment::Right);
    _textShader0->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textShader0->setText("");
    _textShader0->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));

    _textShader1 = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget("TextEdit"));
    _allWidgets.push_back(_textShader1);
    _textShader1->setWidth(200);
    _textShader1->setMaxTextLength(200);
    _textShader1->setHorizontalAlignment(iHorizontalAlignment::Right);
    _textShader1->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textShader1->setText("");
    _textShader1->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));

    _textShader2 = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget("TextEdit"));
    _allWidgets.push_back(_textShader2);
    _textShader2->setWidth(200);
    _textShader2->setMaxTextLength(200);
    _textShader2->setHorizontalAlignment(iHorizontalAlignment::Right);
    _textShader2->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textShader2->setText("");
    _textShader2->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));

    _shader0Button = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _allWidgets.push_back(_shader0Button);
    _shader0Button->setWidth(20);
    _shader0Button->setHeight(20);
    _shader0Button->setText("...");
    _shader0Button->registerOnClickEvent(iClickDelegate(this, &UserControlMaterial::onShader0Button));

    _shader1Button = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _allWidgets.push_back(_shader1Button);
    _shader1Button->setWidth(20);
    _shader1Button->setHeight(20);
    _shader1Button->setText("...");
    _shader1Button->registerOnClickEvent(iClickDelegate(this, &UserControlMaterial::onShader1Button));

    _shader2Button = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _allWidgets.push_back(_shader2Button);
    _shader2Button->setWidth(20);
    _shader2Button->setHeight(20);
    _shader2Button->setText("...");
    _shader2Button->registerOnClickEvent(iClickDelegate(this, &UserControlMaterial::onShader2Button));

    _shaderReload = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _allWidgets.push_back(_shaderReload);
    _shaderReload->setWidth(20);
    _shaderReload->setHeight(20);
    _shaderReload->setText("Reload");
    _shaderReload->setHorizontalAlignment(iHorizontalAlignment::Right);
    _shaderReload->registerOnClickEvent(iClickDelegate(this, &UserControlMaterial::onReloadShader));

    shaderGroupBox->addWidget(gridShadersGroup);

    gridShadersGroup->addWidget(gridShaders, 0, 0);
    gridShadersGroup->addWidget(_shaderReload, 0, 1);

    gridShaders->addWidget(labelShader0, 0, 0);
    gridShaders->addWidget(labelShader1, 0, 1);
    gridShaders->addWidget(labelShader2, 0, 2);

    gridShaders->addWidget(_textShader0, 1, 0);
    gridShaders->addWidget(_textShader1, 1, 1);
    gridShaders->addWidget(_textShader2, 1, 2);

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

    _grid->addWidget(paramGroupBox, 0, 0);
    _grid->addWidget(shaderGroupBox, 0, 1);

    _fileDialog = static_cast<iDialogFileSelect*>(iWidgetManager::getInstance().createDialog("FileSelect"));
}

void UserControlMaterial::onDoUpdateMaterial(iWidget* source)
{
    updateMaterial();
}

void UserControlMaterial::onTextChangedName(iWidget* source)
{
    updateMaterial();
    _materialNameChangedEvent();
}

void UserControlMaterial::deinitGUI()
{
    _renderingOrder->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _textName->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onTextChangedName));

    for (auto widget : _allWidgets)
    {
        iWidgetManager::getInstance().destroyWidget(widget);
    }

    if (_fileDialog != nullptr)
    {
        iWidgetManager::getInstance().destroyDialog(_fileDialog);
        _fileDialog = nullptr;
    }

    _grid = nullptr;
    _textName = nullptr;
    _renderingOrder = nullptr;
}

iWidget* UserControlMaterial::getWidget()
{
    return _grid;
}

void UserControlMaterial::registerNameChangeDelegate(MaterialNameChangedDelegate nameChangedDelegate)
{
    _materialNameChangedEvent.append(nameChangedDelegate);
}

void UserControlMaterial::unregisterNameChangeDelegate(MaterialNameChangedDelegate nameChangedDelegate)
{
    _materialNameChangedEvent.remove(nameChangedDelegate);
}

void UserControlMaterial::onShader0Button(iWidget* source)
{
    _loadShaderNumber = 0;
    _fileDialog->load(iDialogFileSelectCloseDelegate(this, &UserControlMaterial::onFileLoadDialogClosed), "..\\data\\shaders"); // TODO hard coded path
}

void UserControlMaterial::onShader1Button(iWidget* source)
{
    _loadShaderNumber = 1;
    _fileDialog->load(iDialogFileSelectCloseDelegate(this, &UserControlMaterial::onFileLoadDialogClosed), "..\\data\\shaders"); // TODO hard coded path
}

void UserControlMaterial::onShader2Button(iWidget* source)
{
    _loadShaderNumber = 2;
    _fileDialog->load(iDialogFileSelectCloseDelegate(this, &UserControlMaterial::onFileLoadDialogClosed), "..\\data\\shaders"); // TODO hard coded path
}

void UserControlMaterial::onReloadShader(iWidget* source)
{
    iMaterial* material = iMaterialResourceFactory::getInstance().getMaterial(_materialID);

    if (!_ignoreMaterialUpdate &&
        material != nullptr)
    {
        reloadShader(material);
    }
}

void UserControlMaterial::onFileLoadDialogClosed(iFileDialogReturnValue fileDialogReturnValue)
{
    if (_fileDialog->getReturnState() == iFileDialogReturnValue::Ok)
    {
        iaString filename = iResourceManager::getInstance().getRelativePath(_fileDialog->getFullPath());
        switch (_loadShaderNumber)
        {
        case 0:
            _textShader0->setText(filename);
            break;

        case 1:
            _textShader1->setText(filename);
            break;

        case 2:
            _textShader2->setText(filename);
            break;

        default:
            con_err("out of range");
        }
        updateMaterial();
    }
}


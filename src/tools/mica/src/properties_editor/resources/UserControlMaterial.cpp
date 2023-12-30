// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlMaterial.h"

UserControlMaterial::UserControlMaterial(iResourceID resourceID, const iWidgetPtr parent)
    : UserControlResource(resourceID, parent)
{
}

UserControlMaterial::~UserControlMaterial()
{
    if (_fileDialog != nullptr)
    {
        delete _fileDialog;
        _fileDialog = nullptr;
    }
}

void UserControlMaterial::updateResource()
{
    iMaterialPtr material = iResourceManager::getInstance().getResource<iMaterial>(getResourceID());

    if (_ignoreMaterialUpdate ||
        material == nullptr)
    {
        return;
    }

    material->setOrder(static_cast<int32>(_renderingOrder->getValue()));
    material->setRenderState(iRenderState::CullFace, _checkBoxCullFace->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
    material->setRenderState(iRenderState::DepthTest, _checkBoxDepthTest->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
    material->setRenderState(iRenderState::DepthMask, _checkBoxDepthMask->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
    material->setRenderState(iRenderState::Blend, _checkBoxBlend->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
    material->setRenderState(iRenderState::Wireframe, _checkBoxWireframe->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
    material->setRenderState(iRenderState::DepthFunc, static_cast<iRenderStateValue>(_selectBoxDepthFunc->getSelectedIndex() + static_cast<int>(iRenderStateValue::Never)));
    material->setRenderState(iRenderState::CullFaceFunc, static_cast<iRenderStateValue>(_selectBoxCullFaceFunc->getSelectedIndex() + static_cast<int>(iRenderStateValue::Front)));

    // TODO material->setRenderState(iRenderState::Instanced, _checkBoxInstanced->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
    // TODO		_selectBoxInstancedFunc

    iResourceManager::getInstance().saveResource(getResourceID());
}

void UserControlMaterial::update()
{
    UserControlResource::update();

    iMaterialPtr material = iResourceManager::getInstance().loadResource<iMaterial>(getResourceID());
    
    _ignoreMaterialUpdate = true;

    _checkBoxCullFace->setChecked(material->getRenderState(iRenderState::CullFace) == iRenderStateValue::On ? true : false);
    _checkBoxDepthTest->setChecked(material->getRenderState(iRenderState::DepthTest) == iRenderStateValue::On ? true : false);
    _checkBoxDepthMask->setChecked(material->getRenderState(iRenderState::DepthMask) == iRenderStateValue::On ? true : false);
    _checkBoxBlend->setChecked(material->getRenderState(iRenderState::Blend) == iRenderStateValue::On ? true : false);
    _checkBoxWireframe->setChecked(material->getRenderState(iRenderState::Wireframe) == iRenderStateValue::On ? true : false);
    _selectBoxDepthFunc->setSelection(static_cast<int>(material->getRenderState(iRenderState::DepthFunc)) - static_cast<int>(iRenderStateValue::Never));
    _selectBoxCullFaceFunc->setSelection(static_cast<int>(material->getRenderState(iRenderState::CullFaceFunc)) - static_cast<int>(iRenderStateValue::Front));
    // TODO_checkBoxInstanced->setChecked(material->getRenderState(iRenderState::Instanced) == iRenderStateValue::On ? true : false);
    // TODO _selectBoxInstancedFunc = nullptr;
    _renderingOrder->setValue(material->getOrder());

    _ignoreMaterialUpdate = false;
}

void UserControlMaterial::init()
{
    UserControlResource::init();

    iWidgetGridLayoutPtr grid = new iWidgetGridLayout(getLayout());
    grid->appendRows(1);
    grid->setBorder(2);
    grid->setStretchColumn(0);
    grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    grid->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGroupBox *paramGroupBox = new iWidgetGroupBox();
    paramGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    paramGroupBox->setText("Render States");
    paramGroupBox->setHeaderOnly();

    iWidgetGridLayout *gridParam = new iWidgetGridLayout();
    gridParam->appendRows(8);
    gridParam->appendColumns(1);
    gridParam->setBorder(2);
    gridParam->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridParam->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel *labelDepthTest = new iWidgetLabel();
    labelDepthTest->setText("Depth Test");
    labelDepthTest->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxDepthTest = new iWidgetCheckBox();
    _checkBoxDepthTest->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxDepthTest->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelDepthFunction = new iWidgetLabel();
    labelDepthFunction->setText("Depth Function");
    labelDepthFunction->setHorizontalAlignment(iHorizontalAlignment::Left);

    _selectBoxDepthFunc = new iWidgetSelectBox();
    _selectBoxDepthFunc->addItem("Never");
    _selectBoxDepthFunc->addItem("Less");
    _selectBoxDepthFunc->addItem("Equal");
    _selectBoxDepthFunc->addItem("LessOrEqual");
    _selectBoxDepthFunc->addItem("Greater");
    _selectBoxDepthFunc->addItem("NotEqual");
    _selectBoxDepthFunc->addItem("GreaterOrEqual");
    _selectBoxDepthFunc->addItem("Always");
    _selectBoxDepthFunc->setHorizontalAlignment(iHorizontalAlignment::Left);
    _selectBoxDepthFunc->setMinWidth(200);

    iWidgetLabel *labelDepthMask = new iWidgetLabel();
    labelDepthMask->setText("Depth Mask");
    labelDepthMask->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxDepthMask = new iWidgetCheckBox();
    _checkBoxDepthMask->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxDepthMask->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelBlend = new iWidgetLabel();
    labelBlend->setText("Blending");
    labelBlend->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxBlend = new iWidgetCheckBox();
    _checkBoxBlend->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxBlend->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelCullFace = new iWidgetLabel();
    labelCullFace->setText("Cull Face");
    labelCullFace->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxCullFace = new iWidgetCheckBox();
    _checkBoxCullFace->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _checkBoxCullFace->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelCullFaceFunc = new iWidgetLabel();
    labelCullFaceFunc->setText("Cull Face Function");
    labelCullFaceFunc->setHorizontalAlignment(iHorizontalAlignment::Left);

    _selectBoxCullFaceFunc = new iWidgetSelectBox();
    _selectBoxCullFaceFunc->addItem("Front");
    _selectBoxCullFaceFunc->addItem("Back");
    _selectBoxCullFaceFunc->setHorizontalAlignment(iHorizontalAlignment::Left);
    _selectBoxCullFaceFunc->setMinWidth(200);

    iWidgetLabel *labelWireframe = new iWidgetLabel();
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

    iWidgetLabel *labelRenderingOrder = new iWidgetLabel();
    labelRenderingOrder->setText("Rendering Order");
    labelRenderingOrder->setHorizontalAlignment(iHorizontalAlignment::Left);

    _renderingOrder = new iWidgetNumberChooser();
    _renderingOrder->setMinMaxNumber(iMaterial::RENDER_ORDER_MIN, iMaterial::RENDER_ORDER_MAX);
    _renderingOrder->setAfterPoint(0);
    _renderingOrder->setValue(iMaterial::RENDER_ORDER_DEFAULT);
    _renderingOrder->setMinWidth(80);
    _renderingOrder->setSteppingWheel(10.0f, 10.0f);
    _renderingOrder->setStepping(1.0f, 1.0f);
    _renderingOrder->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _renderingOrder->setHorizontalAlignment(iHorizontalAlignment::Left);

    gridParam->addWidget(labelDepthTest, 0, 0);
    gridParam->addWidget(labelDepthFunction, 0, 1);
    gridParam->addWidget(labelDepthMask, 0, 2);
    gridParam->addWidget(labelBlend, 0, 3);
    gridParam->addWidget(labelCullFace, 0, 4);
    gridParam->addWidget(labelCullFaceFunc, 0, 5);
    gridParam->addWidget(labelWireframe, 0, 6);
    gridParam->addWidget(labelRenderingOrder, 0, 7);

    gridParam->addWidget(_checkBoxDepthTest, 1, 0);
    gridParam->addWidget(_selectBoxDepthFunc, 1, 1);
    gridParam->addWidget(_checkBoxDepthMask, 1, 2);
    gridParam->addWidget(_checkBoxBlend, 1, 3);
    gridParam->addWidget(_checkBoxCullFace, 1, 4);
    gridParam->addWidget(_selectBoxCullFaceFunc, 1, 5);
    gridParam->addWidget(_checkBoxWireframe, 1, 6);
    gridParam->addWidget(_renderingOrder, 1, 7);

    paramGroupBox->addWidget(gridParam);

    grid->addWidget(paramGroupBox, 0, 0);
}

void UserControlMaterial::onDoUpdateMaterial(const iWidgetPtr source)
{
    updateResource();
}

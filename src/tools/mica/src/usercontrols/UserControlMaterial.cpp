// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlMaterial.h"

#include <igor/resources/material/iMaterialIO.h>

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
        material != nullptr)
    {
        material->setName(_textName->getText());
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
    }
}

void UserControlMaterial::updateGUI()
{
    auto material = iMaterialResourceFactory::getInstance().getMaterial(_materialID);

    if (material == nullptr)
    {
        return;
    }

    _ignoreMaterialUpdate = true;

    _textName->setText(material->getName());
    _textID->setText(material->getID().getValue());
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

    if (material->getShaderProgram() == nullptr ||
        material->getShaderProgram()->getShaderSources().empty())
    {
        _textShaderVertex->setText("");
        _textShaderVertex->setEnabled();
        _textShaderGeometry->setText("");
        _textShaderGeometry->setEnabled();
        _textShaderFragment->setText("");
        _textShaderFragment->setEnabled();
    }
    else
    {
        const auto &shaderSources = material->getShaderProgram()->getShaderSources();

        for (const auto &source : shaderSources)
        {
            switch (source._type)
            {
            case iShaderObjectType::Vertex:
                if (iaFile::exist(source._filename))
                {
                    _textShaderVertex->setText(iResourceManager::getInstance().getRelativePath(source._filename));
                    _textShaderVertex->setEnabled(true);
                }
                else
                {
                    _textShaderVertex->setText("can't edit");
                    _textShaderVertex->setEnabled(false);
                }
                break;

            case iShaderObjectType::Geometry:
                if (iaFile::exist(source._filename))
                {
                    _textShaderGeometry->setText(iResourceManager::getInstance().getRelativePath(source._filename));
                    _textShaderGeometry->setEnabled(true);
                }
                else
                {
                    _textShaderGeometry->setText("can't edit");
                    _textShaderGeometry->setEnabled(false);
                }
                break;

            case iShaderObjectType::Fragment:
                if (iaFile::exist(source._filename))
                {
                    _textShaderFragment->setText(iResourceManager::getInstance().getRelativePath(source._filename));
                    _textShaderFragment->setEnabled(true);
                }
                else
                {
                    _textShaderFragment->setText("can't edit");
                    _textShaderFragment->setEnabled(false);
                }
                break;
            }
        }
    }

    _ignoreMaterialUpdate = false;
}

void UserControlMaterial::setMaterial(const iMaterialID &materialID)
{
    _materialID = materialID;
    updateGUI();
}

const iMaterialID &UserControlMaterial::getMaterialID() const
{
    return _materialID;
}

void UserControlMaterial::initGUI()
{
    iWidgetGridPtr grid = new iWidgetGrid(this);
    grid->appendRows(2);
    grid->setBorder(2);
    grid->setStretchColumn(0);
    grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    grid->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGrid *gridHeader = new iWidgetGrid();
    gridHeader->appendRows(2);
    gridHeader->appendColumns(1);
    gridHeader->setBorder(2);
    gridHeader->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridHeader->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel *labelName = new iWidgetLabel();
    labelName->setText("Name");
    labelName->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textName = new iWidgetLineTextEdit();
    _textName->setMaxTextLength(100);
    _textName->setWidth(200);
    _textName->setHorizontalAlignment(iHorizontalAlignment::Left);
    _textName->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textName->setText("...");
    _textName->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onTextChangedName));

    iWidgetLabel *labelID = new iWidgetLabel();
    labelID->setText("ID");
    labelID->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textID = new iWidgetLabel();
    _textID->setText("...");
    _textID->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetGroupBox *paramGroupBox = new iWidgetGroupBox();
    paramGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    paramGroupBox->setText("Render States");
    paramGroupBox->setHeaderOnly();

    iWidgetGrid *gridParam = new iWidgetGrid();
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
    _selectBoxCullFaceFunc->addSelectionEntry("Front");
    _selectBoxCullFaceFunc->addSelectionEntry("Back");
    _selectBoxCullFaceFunc->setHorizontalAlignment(iHorizontalAlignment::Left);
    _selectBoxCullFaceFunc->setWidth(200);

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
    _renderingOrder->setWidth(80);
    _renderingOrder->setSteppingWheel(10.0f, 10.0f);
    _renderingOrder->setStepping(1.0f, 1.0f);
    _renderingOrder->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _renderingOrder->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetGroupBox *shaderGroupBox = new iWidgetGroupBox();
    shaderGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    shaderGroupBox->setText("Shader Settings");
    shaderGroupBox->setHeaderOnly();

    iWidgetGrid *gridShadersGroup = new iWidgetGrid();
    gridShadersGroup->appendRows(2);
    gridShadersGroup->setBorder(2);
    gridShadersGroup->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    gridShadersGroup->setVerticalAlignment(iVerticalAlignment::Top);
    gridShadersGroup->setStretchColumn(0);

    iWidgetGrid *gridShaders = new iWidgetGrid();
    gridShaders->appendRows(2);
    gridShaders->appendColumns(2);
    gridShaders->setBorder(2);
    gridShaders->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridShaders->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel *labelShader0 = new iWidgetLabel();
    labelShader0->setText("Vertex");
    labelShader0->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelShader1 = new iWidgetLabel();
    labelShader1->setText("Geometry");
    labelShader1->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelShader2 = new iWidgetLabel();
    labelShader2->setText("Fragment");
    labelShader2->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textShaderVertex = new iWidgetLineTextEdit();
    _textShaderVertex->setEnabled(false);
    _textShaderVertex->setWidth(200);
    _textShaderVertex->setMaxTextLength(200);
    _textShaderVertex->setHorizontalAlignment(iHorizontalAlignment::Right);
    _textShaderVertex->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textShaderVertex->setText("");
    _textShaderVertex->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));

    _textShaderGeometry = new iWidgetLineTextEdit();
    _textShaderGeometry->setEnabled(false);
    _textShaderGeometry->setWidth(200);
    _textShaderGeometry->setMaxTextLength(200);
    _textShaderGeometry->setHorizontalAlignment(iHorizontalAlignment::Right);
    _textShaderGeometry->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textShaderGeometry->setText("");
    _textShaderGeometry->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));

    _textShaderFragment = new iWidgetLineTextEdit();
    _textShaderFragment->setEnabled(false);
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

    _exportMaterial = new iWidgetButton();
    _exportMaterial->setWidth(20);
    _exportMaterial->setHeight(20);
    _exportMaterial->setText("Export");
    _exportMaterial->setTooltip("Export Material to file");
    _exportMaterial->setHorizontalAlignment(iHorizontalAlignment::Right);
    _exportMaterial->registerOnClickEvent(iClickDelegate(this, &UserControlMaterial::onExportMaterial));

    gridHeader->addWidget(labelName, 0, 0);
    gridHeader->addWidget(_textName, 1, 0);
    gridHeader->addWidget(labelID, 0, 1);
    gridHeader->addWidget(_textID, 1, 1);

    shaderGroupBox->addWidget(gridShadersGroup);

    gridShadersGroup->addWidget(gridShaders, 0, 0);
    gridShadersGroup->addWidget(_shaderReload, 0, 1);
    gridShadersGroup->addWidget(_exportMaterial, 0, 2);

    gridShaders->addWidget(labelShader0, 0, 0);
    gridShaders->addWidget(labelShader1, 0, 1);
    gridShaders->addWidget(labelShader2, 0, 2);

    gridShaders->addWidget(_textShaderVertex, 1, 0);
    gridShaders->addWidget(_textShaderGeometry, 1, 1);
    gridShaders->addWidget(_textShaderFragment, 1, 2);

    gridShaders->addWidget(_shader1Button, 2, 0);
    gridShaders->addWidget(_shader0Button, 2, 1);
    gridShaders->addWidget(_shader2Button, 2, 2);

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

    grid->addWidget(gridHeader, 0, 0);
    grid->addWidget(paramGroupBox, 0, 1);
    grid->addWidget(shaderGroupBox, 0, 2);
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

void UserControlMaterial::onExportMaterialDialogClosed(iDialogPtr dialog)
{
    if (_fileDialog != dialog)
    {
        return;
    }

    if (_fileDialog->getReturnState() == iDialogReturnState::Ok)
    {
        iMaterialPtr material = iMaterialResourceFactory::getInstance().getMaterial(_materialID);
        if (material != nullptr)
        {
            iMaterialIO::write(_fileDialog->getFullPath(), material);
        }
    }

    delete _fileDialog;
    _fileDialog = nullptr;
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

void UserControlMaterial::onExportMaterial(const iWidgetPtr source)
{
    if (_fileDialog == nullptr)
    {
        _fileDialog = new iDialogFileSelect();
    }

    _fileDialog->open(iDialogCloseDelegate(this, &UserControlMaterial::onExportMaterialDialogClosed), iFileDialogPurpose::Save);
}

void UserControlMaterial::onReloadShader(const iWidgetPtr source)
{
    auto material = iMaterialResourceFactory::getInstance().getMaterial(_materialID);

    if (!_ignoreMaterialUpdate &&
        material != nullptr)
    {
        reloadShader(material);
    }
}

void UserControlMaterial::reloadShader(iMaterialPtr material)
{
    iShaderProgramPtr program = iShaderProgram::create();

    if (_textShaderGeometry->getText() != "")
    {
        program->addShader(_textShaderGeometry->getText(), iShaderObjectType::Geometry);
    }

    if (_textShaderVertex->getText() != "")
    {
        program->addShader(_textShaderVertex->getText(), iShaderObjectType::Vertex);
    }

    if (_textShaderFragment->getText() != "")
    {
        program->addShader(_textShaderFragment->getText(), iShaderObjectType::Fragment);
    }

    program->compile();

    if (program->isValid())
    {
        material->setShaderProgram(program);
    }
}

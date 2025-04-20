// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlShaderMaterial.h"

UserControlShaderMaterial::UserControlShaderMaterial(iResourceID resourceID, const iWidgetPtr parent)
    : UserControlResource(resourceID, parent)
{
}

static iMeshPtr createSphere()
{
    iMeshBuilder meshBuilder;

    iMeshBuilderUtils::addSphere(meshBuilder, 0.5, 32);
    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

void UserControlShaderMaterial::updateMaterialDisplay(iShaderPtr shader)
{
    if (_ignoreMaterialUpdate)
    {
        return;
    }

    // store current render states
    iRenderer::getInstance().flush();
    const iaRectanglei viewport = iRenderer::getInstance().getViewport();
    const iaMatrixd projectionMatrix = iRenderer::getInstance().getProjectionMatrix();
    const iaMatrixd modelMatrix = iRenderer::getInstance().getModelMatrix();

    const uint32 width = 128;
    const uint32 height = 128;
    uint32 renderTarget = iRenderer::getInstance().createRenderTarget(width, height, iColorFormat::RGBA, iRenderTargetType::ToRenderBuffer, true);
    iRenderer::getInstance().setRenderTarget(renderTarget);
    iRenderer::getInstance().clearColorBuffer(iaColor4f::transparent);

    iRenderer::getInstance().setViewport(0, 0, width, height);
    iRenderer::getInstance().setPerspective(45.0, 1.0, 0.00001, 10.0);

    iParameters param({
        {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
        {IGOR_RESOURCE_PARAM_GENERATE, true},
        {IGOR_RESOURCE_PARAM_SHADER, shader},
        {IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Free}, // drop it right after this use
        {IGOR_RESOURCE_PARAM_AMBIENT, iaColor3f(0.5f, 0.5f, 0.5f)},
        {IGOR_RESOURCE_PARAM_DIFFUSE, iaColor3f(0.5f, 0.5f, 0.5f)},
        {IGOR_RESOURCE_PARAM_SPECULAR, iaColor3f(0.5f, 0.5f, 0.5f)},
        {IGOR_RESOURCE_PARAM_EMISSIVE, iaColor3f(0.0f, 0.0f, 0.0f)},
        {IGOR_RESOURCE_PARAM_ALPHA, 1.0f},
    });
    iMaterialPtr material = iResourceManager::getInstance().loadResource<iMaterial>(param);

    iMeshPtr sphere = createSphere();

    iaMatrixd matrix;
    matrix.translate(0, 0, -1.5);
    iRenderer::getInstance().setModelMatrix(matrix);
    iRenderer::getInstance().drawMesh(sphere, material);

    iPixmapPtr pixmap = iPixmap::createPixmap(width, height, iColorFormat::RGBA);

    iRenderer::getInstance().readPixels(0, 0, width, height, iColorFormat::RGBA, pixmap->getData());

    iRenderer::getInstance().setRenderTarget();
    iRenderer::getInstance().destroyRenderTarget(renderTarget);

    // restore everything
    iRenderer::getInstance().setModelMatrix(modelMatrix);
    iRenderer::getInstance().setProjectionMatrix(projectionMatrix);
    iRenderer::getInstance().setViewport(viewport);

    iParameters paramTex({{IGOR_RESOURCE_PARAM_ID, iaUUID()},
                       {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_TEXTURE},
                       {IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Cache},
                       {IGOR_RESOURCE_PARAM_TEXTURE_BUILD_MODE, iTextureBuildMode::Normal},
                       {IGOR_RESOURCE_PARAM_PIXMAP, pixmap}});

    _materialPicture->setTexture(iResourceManager::getInstance().requestResource<iTexture>(paramTex));
}

void UserControlShaderMaterial::updateResource()
{
    iShaderPtr shader = iResourceManager::getInstance().getResource<iShader>(getResourceID());

    if (_ignoreMaterialUpdate ||
        shader == nullptr)
    {
        return;
    }

    shader->setOrder(static_cast<int32>(_renderingOrder->getValue()));
    shader->setRenderState(iRenderState::CullFace, _checkBoxCullFace->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
    shader->setRenderState(iRenderState::DepthTest, _checkBoxDepthTest->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
    shader->setRenderState(iRenderState::DepthMask, _checkBoxDepthMask->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
    shader->setRenderState(iRenderState::Blend, _checkBoxBlend->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
    shader->setRenderState(iRenderState::Wireframe, _checkBoxWireframe->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
    shader->setRenderState(iRenderState::DepthFunc, static_cast<iRenderStateValue>(_selectBoxDepthFunc->getSelectedIndex() + static_cast<int>(iRenderStateValue::Never)));
    shader->setRenderState(iRenderState::CullFaceFunc, static_cast<iRenderStateValue>(_selectBoxCullFaceFunc->getSelectedIndex() + static_cast<int>(iRenderStateValue::Front)));

    // TODO shader->setRenderState(iRenderState::Instanced, _checkBoxInstanced->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
    // TODO		_selectBoxInstancedFunc

    iResourceManager::getInstance().saveResource(getResourceID());
    updateMaterialDisplay(shader);
}

void UserControlShaderMaterial::update()
{
    UserControlResource::update();

    iShaderPtr shader = iResourceManager::getInstance().loadResource<iShader>(getResourceID());

    _ignoreMaterialUpdate = true;

    _checkBoxCullFace->setChecked(shader->getRenderState(iRenderState::CullFace) == iRenderStateValue::On ? true : false);
    _checkBoxDepthTest->setChecked(shader->getRenderState(iRenderState::DepthTest) == iRenderStateValue::On ? true : false);
    _checkBoxDepthMask->setChecked(shader->getRenderState(iRenderState::DepthMask) == iRenderStateValue::On ? true : false);
    _checkBoxBlend->setChecked(shader->getRenderState(iRenderState::Blend) == iRenderStateValue::On ? true : false);
    _checkBoxWireframe->setChecked(shader->getRenderState(iRenderState::Wireframe) == iRenderStateValue::On ? true : false);
    _selectBoxDepthFunc->setSelection(static_cast<int>(shader->getRenderState(iRenderState::DepthFunc)) - static_cast<int>(iRenderStateValue::Never));
    _selectBoxCullFaceFunc->setSelection(static_cast<int>(shader->getRenderState(iRenderState::CullFaceFunc)) - static_cast<int>(iRenderStateValue::Front));
    // TODO_checkBoxInstanced->setChecked(shader->getRenderState(iRenderState::Instanced) == iRenderStateValue::On ? true : false);
    // TODO _selectBoxInstancedFunc = nullptr;
    _renderingOrder->setValue(shader->getOrder());

    _ignoreMaterialUpdate = false;

    updateMaterialDisplay(shader);
}

void UserControlShaderMaterial::init()
{
    UserControlResource::init();

    iWidgetBoxLayoutPtr mainLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, getLayout());
    mainLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    mainLayout->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGroupBox *paramGroupBox = new iWidgetGroupBox(mainLayout);
    paramGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    paramGroupBox->setText("Render States");
    paramGroupBox->setHeaderOnly();

    iWidgetGridLayout *gridParam = new iWidgetGridLayout(paramGroupBox);
    gridParam->appendRows(8);
    gridParam->appendColumns(1);
    gridParam->setBorder(2);
    gridParam->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridParam->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel *labelDepthTest = new iWidgetLabel();
    labelDepthTest->setText("Depth Test");
    labelDepthTest->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxDepthTest = new iWidgetCheckBox();
    _checkBoxDepthTest->getChangeEvent().add(iChangeDelegate(this, &UserControlShaderMaterial::onDoUpdateShaderMaterial));
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
    _selectBoxDepthFunc->getChangeEvent().add(iChangeDelegate(this, &UserControlShaderMaterial::onDoUpdateShaderMaterial));
    _selectBoxDepthFunc->setMinWidth(200);

    iWidgetLabel *labelDepthMask = new iWidgetLabel();
    labelDepthMask->setText("Depth Mask");
    labelDepthMask->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxDepthMask = new iWidgetCheckBox();
    _checkBoxDepthMask->getChangeEvent().add(iChangeDelegate(this, &UserControlShaderMaterial::onDoUpdateShaderMaterial));
    _checkBoxDepthMask->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelBlend = new iWidgetLabel();
    labelBlend->setText("Blending");
    labelBlend->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxBlend = new iWidgetCheckBox();
    _checkBoxBlend->getChangeEvent().add(iChangeDelegate(this, &UserControlShaderMaterial::onDoUpdateShaderMaterial));
    _checkBoxBlend->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelCullFace = new iWidgetLabel();
    labelCullFace->setText("Cull Face");
    labelCullFace->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxCullFace = new iWidgetCheckBox();
    _checkBoxCullFace->getChangeEvent().add(iChangeDelegate(this, &UserControlShaderMaterial::onDoUpdateShaderMaterial));
    _checkBoxCullFace->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelCullFaceFunc = new iWidgetLabel();
    labelCullFaceFunc->setText("Cull Face Function");
    labelCullFaceFunc->setHorizontalAlignment(iHorizontalAlignment::Left);

    _selectBoxCullFaceFunc = new iWidgetSelectBox();
    _selectBoxCullFaceFunc->addItem("Front");
    _selectBoxCullFaceFunc->addItem("Back");
    _selectBoxCullFaceFunc->setHorizontalAlignment(iHorizontalAlignment::Left);
    _selectBoxCullFaceFunc->setMinWidth(200);
    _selectBoxCullFaceFunc->getChangeEvent().add(iChangeDelegate(this, &UserControlShaderMaterial::onDoUpdateShaderMaterial));

    iWidgetLabel *labelWireframe = new iWidgetLabel();
    labelWireframe->setText("Wireframe");
    labelWireframe->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxWireframe = new iWidgetCheckBox();
    _checkBoxWireframe->getChangeEvent().add(iChangeDelegate(this, &UserControlShaderMaterial::onDoUpdateShaderMaterial));
    _checkBoxWireframe->setHorizontalAlignment(iHorizontalAlignment::Left);

    // TODO
    /*iWidgetLabel* labelInstanced = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label));
    _allWidgets.push_back(labelInstanced);
    labelInstanced->setText("Instanced");
    labelInstanced->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxInstanced = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox));
    _checkBoxInstanced->getChangeEvent().add(iChangeDelegate(this, &UserControlShaderMaterial::onDoUpdateShaderMaterial));

    iWidgetSelectBox* _selectBoxInstancedFunc = nullptr;*/

    iWidgetLabel *labelRenderingOrder = new iWidgetLabel();
    labelRenderingOrder->setText("Rendering Order");
    labelRenderingOrder->setHorizontalAlignment(iHorizontalAlignment::Left);

    _renderingOrder = new iWidgetNumberChooser();
    _renderingOrder->setMinMaxNumber(iShader::RENDER_ORDER_MIN, iShader::RENDER_ORDER_MAX);
    _renderingOrder->setAfterPoint(0);
    _renderingOrder->setValue(iShader::RENDER_ORDER_DEFAULT);
    _renderingOrder->setMinWidth(80);
    _renderingOrder->setSteppingWheel(10.0f, 10.0f);
    _renderingOrder->setStepping(1.0f, 1.0f);
    _renderingOrder->getChangeEvent().add(iChangeDelegate(this, &UserControlShaderMaterial::onDoUpdateShaderMaterial));
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

    iWidgetGroupBox *materialGroupBox = new iWidgetGroupBox(mainLayout);
    materialGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    materialGroupBox->setText("Preview");
    materialGroupBox->setHeaderOnly();

    _materialPicture = new iWidgetPicture(materialGroupBox);
    _materialPicture->setMaxSize(128, 128);
    _materialPicture->setMinSize(128, 128);
    _materialPicture->setCheckerBoard(true);
    _materialPicture->setForeground(iaColor4f::white);
}

void UserControlShaderMaterial::onDoUpdateShaderMaterial(const iWidgetPtr source)
{
    updateResource();
}

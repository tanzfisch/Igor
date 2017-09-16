// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "ModelViewer.h"

#include <iaConsole.h>
#include <iaDirectory.h>
using namespace IgorAux;

#include <iMaterial.h>
#include <iMaterialGroup.h>
#include <iNodeVisitorPrintTree.h>
#include <iTaskManager.h>
#include <iNodeSkyBox.h>
#include <iNodeCamera.h>
#include <iNodeModel.h> 
#include <iNodeSwitch.h>
#include <iNodeEmitter.h>
#include <iNodeParticleSystem.h>
#include <iNodeTransform.h>
#include <iRenderer.h>
#include <iApplication.h>
#include <iSceneFactory.h>
#include <iScene.h>
#include <iNodeFactory.h>
#include <iTimer.h>
#include <iTextureFont.h>
#include <iNodeLight.h>
#include <iModelResourceFactory.h>
#include <iNodeVisitorBoundings.h>
#include <iMaterialResourceFactory.h>
#include <iWidgetDefaultTheme.h>
#include <iDialog.h>
#include <iWidgetGrid.h>
#include <iWidgetScroll.h>
#include <iDialogMessageBox.h>
#include <iStatistics.h>
#include <iMaterialResourceFactory.h>
#include <iTaskFlushTextures.h>
#include <iNodeMesh.h>
#include <iMesh.h>
using namespace Igor;

#include "MenuDialog.h"
#include "PropertiesDialog.h"

ModelViewer::ModelViewer()
{
    iWidgetManager::getInstance().registerDialogType("MenuDialog", iInstanciateDialogDelegate(MenuDialog::createInstance));
    iWidgetManager::getInstance().registerDialogType("PropertiesDialog", iInstanciateDialogDelegate(PropertiesDialog::createInstance));
}

ModelViewer::~ModelViewer()
{
    deinit();
}

void ModelViewer::deinit()
{
    deinitGUI();

    iSceneFactory::getInstance().destroyScene(_scene);

    //! \todo this should happen automatically
    iModelResourceFactory::getInstance().flush(&_window);
    iTextureResourceFactory::getInstance().flush();

    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &ModelViewer::onWindowClosed));
    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &ModelViewer::onWindowResize));

    _window.close();
    _window.removeView(&_view);
    _window.removeView(&_viewOrtho);

    _view.unregisterRenderDelegate(RenderDelegate(this, &ModelViewer::render));
    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &ModelViewer::renderOrtho));

    if (_font)
    {
        delete _font;
    }

    iWidgetManager::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &ModelViewer::onMouseWheel));
    iWidgetManager::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &ModelViewer::onMouseMoved));
    iWidgetManager::getInstance().unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &ModelViewer::onMouseKeyDown));
    iWidgetManager::getInstance().unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &ModelViewer::onMouseKeyUp));
    iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownDelegate(this, &ModelViewer::onKeyPressed));
    iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &ModelViewer::handle));

    iTaskManager::getInstance().abortTask(_taskFlushTextures);
}

void ModelViewer::init(iaString fileName)
{
    con(" -- Model Viewer --" << endl);

    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &ModelViewer::onKeyPressed));
    iWidgetManager::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &ModelViewer::onMouseMoved));
    iWidgetManager::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &ModelViewer::onMouseWheel));
    iWidgetManager::getInstance().registerMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &ModelViewer::onMouseKeyDown));
    iWidgetManager::getInstance().registerMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &ModelViewer::onMouseKeyUp));

    iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &ModelViewer::handle));

    _window.setSize(1280, 1024);
    _window.setCentered();
    _window.setTitle("Igor - Model Viewer");
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &ModelViewer::onWindowClosed));
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &ModelViewer::onWindowResize));

    _view.setClearColor(iaColor4f(0.5f, 0, 0.5f, 1));
    _view.setPerspective(45);
    _view.setClipPlanes(0.1f, 10000.f);
    _view.registerRenderDelegate(RenderDelegate(this, &ModelViewer::render));
    _window.addView(&_view);

    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &ModelViewer::renderOrtho));
    _window.addView(&_viewOrtho);

    _window.setDoubleClick(true);
    _window.open(); // open after adding views to prevent warning message

    _scene = iSceneFactory::getInstance().createScene();
    _view.setScene(_scene);

    _transformModel = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _transformModel->setName("model transform");
    _scene->getRoot()->insertNode(_transformModel);

    _groupNode = static_cast<iNode*>(iNodeFactory::getInstance().createNode(iNodeType::iNode));
    _groupNode->setName("groupNode");
    _transformModel->insertNode(_groupNode);

    // cam
    _cameraCOI = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraCOI->setName("camera COI");
    _cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraHeading->setName("camera heading");
    _cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraPitch->setName("camera pitch");
    _cameraTranslation = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraTranslation->setName("camera translation");
    iNodeCamera* camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
    camera->setName("camera");

    _scene->getRoot()->insertNode(_cameraCOI);
    _cameraCOI->insertNode(_cameraHeading);
    _cameraHeading->insertNode(_cameraPitch);
    _cameraPitch->insertNode(_cameraTranslation);
    _cameraTranslation->insertNode(camera);
    camera->makeCurrent();

    _cameraTranslation->translate(0, 0, 80);

    // default sky box
    _materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterialGroup(_materialSkyBox)->setOrder(10);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setName("SkyBox");

    iNodeSkyBox* skyBoxNode = static_cast<iNodeSkyBox*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSkyBox));
    skyBoxNode->setName("sky box");
    skyBoxNode->setTextures(
        iTextureResourceFactory::getInstance().requestFile("skybox_default/front.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/back.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/left.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/right.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/top.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/bottom.png"));
    skyBoxNode->setTextureScale(10);
    skyBoxNode->setMaterial(_materialSkyBox);
    _scene->getRoot()->insertNode(skyBoxNode);

    _font = new iTextureFont("StandardFont.png");

    // create material for manipulators
    _materialManipulator = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialManipulator)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    _materialBoundingBox = iMaterialResourceFactory::getInstance().createMaterial();

    _materialCelShading = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->addShaderSource("default.vert", iShaderObjectType::Vertex);
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->addShaderSource("yellow.frag", iShaderObjectType::Fragment);
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->compileShader();
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->getRenderStateSet().setRenderState(iRenderState::Wireframe, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->getRenderStateSet().setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->getRenderStateSet().setRenderState(iRenderState::CullFace, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->getRenderStateSet().setRenderState(iRenderState::CullFaceFunc, iRenderStateValue::Front);

    // light
    _directionalLightRotate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _directionalLightRotate->setName("directional light rotate");

    _directionalLightTranslate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _directionalLightTranslate->setName("directional light translate");
    _directionalLightTranslate->translate(10000, 10000, 0);

    _lightNode = static_cast<iNodeLight*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLight));
    _lightNode->setName("directional light");
    _lightNode->setAmbient(iaColor4f(0.5f, 0.5f, 0.5f, 1.0f));
    _lightNode->setDiffuse(iaColor4f(0.9f, 0.9f, 0.9f, 1.0f));
    _lightNode->setSpecular(iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));

    _scene->getRoot()->insertNode(_directionalLightRotate);
    _directionalLightRotate->insertNode(_directionalLightTranslate);
    _directionalLightTranslate->insertNode(_lightNode);

    _modelViewOrtho.translate(0, 0, -30);

    if (!fileName.isEmpty())
    {
        iNodeModel* model = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        iModelDataInputParameter* parameter = new iModelDataInputParameter();
        parameter->_identifier = "";
        parameter->_modelSourceType = iModelSourceType::File;
        parameter->_needsRenderContext = true;
        parameter->_loadPriority = iTask::DEFAULT_PRIORITY;
        parameter->_joinVertexes = true;
        parameter->_keepMesh = true;

        model->setModel(fileName, iResourceCacheMode::Free, parameter);
        _groupNode->insertNode(model);
    }

    initGUI();

    if (fileName.isEmpty())
    {
        _fileDialog->load(iDialogFileSelectCloseDelegate(this, &ModelViewer::onImportFileDialogClosed), "..\\data\\models");
    }
    else
    {
        _menuDialog->setActive();
        _menuDialog->setVisible();

        _propertiesDialog->setActive();
        _propertiesDialog->setVisible();
    }

    forceLoadingNow();

    _menuDialog->setRootNode(_groupNode);
    _menuDialog->refreshView();

    _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));
}

void ModelViewer::onAddTransformation(uint64 atNodeID)
{
    iNode* destination = iNodeFactory::getInstance().getNode(atNodeID);

    if (destination == nullptr)
    {
        destination = _groupNode;
    }

    iNodeTransform* transform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transform->setName("Transformation");
    destination->insertNode(transform);
    _menuDialog->refreshView();
}

void ModelViewer::onAddGroup(uint64 atNodeID)
{
    iNode* destination = iNodeFactory::getInstance().getNode(atNodeID);

    if (destination == nullptr)
    {
        destination = _groupNode;
    }

    iNode* group = static_cast<iNode*>(iNodeFactory::getInstance().createNode(iNodeType::iNode));
    group->setName("Group");
    destination->insertNode(group);
    _menuDialog->refreshView();
}

void ModelViewer::onAddEmitter(uint64 atNodeID)
{
    iNode* destination = iNodeFactory::getInstance().getNode(atNodeID);

    if (destination == nullptr)
    {
        destination = _groupNode;
    }

    iNodeEmitter* emitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
    emitter->setName("Emitter");
    destination->insertNode(emitter);
    _menuDialog->refreshView();
}

void ModelViewer::onAddParticleSystem(uint64 atNodeID)
{
    iNode* destination = iNodeFactory::getInstance().getNode(atNodeID);

    if (destination == nullptr)
    {
        destination = _groupNode;
    }

    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
    particleSystem->setName("ParticleSystem");
    destination->insertNode(particleSystem);
    _menuDialog->refreshView();
}

void ModelViewer::onAddMaterial()
{
    iMaterialResourceFactory::getInstance().createMaterial("new Material");
    _menuDialog->refreshView();
}

void ModelViewer::onAddSwitch(uint64 atNodeID)
{
    iNode* destination = iNodeFactory::getInstance().getNode(atNodeID);

    if (destination == nullptr)
    {
        destination = _groupNode;
    }

    iNodeSwitch* switchNode = static_cast<iNodeSwitch*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSwitch));
    switchNode->setName("Switch");
    destination->insertNode(switchNode);
    _menuDialog->refreshView();
}

void ModelViewer::forceLoadingNow()
{
    // want everything to be loaded now!
    con_endl("loading data synchronously ... ");

    while (true)
    {
        _scene->handle();
        iTextureResourceFactory::getInstance().flush();

        if (!iModelResourceFactory::getInstance().flush(&_window))
        {
            break;
        }
    }

    for (int i = 0; i < 10; ++i)
    {
        _scene->handle();
        iTextureResourceFactory::getInstance().flush();

        iModelResourceFactory::getInstance().flush(&_window);
    }

    // calculate and get the boundings
    iNodeVisitorBoundings visitorBoundings;
    visitorBoundings.traverseTree(_groupNode);
    iSphered sphere;
    visitorBoundings.getSphere(sphere);

    iaMatrixd coiMatrix;
    coiMatrix._pos = sphere._center;
    _cameraCOI->setMatrix(coiMatrix);
    _camDistance = sphere._radius * 3.0f;
    _camMinDistance = sphere._radius * 0.5f;
    _camMaxDistance = sphere._radius * 10.0f;

    updateCamDistance();
}

void ModelViewer::onImportFile(uint64 nodeID)
{
    _fileDialog->load(iDialogFileSelectCloseDelegate(this, &ModelViewer::onImportFileDialogClosed), "");
}

void ModelViewer::onImportFileReference(uint64 nodeID)
{
    _fileDialog->load(iDialogFileSelectCloseDelegate(this, &ModelViewer::onImportFileReferenceDialogClosed), "");
}

void ModelViewer::onLoadFile()
{
    _fileDialog->load(iDialogFileSelectCloseDelegate(this, &ModelViewer::onFileLoadDialogClosed), "");
}

void ModelViewer::onSaveFile()
{
    _fileDialog->save(iDialogFileSelectCloseDelegate(this, &ModelViewer::onFileSaveDialogClosed), "");
}

void ModelViewer::onFileSaveDialogClosed(iFileDialogReturnValue fileDialogReturnValue)
{
    if (_fileDialog->getReturnState() == iFileDialogReturnValue::Ok)
    {
        iaString filename = _fileDialog->getFullPath();

        vector<iNode*> children = _groupNode->getChildren();
        children.insert(children.end(), _groupNode->getInactiveChildren().begin(), _groupNode->getInactiveChildren().end());

        if (children.empty())
        {
            con_warn("nothing to save");
        }
        else if (children.size() == 1)
        {
            iModelResourceFactory::getInstance().exportModelData(filename, children[0]);
        }
        else
        {
            iModelResourceFactory::getInstance().exportModelData(filename, _groupNode);
        }
    }
}

void ModelViewer::onImportFileDialogClosed(iFileDialogReturnValue fileDialogReturnValue)
{
    if (_fileDialog->getReturnState() == iFileDialogReturnValue::Ok)
    {
        iaString filename = _fileDialog->getFullPath();

        iNodeModel* model = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        iModelDataInputParameter* parameter = new iModelDataInputParameter();
        parameter->_identifier = "";
        parameter->_modelSourceType = iModelSourceType::File;
        parameter->_needsRenderContext = true;
        parameter->_loadPriority = iTask::DEFAULT_PRIORITY;
        parameter->_joinVertexes = true;
        parameter->_keepMesh = true;

        model->setModel(filename, iResourceCacheMode::Free, parameter);
        _scene->getRoot()->insertNode(model);
        forceLoadingNow();
        _scene->getRoot()->removeNode(model);

        iNode* groupNode = nullptr;

        auto children = model->getChildren();
        if (children.size() > 1)
        {
            groupNode = static_cast<iNode*>(iNodeFactory::getInstance().createNode(iNodeType::iNode));
            iaString groupName = "group:";
            groupName += filename;
            groupNode->setName(groupName);

            iNode* cursorNode = iNodeFactory::getInstance().getNode(_selectedNodeID);
            if (cursorNode != nullptr)
            {
                cursorNode->insertNode(groupNode);
            }
            else
            {
                _groupNode->insertNode(groupNode);
            }
        }
        else
        {
            iNode* cursorNode = iNodeFactory::getInstance().getNode(_selectedNodeID);
            if (cursorNode != nullptr)
            {
                groupNode = cursorNode;
            }
            else
            {
                groupNode = _groupNode;
            }
        }

        auto child = children.begin();
        while (child != children.end())
        {
            model->removeNode((*child));
            groupNode->insertNode((*child));
            child++;
        }

        iNodeFactory::getInstance().destroyNodeAsync(model);

        forceLoadingNow();
    }

    _menuDialog->setActive();
    _menuDialog->setVisible();
    _menuDialog->refreshView();

    _propertiesDialog->setActive();
    _propertiesDialog->setVisible();
}

void ModelViewer::onImportFileReferenceDialogClosed(iFileDialogReturnValue fileDialogReturnValue)
{
    if (_fileDialog->getReturnState() == iFileDialogReturnValue::Ok)
    {
        iaString filename = _fileDialog->getFullPath();

        iNodeModel* model = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        iModelDataInputParameter* parameter = new iModelDataInputParameter();
        parameter->_identifier = "";
        parameter->_modelSourceType = iModelSourceType::File;
        parameter->_needsRenderContext = true;
        parameter->_loadPriority = iTask::DEFAULT_PRIORITY;
        parameter->_joinVertexes = true;
        parameter->_keepMesh = true;

        model->setModel(filename, iResourceCacheMode::Free, parameter);

        iNode* cursorNode = iNodeFactory::getInstance().getNode(_selectedNodeID);
        if (cursorNode != nullptr)
        {
            cursorNode->insertNode(model);
        }
        else
        {
            _groupNode->insertNode(model);
        }
        forceLoadingNow();
    }

    _menuDialog->setActive();
    _menuDialog->setVisible();
    _menuDialog->refreshView();

    _propertiesDialog->setActive();
    _propertiesDialog->setVisible();
}

void ModelViewer::onFileLoadDialogClosed(iFileDialogReturnValue fileDialogReturnValue)
{
    if (_fileDialog->getReturnState() == iFileDialogReturnValue::Ok)
    {
        iaString filename = _fileDialog->getFullPath();

        if (_groupNode->getChildren().size() > 0)
        {
            auto children = _groupNode->getChildren();
            auto child = children.begin();
            while (child != children.end())
            {
                _groupNode->removeNode((*child));
                iNodeFactory::getInstance().destroyNodeAsync((*child));
                child++;
            }
        }

        iNodeModel* model = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        iModelDataInputParameter* parameter = new iModelDataInputParameter();
        parameter->_identifier = "";
        parameter->_modelSourceType = iModelSourceType::File;
        parameter->_needsRenderContext = true;
        parameter->_loadPriority = iTask::DEFAULT_PRIORITY;
        parameter->_joinVertexes = true;
        parameter->_keepMesh = true;

        model->setModel(filename, iResourceCacheMode::Free, parameter);
        _scene->getRoot()->insertNode(model);
        forceLoadingNow();
        _scene->getRoot()->removeNode(model);

        iNode* groupNode = nullptr;

        auto children = model->getChildren();
        if (children.size() > 1)
        {
            groupNode = static_cast<iNode*>(iNodeFactory::getInstance().createNode(iNodeType::iNode));
            iaString groupName = "group:";
            groupName += filename;
            groupNode->setName(groupName);

            iNode* cursorNode = iNodeFactory::getInstance().getNode(_selectedNodeID);
            if (cursorNode != nullptr)
            {
                cursorNode->insertNode(groupNode);
            }
            else
            {
                _groupNode->insertNode(groupNode);
            }
        }
        else
        {
            iNode* cursorNode = iNodeFactory::getInstance().getNode(_selectedNodeID);
            if (cursorNode != nullptr)
            {
                groupNode = cursorNode;
            }
            else
            {
                groupNode = _groupNode;
            }
        }

        auto child = children.begin();
        while (child != children.end())
        {
            model->removeNode((*child));
            groupNode->insertNode((*child));
            child++;
        }

        iNodeFactory::getInstance().destroyNodeAsync(model);

        forceLoadingNow();
    }

    _menuDialog->setActive();
    _menuDialog->setVisible();
    _menuDialog->refreshView();

    _propertiesDialog->setActive();
    _propertiesDialog->setVisible();
}

void ModelViewer::initGUI()
{
    _widgetTheme = new iWidgetDefaultTheme("StandardFont.png", "WidgetThemePattern.png");
    iWidgetManager::getInstance().setTheme(_widgetTheme);
    iWidgetManager::getInstance().setDesktopDimensions(_window.getClientWidth(), _window.getClientHeight());

    _menuDialog = static_cast<MenuDialog*>(iWidgetManager::getInstance().createDialog("MenuDialog"));
    _menuDialog->registerOnExitModelViewer(ExitModelViewerDelegate(this, &ModelViewer::onExitModelViewer));
    _menuDialog->registerOnLoadFile(LoadFileDelegate(this, &ModelViewer::onLoadFile));
    _menuDialog->registerOnImportFile(ImportFileDelegate(this, &ModelViewer::onImportFile));
    _menuDialog->registerOnImportFileReference(ImportFileReferenceDelegate(this, &ModelViewer::onImportFileReference));
    _menuDialog->registerOnSaveFile(SaveFileDelegate(this, &ModelViewer::onSaveFile));
    _menuDialog->registerOnAddTransformation(AddTransformationDelegate(this, &ModelViewer::onAddTransformation));
    _menuDialog->registerOnAddSwitch(AddSwitchDelegate(this, &ModelViewer::onAddSwitch));
    _menuDialog->registerOnAddGroup(AddGroupDelegate(this, &ModelViewer::onAddGroup));
    _menuDialog->registerOnAddEmitter(AddEmitterDelegate(this, &ModelViewer::onAddEmitter));
    _menuDialog->registerOnAddParticleSystem(AddParticleSystemDelegate(this, &ModelViewer::onAddParticleSystem));
    _menuDialog->registerOnAddMaterial(AddMaterialDelegate(this, &ModelViewer::onAddMaterial));

    _fileDialog = static_cast<iDialogFileSelect*>(iWidgetManager::getInstance().createDialog("DialogFileSelect"));
    _messageBox = static_cast<iDialogMessageBox*>(iWidgetManager::getInstance().createDialog("DialogMessageBox"));
    _propertiesDialog = static_cast<PropertiesDialog*>(iWidgetManager::getInstance().createDialog("PropertiesDialog"));

    _propertiesDialog->registerStructureChangedDelegate(StructureChangedDelegate(_menuDialog, &MenuDialog::refreshView));

    _menuDialog->registerOnGraphSelectionChanged(GraphSelectionChangedDelegate(_propertiesDialog, &PropertiesDialog::onGraphViewSelectionChanged));
    _menuDialog->registerOnGraphSelectionChanged(GraphSelectionChangedDelegate(this, &ModelViewer::onGraphViewSelectionChanged));
    _menuDialog->registerOnMaterialSelectionChanged(MaterialSelectionChangedDelegate(_propertiesDialog, &PropertiesDialog::onMaterialSelectionChanged));
}

void ModelViewer::onGraphViewSelectionChanged(uint64 nodeID)
{
    _selectedNodeID = nodeID;
}

void ModelViewer::deinitGUI()
{
    if (_menuDialog != nullptr &&
        _propertiesDialog != nullptr)
    {
        _propertiesDialog->unregisterStructureChangedDelegate(StructureChangedDelegate(_menuDialog, &MenuDialog::refreshView));
        _menuDialog->unregisterOnGraphSelectionChanged(GraphSelectionChangedDelegate(_propertiesDialog, &PropertiesDialog::onGraphViewSelectionChanged));
        _menuDialog->unregisterOnGraphSelectionChanged(GraphSelectionChangedDelegate(this, &ModelViewer::onGraphViewSelectionChanged));
        _menuDialog->unregisterOnMaterialSelectionChanged(MaterialSelectionChangedDelegate(_propertiesDialog, &PropertiesDialog::onMaterialSelectionChanged));
    }

    if (_menuDialog != nullptr)
    {
        _menuDialog->unregisterOnExitModelViewer(ExitModelViewerDelegate(this, &ModelViewer::onExitModelViewer));
        _menuDialog->unregisterOnLoadFile(LoadFileDelegate(this, &ModelViewer::onLoadFile));
        _menuDialog->unregisterOnImportFile(ImportFileDelegate(this, &ModelViewer::onImportFile));
        _menuDialog->unregisterOnImportFileReference(ImportFileReferenceDelegate(this, &ModelViewer::onImportFileReference));
        _menuDialog->unregisterOnSaveFile(SaveFileDelegate(this, &ModelViewer::onSaveFile));
        _menuDialog->unregisterOnAddTransformation(AddTransformationDelegate(this, &ModelViewer::onAddTransformation));
        _menuDialog->unregisterOnAddSwitch(AddSwitchDelegate(this, &ModelViewer::onAddSwitch));
        _menuDialog->unregisterOnAddGroup(AddGroupDelegate(this, &ModelViewer::onAddGroup));
        _menuDialog->unregisterOnAddEmitter(AddEmitterDelegate(this, &ModelViewer::onAddEmitter));
        _menuDialog->unregisterOnAddParticleSystem(AddParticleSystemDelegate(this, &ModelViewer::onAddParticleSystem));

        iWidgetManager::getInstance().destroyDialog(_menuDialog);
        _menuDialog = nullptr;
    }

    if (_propertiesDialog != nullptr)
    {
        iWidgetManager::getInstance().destroyDialog(_propertiesDialog);
        _propertiesDialog = nullptr;
    }

    if (_fileDialog != nullptr)
    {
        iWidgetManager::getInstance().destroyDialog(_fileDialog);
        _fileDialog = nullptr;
    }

    if (_messageBox != nullptr)
    {
        iWidgetManager::getInstance().destroyDialog(_messageBox);
        _messageBox = nullptr;
    }

    iWidgetManager::getInstance().setTheme(nullptr);
    if (_widgetTheme != nullptr)
    {
        delete _widgetTheme;
        _widgetTheme = nullptr;
    }
}

void ModelViewer::onWindowResize(int32 clientWidth, int32 clientHeight)
{
    iWidgetManager::getInstance().setDesktopDimensions(_window.getClientWidth(), _window.getClientHeight());
    _viewOrtho.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
}

void ModelViewer::updateCamDistance()
{
    _cameraTranslation->identity();
    _cameraTranslation->translate(0, 0, _camDistance);
}

void ModelViewer::onMouseKeyDown(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::MouseLeft:
        _mouseKey0Pressed = true;
        break;
    case iKeyCode::MouseRight:
        _mouseKey1Pressed = true;
        break;
    case iKeyCode::MouseMiddle:
        _mouseKey2Pressed = true;
        break;
    case iKeyCode::MouseButton4:
        _mouseKey3Pressed = true;
        break;
    case iKeyCode::MouseButton5:
        _mouseKey4Pressed = true;
        break;
    }
}

void ModelViewer::onMouseKeyUp(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::MouseLeft:
        _mouseKey0Pressed = false;
        break;
    case iKeyCode::MouseRight:
        _mouseKey1Pressed = false;
        break;
    case iKeyCode::MouseMiddle:
        _mouseKey2Pressed = false;
        break;
    case iKeyCode::MouseButton4:
        _mouseKey3Pressed = false;
        break;
    case iKeyCode::MouseButton5:
        _mouseKey4Pressed = false;
        break;
    }
}

void ModelViewer::onMouseWheel(int32 d)
{
    if (d < 0)
    {
        if (_camDistance < _camMaxDistance)
        {
            _camDistance *= 2.0f;
        }
    }
    else
    {
        if (_camDistance > _camMinDistance)
        {
            _camDistance *= 0.5f;
        }
    }

    updateCamDistance();
}

void ModelViewer::onMouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* _window)
{
    if (_mouseKey0Pressed)
    {
        _cameraPitch->rotate((y2 - y1) * 0.005f, iaAxis::X);
        _cameraHeading->rotate((x1 - x2) * 0.005f, iaAxis::Y);

        iMouse::getInstance().setCenter(true);
    }

    if (_mouseKey1Pressed)
    {
        float32 dx = static_cast<float32>(x1 - x2) * 0.005f;
        _directionalLightRotate->rotate(dx, iaAxis::Y);

        iMouse::getInstance().setCenter(true);
    }
}

void ModelViewer::onExitModelViewer()
{
    iApplication::getInstance().stop();
}

void ModelViewer::onWindowClosed()
{
    iApplication::getInstance().stop();
}

void ModelViewer::onKeyPressed(iKeyCode key)
{
    switch (key)
    {

    case iKeyCode::F1:
    {
        iNodeVisitorPrintTree printTree;
        printTree.printToConsole(_scene->getRoot());
    }
    break;

    case iKeyCode::W:
        _view.setWireframeVisible(!_view.isWireframeVisible());
        break;

    case iKeyCode::O:
        _view.setOctreeVisible(!_view.isOctreeVisible());
        break;

    case iKeyCode::B:
        _view.setBoundingBoxVisible(!_view.isBoundingBoxVisible());
        break;
    }
}

void ModelViewer::handle()
{
    _scene->handle();
}

void ModelViewer::render()
{


    if (_selectedNodeID != iNode::INVALID_NODE_ID)
    {
        iNode* node = iNodeFactory::getInstance().getNode(_selectedNodeID);



        if (node->getKind() == iNodeKind::Volume)
        {
            iNodeVolume* volume = static_cast<iNodeVolume*>(node);
            iaMatrixd matrix = volume->getWorldMatrix();
            iRenderer::getInstance().setModelMatrix(matrix);

            if (node->getType() == iNodeType::iNodeMesh)
            {
                iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading));

                iNodeMesh* meshNode = static_cast<iNodeMesh*>(node);
                shared_ptr<iMeshBuffers> buffers = meshNode->getMeshBuffers();
                iRenderer::getInstance().setLineWidth(4);
                iRenderer::getInstance().drawMesh(buffers);
            }
            else
            {
                iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getMaterial(_materialBoundingBox));

                iAABoxd box = volume->getBoundingBox();
                
                iRenderer::getInstance().setColor(1, 1, 0, 1);
                iRenderer::getInstance().drawBBox(box);
            }
        }
    }
}

void ModelViewer::renderOrtho()
{
    iaMatrixd identity;
    iRenderer::getInstance().setViewMatrix(identity);
    iRenderer::getInstance().setModelMatrix(_modelViewOrtho);

    iWidgetManager::getInstance().draw();
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
}

void ModelViewer::run(iaString fileName)
{
    init(fileName);
    iApplication::getInstance().run();
}
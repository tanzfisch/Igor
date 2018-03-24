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
#include <iRenderEngine.h>
using namespace Igor;

#include "MenuDialog.h"
#include "PropertiesDialog.h"

static const wchar_t* WINDOW_TITLE_PREFIX = L"Igor::Model Viewer";

ModelViewer::ModelViewer()
{
    iWidgetManager::getInstance().registerDialogType("MenuDialog", iInstanciateDialogDelegate(MenuDialog::createInstance));
    iWidgetManager::getInstance().registerDialogType("PropertiesDialog", iInstanciateDialogDelegate(PropertiesDialog::createInstance));
}

ModelViewer::~ModelViewer()
{
    deinit();
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

    _window.setSize(1280, 800);
    _window.setTitle(WINDOW_TITLE_PREFIX);
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &ModelViewer::onWindowClosed));
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &ModelViewer::onWindowResize));

    _view.setClearColor(iaColor4f(0.25f, 0.25f, 0.25f, 1.0f));
    _view.setPerspective(45.0f);
    _view.setClipPlanes(0.1f, 10000.f);
    _view.registerRenderDelegate(RenderDelegate(this, &ModelViewer::render));
    _window.addView(&_view);

    _viewManipulator.setClearColor(false);
    _viewManipulator.setClearDepth(true);
    _viewManipulator.setPerspective(45.0f);
    _viewManipulator.setClipPlanes(0.1f, 10000.f);
    _window.addView(&_viewManipulator);

    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setOrthogonal(0.0f, static_cast<float32>(_window.getClientWidth()), static_cast<float32>(_window.getClientHeight()), 0.0f);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &ModelViewer::renderOrtho));
    _window.addView(&_viewOrtho);

    _window.setDoubleClick(true);
    _window.open(); // open after adding views to prevent warning message

    _scene = iSceneFactory::getInstance().createScene();
    _scene->setName("Model Scene");
    _view.setScene(_scene);

    _sceneManipulator = iSceneFactory::getInstance().createScene();
    _sceneManipulator->setName("Modifier Scene");
    _viewManipulator.setScene(_sceneManipulator);

    _transformModel = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _transformModel->setName("model transform");
    _scene->getRoot()->insertNode(_transformModel);

    _groupNode = static_cast<iNode*>(iNodeFactory::getInstance().createNode(iNodeType::iNode));
    _groupNode->setName("groupNode");
    _transformModel->insertNode(_groupNode);

    // modifier
    _manipulator = new Manipulator(_sceneManipulator->getRoot());

    // cam
    _cameraCOI = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraCOI->setName("camera COI");
    _cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraHeading->setName("camera heading");
    _cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraPitch->setName("camera pitch");
    _cameraTranslation = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraTranslation->setName("camera translation");
    _camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
    _camera->setName("camera");

    _scene->getRoot()->insertNode(_cameraCOI);
    _cameraCOI->insertNode(_cameraHeading);
    _cameraHeading->insertNode(_cameraPitch);
    _cameraPitch->insertNode(_cameraTranslation);
    _cameraTranslation->insertNode(_camera);
    _view.setCurrentCamera(_camera->getID());

    _cameraTranslation->translate(0, 0, 80);

    // camUI
    _cameraCOIUI = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraCOIUI->setName("camera COI UI");
    _cameraHeadingUI = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraHeadingUI->setName("camera heading UI");
    _cameraPitchUI = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraPitchUI->setName("camera pitch UI");
    _cameraTranslationUI = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraTranslationUI->setName("camera translation UI");
    _cameraUI = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
    _cameraUI->setName("camera UI");

    _sceneManipulator->getRoot()->insertNode(_cameraCOIUI);
    _cameraCOIUI->insertNode(_cameraHeadingUI);
    _cameraHeadingUI->insertNode(_cameraPitchUI);
    _cameraPitchUI->insertNode(_cameraTranslationUI);
    _cameraTranslationUI->insertNode(_cameraUI);
    _viewManipulator.setCurrentCamera(_cameraUI->getID());

    _cameraTranslationUI->translate(0, 0, 80);

    // default sky box
    _materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterialGroup(_materialSkyBox)->setOrder(iMaterial::RENDER_ORDER_MIN);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setName("SkyBox");

    _skyBoxNode = static_cast<iNodeSkyBox*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSkyBox));
    _skyBoxNode->setName("sky box");
    _skyBoxNode->setTextures(
        iTextureResourceFactory::getInstance().requestFile("skybox_default/front.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/back.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/left.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/right.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/top.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/bottom.png"));
    _skyBoxNode->setTextureScale(10);
    _skyBoxNode->setMaterial(_materialSkyBox);
    //_scene->getRoot()->insertNode(_skyBoxNode);

    _font = new iTextureFont("StandardFont.png");

    // create material for manipulators
    _materialManipulator = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialManipulator)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    _materialBoundingBox = iMaterialResourceFactory::getInstance().createMaterial();

    _materialCelShading = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->addShaderSource("ModelViewer/yellow.frag", iShaderObjectType::Fragment);
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->compileShader();
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->getRenderStateSet().setRenderState(iRenderState::Wireframe, iRenderStateValue::On);
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

    _menuDialog->setRootNode(_groupNode);

    if (fileName.isEmpty())
    {
        _fileDialog->load(iDialogFileSelectCloseDelegate(this, &ModelViewer::onFileLoadDialogClosed), "..\\data\\models");
    }
    else
    {
        _menuDialog->setActive();
        _menuDialog->setVisible();

        _propertiesDialog->setActive();
        _propertiesDialog->setVisible();
    }

    _menuDialog->refreshView();

    _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));
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
    _menuDialog->setSelectedNode(transform);
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
    _menuDialog->setSelectedNode(group);
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
    _menuDialog->setSelectedNode(emitter);
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
    _menuDialog->setSelectedNode(particleSystem);
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
    _menuDialog->setSelectedNode(switchNode);
}

void ModelViewer::forceLoadingNow(iNodeModel* modelNode)
{
    if (modelNode != nullptr)
    {
        iScene* tempScene = iSceneFactory::getInstance().createScene();
        tempScene->getRoot()->insertNode(modelNode);

        // want everything to be loaded now!
        con_endl("loading data synchronously ... ");

        while (!modelNode->isReady())
        {
            tempScene->handle();
            iTextureResourceFactory::getInstance().flush();
            iModelResourceFactory::getInstance().flush(&_window);
        }

        tempScene->getRoot()->removeNode(modelNode);
        iSceneFactory::getInstance().destroyScene(tempScene);
    }
}

void ModelViewer::centerCamOnNode(iNode* node)
{
    if (node != nullptr)
    {
        iNodeVisitorBoundings visitorBoundings;
        visitorBoundings.traverseTree(node);
        iSphered sphere;
        visitorBoundings.getSphere(sphere);

        iaMatrixd coiMatrix;
        coiMatrix._pos = sphere._center;
        _cameraCOI->setMatrix(coiMatrix);
        _cameraCOIUI->setMatrix(coiMatrix);
        if (sphere._radius > 0.0f)
        {
            _camDistance = sphere._radius * 4.0f;
        }
        else
        {
            _camDistance = 1.0f;
        }
    }
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
    iNode* selectNode = nullptr;

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
        forceLoadingNow(model);

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

            selectNode = groupNode;
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

            if (!children.empty())
            {
                selectNode = children.front();
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
    }

    _menuDialog->setActive();
    _menuDialog->setVisible();
    _menuDialog->refreshView();

    _propertiesDialog->setActive();
    _propertiesDialog->setVisible();

    _menuDialog->setSelectedNode(selectNode);
    centerCamOnSelectedNode();
}

void ModelViewer::onImportFileReferenceDialogClosed(iFileDialogReturnValue fileDialogReturnValue)
{
    iNode* selectNode = nullptr;

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
        forceLoadingNow(model);

        iNode* cursorNode = iNodeFactory::getInstance().getNode(_selectedNodeID);
        if (cursorNode != nullptr)
        {
            cursorNode->insertNode(model);
        }
        else
        {
            _groupNode->insertNode(model);
        }
        
        selectNode = model;
    }

    _menuDialog->setActive();
    _menuDialog->setVisible();
    _menuDialog->refreshView();

    _propertiesDialog->setActive();
    _propertiesDialog->setVisible();

    _menuDialog->setSelectedNode(selectNode);
    centerCamOnSelectedNode();
}

void ModelViewer::onFileLoadDialogClosed(iFileDialogReturnValue fileDialogReturnValue)
{
    iNode* selectNode = nullptr;

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
        forceLoadingNow(model);

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

            selectNode = groupNode;
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

            selectNode = children.front();
        }

        auto child = children.begin();
        while (child != children.end())
        {
            model->removeNode((*child));
            groupNode->insertNode((*child));
            child++;
        }

        iNodeFactory::getInstance().destroyNodeAsync(model);
    }

    _menuDialog->setActive();
    _menuDialog->setVisible();
    _menuDialog->refreshView();

    _propertiesDialog->setActive();
    _propertiesDialog->setVisible();

    _menuDialog->setSelectedNode(selectNode);
    centerCamOnSelectedNode();
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

void ModelViewer::updateManipulator()
{
    updateCamDistanceTransform();

    iNode* node = iNodeFactory::getInstance().getNode(_selectedNodeID);

    if (node != nullptr)
    {
        if (node->getKind() == iNodeKind::Renderable ||
            node->getKind() == iNodeKind::Volume)
        {
            iNodeRender* renderNode = static_cast<iNodeRender*>(node);
            iaMatrixd matrix = renderNode->getWorldMatrix();
            _manipulator->setMatrix(matrix);

            _manipulator->setVisible(true);
        }
        else if (node->getKind() == iNodeKind::Transformation)
        {
            iNodeTransform* transform = static_cast<iNodeTransform*>(node);
            iaMatrixd matrix;
            transform->calcWorldTransformation(matrix);

            _manipulator->setMatrix(matrix);

            _manipulator->setVisible(true);
        }
        else
        {
            _manipulator->setVisible(false);
        }
    }
    else
    {
        _manipulator->setVisible(false);
    }
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

void ModelViewer::updateCamDistanceTransform()
{
    _cameraTranslation->identity();
    _cameraTranslation->translate(0, 0, _camDistance);
    _cameraTranslationUI->identity();
    _cameraTranslationUI->translate(0, 0, _camDistance);

    iaMatrixd matrix;
    _cameraUI->calcWorldTransformation(matrix);
    _manipulator->updateCamMatrix(matrix);
}

void ModelViewer::onMouseKeyDown(iKeyCode key)
{
}

void ModelViewer::pickcolorID()
{
    _skyBoxNode->setVisible(false);

    uint64 nodeID = _view.pickcolorID(iMouse::getInstance().getPos()._x, iMouse::getInstance().getPos()._y);
    iNode* node = iNodeFactory::getInstance().getNode(nodeID);
    _menuDialog->setSelectedNode(node);

    _skyBoxNode->setVisible(true);
}

void ModelViewer::onMouseKeyUp(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::MouseLeft:
        if (!iKeyboard::getInstance().getKey(iKeyCode::LAlt))
        {
            pickcolorID();
        }
        break;
    }
}

void ModelViewer::onMouseWheel(int32 d)
{
    if (d < 0)
    {
        _camDistance *= 2.0f;
    }
    else
    {
        _camDistance *= 0.5f;
    }
}

void ModelViewer::onMouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* _window)
{
    const float32 rotateSensitivity = 0.0075f;

    if (iMouse::getInstance().getLeftButton() &&
        iKeyboard::getInstance().getKey(iKeyCode::LAlt))
    {
        _cameraPitch->rotate((y1 - y2) * rotateSensitivity, iaAxis::X);
        _cameraHeading->rotate((x1 - x2) * rotateSensitivity, iaAxis::Y);
        _cameraPitchUI->rotate((y1 - y2) * rotateSensitivity, iaAxis::X);
        _cameraHeadingUI->rotate((x1 - x2) * rotateSensitivity, iaAxis::Y);

        iaMatrixd matrix;
        _cameraUI->calcWorldTransformation(matrix);
        _manipulator->updateCamMatrix(matrix);
    }

    if (iMouse::getInstance().getRightButton())
    {
        _directionalLightRotate->rotate((y1 - y2) * rotateSensitivity, iaAxis::X);
        _directionalLightRotate->rotate((x1 - x2) * rotateSensitivity, iaAxis::Y);
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

    case iKeyCode::F:
    {
        centerCamOnSelectedNode();
    }
    break;

    case iKeyCode::F1:
    {
        iNodeVisitorPrintTree printTree;
        printTree.printToConsole(_scene->getRoot());
    }
    break;

    case iKeyCode::F5:
        _view.setWireframeVisible(!_view.isWireframeVisible());
        break;

    case iKeyCode::F6:
        _view.setOctreeVisible(!_view.isOctreeVisible());
        break;

    case iKeyCode::F7:
        _view.setBoundingBoxVisible(!_view.isBoundingBoxVisible());
        break;

    case iKeyCode::F8:
        _statisticsVisualizer.cycleVerbosity();
        break;

    case iKeyCode::Q:
        _manipulator->setModifierMode(ModifierMode::Locator);
        break;

    case iKeyCode::W:
        _manipulator->setModifierMode(ModifierMode::Translate);
        break;

    case iKeyCode::E:
        _manipulator->setModifierMode(ModifierMode::Rotate);
        break;

    case iKeyCode::R:
        _manipulator->setModifierMode(ModifierMode::Scale);
        break;


    }
}

void ModelViewer::centerCamOnSelectedNode()
{
    iNode* node = iNodeFactory::getInstance().getNode(_selectedNodeID);
    centerCamOnNode(node);
}

void ModelViewer::handle()
{
    _scene->handle();
}

void ModelViewer::render()
{
    updateManipulator();

    if (_selectedNodeID != iNode::INVALID_NODE_ID)
    {
        iNode* node = iNodeFactory::getInstance().getNode(_selectedNodeID);

        if (node->getKind() == iNodeKind::Renderable ||
            node->getKind() == iNodeKind::Volume)
        {
            iNodeRender* renderNode = static_cast<iNodeRender*>(node);
            iaMatrixd matrix = renderNode->getWorldMatrix();
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
                if (node->getKind() == iNodeKind::Volume)
                {
                    iNodeVolume* renderVolume = static_cast<iNodeVolume*>(node);
                    iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getMaterial(_materialBoundingBox));

                    iAABoxd box = renderVolume->getBoundingBox();

                    iRenderer::getInstance().setColor(1, 1, 0, 1);
                    iRenderer::getInstance().drawBBox(box);
                }
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

    _statisticsVisualizer.drawStatistics(&_window, _font, iaColor4f(0, 1, 0, 1));
}

void ModelViewer::run(iaString fileName)
{
    init(fileName);
    iApplication::getInstance().run();
}
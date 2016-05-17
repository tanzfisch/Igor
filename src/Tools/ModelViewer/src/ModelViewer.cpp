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
#include <iNodeTransform.h>
#include <iRenderer.h>
#include <iApplication.h>
#include <iSceneFactory.h>
#include <iScene.h>
#include <iNodeFactory.h>
#include <iMouse.h>
#include <iTimer.h>
#include <iTextureFont.h>
#include <iNodeLight.h>
#include <iModelResourceFactory.h>
#include <iNodeVisitorBoundings.h>
#include <iMaterialResourceFactory.h>
#include <iWidgetDefaultTheme.h>
#include <iWidgetDialog.h>
#include <iWidgetGrid.h>
#include <iWidgetScroll.h>
#include <iMessageBox.h>
#include <iStatistics.h>
using namespace Igor;

#include "MenuDialog.h"

ModelViewer::ModelViewer()
{
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

    iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &ModelViewer::onMouseWheel));
    iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &ModelViewer::onMouseMoved));
    iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownDelegate(this, &ModelViewer::onKeyPressed));
}

void ModelViewer::init(iaString fileName)
{
    con(" -- Model Viewer --" << endl);

    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &ModelViewer::onKeyPressed));
    iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &ModelViewer::onMouseMoved));
    iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &ModelViewer::onMouseWheel));

    iApplication::getInstance().registerApplicationHandleDelegate(iApplicationHandleDelegate(this, &ModelViewer::handle));

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
        "skybox_default/front.png",
        "skybox_default/back.png",
        "skybox_default/left.png",
        "skybox_default/right.png",
        "skybox_default/top.png",
        "skybox_default/bottom.png");
    skyBoxNode->setTextureScale(10);
    skyBoxNode->setMaterial(_materialSkyBox);
    _scene->getRoot()->insertNode(skyBoxNode);

    _font = new iTextureFont("StandardFont.png");

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

    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    _modelViewOrtho.translate(iaVector3f(0, 0, -30));

    if (!fileName.isEmpty())
    {
        iNodeModel* model = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        model->setModel(fileName);
        _groupNode->insertNode(model);
    }

    initGUI();

    if (fileName.isEmpty())
    {
        _fileDialog->load(FileDialogCloseDelegate(this, &ModelViewer::onFileLoadDialogClosed), "..\\data\\models");
    }
    else
    {
        _menuDialog->setActive();
        _menuDialog->setVisible();
    }
    iStatistics::getInstance().setVerbosity(iRenderStatisticsVerbosity::FPSAndMetrics);

    forceLoadingNow();

    _menuDialog->setRootNode(_groupNode);
    _menuDialog->updateGraph();
}

void ModelViewer::onAddTransformation(uint32 atNodeID)
{
    iNode* destination = iNodeFactory::getInstance().getNode(atNodeID);

    if (destination == nullptr)
    {
        destination = _groupNode;
    }

    iNodeTransform* transform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    destination->insertNode(transform);
    _menuDialog->updateGraph();
}

void ModelViewer::onAddGroup(uint32 atNodeID)
{
    iNode* destination = iNodeFactory::getInstance().getNode(atNodeID);

    if (destination == nullptr)
    {
        destination = _groupNode;
    }

    iNode* group = static_cast<iNode*>(iNodeFactory::getInstance().createNode(iNodeType::iNode));
    destination->insertNode(group);
    _menuDialog->updateGraph();
}

void ModelViewer::onAddSwitch(uint32 atNodeID)
{
    iNode* destination = iNodeFactory::getInstance().getNode(atNodeID);

    if (destination == nullptr)
    {
        destination = _groupNode;
    }

    iNodeSwitch* switchNode = static_cast<iNodeSwitch*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSwitch));
    destination->insertNode(switchNode);
    _menuDialog->updateGraph();
}

void ModelViewer::forceLoadingNow()
{
    // want everything to be loaded now!
    con_endl("loading data synchronously ... ");

    while(true)
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
    iSpheref sphere;
    visitorBoundings.getSphere(sphere);

    iaMatrixf coiMatrix;
    coiMatrix._pos = sphere._center;
    _cameraCOI->setMatrix(coiMatrix);
    _camDistance = sphere._radius * 3.0f;
    _camMinDistance = sphere._radius * 0.5f;
    _camMaxDistance = sphere._radius * 10.0f;

    updateCamDistance();
}

void ModelViewer::onImportFile()
{
    _fileDialog->load(FileDialogCloseDelegate(this, &ModelViewer::onImportFileDialogClosed), "");
}

void ModelViewer::onImportFileReference()
{
    _fileDialog->load(FileDialogCloseDelegate(this, &ModelViewer::onImportFileReferenceDialogClosed), "");
}

void ModelViewer::onLoadFile()
{
    _fileDialog->load(FileDialogCloseDelegate(this, &ModelViewer::onFileLoadDialogClosed), "");
}

void ModelViewer::onSaveFile()
{
    _fileDialog->save(FileDialogCloseDelegate(this, &ModelViewer::onFileSaveDialogClosed), "");
}

void ModelViewer::onFileSaveDialogClosed(iFileDialogReturnValue fileDialogReturnValue)
{
    if (_fileDialog->getReturnState() == iFileDialogReturnValue::Ok)
    {
        iaString filename = _fileDialog->getFullPath();
        iModelResourceFactory::getInstance().exportModelData(filename, _groupNode);
    }
}

void ModelViewer::onImportFileDialogClosed(iFileDialogReturnValue fileDialogReturnValue)
{
    if (_fileDialog->getReturnState() == iFileDialogReturnValue::Ok)
    {
        iaString filename = _fileDialog->getFullPath();

        iNodeModel* model = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        model->setModel(filename);
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
            _groupNode->insertNode(groupNode);
        }

        auto child = children.begin();
        while (child != children.end())
        {
            model->removeNode((*child));

            if (groupNode != nullptr)
            {
                groupNode->insertNode((*child));
            }
            else
            {
                _groupNode->insertNode((*child));
            }
            child++;
        }

        iNodeFactory::getInstance().destroyNode(model);

        forceLoadingNow();
    }

    _menuDialog->setActive();
    _menuDialog->setVisible();
    _menuDialog->updateGraph();
}

void ModelViewer::onImportFileReferenceDialogClosed(iFileDialogReturnValue fileDialogReturnValue)
{
    if (_fileDialog->getReturnState() == iFileDialogReturnValue::Ok)
    {
        iaString filename = _fileDialog->getFullPath();

        iNodeModel* model = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        model->setModel(filename);
        _groupNode->insertNode(model);
        forceLoadingNow();
    }

    _menuDialog->setActive();
    _menuDialog->setVisible();
    _menuDialog->updateGraph();
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
                iNodeFactory::getInstance().destroyNode((*child));
                child++;
            }
        }

        iNodeModel* model = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        model->setModel(filename);
        _groupNode->insertNode(model);

        _mouseOverDialogs = false;
        forceLoadingNow();
    }

    _menuDialog->setActive();
    _menuDialog->setVisible();
    _menuDialog->updateGraph();
}

void ModelViewer::initGUI()
{
    _widgetTheme = new iWidgetDefaultTheme("StandardFont.png", "WidgetThemePattern.png");
    iWidgetManager::getInstance().setTheme(_widgetTheme);
    iWidgetManager::getInstance().setDesktopDimensions(_window.getClientWidth(), _window.getClientHeight());

    _menuDialog = new MenuDialog();
    _menuDialog->registerOnMouseOverEvent(iMouseOverDelegate(this, &ModelViewer::onMouseOverDialogs));
    _menuDialog->registerOnMouseOffEvent(iMouseOffDelegate(this, &ModelViewer::onMouseOffDialogs));
    _menuDialog->registerOnExitModelViewer(ExitModelViewerDelegate(this, &ModelViewer::onExitModelViewer));
    _menuDialog->registerOnLoadFile(LoadFileDelegate(this, &ModelViewer::onLoadFile));
    _menuDialog->registerOnImportFile(ImportFileDelegate(this, &ModelViewer::onImportFile));
    _menuDialog->registerOnImportFileReference(ImportFileReferenceDelegate(this, &ModelViewer::onImportFileReference));
    _menuDialog->registerOnSaveFile(SaveFileDelegate(this, &ModelViewer::onSaveFile));
    _menuDialog->registerOnAddTransformation(AddTransformationDelegate(this, &ModelViewer::onAddTransformation));
    _menuDialog->registerOnAddSwitch(AddSwitchDelegate(this, &ModelViewer::onAddSwitch));
    _menuDialog->registerOnAddGroup(AddGroupDelegate(this, &ModelViewer::onAddGroup));

    _fileDialog = new iFileDialog();
    _fileDialog->registerOnMouseOverEvent(iMouseOverDelegate(this, &ModelViewer::onMouseOverDialogs));
    _fileDialog->registerOnMouseOffEvent(iMouseOffDelegate(this, &ModelViewer::onMouseOffDialogs));

    _messageBox = new iMessageBox();
}

void ModelViewer::deinitGUI()
{
    if (_menuDialog != nullptr)
    {
        _menuDialog->unregisterOnMouseOverEvent(iMouseOverDelegate(this, &ModelViewer::onMouseOverDialogs));
        _menuDialog->unregisterOnMouseOffEvent(iMouseOffDelegate(this, &ModelViewer::onMouseOffDialogs));
        _menuDialog->unregisterOnExitModelViewer(ExitModelViewerDelegate(this, &ModelViewer::onExitModelViewer));
        _menuDialog->unregisterOnLoadFile(LoadFileDelegate(this, &ModelViewer::onLoadFile));
        _menuDialog->unregisterOnImportFile(ImportFileDelegate(this, &ModelViewer::onImportFile));
        _menuDialog->unregisterOnImportFileReference(ImportFileReferenceDelegate(this, &ModelViewer::onImportFileReference));
        _menuDialog->unregisterOnSaveFile(SaveFileDelegate(this, &ModelViewer::onSaveFile));
        _menuDialog->unregisterOnAddTransformation(AddTransformationDelegate(this, &ModelViewer::onAddTransformation));
        _menuDialog->unregisterOnAddSwitch(AddSwitchDelegate(this, &ModelViewer::onAddSwitch));
        _menuDialog->unregisterOnAddGroup(AddGroupDelegate(this, &ModelViewer::onAddGroup));

        delete _menuDialog;
        _menuDialog = nullptr;
    }

    if (_fileDialog != nullptr)
    {
        _fileDialog->unregisterOnMouseOverEvent(iMouseOverDelegate(this, &ModelViewer::onMouseOverDialogs));
        _fileDialog->unregisterOnMouseOffEvent(iMouseOffDelegate(this, &ModelViewer::onMouseOffDialogs));
        delete _fileDialog;
        _fileDialog = nullptr;
    }

    if (_messageBox != nullptr)
    {
        delete _messageBox;
        _messageBox = nullptr;
    }

    iWidgetManager::getInstance().setTheme(nullptr);
    if (_widgetTheme != nullptr)
    {
        delete _widgetTheme;
        _widgetTheme = nullptr;
    }
}

void ModelViewer::onMouseOffDialogs(iWidget* source)
{
    _mouseOverDialogs = false;
}

void ModelViewer::onMouseOverDialogs(iWidget* source)
{
    _mouseOverDialogs = true;
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

void ModelViewer::onMouseWheel(int32 d)
{
    if (!_mouseOverDialogs)
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
}

void ModelViewer::onMouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* _window)
{
    if (!_mouseOverDialogs)
    {
        if (iMouse::getInstance().getLeftButton())
        {

            _cameraPitch->rotate((y2 - y1) * 0.005f, iaAxis::X);
            _cameraHeading->rotate((x1 - x2) * 0.005f, iaAxis::Y);

            iMouse::getInstance().setCenter(true);
        }

        if (iMouse::getInstance().getRightButton())
        {
            float32 dx = static_cast<float32>(x1 - x2) * 0.005f;
            _directionalLightRotate->rotate(dx, iaAxis::Y);

            iMouse::getInstance().setCenter(true);
        }
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
    if (key == iKeyCode::ESC)
    {
        iApplication::getInstance().stop();
    }

    if (key == iKeyCode::F1)
    {
        iNodeVisitorPrintTree printTree;
        printTree.printToConsole(_scene->getRoot());
    }
}

void ModelViewer::handle()
{
    _scene->handle();
}

void ModelViewer::render()
{

}

void ModelViewer::renderOrtho()
{
    iaMatrixf identity;
    iRenderer::getInstance().setViewMatrix(identity);
    iRenderer::getInstance().setModelMatrix(_modelViewOrtho);

    iWidgetManager::getInstance().draw();

    iStatistics::getInstance().drawStatistics(&_window, _font, iaColor4f(0,0,0,1));

    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
}

void ModelViewer::run(iaString fileName)
{
    init(fileName);
    iApplication::getInstance().run();
}
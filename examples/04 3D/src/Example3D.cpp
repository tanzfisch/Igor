#include "Example3D.h"

#include <iaConsole.h>
#include <iaString.h>
using namespace IgorAux;

#include <iMaterial.h>
#include <iNodeVisitorPrintTree.h>
#include <iTaskManager.h>
#include <iNodeSkyBox.h>
#include <iNodeCamera.h>
#include <iNodeModel.h> 
#include <iNodeTransform.h>
#include <iRenderer.h>
#include <iApplication.h>
#include <iSceneFactory.h>
#include <iScene.h>
#include <iNodeManager.h>
#include <iMouse.h>
#include <iKeyboard.h>
#include <iTimer.h>
#include <iTextureFont.h>
#include <iNodeLight.h>
#include <iModelResourceFactory.h>
#include <iTaskFlushModels.h>
#include <iTaskFlushTextures.h>
#include <iaString.h>
#include <iMaterialResourceFactory.h>
#include <iProfiler.h>
#include <iNodeSwitch.h>
#include <iNodeLODSwitch.h>
#include <iNodeLODTrigger.h>
#include <iNodeVisitorRenderColorID.h>
#include <iEvaluationManager.h>
#include <iEvaluationTransform.h>
using namespace Igor;

Example3D::Example3D()
{
    init();
}

Example3D::~Example3D()
{
    deinit();
}

void Example3D::init()
{
    con(" -- 3D Example --" << endl);

    // setup window
    _window.setTitle("Igor - 3D Example");
    _window.setClientSize(1024, 768);
    _window.setCentered();
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &Example3D::onWindowClosed));
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &Example3D::onWindowResized));

    // setup perspective view
    _view.setClearColor(iaColor4f(0.5f, 0, 0.5f, 1));
    _view.setPerspective(45);
    _view.setClipPlanes(0.1f, 10000.f);
    _window.addView(&_view);

    // setup orthogonal view
    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setOrthogonal(0.0, static_cast<float32>(_window.getClientWidth()), static_cast<float32>(_window.getClientHeight()), 0.0);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &Example3D::onRenderOrtho));
    _window.addView(&_viewOrtho);
    // and open the window
    _window.open();

    // init scene
    _scene = iSceneFactory::getInstance().createScene();
    // bind scene to perspective view
    _view.setScene(_scene);

    // setup camera
    // we want a camera which can be rotated arround the origin
    // we will acchive that with 3 transform nodes
    // one is for the heading
    iNodeTransform* cameraHeading = iNodeManager::getInstance().createNode<iNodeTransform>();
    // give the transform node a name. naming is optional and ist jus for helping to debug. 
    // Names do not have to be unique but since it is possible to find nodes by name they better are
    cameraHeading->setName("camera heading");
    _cameraHeading = cameraHeading->getID();
    // one is for the pitch
    iNodeTransform* cameraPitch = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraPitch->setName("camera pitch");
    _cameraPitch = cameraPitch->getID();
    // and one is for translation or distance from the origin
    iNodeTransform* cameraTranslation = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraTranslation->setName("camera translation");
    // translate away from origin
    cameraTranslation->translate(0, 0, 10);
    _cameraTranslation = cameraTranslation->getID();
    // from all nodes that we want to control later we save the node ID
    // and last but not least we create a camera node
    iNodeCamera* camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    camera->setName("camera");
    // and build everything together
    // first we add the heading to the root node
    _scene->getRoot()->insertNode(cameraHeading);
    // than the pitch to the heading node
    cameraHeading->insertNode(cameraPitch);
    // then the translation to the pitch node
    cameraPitch->insertNode(cameraTranslation);
    // and than we add the camera to the translation
    cameraTranslation->insertNode(camera);
    // and finally we set the camera active. for this to work a camera must be part of a scene 
    // wich we achived by adding all those nodes on to an other starting with the root node
    _view.setCurrentCamera(camera->getID());

    // create a single cat model
    iNodeTransform* justCatTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    justCatTransform->setName("just a cat transform");
    justCatTransform->translate(0, 1, 0);
    // create a cat model
    iNodeModel* justCatModel = iNodeManager::getInstance().createNode<iNodeModel>();
    // Node model names can be altered but they also are generated based on the file name
    justCatModel->setModel("cat.ompf");
    // building the created nodes together and insert them in the scene
    _scene->getRoot()->insertNode(justCatTransform);
    justCatTransform->insertNode(justCatModel);

    iEvaluationTransformPtr evalTrans = iEvaluationManager::getInstance().createEvaluation<iEvaluationTransform>(justCatTransform->getID());
    if (evalTrans != nullptr)
    {
        iaMatrixd sourceMatrix;
        iaMatrixd destinationMatrix;
        justCatTransform->getMatrix(sourceMatrix);
        destinationMatrix = sourceMatrix;
        destinationMatrix.translate(0, -1, 0);
        evalTrans->setSource(sourceMatrix);
        evalTrans->setDestination(destinationMatrix);
        evalTrans->setStart(5);
        evalTrans->setDuration(2);
        evalTrans->setEasingFunction(Easing::EasingFunction::OutElastic);
    }

    // create a group of models that can be moved together due to being child to the same transform node
    // creating transformation node used for the heading of it's children
    iNodeTransform* allObjectsHeading = iNodeManager::getInstance().createNode<iNodeTransform>();
    allObjectsHeading->setName("all objects heading");
    _allObjectsHeading = allObjectsHeading->getID();
    // creating transformation node used for the pitch of it's children
    iNodeTransform* allObjectsPitch = iNodeManager::getInstance().createNode<iNodeTransform>();
    allObjectsPitch->setName("all objects pitch");
    _allObjectsPitch = allObjectsPitch->getID();
    // and add the nodes to the scene
    _scene->getRoot()->insertNode(allObjectsHeading);
    allObjectsHeading->insertNode(allObjectsPitch);

    // next we create a couple of model nodes that will be connected to be alternative representations of each other controled by a switch node
    // create a cat model including a transform node to add later to the switch node
    iNodeTransform* catTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    catTransform->setName("cat transform");
    catTransform->translate(1, -1, 0);
    iNodeModel* catModel = iNodeManager::getInstance().createNode<iNodeModel>();
    catModel->setModel("cat.ompf");
    // add the cat to the cat transform
    catTransform->insertNode(catModel);
    // repead for the teapot
    iNodeTransform* teapotTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    teapotTransform->setName("teapot transform");
    teapotTransform->translate(1, -1, 0);
    teapotTransform->scale(0.33, 0.33, 0.33);
    iNodeModel* teapotModel = iNodeManager::getInstance().createNode<iNodeModel>();
    teapotModel->setModel("teapot.ompf");
    teapotTransform->insertNode(teapotModel);
    // repeat for a crate
    iNodeTransform* crateTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    crateTransform->setName("crate transform");
    crateTransform->translate(1, -0.5, 0);
    iNodeModel* crateModel = iNodeManager::getInstance().createNode<iNodeModel>();
    crateModel->setModel("crate.ompf");
    crateTransform->insertNode(crateModel);
    // creating the switch node
    iNodeSwitch* switchNode = iNodeManager::getInstance().createNode<iNodeSwitch>();
    _switchNode = switchNode->getID();
    // add the switch node to the all objects pitch / group
    allObjectsPitch->insertNode(switchNode);
    // than add the transform nodes to the switch node
    switchNode->insertNode(crateTransform);
    switchNode->insertNode(teapotTransform);
    switchNode->insertNode(catTransform);
    // set the current activated child
    switchNode->setActiveChild("crate transform");

    // now we basically do the same again. creating three models and tree transform nodes but this time we add them not to a switch node but to an level of detail node
    // the teapot as LOD0
    iNodeTransform* lod0Transform = iNodeManager::getInstance().createNode<iNodeTransform>();
    lod0Transform->setName("lod0 transform");
    lod0Transform->translate(-1, -1, 0);
    lod0Transform->scale(0.33, 0.33, 0.33);
    iNodeModel* lod0Model = iNodeManager::getInstance().createNode<iNodeModel>();
    lod0Model->setModel("teapot.ompf");
    lod0Transform->insertNode(lod0Model);
    // the cat as LOD1
    iNodeTransform* lod1Transform = iNodeManager::getInstance().createNode<iNodeTransform>();
    lod1Transform->setName("lod1 transform");
    lod1Transform->translate(-1, -1, 0);
    iNodeModel* lod1Model = iNodeManager::getInstance().createNode<iNodeModel>();
    lod1Model->setModel("cat.ompf");
    lod1Transform->insertNode(lod1Model);
    // the create as LOD2
    iNodeTransform* lod2Transform = iNodeManager::getInstance().createNode<iNodeTransform>();
    lod2Transform->setName("lod0 transform");
    lod2Transform->translate(-1, -0.5, 0);
    iNodeModel* lod2Model = iNodeManager::getInstance().createNode<iNodeModel>();
    lod2Model->setModel("crate.ompf");
    lod2Transform->insertNode(lod2Model);
    // creating the LOD switch node
    iNodeLODSwitch* lodswitch = iNodeManager::getInstance().createNode<iNodeLODSwitch>();
    // add it to the scene
    allObjectsPitch->insertNode(lodswitch);
    // and add the nodes to it
    lodswitch->insertNode(lod0Transform);
    lodswitch->insertNode(lod1Transform);
    lodswitch->insertNode(lod2Transform);
    // set up the nodes switch distances. the switch distances are allowed to have gaps or overlap
    lodswitch->setThresholds(lod0Transform, 0.0f, 5.0f);
    lodswitch->setThresholds(lod1Transform, 5.0f, 11.0f);
    lodswitch->setThresholds(lod2Transform, 11.0f, 22.0f);
    // in order to causing the LOD switch to change the active child node it needs to be triggered by a LOD trigger node
    // creating the lod trigger node
    iNodeLODTrigger* lodtrigger = iNodeManager::getInstance().createNode<iNodeLODTrigger>();
    // bind the lod switch node with the lod trigger node
    lodswitch->addTrigger(lodtrigger);
    // and add the lod trigger to the scene by attaching it to the camera. there can be multiple LOD triggers and any lod switch can react on any lod trigger
    camera->insertNode(lodtrigger);

    // create a skybox
    iNodeSkyBox* skyBoxNode = iNodeManager::getInstance().createNode<iNodeSkyBox>();
    // set it up with the default skybox texture
    skyBoxNode->setTextures(
        iTextureResourceFactory::getInstance().requestFile("skybox_default/front.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/back.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/left.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/right.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/top.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/bottom.png"));
    // create a material for the sky box because the default material for all iNodeRender and deriving classes has no textures and uses depth test
    _materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial();
    auto material = iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox);
    material->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    material->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    material->setOrder(iMaterial::RENDER_ORDER_MIN);
    material->setName("SkyBox");
    // set that material
    skyBoxNode->setMaterial(_materialSkyBox);
    // and add it to the scene
    _scene->getRoot()->insertNode(skyBoxNode);

    // setup light
    // transform node for the lights orientation
    iNodeTransform* directionalLightRotate = iNodeManager::getInstance().createNode<iNodeTransform>();
    // keep transform node id so we can manipulate the light's position later
    _directionalLightRotate = directionalLightRotate->getID();
    // transform node for the lights distance to the origin
    iNodeTransform* directionalLightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    directionalLightTranslate->translate(100, 100, 100);
    // the light node
    iNodeLight* lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    lightNode->setAmbient(iaColor4f(0.3f, 0.3f, 0.3f, 1.0f));
    lightNode->setDiffuse(iaColor4f(0.8f, 0.8f, 0.8f, 1.0f));
    lightNode->setSpecular(iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
    // insert light to scene
    _scene->getRoot()->insertNode(directionalLightRotate);
    directionalLightRotate->insertNode(directionalLightTranslate);
    directionalLightTranslate->insertNode(lightNode);

    // init font for render profiler
    _font = new iTextureFont("StandardFont.png");
    _profilerVisualizer.setVerbosity(iProfilerVerbosity::FPSAndMetrics);

    // prepare igor logo
    _igorLogo = iTextureResourceFactory::getInstance().loadFile("special/splash.png", iResourceCacheMode::Free, iTextureBuildMode::Normal);
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    material = iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending);
    material->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    material->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    material->setRenderState(iRenderState::Blend, iRenderStateValue::On);
    material->setName("LogoMaterial");

    // animation
    _animationTimingHandle = new iTimerHandle(iTimerTickDelegate(this, &Example3D::onTimer), 10);
    _animationTimingHandle->start();

    // start resource tasks
    _taskFlushModels = iTaskManager::getInstance().addTask(new iTaskFlushModels(&_window));
    _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

    // register some callbacks
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &Example3D::onKeyPressed));
    iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &Example3D::onMouseMoved));
    iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &Example3D::onMouseWheel));
}

void Example3D::deinit()
{
    // unregister some callbacks
    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &Example3D::onKeyPressed));
    iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &Example3D::onMouseMoved));
    iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &Example3D::onMouseWheel));
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &Example3D::onWindowClosed));
    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &Example3D::onWindowResized));
    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &Example3D::onRenderOrtho));

    // release resources
    if (_font != nullptr)
    {
        delete _font;
        _font = nullptr;
    }

    _igorLogo = nullptr;

    // stop light animation
    if (_animationTimingHandle)
    {
        delete _animationTimingHandle;
        _animationTimingHandle = nullptr;
    }

    iSceneFactory::getInstance().destroyScene(_scene);
    _scene = nullptr;

    // abort resource tasks
    iTaskManager::getInstance().abortTask(_taskFlushModels);
    iTaskManager::getInstance().abortTask(_taskFlushTextures);

    iMaterialResourceFactory::getInstance().destroyMaterial(_materialSkyBox);

    if (_window.isOpen())
    {
        _window.close();
        _window.removeView(&_view);
        _window.removeView(&_viewOrtho);
    }
}

void Example3D::onMouseWheel(int32 d)
{
    iNodeTransform* camTranslation = static_cast<iNodeTransform*>(iNodeManager::getInstance().getNode(_cameraTranslation));
    if (camTranslation != nullptr)
    {
        if (d < 0)
        {
            camTranslation->translate(0, 0, 1);
        }
        else
        {
            camTranslation->translate(0, 0, -1);
        }
    }
}

void Example3D::onMouseMoved(const iaVector2i& from, const iaVector2i& to, iWindow* _window)
{
    if (iMouse::getInstance().getRightButton())
    {
        iNodeTransform* allObjectsPitch = static_cast<iNodeTransform*>(iNodeManager::getInstance().getNode(_allObjectsPitch));
        iNodeTransform* allObjectsHeading = static_cast<iNodeTransform*>(iNodeManager::getInstance().getNode(_allObjectsHeading));

        if (allObjectsPitch != nullptr &&
            allObjectsHeading != nullptr)
        {
            allObjectsPitch->rotate((to._y - from._y) * 0.005f, iaAxis::X);
            allObjectsHeading->rotate((to._x - from._x) * 0.005f, iaAxis::Y);

            iMouse::getInstance().setCenter();
        }
    }
    else if (iMouse::getInstance().getLeftButton())
    {
        iNodeTransform* cameraPitch = static_cast<iNodeTransform*>(iNodeManager::getInstance().getNode(_cameraPitch));
        iNodeTransform* cameraHeading = static_cast<iNodeTransform*>(iNodeManager::getInstance().getNode(_cameraHeading));

        if (cameraPitch != nullptr &&
            cameraHeading != nullptr)
        {
            cameraPitch->rotate((to._y - from._y) * 0.005f, iaAxis::X);
            cameraHeading->rotate((to._x - from._x) * 0.005f, iaAxis::Y);

            iMouse::getInstance().setCenter();
        }
    }
}

void Example3D::onWindowClosed()
{
    iApplication::getInstance().stop();
}

void Example3D::onWindowResized(int32 clientWidth, int32 clientHeight)
{
    _viewOrtho.setOrthogonal(0.0, static_cast<float32>(clientWidth), static_cast<float32>(clientHeight), 0.0);
}

void Example3D::onKeyPressed(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::ESC:
        iApplication::getInstance().stop();
        break;

    case iKeyCode::F8:
        _profilerVisualizer.cycleVerbosity();
        break;

    case iKeyCode::F9:
    {
        iNodeVisitorPrintTree printTree;
        if (_scene != nullptr)
        {
            printTree.printToConsole(_scene->getRoot());
        }
    }
    break;

    case iKeyCode::F10:
        _view.setWireframeVisible(!_view.isWireframeVisible());
        break;

    case iKeyCode::F11:
        _view.setOctreeVisible(!_view.isOctreeVisible());
        break;

    case iKeyCode::F12:
        _view.setBoundingBoxVisible(!_view.isBoundingBoxVisible());
        break;

    case iKeyCode::Space:
    {
        _activeNode++;
        if (_activeNode > 2)
        {
            _activeNode = 0;
        }

        iNodeSwitch* switchNode = static_cast<iNodeSwitch*>(iNodeManager::getInstance().getNode(_switchNode));
        if (switchNode != nullptr)
        {
            switch (_activeNode)
            {
            case 0:
                switchNode->setActiveChild("crate transform");
                break;
            case 1:
                switchNode->setActiveChild("cat transform");
                break;
            case 2:
                switchNode->setActiveChild("teapot transform");
                break;
            }
        }
    }
    break;
    }
}

void Example3D::onTimer()
{
    iNodeTransform* directionalLightRotate = static_cast<iNodeTransform*>(iNodeManager::getInstance().getNode(_directionalLightRotate));
    directionalLightRotate->rotate(0.005f, iaAxis::Y);
}

void Example3D::onRenderOrtho()
{
    iaMatrixd viewMatrix;
    iRenderer::getInstance().setViewMatrix(viewMatrix);

    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(modelMatrix);

    drawLogo();

    // draw frame rate in lower right corner
    _profilerVisualizer.draw(&_window, _font, iaColor4f(0, 1, 0, 1));
}

void Example3D::drawLogo()
{
    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    float32 width = static_cast<float32>(_igorLogo->getWidth());
    float32 height = static_cast<float32>(_igorLogo->getHeight());
    float32 x = static_cast<float32>(_window.getClientWidth()) - width;
    float32 y = static_cast<float32>(_window.getClientHeight()) - height;

    iRenderer::getInstance().drawTexture(x, y, width, height, _igorLogo);
}

void Example3D::run()
{
    iApplication::getInstance().run();
}

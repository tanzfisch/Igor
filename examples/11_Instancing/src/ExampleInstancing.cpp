// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "ExampleInstancing.h"

#include <iaConsole.h>
#include <iaux/data/iaString.h>
using namespace IgorAux;

#include <igor/resources/material/iMaterial.h>
#include <igor/graphics/scene/traversal/iNodeVisitorPrintTree.h>
#include <igor/threading/iTaskManager.h>
#include <iNodeSkyBox.h>
#include <iNodeCamera.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <iRenderer.h>
#include <iApplication.h>
#include <iSceneFactory.h>
#include <igor/scene/iScene.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <iMouse.h>
#include <iKeyboard.h>
#include <iTimer.h>
#include <iTextureFont.h>
#include <iNodeLight.h>
#include <iModelResourceFactory.h>
#include <iTaskFlushModels.h>
#include <iTaskFlushTextures.h>
#include <iaux/data/iaString.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <iProfiler.h>
#include <iNodeSwitch.h>
#include <iNodeLODSwitch.h>
#include <iNodeLODTrigger.h>
#include <iNodeMesh.h>
using namespace Igor;

ExampleInstancing::ExampleInstancing()
{
    init();
}

ExampleInstancing::~ExampleInstancing()
{
    deinit();
}

void ExampleInstancing::init()
{
    con(" -- 3D Example --" << endl);

    // setup window
    _window.setTitle("Igor - 3D Example");
    _window.setClientSize(1024, 768);
    _window.setCentered();
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &ExampleInstancing::onWindowClosed));
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &ExampleInstancing::onWindowResized));

    // setup perspective view
    _view.setClearColor(iaColor4f(0.5f, 0, 0.5f, 1));
    _view.setPerspective(45);
    _view.setClipPlanes(0.1f, 10000.f);
    _window.addView(&_view);

    // setup orthogonal view
    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setOrthogonal(0.0f, static_cast<float32>(_window.getClientWidth()), static_cast<float32>(_window.getClientHeight()), 0.0f);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &ExampleInstancing::onRenderOrtho));
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
    iNodeTransform *cameraHeading = iNodeManager::getInstance().createNode<iNodeTransform>();
    // give the transform node a name. naming is optional and ist jus for helping to debug.
    // Names do not have to be unique but since it is possible to find nodes by name they better are
    cameraHeading->setName("camera heading");
    _cameraHeading = cameraHeading->getID();
    // one is for the pitch
    iNodeTransform *cameraPitch = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraPitch->setName("camera pitch");
    _cameraPitch = cameraPitch->getID();
    // and one is for translation or distance from the origin
    iNodeTransform *cameraTranslation = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraTranslation->setName("camera translation");
    // translate away from origin
    cameraTranslation->translate(0, 0, 10);
    _cameraTranslation = cameraTranslation->getID();
    // from all nodes that we want to control later we save the node ID
    // and last but not least we create a camera node
    iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>();
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
    // and finally we tell the view which camera shall be the current one. for this to work a camera must be part of a
    // scene assiciated with the view wich we achived by adding all those nodes on to an other starting with the root node
    _view.setCurrentCamera(camera->getID());

    // first we have to override the material which is stored within the model
    // to do that we create a new material using instancing
    _materialWithInstancing = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithInstancing)->setRenderState(iRenderState::Instanced, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithInstancing)->setRenderState(iRenderState::InstancedFunc, iRenderStateValue::PositionOrientation);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithInstancing)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithInstancing)->addShaderSource("igor/textured_ipo.vert", iShaderObjectType::Vertex);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithInstancing)->addShaderSource("igor/textured_ipo_directional_light.frag", iShaderObjectType::Fragment);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithInstancing)->compileShader();

    const float32 spacing = 3.0f;
    const int32 amountPerDimension = 3;

    // now we can just put copies of that model in the scene
    iNodeTransform *transformGroup = iNodeManager::getInstance().createNode<iNodeTransform>();
    transformGroup->translate(-((amountPerDimension - 1) * spacing * 0.5), -((amountPerDimension - 1) * spacing * 0.5), ((amountPerDimension - 1) * spacing * 0.5));
    _scene->getRoot()->insertNode(transformGroup);

    // create an array of models
    for (int z = 0; z < amountPerDimension; ++z)
    {
        for (int y = 0; y < amountPerDimension; ++y)
        {
            for (int x = 0; x < amountPerDimension; ++x)
            {
                iNodeTransform *transform = iNodeManager::getInstance().createNode<iNodeTransform>();
                transform->translate(x * spacing, y * spacing, -z * spacing);
                transform->rotate(((rand() % 100) / 100.0) * M_PI * 2, iaAxis::X);
                transform->rotate(((rand() % 100) / 100.0) * M_PI * 2, iaAxis::Y);
                transform->rotate(((rand() % 100) / 100.0) * M_PI * 2, iaAxis::Z);

                iNodeModel *modelNode = iNodeManager::getInstance().createNode<iNodeModel>();
                // it is important to use the exact same parameters here as before when we direclty loaded the model
                // because here it will not load it again but get it from the cache where we is still the version with manipulated material
                modelNode->setModel("cat.ompf", iResourceCacheMode::Keep);
                modelNode->registerModelReadyDelegate(iModelReadyDelegate(this, &ExampleInstancing::onModelReady));

                // building the created nodes together and insert them in the scene
                transformGroup->insertNode(transform);
                transform->insertNode(modelNode);
            }
        }
    }

    // create a skybox
    iNodeSkyBox *skyBoxNode = iNodeManager::getInstance().createNode<iNodeSkyBox>();
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
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setOrder(iMaterial::RENDER_ORDER_MIN);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setName("SkyBox");
    // set that material
    skyBoxNode->setMaterial(_materialSkyBox);
    // and add it to the scene
    _scene->getRoot()->insertNode(skyBoxNode);

    // setup light
    // transform node for the lights orientation
    iNodeTransform *directionalLightRotate = iNodeManager::getInstance().createNode<iNodeTransform>();
    // keep transform node id so we can manipulate the light's position later
    _directionalLightRotate = directionalLightRotate->getID();
    // transform node for the lights distance to the origin
    iNodeTransform *directionalLightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    directionalLightTranslate->translate(100, 100, 100);
    // the light node
    iNodeLight *lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    lightNode->setAmbient(iaColor4f(0.3f, 0.3f, 0.3f, 1.0f));
    lightNode->setDiffuse(iaColor4f(0.8f, 0.8f, 0.8f, 1.0f));
    lightNode->setSpecular(iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
    // insert light to scene
    _scene->getRoot()->insertNode(directionalLightRotate);
    directionalLightRotate->insertNode(directionalLightTranslate);
    directionalLightTranslate->insertNode(lightNode);

    // init font for render statistics
    _font = new iTextureFont("StandardFont.png");
    _profilerVisualizer.setVerbosity(iProfilerVerbosity::FPSAndMetrics);

    // prepare igor logo
    _igorLogo = iTextureResourceFactory::getInstance().loadFile("special/splash.png", iResourceCacheMode::Free, iTextureBuildMode::Normal);
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Blend, iRenderStateValue::On);

    // animation
    _animationTimingHandle = new iTimerHandle(iTimerTickDelegate(this, &ExampleInstancing::onTimer), iaTime::fromMilliseconds(10));
    _animationTimingHandle->start();

    // start resource tasks
    _taskFlushModels = iTaskManager::getInstance().addTask(new iTaskFlushModels(&_window));
    _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

    // register some callbacks
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &ExampleInstancing::onKeyPressed));
    iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &ExampleInstancing::onMouseMoved));
    iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &ExampleInstancing::onMouseWheel));
}

void ExampleInstancing::deinit()
{
    // unregister some callbacks
    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &ExampleInstancing::onKeyPressed));
    iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &ExampleInstancing::onMouseMoved));
    iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &ExampleInstancing::onMouseWheel));
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &ExampleInstancing::onWindowClosed));
    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &ExampleInstancing::onWindowResized));
    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &ExampleInstancing::onRenderOrtho));

    // deinit statistics
    if (_font != nullptr)
    {
        delete _font;
        _font = nullptr;
    }

    // stop light animation
    if (_animationTimingHandle)
    {
        _animationTimingHandle->unregisterTimerDelegate(iTimerTickDelegate(this, &ExampleInstancing::onTimer));
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

void ExampleInstancing::onMouseWheel(int32 d)
{
    iNodeTransform *camTranslation = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraTranslation));
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

void ExampleInstancing::onMouseMoved(const iaVector2i &from, const iaVector2i &to, iWindow *_window)
{
    if (iMouse::getInstance().getLeftButton())
    {
        iNodeTransform *cameraPitch = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraPitch));
        iNodeTransform *cameraHeading = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraHeading));

        if (cameraPitch != nullptr &&
            cameraHeading != nullptr)
        {
            cameraPitch->rotate((to._y - from._y) * 0.005f, iaAxis::X);
            cameraHeading->rotate((from._x - to._x) * 0.005f, iaAxis::Y);

            iMouse::getInstance().setCenter();
        }
    }
}

void ExampleInstancing::onWindowClosed()
{
    iApplication::getInstance().stop();
}

void ExampleInstancing::onWindowResized(int32 clientWidth, int32 clientHeight)
{
    _viewOrtho.setOrthogonal(0.0f, static_cast<float32>(clientWidth), static_cast<float32>(clientHeight), 0.0f);
}

void ExampleInstancing::onKeyPressed(iKeyCode key)
{
    if (key == iKeyCode::ESC)
    {
        iApplication::getInstance().stop();
    }

    if (key == iKeyCode::F1)
    {
        iNodeVisitorPrintTree printTree;
        if (_scene != nullptr)
        {
            printTree.printToConsole(_scene->getRoot());
        }
    }
}

void ExampleInstancing::onTimer()
{
    iNodeTransform *directionalLightRotate = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_directionalLightRotate));
    directionalLightRotate->rotate(0.005f, iaAxis::Y);
}

void ExampleInstancing::onRenderOrtho()
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

void ExampleInstancing::drawLogo()
{
    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    float32 width = static_cast<float32>(_igorLogo->getWidth());
    float32 height = static_cast<float32>(_igorLogo->getHeight());
    float32 x = static_cast<float32>(_window.getClientWidth()) - width;
    float32 y = static_cast<float32>(_window.getClientHeight()) - height;

    iRenderer::getInstance().drawTexture(x, y, width, height, _igorLogo);
}

void ExampleInstancing::onModelReady(uint64 modelNodeID)
{
    iNodeModel *modelNode = static_cast<iNodeModel *>(iNodeManager::getInstance().getNode(modelNodeID));
    if (modelNode != nullptr &&
        modelNode->isValid())
    {
        iNodeMesh *meshNode = static_cast<iNodeMesh *>(modelNode->getChild("mesh0001"));
        meshNode->setMaterial(_materialWithInstancing);
    }
}

void ExampleInstancing::run()
{
    iApplication::getInstance().run();
}
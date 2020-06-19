#include "Evaluation.h"

#include <iaux/system/iaConsole.h>
#include <iaux/data/iaString.h>
#include <iaux/math/iaBSpline.h>
#include <iaux/data/iaConvert.h>
using namespace iaux;

#include <igor/resources/material/iMaterial.h>
#include <igor/scene/traversal/iNodeVisitorPrintTree.h>
#include <igor/threading/iTaskManager.h>
#include <igor/scene/nodes/iNodeSkyBox.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/graphics/iRenderer.h>
#include <igor/system/iApplication.h>
#include <igor/scene/iSceneFactory.h>
#include <igor/scene/iScene.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/system/iMouse.h>
#include <igor/system/iKeyboard.h>
#include <igor/system/iTimer.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/scene/nodes/iNodeLight.h>
#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/threading/tasks/iTaskFlushModels.h>
#include <igor/threading/tasks/iTaskFlushTextures.h>
#include <iaux/data/iaString.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/profiler/iProfiler.h>
#include <igor/scene/nodes/iNodeSwitch.h>
#include <igor/scene/nodes/iNodeLODSwitch.h>
#include <igor/scene/nodes/iNodeLODTrigger.h>
#include <igor/scene/traversal/iNodeVisitorRenderColorID.h>
#include <igor/evaluation/iEvaluationManager.h>
#include <igor/evaluation/iEvaluationTransform.h>
#include <igor/evaluation/iEvaluationScript.h>
using namespace igor;

Evaluation::Evaluation()
{
    init();
}

Evaluation::~Evaluation()
{
    deinit();
}

void Evaluation::createCamera()
{
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
    // and finally we set the camera active. for this to work a camera must be part of a scene
    // wich we achived by adding all those nodes on to an other starting with the root node
    _view.setCurrentCamera(camera->getID());
}

void Evaluation::createSkybox()
{
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
    auto material = iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox);
    material->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    material->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    material->setOrder(iMaterial::RENDER_ORDER_MIN);
    material->setName("SkyBox");
    // set that material
    skyBoxNode->setMaterial(_materialSkyBox);
    // and add it to the scene
    _scene->getRoot()->insertNode(skyBoxNode);
}

void Evaluation::init()
{
    con_endl(" -- Evaluation Example --");

    // setup window
    _window.setTitle("Igor - Evaluation Example");
    _window.setClientSize(1024, 768);
    _window.setCentered();
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &Evaluation::onWindowClosed));
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &Evaluation::onWindowResized));

    // setup perspective view
    _view.setClearColor(iaColor4f(0.5f, 0.0f, 0.5f, 1.0f));
    _view.setPerspective(45);
    _view.setClipPlanes(0.1f, 10000.f);
    _window.addView(&_view);

    // setup orthogonal view
    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setOrthogonal(0.0, static_cast<float32>(_window.getClientWidth()), static_cast<float32>(_window.getClientHeight()), 0.0);
    _viewOrtho.registerRenderDelegate(iDrawDelegate(this, &Evaluation::onRenderOrtho));
    _window.addView(&_viewOrtho);
    // and open the window
    _window.open();

    // init scene
    _scene = iSceneFactory::getInstance().createScene();
    // bind scene to perspective view
    _view.setScene(_scene);

    // add camera
    createCamera();

    // create a skybox
    createSkybox();

    // setup light
    createDirectionalLight();

    // setup what this example is actually about
    setupEvaluation();

    // init font for render profiler
    _font = new iTextureFont("StandardFont.png");
    _profilerVisualizer.setVerbosity(iProfilerVerbosity::FPSAndMetrics);

    // prepare igor logo
    _igorLogo = iTextureResourceFactory::getInstance().loadFile("special/splash.png", iResourceCacheMode::Free, iTextureBuildMode::Normal);
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    auto material = iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending);
    material->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    material->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    material->setRenderState(iRenderState::Blend, iRenderStateValue::On);
    material->setName("LogoMaterial");

    // start resource tasks
    _taskFlushModels = iTaskManager::getInstance().addTask(new iTaskFlushModels(&_window));
    _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

    // register some callbacks
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &Evaluation::onKeyPressed));
    iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &Evaluation::onMouseMoved));
    iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &Evaluation::onMouseWheel));
}

void Evaluation::setupEvaluation()
{
    // create a cat model
    iNodeTransformPtr catTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    catTransform->setName("cat transform");
    catTransform->translate(0, 0, 0);
    iNodeModel *catModel = iNodeManager::getInstance().createNode<iNodeModel>();
    // Node model names can be altered but they also are generated based on the file name
    catModel->setModel("cat.ompf");
    // building the created nodes together and insert them in the scene
    _scene->getRoot()->insertNode(catTransform);
    catTransform->insertNode(catModel);

    // animate the cat model using evaluation
    iEvaluationTransform *transformEvaluation = iEvaluationManager::getInstance().createEvaluation<iEvaluationTransform>(catTransform->getID());

    // create some key frames using a bspline
    iaBSpline bspline;
    bspline.addSupportPoint(iaVector3f(-4, 0, 0));
    bspline.addSupportPoint(iaVector3f(-2, 5, 0));
    bspline.addSupportPoint(iaVector3f(2, -5, 0));
    bspline.addSupportPoint(iaVector3f(4, 0, 0));

    std::vector<iaVector3f> points;
    bspline.getPoints(points, 20);

    for (auto point : points)
    {
        iaMatrixd matrix;
        matrix.translate(point._x, point._y, point._z);
        transformEvaluation->addKeyframe(matrix);
    }

    // keep repeating the animation
    transformEvaluation->setLooped();

    // set smooth easing function
    transformEvaluation->setEasingFunction(Easing::EasingFunction::InOutCubic);

    // set duration aka period to 4 seconds
    transformEvaluation->setDuration(iaTime::fromSeconds(4.0));
    // start now
    transformEvaluation->setStart(iTimer::getInstance().getFrameTime());

    // add a script evaluation on top
    iEvaluationScript *scriptEvaluation = iEvaluationManager::getInstance().createEvaluation<iEvaluationScript>(catTransform->getID());
    scriptEvaluation->setLooped();
    scriptEvaluation->setDuration(iaTime::fromSeconds(4.0));
    scriptEvaluation->setStart(iTimer::getInstance().getFrameTime());
    scriptEvaluation->setScript(iEvaluationScriptDelegate(this, &Evaluation::evalScript));
    scriptEvaluation->setEasingFunction(Easing::EasingFunction::OutElastic);
}

void Evaluation::evalScript(iNodeTransformPtr transform, float64 t)
{
    transform->rotate(t, iaAxis::Y);
}

void Evaluation::createDirectionalLight()
{
    // transform node for the lights orientation
    iNodeTransform *directionalLightRotate = iNodeManager::getInstance().createNode<iNodeTransform>();
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
}

void Evaluation::deinit()
{
    // unregister some callbacks
    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &Evaluation::onKeyPressed));
    iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &Evaluation::onMouseMoved));
    iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &Evaluation::onMouseWheel));
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &Evaluation::onWindowClosed));
    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &Evaluation::onWindowResized));
    _viewOrtho.unregisterRenderDelegate(iDrawDelegate(this, &Evaluation::onRenderOrtho));

    // release resources
    if (_font != nullptr)
    {
        delete _font;
        _font = nullptr;
    }

    _igorLogo = nullptr;

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

void Evaluation::onMouseWheel(int32 d)
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

void Evaluation::onMouseMoved(const iaVector2i &from, const iaVector2i &to, iWindow *_window)
{
    if (iMouse::getInstance().getLeftButton())
    {
        iNodeTransform *cameraPitch = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraPitch));
        iNodeTransform *cameraHeading = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraHeading));

        if (cameraPitch != nullptr &&
            cameraHeading != nullptr)
        {
            cameraPitch->rotate((to._y - from._y) * 0.005f, iaAxis::X);
            cameraHeading->rotate((to._x - from._x) * 0.005f, iaAxis::Y);

            iMouse::getInstance().setCenter();
        }
    }
}

void Evaluation::onWindowClosed()
{
    iApplication::getInstance().stop();
}

void Evaluation::onWindowResized(int32 clientWidth, int32 clientHeight)
{
    _viewOrtho.setOrthogonal(0.0, static_cast<float32>(clientWidth), static_cast<float32>(clientHeight), 0.0);
}

void Evaluation::onKeyPressed(iKeyCode key)
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
    }
}

void Evaluation::onRenderOrtho()
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

void Evaluation::drawLogo()
{
    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    float32 width = static_cast<float32>(_igorLogo->getWidth());
    float32 height = static_cast<float32>(_igorLogo->getHeight());
    float32 x = static_cast<float32>(_window.getClientWidth()) - width;
    float32 y = static_cast<float32>(_window.getClientHeight()) - height;

    iRenderer::getInstance().drawTexture(x, y, width, height, _igorLogo);
}

void Evaluation::run()
{
    iApplication::getInstance().run();
}

#include "ExampleCharacterController.h"

#include <iaConsole.h>
#include <iaString.h>
using namespace IgorAux;

#include <iMaterial.h>
#include <iMaterialGroup.h>
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
#include <iNodeFactory.h>
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
#include <iStatistics.h>
#include <iNodeSwitch.h>
#include <iNodeLODSwitch.h>
#include <iNodeLODTrigger.h>
#include <iPhysics.h>
#include <iPhysicsCollision.h>
#include <iPhysicsBody.h>
#include <iPhysicsJoint.h>
using namespace Igor;

ExampleCharacterController::ExampleCharacterController()
{
    init();
}

ExampleCharacterController::~ExampleCharacterController()
{
    deinit();
}

void ExampleCharacterController::init()
{
    con(" -- 3D Example --" << endl);

    // setup window
    _window.setTitle("Igor - 3D Example");
    _window.setSize(1280, 758);
    _window.setCentered();
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &ExampleCharacterController::onWindowClosed));
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &ExampleCharacterController::onWindowResized));

    // setup perspective view
    _view.setClearColor(iaColor4f(0.5f, 0, 0.5f, 1));
    _view.setPerspective(45);
    _view.setClipPlanes(0.1f, 10000.f);
    _window.addView(&_view);

    // setup orthogonal view
    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &ExampleCharacterController::onRenderOrtho));
    _window.addView(&_viewOrtho);
    // and open the window
    _window.open();

    // init scene
    _scene = iSceneFactory::getInstance().createScene();
    // bind scene to perspective view
    _view.setScene(_scene);

    // setup floor model
    iNodeModel* floorModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    floorModel->setModel("crate.ompf");
    iNodeTransform* floorTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    floorTransform->scale(20, 1, 20);
    floorTransform->insertNode(floorModel);
    _scene->getRoot()->insertNode(floorTransform);

    // setup floor physics
    iaMatrixd offsetFloor;
    offsetFloor.translate(0, 0, 0);
    iPhysicsCollision* floorCollision = iPhysics::getInstance().createBox(20, 1, 20, offsetFloor.getData());
    iPhysicsBody* floorBody = iPhysics::getInstance().createBody(floorCollision);
    floorBody->setMass(0);
    iaMatrixd floorMatrix;
    floorMatrix.translate(0, 0, 0);
    floorBody->setMatrix(floorMatrix);

    // create a box that drops on floor
    iPhysicsCollision* boxCollision = iPhysics::getInstance().createBox(1, 1, 1, iaMatrixd());
    iPhysicsBody* boxBody = iPhysics::getInstance().createBody(boxCollision);
    boxBody->setMass(100);
    boxBody->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &ExampleCharacterController::onApplyForceAndTorque));

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformNode->translate(0, 5, 0);

    iNodeModel* crate = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    crate->setModel("crate.ompf");
    transformNode->insertNode(crate);

    iPhysics::getInstance().bindTransformNode(boxBody, transformNode);
    _scene->getRoot()->insertNode(transformNode);

    // setup character and attache camera to it
    iPhysicsCollision* charCollision = iPhysics::getInstance().createCylinder(1, 2, iaMatrixd());
    iPhysicsBody* charBody = iPhysics::getInstance().createBody(charCollision);
    charBody->setMass(10);
    charBody->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &ExampleCharacterController::onApplyForceAndTorque));

    iNodeTransform* charTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    charTransform->translate(0, 5, 5);
    _scene->getRoot()->insertNode(charTransform);

    iPhysics::getInstance().bindTransformNode(charBody, charTransform);

   // iPhysicsJoint* joint = iPhysics::getInstance().createJoint(charBody, nullptr, 4);
    //joint->registerSubmitConstraintsDelegate(iSubmitConstraintsDelegate(this, &ExampleCharacterController::onSubmitConstraints));
    
    // setup camera
    iNodeCamera* camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
    charTransform->insertNode(camera);
    camera->makeCurrent();

    // create a skybox
    iNodeSkyBox* skyBoxNode = static_cast<iNodeSkyBox*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSkyBox));
    // set it up with the default skybox texture
    skyBoxNode->setTextures(
        iTextureResourceFactory::getInstance().requestFile("skybox_default/front.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/back.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/left.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/right.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/top.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/bottom.png"));
    // manipulate the texture scale so we can see the repeating pattern of the textures
    skyBoxNode->setTextureScale(10);
    // create a material for the sky box because the default material for all iNodeRender and deriving classes has no textures and uses depth test
    _materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterialGroup(_materialSkyBox)->setOrder(iMaterial::RENDER_ORDER_EARLY);
    iMaterialResourceFactory::getInstance().getMaterialGroup(_materialSkyBox)->getMaterial()->setName("SkyBox");
    // set that material
    skyBoxNode->setMaterial(_materialSkyBox);
    // and add it to the scene
    _scene->getRoot()->insertNode(skyBoxNode);

    // setup light
    iNodeTransform* directionalLightTranslate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    directionalLightTranslate->translate(100, 100, 100);
    // the light node
    iNodeLight* lightNode = static_cast<iNodeLight*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLight));
    lightNode->setAmbient(iaColor4f(0.3f, 0.3f, 0.3f, 1.0f));
    lightNode->setDiffuse(iaColor4f(0.8f, 0.8f, 0.8f, 1.0f));
    lightNode->setSpecular(iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
    // insert light to scene
    _scene->getRoot()->insertNode(directionalLightTranslate);
    directionalLightTranslate->insertNode(lightNode);

    // init font for render statistics
    _font = new iTextureFont("StandardFont.png");
    _statisticsVisualizer.setVerbosity(iRenderStatisticsVerbosity::FPSAndMetrics);

    // prepare igor logo
    _igorLogo = iTextureResourceFactory::getInstance().loadFile("special/splash.png", iResourceCacheMode::Free, iTextureBuildMode::Normal);
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);

    // start resource tasks
    _taskFlushModels = iTaskManager::getInstance().addTask(new iTaskFlushModels(&_window));
    _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

    // register some callbacks
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &ExampleCharacterController::onKeyPressed));
    iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &ExampleCharacterController::onMouseMoved));
    iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &ExampleCharacterController::onMouseWheel));
}

void ExampleCharacterController::onSubmitConstraints(iPhysicsJoint* joint, float32 timestep)
{
    iPhysicsBody* body0 = iPhysics::getInstance().getBody(joint->getBody0ID());
    iaMatrixd matrixBody0;
    body0->getMatrix(matrixBody0);

    bool frontset = false;
    bool upset = false;

    iaVector3d lateralDir(matrixBody0._right % iaVector3d(1, 0, 0));
    float64 mag = lateralDir * lateralDir;
    
    if (mag > 1.0e-6f)
    {
        // if the side vector is not zero, it means the body has rotated
        mag = sqrt(mag);
        lateralDir /= mag;
        float64 angle = sin(mag);

        // add an angular constraint to correct the error angle
        iPhysics::getInstance().setUserJointAddAngularRow(joint, angle, lateralDir);

        // in theory only one correction is needed, but this produces instability as the body may move sideway.
        // a lateral correction prevent this from happening.
        iaVector3d frontDir(lateralDir % iaVector3d(1, 0, 0));
        iPhysics::getInstance().setUserJointAddAngularRow(joint, 0.0, frontDir);

        frontset = true;
    }

    lateralDir = matrixBody0._top % iaVector3d(0, 1, 0);
    mag = lateralDir * lateralDir;
    if (mag > 1.0e-6f)
    {
        // if the side vector is not zero, it means the body has rotated
        mag = sqrt(mag);
        lateralDir /= mag;
        float64 angle = sin(mag);

        // add an angular constraint to correct the error angle
        iPhysics::getInstance().setUserJointAddAngularRow(joint, angle, lateralDir);

        // in theory only one correction is needed, but this produces instability as the body may move sideway.
        // a lateral correction prevent this from happening.
        iaVector3d topDir(lateralDir % iaVector3d(0, 1, 0));
        iPhysics::getInstance().setUserJointAddAngularRow(joint, 0.0, topDir);

        upset = true;
    }

    if (frontset && upset)
    {
    }
    else
    {
        if ((!frontset) && (!upset))
        {
            iPhysics::getInstance().setUserJointAddAngularRow(joint, 0.0, iaVector3d(1, 0, 0));
            iPhysics::getInstance().setUserJointAddAngularRow(joint, 0.0, iaVector3d(0, 1, 0));
        }
        iPhysics::getInstance().setUserJointAddAngularRow(joint, 0.0, iaVector3d(0, 0, 1));
    }
}

void ExampleCharacterController::onApplyForceAndTorque(iPhysicsBody* body, float32 timestep)
{
    float64 Ixx;
    float64 Iyy;
    float64 Izz;
    float64 mass;
    iaVector3d force;

    iPhysics::getInstance().getMassMatrix(static_cast<void*>(body->getNewtonBody()), mass, Ixx, Iyy, Izz);
    force.set(0.0f, -mass * static_cast<float32>(__IGOR_GRAVITY__), 0.0f);

    body->setForce(force);
}

void ExampleCharacterController::deinit()
{
    // unregister some callbacks
    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &ExampleCharacterController::onKeyPressed));
    iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &ExampleCharacterController::onMouseMoved));
    iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &ExampleCharacterController::onMouseWheel));
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &ExampleCharacterController::onWindowClosed));
    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &ExampleCharacterController::onWindowResized));
    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &ExampleCharacterController::onRenderOrtho));

    // deinit statistics
    if (_font != nullptr)
    {
        delete _font;
        _font = nullptr;
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

void ExampleCharacterController::onMouseWheel(int32 d)
{
    iNodeTransform* camTranslation = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_cameraTranslation));
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

void ExampleCharacterController::onMouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* _window)
{
    if (iMouse::getInstance().getLeftButton())
    {
        iNodeTransform* cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_cameraPitch));
        iNodeTransform* cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_cameraHeading));

        if (cameraPitch != nullptr &&
            cameraHeading != nullptr)
        {
            cameraPitch->rotate((y2 - y1) * 0.005f, iaAxis::X);
            cameraHeading->rotate((x1 - x2) * 0.005f, iaAxis::Y);

            iMouse::getInstance().setCenter(true);
        }
    }
}

void ExampleCharacterController::onWindowClosed()
{
    iApplication::getInstance().stop();
}

void ExampleCharacterController::onWindowResized(int32 clientWidth, int32 clientHeight)
{
    _viewOrtho.setOrthogonal(0, clientWidth, clientHeight, 0);
}

void ExampleCharacterController::onKeyPressed(iKeyCode key)
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

void ExampleCharacterController::onRenderOrtho()
{
    iaMatrixd viewMatrix;
    iRenderer::getInstance().setViewMatrix(viewMatrix);

    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(modelMatrix);

    drawLogo();

    // draw frame rate in lower right corner
    _statisticsVisualizer.drawStatistics(&_window, _font, iaColor4f(0, 1, 0, 1));
}

void ExampleCharacterController::drawLogo()
{
    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    float32 width = _igorLogo->getWidth();
    float32 height = _igorLogo->getHeight();
    float32 x = _window.getClientWidth() - width;
    float32 y = _window.getClientHeight() - height;

    iRenderer::getInstance().drawTexture(x, y, width, height, _igorLogo);
}

void ExampleCharacterController::run()
{
    iApplication::getInstance().run();
}

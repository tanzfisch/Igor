#include "Particles.h"

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
#include <iMaterialResourceFactory.h>
#include <iStatistics.h>
#include <iNodeSwitch.h>
#include <iNodeLODSwitch.h>
#include <iNodeLODTrigger.h>
#include <iNodeParticleSystem.h>
#include <iNodeEmitter.h>
using namespace Igor;

#include <iaConsole.h>
#include <iaString.h>
using namespace IgorAux;

Particles::Particles()
{
    init();
}

Particles::~Particles()
{
    deinit();
}

void Particles::init()
{
    con(" -- 3D Example --" << endl);

    // setup window
    _window.setTitle("Igor - 3D Example");
    _window.setSize(1280, 758);
    _window.setCentered();
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &Particles::onWindowClosed));
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &Particles::onWindowResized));

    // setup perspective view
    _view.setClearColor(iaColor4f(0.5f, 0, 0.5f, 1));
    _view.setPerspective(45);
    _view.setClipPlanes(0.1f, 10000.f);
    _window.addView(&_view);

    // setup orthogonal view
    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &Particles::onRenderOrtho));
    _window.addView(&_viewOrtho);

    _window.open();

    // init scene
    _scene = iSceneFactory::getInstance().createScene();
    // bind scene to perspective view
    _view.setScene(_scene);

    // setup camera
    // we want a camera which can be rotated arround the origin
    // we will acchive that with 3 transform nodes
    // one is for the heading
    iNodeTransform* cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    // give the transform node a name. naming is optional and ist jus for helping to debug. 
    // Names do not have to be unique but since it is possible to find nodes by name they better are
    cameraHeading->setName("camera heading");
    _cameraHeading = cameraHeading->getID();
    // one is for the pitch
    iNodeTransform* cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    cameraPitch->setName("camera pitch");
    _cameraPitch = cameraPitch->getID();
    // and one is for translation or distance from the origin
    iNodeTransform* cameraTranslation = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    cameraTranslation->setName("camera translation");
    // translate away from origin
    cameraTranslation->translate(0, 0, 100);
    _cameraTranslation = cameraTranslation->getID();
    // from all nodes that we want to control later we save the node ID
    // and last but not least we create a camera node
    iNodeCamera* camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
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
    camera->makeCurrent();

    _particlesMaterial = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_particlesMaterial)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_particlesMaterial)->getRenderStateSet().setRenderState(iRenderState::CullFace, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_particlesMaterial)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_particlesMaterial)->getRenderStateSet().setRenderState(iRenderState::Texture2D1, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_particlesMaterial)->getRenderStateSet().setRenderState(iRenderState::Texture2D2, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_particlesMaterial)->getRenderStateSet().setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_particlesMaterial)->getRenderStateSet().setRenderState(iRenderState::BlendFuncSource, iRenderStateValue::SourceAlpha);
    iMaterialResourceFactory::getInstance().getMaterial(_particlesMaterial)->getRenderStateSet().setRenderState(iRenderState::BlendFuncDestination, iRenderStateValue::OneMinusSourceAlpha);

    iRainbow rainbow;
    rainbow.insertColor(iaColor4f(0, 0, 1, 0), 0.0);
    rainbow.insertColor(iaColor4f(0, 0, 1, 1), 0.1);
    rainbow.insertColor(iaColor4f(0, 0, 1, 1), 0.3);
    rainbow.insertColor(iaColor4f(0, 1, 1, 1), 0.9);
    rainbow.insertColor(iaColor4f(0, 1, 1, 0), 1.0);

    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
    particleSystem->setMaterial(_particlesMaterial);
    particleSystem->setTextureA("simpleParticle.png");
    particleSystem->setTextureB("octave1.png");
    particleSystem->setTextureC("octave2.png");
    particleSystem->setParticleCount(1000);
    particleSystem->setVortexTorque(0.1, 0.2);
    particleSystem->setSize(3,6);
    particleSystem->setLift(0,0);
    particleSystem->setLiftDecrease(0);
    particleSystem->setWeight(0,0);
    particleSystem->setVortexParticleCount(0);
    particleSystem->setColor(iaColor4f(1,0,0,1));
    particleSystem->setRainbow(rainbow);

    iNodeTransform* particleTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    particleTransform->translate(0,0,0);

    particleTransform->insertNode(particleSystem);
    _scene->getRoot()->insertNode(particleTransform);

    iNodeEmitter* emitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
    emitter->setType(iEmitterType::Square);
    emitter->setSize(10);
    emitter->setVelocity(0.3);
    iEmitterTriangle triangle;
    triangle.pos[0] = iaVector3f(0, 0, 0);
    triangle.pos[1] = iaVector3f(1, 0, 0);
    triangle.pos[2] = iaVector3f(0, 0, 1);
    emitter->addTriangleEmitter(triangle);
    triangle.pos[0] = iaVector3f(1, 0, 0);
    triangle.pos[1] = iaVector3f(1, 1, 0);
    triangle.pos[2] = iaVector3f(0, 0, 1);
    emitter->addTriangleEmitter(triangle);
    iNodeTransform* emitterTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    emitterTransform->translate(0,0,0);

    emitterTransform->insertNode(emitter);
    _scene->getRoot()->insertNode(emitterTransform);

    particleSystem->setEmitter(emitter->getID());

    // create a skybox
    iNodeSkyBox* skyBoxNode = static_cast<iNodeSkyBox*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSkyBox));
    // set it up with the default skybox texture
    skyBoxNode->setTextures(
        "skybox_default/front.png",
        "skybox_default/back.png",
        "skybox_default/left.png",
        "skybox_default/right.png",
        "skybox_default/top.png",
        "skybox_default/bottom.png");
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

    // init render statistics
    _font = new iTextureFont("StandardFont.png");
    iStatistics::getInstance().setVerbosity(iRenderStatisticsVerbosity::FPSAndMetrics);

    // animation
    _animationTimingHandle = new iTimerHandle();
    _animationTimingHandle->setIntervall(10);
    _animationTimingHandle->registerTimerDelegate(TimerDelegate(this, &Particles::onTimer));

    _taskFlushTexturesID = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

    // register some callbacks
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &Particles::onKeyPressed));
    iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &Particles::onMouseMoved));
    iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &Particles::onMouseWheel));
}

void Particles::deinit()
{
    // unregister some callbacks
    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &Particles::onKeyPressed));
    iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &Particles::onMouseMoved));
    iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &Particles::onMouseWheel));
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &Particles::onWindowClosed));
    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &Particles::onWindowResized));
    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &Particles::onRenderOrtho));

    // deinit statistics
    if (_font != nullptr)
    {
        delete _font;
        _font = nullptr;
    }

    // stop light animation
    if (_animationTimingHandle)
    {
        _animationTimingHandle->unregisterTimerDelegate(TimerDelegate(this, &Particles::onTimer));
        delete _animationTimingHandle;
        _animationTimingHandle = nullptr;
    }

    iTaskManager::getInstance().getTask(_taskFlushTexturesID)->abort();

    iSceneFactory::getInstance().destroyScene(_scene);
    _scene = nullptr;

    iMaterialResourceFactory::getInstance().destroyMaterial(_materialSkyBox);

    if (_window.isOpen())
    {
        _window.close();
        _window.removeView(&_view);
        _window.removeView(&_viewOrtho);
    }

    if (_font != nullptr)
    {
        delete _font;
        _font = nullptr;
    }
}

void Particles::onMouseWheel(int32 d)
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

void Particles::onMouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* _window)
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

void Particles::onWindowClosed()
{
    iApplication::getInstance().stop();
}

void Particles::onWindowResized(int32 clientWidth, int32 clientHeight)
{
    _viewOrtho.setOrthogonal(0, clientWidth, clientHeight, 0);
}

void Particles::onKeyPressed(iKeyCode key)
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

void Particles::onTimer()
{
}

void Particles::onRenderOrtho()
{
    if (_font != nullptr)
    {
        iStatistics::getInstance().drawStatistics(&_window, _font, iaColor4f(1, 1, 1, 1));
    }
}

void Particles::run()
{
    iApplication::getInstance().run();
}
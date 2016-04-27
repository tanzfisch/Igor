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
#include <iGradient.h>
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
    _window.setTitle("Igor - Particles");
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
    cameraHeading->rotate(0.3, iaAxis::Y);
    cameraHeading->rotate(-0.3, iaAxis::X);
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

    iGradientColor4f redGradient;
    redGradient.insertValue(0.0, iaColor4f(1, 0, 0, 0));
    redGradient.insertValue(0.1, iaColor4f(1, 0, 0, 1));
    redGradient.insertValue(0.9, iaColor4f(1, 0, 0, 1));
    redGradient.insertValue(1.0, iaColor4f(1, 0, 0, 0));

    iGradientColor4f greenGradient;
    greenGradient.insertValue(0.0, iaColor4f(0, 1, 0, 0));
    greenGradient.insertValue(0.1, iaColor4f(0, 1, 0, 1));
    greenGradient.insertValue(0.9, iaColor4f(0, 1, 0, 1));
    greenGradient.insertValue(1.0, iaColor4f(0, 1, 0, 0));

    iGradientColor4f blueGradient;
    blueGradient.insertValue(0.0, iaColor4f(0, 0, 1, 0));
    blueGradient.insertValue(0.1, iaColor4f(0, 0, 1, 1));
    blueGradient.insertValue(0.9, iaColor4f(0, 0, 1, 1));
    blueGradient.insertValue(1.0, iaColor4f(0, 0, 1, 0));

    iGradientColor4f whiteGradient;
    whiteGradient.insertValue(0.0, iaColor4f(1, 1, 1, 0));
    whiteGradient.insertValue(0.1, iaColor4f(1, 1, 1, 1));
    whiteGradient.insertValue(0.9, iaColor4f(1, 1, 1, 1));
    whiteGradient.insertValue(1.0, iaColor4f(1, 1, 1, 0));

    iNodeParticleSystem* waveParticleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
    waveParticleSystem->setMaterial(_particlesMaterial);
    waveParticleSystem->setTextureA("simpleParticle.png");
    waveParticleSystem->setTextureB("octave1.png");
    waveParticleSystem->setTextureC("octave2.png");
    waveParticleSystem->setParticleCount(1000);
    waveParticleSystem->setSize(4, 7);
    waveParticleSystem->setRainbow(greenGradient);
    _scene->getRoot()->insertNode(waveParticleSystem);

    iNodeEmitter* waveEmitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
    waveParticleSystem->setEmitter(waveEmitter->getID());
    waveEmitter->setType(iEmitterType::Mesh);

    iEmitterTriangle triangle;
    for (int i = 0; i < 60; i++)
    {
        float32 h1 = sin(i * 0.3) * 5.0;
        float32 h2 = sin((i + 1) * 0.3) * 5.0;

        triangle.pos[0] = iaVector3f(0 + i, h1, 0);
        triangle.pos[1] = iaVector3f(1 + i, h2, 0);
        triangle.pos[2] = iaVector3f(0 + i, h1, 30);
        triangle.vel[0] = iaVector3f(0, -1, 0);
        triangle.vel[1] = iaVector3f(0, -1, 0);
        triangle.vel[2] = iaVector3f(0, -1, 0);
        waveEmitter->addTriangle(triangle);

        triangle.pos[0] = iaVector3f(1 + i, h2, 0);
        triangle.pos[1] = iaVector3f(1 + i, h2, 30);
        triangle.pos[2] = iaVector3f(0 + i, h1, 30);
        triangle.vel[0] = iaVector3f(0, -1, 0);
        triangle.vel[1] = iaVector3f(0, -1, 0);
        triangle.vel[2] = iaVector3f(0, -1, 0);
        waveEmitter->addTriangle(triangle);
    }

    iNodeTransform* waveEmitterTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    waveEmitterTransform->translate(-50, 5, -15);
    waveEmitterTransform->insertNode(waveEmitter);
    _scene->getRoot()->insertNode(waveEmitterTransform);

    iNodeParticleSystem* dotParticleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
    dotParticleSystem->setMaterial(_particlesMaterial);
    dotParticleSystem->setTextureA("simpleParticle.png");
    dotParticleSystem->setParticleCount(50);
    dotParticleSystem->setSize(4, 7);
    dotParticleSystem->setRainbow(redGradient);
    _scene->getRoot()->insertNode(dotParticleSystem);

    iNodeEmitter* dotEmitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
    dotParticleSystem->setEmitter(dotEmitter->getID());
    dotEmitter->setType(iEmitterType::Point);

    iNodeTransform* dotEmitterTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    dotEmitterTransform->translate(40, 0, 0);
    dotEmitterTransform->insertNode(dotEmitter);
    _scene->getRoot()->insertNode(dotEmitterTransform);

    iNodeParticleSystem* circleParticleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
    circleParticleSystem->setMaterial(_particlesMaterial);
    circleParticleSystem->setTextureA("simpleParticle.png");
    circleParticleSystem->setParticleCount(200);
    circleParticleSystem->setParticleLifeTime(10);
    circleParticleSystem->setSize(4, 7);
    circleParticleSystem->setRainbow(blueGradient);
    _scene->getRoot()->insertNode(circleParticleSystem);

    iNodeEmitter* circleEmitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
    _circleEmitterID = circleEmitter->getID();
    circleParticleSystem->setEmitter(circleEmitter->getID());
    circleEmitter->setType(iEmitterType::Circle);
    circleEmitter->setSize(10);

    iNodeTransform* circleEmitterTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _circlEmitterTransformID = circleEmitterTransform->getID();
    circleEmitterTransform->translate(-30, 0, 40);
    circleEmitterTransform->rotate(-0.5 * M_PI, iaAxis::Z);
    circleEmitterTransform->insertNode(circleEmitter);
    _scene->getRoot()->insertNode(circleEmitterTransform);

    iNodeParticleSystem* smokeParticleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
    smokeParticleSystem->setMaterial(_particlesMaterial);
    smokeParticleSystem->setTextureA("simpleParticle.png");
    smokeParticleSystem->setTextureB("octave1.png");
    smokeParticleSystem->setTextureC("octave2.png");
    smokeParticleSystem->setParticleCount(500);
    smokeParticleSystem->setVortexParticleCount(30);
    smokeParticleSystem->setParticleLifeTime(300);
    smokeParticleSystem->setLift(0.001, 0.002);
    smokeParticleSystem->setSize(3, 6);
    smokeParticleSystem->setSizeIncrease(0.01);
    smokeParticleSystem->setRainbow(whiteGradient);
    _scene->getRoot()->insertNode(smokeParticleSystem);

    iNodeEmitter* smokeEmitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
    smokeParticleSystem->setEmitter(smokeEmitter->getID());
    smokeEmitter->setType(iEmitterType::Disc);
    smokeEmitter->setSize(7);

    iNodeTransform* smokeEmitterTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    smokeEmitterTransform->translate(20, 0, 40);
    smokeEmitterTransform->insertNode(smokeEmitter);
    _scene->getRoot()->insertNode(smokeEmitterTransform);

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
    _animationTimingHandle->setIntervall(100);
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
    static int counter = 0;

    iNodeEmitter* emitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().getNode(_circleEmitterID));
    if (emitter != nullptr)
    {

    }

    iNodeTransform* transform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_circlEmitterTransformID));
    transform->rotate(0.1, iaAxis::Z);
    transform->rotate(0.2, iaAxis::X);
    transform->rotate(0.3, iaAxis::Y);

    counter++;
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
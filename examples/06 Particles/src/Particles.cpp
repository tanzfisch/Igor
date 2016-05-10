#include "Particles.h"

#include <iMaterial.h>
#include <iMaterialGroup.h>
#include <iNodeVisitorPrintTree.h>
#include <iTaskManager.h>
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
    _view.setClearColor(iaColor4f(0.0f, 0.0f, 0.0f, 1));
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
    cameraHeading->rotate(0.1, iaAxis::Y);
    cameraHeading->rotate(-0.5, iaAxis::X);
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

    createDotParticleSystem();
    createSmokeParticleSystem();
    createRingParticleSystem();
	createWaveParticleSystem();
    creasteFontainParticleSystem();

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

void Particles::createWaveParticleSystem()
{
    iGradientColor4f colorGradient;
    colorGradient.insertValue(0.0, iaColor4f(0, 0, 0, 0));
    colorGradient.insertValue(0.5, iaColor4f(1, 1, 0, 0.5));
    colorGradient.insertValue(1.0, iaColor4f(0, 0, 0, 0));

    iGradientVector2f dotVelocity;
    dotVelocity.insertValue(0.0, iaVector2f(0.2, 0.4));

    iGradientVector2f dotVisibility;
    dotVisibility.insertValue(0.0, iaVector2f(0.7, 1.0));

    iGradientVector2f size;
    size.insertValue(0.0, iaVector2f(2.0, 3.0));

    iGradientui emission;
    emission.insertValue(0.0, 5);

    iGradientVector2f startOrientation;
    startOrientation.insertValue(0.0, iaVector2f(0.25 * M_PI, 0.25 * M_PI));

    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
	_particleSystemIDs.push_back(particleSystem->getID());
	particleSystem->setMaterial(_particlesMaterial);
	particleSystem->setTextureA("particleStar.png");
	particleSystem->setColorGradient(colorGradient);
	particleSystem->setStartVelocityGradient(dotVelocity);
    particleSystem->setStartOrientationGradient(startOrientation);
	particleSystem->setStartVisibleTimeGradient(dotVisibility);
	particleSystem->setStartSizeGradient(size);
	particleSystem->setEmissionGradient(emission);
    _scene->getRoot()->insertNode(particleSystem);
	particleSystem->start();

    iNodeEmitter* waveEmitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
	particleSystem->setEmitter(waveEmitter->getID());
    waveEmitter->setType(iEmitterType::Mesh);

    iEmitterTriangle triangle;
    for (int i = 0; i < 65; i++)
    {
        float32 h1 = cos(i * 0.2) * 6.0;
        float32 h2 = cos((i + 1) * 0.2) * 6.0;

        triangle.pos[0] = iaVector3f(0 + i, h1, 0);
        triangle.pos[1] = iaVector3f(1 + i, h2, 0);
        triangle.pos[2] = iaVector3f(0 + i, h1, 10);
        triangle.vel[0] = iaVector3f(0, 1, 0);
        triangle.vel[1] = iaVector3f(0, 1, 0);
        triangle.vel[2] = iaVector3f(0, 1, 0);
        waveEmitter->addTriangle(triangle);

        triangle.pos[0] = iaVector3f(1 + i, h2, 0);
        triangle.pos[1] = iaVector3f(1 + i, h2, 10);
        triangle.pos[2] = iaVector3f(0 + i, h1, 10);
        triangle.vel[0] = iaVector3f(0, 1, 0);
        triangle.vel[1] = iaVector3f(0, 1, 0);
        triangle.vel[2] = iaVector3f(0, 1, 0);
        waveEmitter->addTriangle(triangle);
    }

    iNodeTransform* waveEmitterTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _waveEmitterTransformID = waveEmitterTransform->getID();
    waveEmitterTransform->translate(-60, 15, -15);
    waveEmitterTransform->insertNode(waveEmitter);
    _scene->getRoot()->insertNode(waveEmitterTransform);
}

void Particles::creasteFontainParticleSystem()
{
    iGradientColor4f colors;
    colors.insertValue(0.0, iaColor4f(0.0, 0.8, 1.0, 1));
    colors.insertValue(0.5, iaColor4f(0.0, 0.2, 1.0, 1));
    colors.insertValue(1.0, iaColor4f(0.0, 0.0, 1.0, 0));

    iGradientVector2f visibility;
    visibility.insertValue(0.0, iaVector2f(4.5, 5.0));
    visibility.insertValue(1.0, iaVector2f(1.5, 2.0));
    visibility.insertValue(2.0, iaVector2f(4.5, 5.0));

    iGradientui emission;
    emission.insertValue(0.0, 3);

    iGradientVector2f velocity;
    velocity.insertValue(0.0, iaVector2f(0.4, 0.5));
    velocity.insertValue(1.0, iaVector2f(0.6, 0.6));
    velocity.insertValue(2.0, iaVector2f(0.4, 0.5));

    // using negative lift here so it's basically weight
    iGradientVector2f lift;
    lift.insertValue(0.0, iaVector2f(-0.002, -0.002));

    iGradientVector2f startSize;
    startSize.insertValue(0.0, iaVector2f(0.5, 0.8));

    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
	_particleSystemIDs.push_back(particleSystem->getID());
    particleSystem->setMaterial(_particlesMaterial);
    particleSystem->setTextureA("particleTrail.png");
    particleSystem->setColorGradient(colors);
    particleSystem->setEmissionGradient(emission);
    particleSystem->setStartVisibleTimeGradient(visibility);
    particleSystem->setVelocityOriented();
    particleSystem->setStartSizeGradient(startSize);
    particleSystem->setStartVelocityGradient(velocity);
    particleSystem->setStartLiftGradient(lift);
    particleSystem->setAirDrag(0.985);
    particleSystem->setPeriodTime(2.0);
    _scene->getRoot()->insertNode(particleSystem);
    particleSystem->start();

    iNodeEmitter* emitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
    particleSystem->setEmitter(emitter->getID());
    emitter->setType(iEmitterType::Disc);
    emitter->setSize(1);

    iNodeTransform* transform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transform->translate(-30, 12, 40);
    transform->rotate(-0.25 * M_PI, iaAxis::Z);
    transform->insertNode(emitter);
    _scene->getRoot()->insertNode(transform);
}

void Particles::createRingParticleSystem()
{
    iGradientColor4f ringColors;
    ringColors.insertValue(0.0, iaColor4f(0, 1, 1, 1));
    ringColors.insertValue(0.33, iaColor4f(1, 0, 1, 1));
    ringColors.insertValue(0.66, iaColor4f(0, 0, 1, 1));
    ringColors.insertValue(1.0, iaColor4f(0, 0, 0, 0));

    iGradientVector2f visibility;
    visibility.insertValue(0.0, iaVector2f(3.5, 4.0));

    iGradientui emission;
    emission.insertValue(0.0, 10);

    iGradientVector2f startSize;
    startSize.insertValue(0.0, iaVector2f(2.0, 2.5));
    startSize.insertValue(1.0, iaVector2f(0.1, 0.5));
    startSize.insertValue(2.0, iaVector2f(2.0, 2.5));
    startSize.insertValue(3.0, iaVector2f(0.1, 0.5));
    startSize.insertValue(4.0, iaVector2f(2.0, 2.5));

    iGradientVector2f startOrientationRate;
    startOrientationRate.insertValue(0.0, iaVector2f(-0.1, 0.1));

    iNodeParticleSystem* circleParticleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
	_particleSystemIDs.push_back(circleParticleSystem->getID());
    circleParticleSystem->setMaterial(_particlesMaterial);
    circleParticleSystem->setTextureA("particleGem.png");
    circleParticleSystem->setTextureB("octave1.png");
    circleParticleSystem->setColorGradient(ringColors);
    circleParticleSystem->setEmissionGradient(emission);
    circleParticleSystem->setStartVisibleTimeGradient(visibility);
    circleParticleSystem->setStartSizeGradient(startSize);
    circleParticleSystem->setStartOrientationRateGradient(startOrientationRate);
    circleParticleSystem->setPeriodTime(4.0);
    _scene->getRoot()->insertNode(circleParticleSystem);
    circleParticleSystem->start();

    iNodeEmitter* circleEmitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
    circleParticleSystem->setEmitter(circleEmitter->getID());
    circleEmitter->setType(iEmitterType::Circle);
    circleEmitter->setSize(10);

    iNodeTransform* circleEmitterTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _circleEmitterTransformID = circleEmitterTransform->getID();
    circleEmitterTransform->translate(-30, 0, 40);
    circleEmitterTransform->rotate(-0.5 * M_PI, iaAxis::Z);
    circleEmitterTransform->insertNode(circleEmitter);
    _scene->getRoot()->insertNode(circleEmitterTransform);
}

void Particles::createSmokeParticleSystem()
{
    iGradientVector2f startOrientation;
    startOrientation.insertValue(0.0, iaVector2f(0.0, 2.0 * M_PI));

    iGradientVector2f startOrientationRate;
    startOrientationRate.insertValue(0.0, iaVector2f(-0.05, 0.05));

    iGradientColor4f smokeGradient;
    smokeGradient.insertValue(0.0, iaColor4f(1, 1, 1, 0));
    smokeGradient.insertValue(0.2, iaColor4f(1, 1, 1, 0.6));
    smokeGradient.insertValue(0.5, iaColor4f(1, 1, 1, 0.5));
    smokeGradient.insertValue(1.0, iaColor4f(1, 1, 1, 0));

    iGradientVector2f smokeSize;
    smokeSize.insertValue(0.0, iaVector2f(3.0, 6.0));

    iGradientVector2f smokeVisibility;
    smokeVisibility.insertValue(0.0, iaVector2f(7.0, 10.0));

    iGradientui emission;
    emission.insertValue(0.0, 1);

    iGradientVector2f lift;
    lift.insertValue(0.0, iaVector2f(0.0007, 0.0015));

	iGradientf sizeScale;
	sizeScale.insertValue(0.0, 1.0);
	sizeScale.insertValue(10.0, 10.0);

    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
	_particleSystemIDs.push_back(particleSystem->getID());
	particleSystem->setMaterial(_particlesMaterial);
	particleSystem->setTextureA("particleDot.png");
	particleSystem->setTextureB("octave1.png");
	particleSystem->setTextureC("octave2.png");
	particleSystem->setVortexTorque(0.5, 0.7);
    particleSystem->setStartOrientationGradient(startOrientation);
    particleSystem->setStartOrientationRateGradient(startOrientationRate);
	particleSystem->setVorticityConfinement(0.05);
	particleSystem->setVortexRange(20.0, 40.0);
	particleSystem->setVortexParticleLikeliness(1);
	particleSystem->setStartSizeGradient(smokeSize);
	particleSystem->setSizeScaleGradient(sizeScale);
	particleSystem->setColorGradient(smokeGradient);
	particleSystem->setStartVisibleTimeGradient(smokeVisibility);
	particleSystem->setEmissionGradient(emission);
	particleSystem->setStartLiftGradient(lift);
    _scene->getRoot()->insertNode(particleSystem);
	particleSystem->start();

    iNodeEmitter* smokeEmitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
	particleSystem->setEmitter(smokeEmitter->getID());
    smokeEmitter->setType(iEmitterType::Sphere);
    smokeEmitter->setSize(6);

    iNodeTransform* smokeEmitterTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    smokeEmitterTransform->translate(30, 0, 30);
    smokeEmitterTransform->insertNode(smokeEmitter);
    _scene->getRoot()->insertNode(smokeEmitterTransform);
}

void Particles::createDotParticleSystem()
{
    iGradientui emission;
    emission.insertValue(0.0, 60);
    emission.insertValue(0.1, 0);

    iGradientColor4f colorGradient;
    colorGradient.insertValue(0.0, iaColor4f(0, 1, 1, 0));
    colorGradient.insertValue(0.5, iaColor4f(0, 0.8, 1.0, 1));
    colorGradient.insertValue(0.8, iaColor4f(0, 0.3, 0.8, 1));
    colorGradient.insertValue(1.0, iaColor4f(0, 0.3, 0.8, 0));

    iGradientVector2f velocity;
    velocity.insertValue(0.0, iaVector2f(0.05, 0.1));

    iGradientVector2f visibility;
    visibility.insertValue(0.0, iaVector2f(1.5, 2.0));

    iGradientVector2f size;
    size.insertValue(0.0, iaVector2f(1.0, 1.5));


    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
	_particleSystemIDs.push_back(particleSystem->getID());
    particleSystem->setMaterial(_particlesMaterial);
    particleSystem->setTextureA("particleTrail.png");
    particleSystem->setColorGradient(colorGradient);
    particleSystem->setStartVelocityGradient(velocity);
    particleSystem->setStartVisibleTimeGradient(visibility);
    particleSystem->setVelocityOriented();
    particleSystem->setStartSizeGradient(size);
    particleSystem->setEmissionGradient(emission);
    particleSystem->setPeriodTime(3.0);
    _scene->getRoot()->insertNode(particleSystem);
    particleSystem->start();

    iGradientui emission2;
    emission2.insertValue(0.0, 0);
    emission2.insertValue(0.6, 0);
    emission2.insertValue(0.7, 80);
    emission2.insertValue(0.8, 0);

    iGradientColor4f colorGradient2;
    colorGradient2.insertValue(0.0, iaColor4f(1, 1, 0, 0));
    colorGradient2.insertValue(0.1, iaColor4f(1, 1, 0, 1));
    colorGradient2.insertValue(0.4, iaColor4f(1, 0, 0, 1));
    colorGradient2.insertValue(1.0, iaColor4f(0, 0, 0, 0));

    iGradientVector2f velocity2;
    velocity2.insertValue(0.0, iaVector2f(0.1, 0.3));

    iGradientVector2f visibility2;
    visibility2.insertValue(0.0, iaVector2f(0.9, 1.4));

    iGradientVector2f size2;
    size2.insertValue(0.0, iaVector2f(1.0, 4.0));

    iGradientVector2f startOrientation;
    startOrientation.insertValue(0.0, iaVector2f(0.0, 2.0 * M_PI));

    iGradientVector2f startOrientationRate;
    startOrientationRate.insertValue(0.0, iaVector2f(-0.1, 0.1));

    iNodeParticleSystem* particleSystem2 = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
	_particleSystemIDs.push_back(particleSystem2->getID());
    particleSystem2->setMaterial(_particlesMaterial);
    particleSystem2->setTextureA("particleFlame.png");
    particleSystem2->setTextureB("octave1.png");
    particleSystem2->setTextureC("octave2.png");
    particleSystem2->setColorGradient(colorGradient2);
    particleSystem2->setStartVelocityGradient(velocity2);
    particleSystem2->setStartVisibleTimeGradient(visibility2);
    particleSystem2->setStartOrientationGradient(startOrientation);
    particleSystem2->setStartOrientationRateGradient(startOrientationRate);
    particleSystem2->setStartSizeGradient(size2);
    particleSystem2->setEmissionGradient(emission2);
    particleSystem2->setPeriodTime(3.0);
    _scene->getRoot()->insertNode(particleSystem2);
    particleSystem2->start();
    
    iNodeEmitter* dotEmitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
    particleSystem->setEmitter(dotEmitter->getID());
    particleSystem2->setEmitter(dotEmitter->getID());
    dotEmitter->setType(iEmitterType::Point);

    iNodeTransform* dotEmitterTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    dotEmitterTransform->translate(10, 0, 20);
    dotEmitterTransform->insertNode(dotEmitter);
    _scene->getRoot()->insertNode(dotEmitterTransform);
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
            camTranslation->translate(0, 0, 10);
        }
        else
        {
            camTranslation->translate(0, 0, -10);
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

    if (key == iKeyCode::Space)
    {
		for (auto particleSystemID : _particleSystemIDs)
		{
			iNodeParticleSystem* circleParticleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(particleSystemID));
			if (circleParticleSystem != nullptr)
			{
				if (circleParticleSystem->isRunning())
				{
					circleParticleSystem->stop();
				}
				else
				{
					circleParticleSystem->start();
				}
			}
		}
    }

    if (key == iKeyCode::R)
    {
		for (auto particleSystemID : _particleSystemIDs)
		{
			iNodeParticleSystem* circleParticleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(particleSystemID));
			if (circleParticleSystem != nullptr)
			{
				circleParticleSystem->reset();
			}
		}
    }
}

void Particles::onTimer()
{
    iNodeTransform* transform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_circleEmitterTransformID));
    if (transform != nullptr)
    {
        transform->rotate(0.1, iaAxis::Z);
        transform->rotate(0.04, iaAxis::X);
        transform->rotate(0.06, iaAxis::Y);
    }
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
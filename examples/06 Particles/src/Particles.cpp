#include "Particles.h"

#include <iMaterial.h>
#include <iNodeVisitorPrintTree.h>
#include <iTaskManager.h>
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
#include <iMaterialResourceFactory.h>
#include <iProfiler.h>
#include <iNodeSwitch.h>
#include <iNodeLODSwitch.h>
#include <iNodeLODTrigger.h>
#include <iNodeParticleSystem.h>
#include <iNodeEmitter.h>
#include <iaGradient.h>
#include <iTextureResourceFactory.h>
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
    _window.setClientSize(1024, 768);
    _window.setCentered();
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &Particles::onWindowClosed));
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &Particles::onWindowResized));

    // setup perspective view
    _view.setClearColor(iaColor4f(0.25f, 0.25f, 0.25f, 1));
    _view.setPerspective(45);
    _view.setClipPlanes(0.1f, 10000.f);
    _window.addView(&_view);

    // setup orthogonal view
    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setOrthogonal(0.0f, static_cast<float32>(_window.getClientWidth()), static_cast<float32>(_window.getClientHeight()), 0.0f);
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
    iNodeTransform* cameraHeading = static_cast<iNodeTransform*>(iNodeManager::getInstance().createNode(iNodeType::iNodeTransform));
    // give the transform node a name. naming is optional and ist jus for helping to debug. 
    // Names do not have to be unique but since it is possible to find nodes by name they better are
    cameraHeading->setName("camera heading");
    cameraHeading->rotate(0.1, iaAxis::Y);
    cameraHeading->rotate(-0.5, iaAxis::X);
    _cameraHeading = cameraHeading->getID();
    // one is for the pitch
    iNodeTransform* cameraPitch = static_cast<iNodeTransform*>(iNodeManager::getInstance().createNode(iNodeType::iNodeTransform));
    cameraPitch->setName("camera pitch");
    _cameraPitch = cameraPitch->getID();
    // and one is for translation or distance from the origin
    iNodeTransform* cameraTranslation = static_cast<iNodeTransform*>(iNodeManager::getInstance().createNode(iNodeType::iNodeTransform));
    cameraTranslation->setName("camera translation");
    // translate away from origin
    cameraTranslation->translate(0, 0, 120);
    _cameraTranslation = cameraTranslation->getID();
    // from all nodes that we want to control later we save the node ID
    // and last but not least we create a camera node
    iNodeCamera* camera = static_cast<iNodeCamera*>(iNodeManager::getInstance().createNode(iNodeType::iNodeCamera));
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

    // set up a meterial for the particles
    _particlesMaterial = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_particlesMaterial)->setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_particlesMaterial)->setRenderState(iRenderState::CullFace, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_particlesMaterial)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_particlesMaterial)->setRenderState(iRenderState::Texture2D1, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_particlesMaterial)->setRenderState(iRenderState::Texture2D2, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_particlesMaterial)->setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_particlesMaterial)->setRenderState(iRenderState::BlendFuncSource, iRenderStateValue::SourceAlpha);
    iMaterialResourceFactory::getInstance().getMaterial(_particlesMaterial)->setRenderState(iRenderState::BlendFuncDestination, iRenderStateValue::OneMinusSourceAlpha);

    // create the various particle systems
    createWaveParticleSystem();
    createDotParticleSystem();
    createSmokeParticleSystem();
    createRingParticleSystem();
    createFontainParticleSystem();
    createFireParticleSystem();

    // init render statistics
    _font = new iTextureFont("StandardFont.png");
    _profilerVisualizer.setVerbosity(iProfilerVerbosity::FPSAndMetrics);

    // animation
    _animationTimingHandle = new iTimerHandle(iTimerTickDelegate(this, &Particles::onTimer), 100);
	_animationTimingHandle->start();

    _taskFlushTexturesID = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

    // init igor logo
    _igorLogo = iTextureResourceFactory::getInstance().loadFile("special/splash.png", iResourceCacheMode::Free, iTextureBuildMode::Normal);
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Blend, iRenderStateValue::On);

    // register some callbacks
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &Particles::onKeyPressed));
    iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &Particles::onMouseMoved));
    iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &Particles::onMouseWheel));
}

void Particles::createWaveParticleSystem()
{
    // create the particle system
    iaGradientColor4f colorGradient;
    colorGradient.setValue(0.0, iaColor4f(0.0f, 0.0f, 0.0f, 0.0f));
    colorGradient.setValue(0.5, iaColor4f(1.0f, 1.0f, 0.0f, 1.0f));
    colorGradient.setValue(1.0, iaColor4f(0.0f, 0.0f, 0.0f, 0.0f));

    iaGradientVector2f velocity;
    velocity.setValue(0.0, iaVector2f(0.2f, 0.4f));

    iaGradientVector2f visibility;
    visibility.setValue(0.0, iaVector2f(0.7f, 1.0f));

    iaGradientVector2f size;
    size.setValue(0.0, iaVector2f(2.0f, 3.0f));

    iaGradientf emission;
    emission.setValue(0.0f, 5.0f);

    iaGradientVector2f startOrientation;
    startOrientation.setValue(0.0f, iaVector2f(0.0f, 2.0f * static_cast<float32>(M_PI)));

    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeManager::getInstance().createNode(iNodeType::iNodeParticleSystem));
    _particleSystemIDs.push_back(particleSystem->getID());
    particleSystem->setMaterial(_particlesMaterial);
    particleSystem->setTextureA("particleKreuzHerzPikKaro.png");
    particleSystem->setFirstTextureTiling(2, 2);
    particleSystem->setColorGradient(colorGradient);
    particleSystem->setStartVelocityGradient(velocity);
    particleSystem->setStartOrientationGradient(startOrientation);
    particleSystem->setStartVisibleTimeGradient(visibility);
    particleSystem->setStartSizeGradient(size);
    particleSystem->setEmissionGradient(emission);
    _scene->getRoot()->insertNode(particleSystem);
    particleSystem->start();

    // create an mesh emitter with the shape of a cos wave
    iNodeEmitter* emitter = static_cast<iNodeEmitter*>(iNodeManager::getInstance().createNode(iNodeType::iNodeEmitter));
    particleSystem->setEmitter(emitter->getID());
    emitter->setEmitterType(iEmitterType::Mesh);

    iEmitterTriangle triangle;
    for (int i = 0; i < 65; i++)
    {
        float32 h1 = cos(i * 0.2f) * 6.0f;
        float32 h2 = cos((i + 1) * 0.2f) * 6.0f;

        triangle.pos[0] = iaVector3d(0 + i, h1, 0);
        triangle.pos[1] = iaVector3d(1 + i, h2, 0);
        triangle.pos[2] = iaVector3d(0 + i, h1, 10);
        triangle.vel[0] = iaVector3d(0, 1, 0);
        triangle.vel[1] = iaVector3d(0, 1, 0);
        triangle.vel[2] = iaVector3d(0, 1, 0);
        emitter->addTriangle(triangle);

        triangle.pos[0] = iaVector3d(1 + i, h2, 0);
        triangle.pos[1] = iaVector3d(1 + i, h2, 10);
        triangle.pos[2] = iaVector3d(0 + i, h1, 10);
        triangle.vel[0] = iaVector3d(0, 1, 0);
        triangle.vel[1] = iaVector3d(0, 1, 0);
        triangle.vel[2] = iaVector3d(0, 1, 0);
        emitter->addTriangle(triangle);
    }

    // position the emitter in scene using a transform node
    iNodeTransform* emitterTransform = static_cast<iNodeTransform*>(iNodeManager::getInstance().createNode(iNodeType::iNodeTransform));
    _waveEmitterTransformID = emitterTransform->getID();
    emitterTransform->translate(-60, 15, -15);
    emitterTransform->insertNode(emitter);
    _scene->getRoot()->insertNode(emitterTransform);
}

void Particles::createFireParticleSystem()
{
    iaGradientColor4f colors;
    colors.setValue(0.0f, iaColor4f(1.0f, 1.0f, 1.0f, 0.0f));
    colors.setValue(0.2f, iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
    colors.setValue(0.4f, iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
    colors.setValue(1.0f, iaColor4f(0.0f, 0.0f, 0.0f, 0.0f));

    iaGradientVector2f visibility;
    visibility.setValue(0.0f, iaVector2f(0.2f, 0.5f));

    iaGradientf emission;
    emission.setValue(0.0f, 4.0f);

    iaGradientVector2f velocity;
    velocity.setValue(0.0f, iaVector2f(0.4f, 0.7f));

    iaGradientVector2f startSize;
    startSize.setValue(0.0f, iaVector2f(3.0f, 4.0f));

    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeManager::getInstance().createNode(iNodeType::iNodeParticleSystem));
    _particleSystemIDs.push_back(particleSystem->getID());
    particleSystem->setMaterial(_particlesMaterial);
    particleSystem->setTextureA("particleFire.png");
    particleSystem->setFirstTextureTiling(4, 4);
    particleSystem->setColorGradient(colors);
    particleSystem->setEmissionGradient(emission);
    particleSystem->setStartVisibleTimeGradient(visibility);
    particleSystem->setStartSizeGradient(startSize);
    particleSystem->setStartVelocityGradient(velocity);
    particleSystem->setPeriodTime(2.0f);
    particleSystem->setVortexTorque(0.2f, 0.5f);
    particleSystem->setVorticityConfinement(0.05f);
    particleSystem->setVortexRange(10.0f, 15.0f);
    particleSystem->setVortexToParticleRate(0.05f);
    _scene->getRoot()->insertNode(particleSystem);
    particleSystem->start();

    iNodeEmitter* emitter = static_cast<iNodeEmitter*>(iNodeManager::getInstance().createNode(iNodeType::iNodeEmitter));
    particleSystem->setEmitter(emitter->getID());
    emitter->setEmitterType(iEmitterType::Disc);
    emitter->setSize(3.0f);

    iNodeTransform* transform = static_cast<iNodeTransform*>(iNodeManager::getInstance().createNode(iNodeType::iNodeTransform));
    transform->translate(-2, -4, 40);
    transform->insertNode(emitter);
    _scene->getRoot()->insertNode(transform);
}

void Particles::createFontainParticleSystem()
{
    iaGradientColor4f colors;
    colors.setValue(0.0f, iaColor4f(0.0f, 0.8f, 1.0f, 1.0f));
    colors.setValue(0.5f, iaColor4f(0.0f, 0.2f, 1.0f, 1.0f));
    colors.setValue(1.0f, iaColor4f(0.0f, 0.0f, 1.0f, 0.0f));

    iaGradientVector2f visibility;
    visibility.setValue(0.0f, iaVector2f(4.5f, 5.0f));
    visibility.setValue(1.0f, iaVector2f(1.5f, 2.0f));
    visibility.setValue(2.0f, iaVector2f(4.5f, 5.0f));

    iaGradientf emission;
    emission.setValue(0.0, 3);

    iaGradientVector2f velocity;
    velocity.setValue(0.0f, iaVector2f(0.4f, 0.5f));
    velocity.setValue(1.0f, iaVector2f(0.6f, 0.6f));
    velocity.setValue(2.0f, iaVector2f(0.4f, 0.5f));

    // using negative lift here so it's basically weight
    iaGradientVector2f lift;
    lift.setValue(0.0f, iaVector2f(-0.002f, -0.002f));

    iaGradientVector2f startSize;
    startSize.setValue(0.0f, iaVector2f(0.5f, 0.8f));

    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeManager::getInstance().createNode(iNodeType::iNodeParticleSystem));
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
    particleSystem->setAirDrag(0.985f);
    particleSystem->setPeriodTime(2.0f);
    _scene->getRoot()->insertNode(particleSystem);
    particleSystem->start();

    iNodeEmitter* emitter = static_cast<iNodeEmitter*>(iNodeManager::getInstance().createNode(iNodeType::iNodeEmitter));
    particleSystem->setEmitter(emitter->getID());
    emitter->setEmitterType(iEmitterType::Disc);
    emitter->setSize(1);

    iNodeTransform* transform = static_cast<iNodeTransform*>(iNodeManager::getInstance().createNode(iNodeType::iNodeTransform));
    transform->translate(-30, 12, 40);
    transform->rotate(-0.25 * M_PI, iaAxis::Z);
    transform->insertNode(emitter);
    _scene->getRoot()->insertNode(transform);
}

void Particles::createRingParticleSystem()
{
    iaGradientColor4f ringColors;
    ringColors.setValue(0.0f, iaColor4f(0.0f, 1.0f, 1.0f, 1.0f));
    ringColors.setValue(0.33f, iaColor4f(1.0f, 0.0f, 1.0f, 1.0f));
    ringColors.setValue(0.66f, iaColor4f(0.0f, 0.0f, 1.0f, 1.0f));
    ringColors.setValue(1.0f, iaColor4f(0.0f, 0.0f, 0.0f, 0.0f));

    iaGradientVector2f visibility;
    visibility.setValue(0.0f, iaVector2f(3.5f, 4.0f));

    iaGradientf emission;
    emission.setValue(0.0f, 10.0f);

    iaGradientVector2f startSize;
    startSize.setValue(0.0f, iaVector2f(2.0f, 2.5f));
    startSize.setValue(1.0f, iaVector2f(0.1f, 0.5f));
    startSize.setValue(2.0f, iaVector2f(2.0f, 2.5f));
    startSize.setValue(3.0f, iaVector2f(0.1f, 0.5f));
    startSize.setValue(4.0f, iaVector2f(2.0f, 2.5f));

    iaGradientVector2f startOrientationRate;
    startOrientationRate.setValue(0.0f, iaVector2f(-0.1f, 0.1f));

    iNodeParticleSystem* circleParticleSystem = static_cast<iNodeParticleSystem*>(iNodeManager::getInstance().createNode(iNodeType::iNodeParticleSystem));
    _particleSystemIDs.push_back(circleParticleSystem->getID());
    circleParticleSystem->setMaterial(_particlesMaterial);
    circleParticleSystem->setTextureA("particleGem.png");
    circleParticleSystem->setTextureB("octave1.png");
    circleParticleSystem->setColorGradient(ringColors);
    circleParticleSystem->setEmissionGradient(emission);
    circleParticleSystem->setStartVisibleTimeGradient(visibility);
    circleParticleSystem->setStartSizeGradient(startSize);
    circleParticleSystem->setStartOrientationRateGradient(startOrientationRate);
    circleParticleSystem->setPeriodTime(4.0f);
    _scene->getRoot()->insertNode(circleParticleSystem);
    circleParticleSystem->start();

    iNodeEmitter* circleEmitter = static_cast<iNodeEmitter*>(iNodeManager::getInstance().createNode(iNodeType::iNodeEmitter));
    circleParticleSystem->setEmitter(circleEmitter->getID());
    circleEmitter->setEmitterType(iEmitterType::Circle);
    circleEmitter->setSize(10.0f);

    iNodeTransform* circleEmitterTransform = static_cast<iNodeTransform*>(iNodeManager::getInstance().createNode(iNodeType::iNodeTransform));
    _circleEmitterTransformID = circleEmitterTransform->getID();
    circleEmitterTransform->translate(-30, 0, 40);
    circleEmitterTransform->rotate(-0.5 * M_PI, iaAxis::Z);
    circleEmitterTransform->insertNode(circleEmitter);
    _scene->getRoot()->insertNode(circleEmitterTransform);
}

void Particles::createSmokeParticleSystem()
{
    iaGradientVector2f startOrientation;
    startOrientation.setValue(0.0f, iaVector2f(0.0f, 2.0f * static_cast<float32>(M_PI)));

    iaGradientVector2f startOrientationRate;
    startOrientationRate.setValue(0.0f, iaVector2f(-0.05f, 0.05f));

    iaGradientColor4f smokeGradient;
    smokeGradient.setValue(0.0f, iaColor4f(1.0f, 1.0f, 1.0f, 0.0f));
    smokeGradient.setValue(0.2f, iaColor4f(1.0f, 1.0f, 1.0f, 0.6f));
    smokeGradient.setValue(0.5f, iaColor4f(1.0f, 1.0f, 1.0f, 0.5f));
    smokeGradient.setValue(1.0f, iaColor4f(1.0f, 1.0f, 1.0f, 0.0f));

    iaGradientVector2f smokeSize;
    smokeSize.setValue(0.0f, iaVector2f(3.0f, 6.0f));

    iaGradientVector2f smokeVisibility;
    smokeVisibility.setValue(0.0f, iaVector2f(7.0f, 10.0f));

    iaGradientf emission;
    emission.setValue(0.0f, 1.0f);

    iaGradientVector2f lift;
    lift.setValue(0.0f, iaVector2f(0.0007f, 0.0015f));

    iaGradientf sizeScale;
    sizeScale.setValue(0.0f, 1.0f);
    sizeScale.setValue(10.0f, 10.0f);

    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeManager::getInstance().createNode(iNodeType::iNodeParticleSystem));
    _particleSystemIDs.push_back(particleSystem->getID());
    particleSystem->setMaterial(_particlesMaterial);
    particleSystem->setTextureA("particleDot.png");
    particleSystem->setTextureB("octave1.png");
    particleSystem->setTextureC("octave2.png");
    particleSystem->setVortexTorque(0.5f, 0.7f);
    particleSystem->setStartOrientationGradient(startOrientation);
    particleSystem->setStartOrientationRateGradient(startOrientationRate);
    particleSystem->setVorticityConfinement(0.05f);
    particleSystem->setVortexRange(20.0f, 40.0f);
    particleSystem->setVortexToParticleRate(0.1f);
    particleSystem->setStartSizeGradient(smokeSize);
    particleSystem->setSizeScaleGradient(sizeScale);
    particleSystem->setColorGradient(smokeGradient);
    particleSystem->setStartVisibleTimeGradient(smokeVisibility);
    particleSystem->setEmissionGradient(emission);
    particleSystem->setStartLiftGradient(lift);
    _scene->getRoot()->insertNode(particleSystem);
    particleSystem->start();

    iNodeEmitter* smokeEmitter = static_cast<iNodeEmitter*>(iNodeManager::getInstance().createNode(iNodeType::iNodeEmitter));
    particleSystem->setEmitter(smokeEmitter->getID());
    smokeEmitter->setEmitterType(iEmitterType::Sphere);
    smokeEmitter->setSize(6.0f);

    iNodeTransform* smokeEmitterTransform = static_cast<iNodeTransform*>(iNodeManager::getInstance().createNode(iNodeType::iNodeTransform));
    smokeEmitterTransform->translate(35, 0, 30);
    smokeEmitterTransform->insertNode(smokeEmitter);
    _scene->getRoot()->insertNode(smokeEmitterTransform);
}

void Particles::createDotParticleSystem()
{
    iaGradientf emission;
    emission.setValue(0.0f, 60.0f);
    emission.setValue(0.2f, 0.0f);

    iaGradientColor4f colorGradient;
    colorGradient.setValue(0.0f, iaColor4f(1.0f, 1.0f, 1.0f, 0.0f));
    colorGradient.setValue(0.5f, iaColor4f(0.8f, 0.8f, 1.0f, 1.0f));
    colorGradient.setValue(0.8f, iaColor4f(0.3f, 0.3f, 0.5f, 1.0f));
    colorGradient.setValue(1.0f, iaColor4f(0.3f, 0.3f, 0.0f, 0.0f));

    iaGradientVector2f velocity;
    velocity.setValue(0.0f, iaVector2f(0.1f, 0.3f));

    iaGradientVector2f visibility;
    visibility.setValue(0.0f, iaVector2f(1.5f, 2.0f));

    iaGradientVector2f size;
    size.setValue(0.0f, iaVector2f(1.0f, 1.5f));

    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeManager::getInstance().createNode(iNodeType::iNodeParticleSystem));
    _particleSystemIDs.push_back(particleSystem->getID());
    particleSystem->setMaterial(_particlesMaterial);
    particleSystem->setTextureA("particleTrail.png");
    particleSystem->setColorGradient(colorGradient);
    particleSystem->setStartVelocityGradient(velocity);
    particleSystem->setStartVisibleTimeGradient(visibility);
    particleSystem->setVelocityOriented();
    particleSystem->setStartSizeGradient(size);
    particleSystem->setEmissionGradient(emission);
    particleSystem->setPeriodTime(3.0f);
    particleSystem->start();

    iaGradientf emission2;
    emission2.setValue(0.0f, 10.0f);
    emission2.setValue(0.5f, 0.0f);

    iaGradientColor4f colorGradient2;
    colorGradient2.setValue(0.0f, iaColor4f(1.0f, 1.0f, 1.0f, 0.0f));
    colorGradient2.setValue(0.1f, iaColor4f(1.0f, 1.0f, 1.0f, 0.5f));
    colorGradient2.setValue(0.4f, iaColor4f(1.0f, 1.0f, 1.0f, 0.25f));
    colorGradient2.setValue(1.0f, iaColor4f(1.0f, 1.0f, 1.0f, 0.0f));

    iaGradientVector2f velocity2;
    velocity2.setValue(0.0f, iaVector2f(0.05f, 0.3f));

    iaGradientVector2f visibility2;
    visibility2.setValue(0.0f, iaVector2f(0.5f, 1.0f));

    iaGradientVector2f size2;
    size2.setValue(0.0f, iaVector2f(1.0f, 2.5f));

    iaGradientf sizeScale2;
    sizeScale2.setValue(0.0f, 1.0f);
    sizeScale2.setValue(3.0f, 20.0f);

    iaGradientVector2f startOrientation;
    startOrientation.setValue(0.0f, iaVector2f(0.0f, 2.0f * static_cast<float32>(M_PI)));

    iaGradientVector2f startOrientationRate;
    startOrientationRate.setValue(0.0f, iaVector2f(-0.01f, 0.01f));

    iNodeParticleSystem* particleSystem2 = static_cast<iNodeParticleSystem*>(iNodeManager::getInstance().createNode(iNodeType::iNodeParticleSystem));
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
    particleSystem2->setSizeScaleGradient(sizeScale2);
    particleSystem2->setEmissionGradient(emission2);
    particleSystem2->setPeriodTime(3.0f);
    particleSystem2->start();

    iNodeEmitter* dotEmitter = static_cast<iNodeEmitter*>(iNodeManager::getInstance().createNode(iNodeType::iNodeEmitter));
    particleSystem->setEmitter(dotEmitter->getID());
    particleSystem2->setEmitter(dotEmitter->getID());
    dotEmitter->setEmitterType(iEmitterType::Point);

    iNodeTransform* dotEmitterTransform = static_cast<iNodeTransform*>(iNodeManager::getInstance().createNode(iNodeType::iNodeTransform));
    dotEmitterTransform->translate(20, 0, 20);

    _scene->getRoot()->insertNode(particleSystem);
    _scene->getRoot()->insertNode(particleSystem2);

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

    _igorLogo = nullptr;

    // stop light animation
    if (_animationTimingHandle)
    {
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
}

void Particles::onMouseWheel(int32 d)
{
    iNodeTransform* camTranslation = static_cast<iNodeTransform*>(iNodeManager::getInstance().getNode(_cameraTranslation));
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

void Particles::onMouseMoved(const iaVector2i& from, const iaVector2i& to, iWindow* _window)
{
    if (iMouse::getInstance().getLeftButton())
    {
        iNodeTransform* cameraPitch = static_cast<iNodeTransform*>(iNodeManager::getInstance().getNode(_cameraPitch));
        iNodeTransform* cameraHeading = static_cast<iNodeTransform*>(iNodeManager::getInstance().getNode(_cameraHeading));

        if (cameraPitch != nullptr &&
            cameraHeading != nullptr)
        {
            cameraPitch->rotate((to._y - from._y) * 0.005f, iaAxis::X);
            cameraHeading->rotate((from._x - to._x) * 0.005f, iaAxis::Y);

            iMouse::getInstance().setCenter();
        }
    }
}

void Particles::onWindowClosed()
{
    iApplication::getInstance().stop();
}

void Particles::onWindowResized(int32 clientWidth, int32 clientHeight)
{
    _viewOrtho.setOrthogonal(0.0f, static_cast<float32>(clientWidth), static_cast<float32>(clientHeight), 0);
}

void Particles::onKeyPressed(iKeyCode key)
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
        for (auto particleSystemID : _particleSystemIDs)
        {
            iNodeParticleSystem* circleParticleSystem = static_cast<iNodeParticleSystem*>(iNodeManager::getInstance().getNode(particleSystemID));
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
        break;

    case iKeyCode::R:
        for (auto particleSystemID : _particleSystemIDs)
        {
            iNodeParticleSystem* circleParticleSystem = static_cast<iNodeParticleSystem*>(iNodeManager::getInstance().getNode(particleSystemID));
            if (circleParticleSystem != nullptr)
            {
                circleParticleSystem->reset();
            }
        }
        break;
    }
}

void Particles::onTimer()
{
    iNodeTransform* transform = static_cast<iNodeTransform*>(iNodeManager::getInstance().getNode(_circleEmitterTransformID));
    if (transform != nullptr)
    {
        transform->rotate(0.1, iaAxis::Z);
        transform->rotate(0.04, iaAxis::X);
        transform->rotate(0.06, iaAxis::Y);
    }
}

void Particles::onRenderOrtho()
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

void Particles::drawLogo()
{
    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    float32 width = static_cast<float32>(_igorLogo->getWidth());
    float32 height = static_cast<float32>(_igorLogo->getHeight());
    float32 x = static_cast<float32>(_window.getClientWidth()) - width;
    float32 y = static_cast<float32>(_window.getClientHeight()) - height;

    iRenderer::getInstance().drawTexture(x, y, width, height, _igorLogo);
}

void Particles::run()
{
    iApplication::getInstance().run();
}
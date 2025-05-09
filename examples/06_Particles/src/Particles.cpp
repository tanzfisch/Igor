// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "Particles.h"

Particles::Particles(iWindowPtr window)
    : ExampleBase(window, "Particles", true, "example_texture_skybox")
{
}

void Particles::onInit()
{
    // setup camera
    // we want a camera which can be rotated around the origin
    // we will achieve that with 3 transform nodes
    // one is for the heading
    iNodeTransform *cameraHeading = iNodeManager::getInstance().createNode<iNodeTransform>();
    // give the transform node a name. naming is optional and ist jus for helping to debug.
    // Names do not have to be unique but since it is possible to find nodes by name they better are
    cameraHeading->setName("camera heading");
    cameraHeading->rotate(0.1, iaAxis::Y);
    cameraHeading->rotate(-0.5, iaAxis::X);
    _cameraHeading = cameraHeading->getID();
    // one is for the pitch
    iNodeTransform *cameraPitch = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraPitch->setName("camera pitch");
    _cameraPitch = cameraPitch->getID();
    // and one is for translation or distance from the origin
    iNodeTransform *cameraTranslation = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraTranslation->setName("camera translation");
    // translate away from origin
    cameraTranslation->translate(0, 0, 120);
    _cameraTranslation = cameraTranslation->getID();
    // from all nodes that we want to control later we save the node ID
    // and last but not least we create a camera node
    iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    camera->setName("camera");
    // and build everything together
    // first we add the heading to the root node
    getScene()->getRoot()->insertNode(cameraHeading);
    // than the pitch to the heading node
    cameraHeading->insertNode(cameraPitch);
    // then the translation to the pitch node
    cameraPitch->insertNode(cameraTranslation);
    // and than we add the camera to the translation
    cameraTranslation->insertNode(camera);
    // and finally we tell the view which camera shall be the current one. for this to work a camera must be part of a
    // scene assiciated with the view wich we achived by adding all those nodes on to an other starting with the root node
    getView().setCamera(camera->getID());

    // set up a material for the particles
    _shaderMaterial = iResourceManager::getInstance().loadResource<iShaderMaterial>("igor_shader_material_particles");

    // create the various particle systems
    createWaveParticleSystem();
    createDotParticleSystem();
    createSmokeParticleSystem();
    createRingParticleSystem();
    createFontainParticleSystem();
    createFireParticleSystem();

    // animation
    _animationTimingHandle = new iTimerHandle(iTimerTickDelegate(this, &Particles::onUpdate), iaTime::fromMilliseconds(100));
    _animationTimingHandle->start();
}

void Particles::createWaveParticleSystem()
{
    // create the particle system
    iaKeyFrameGraphColor4f colorGradient;
    colorGradient.setValue(1.0, iaColor4f(0.0f, 0.0f, 0.0f, 0.0f));
    colorGradient.setValue(0.5, iaColor4f(1.0f, 1.0f, 0.0f, 1.0f));
    colorGradient.setValue(0.0, iaColor4f(0.0f, 0.0f, 0.0f, 0.0f));

    iaKeyFrameGraphVector2f velocity;
    velocity.setValue(0.0, iaVector2f(0.2f, 0.4f));

    iaKeyFrameGraphVector2f maxAge;
    maxAge.setValue(0.0, iaVector2f(0.7f, 1.0f));

    iaKeyFrameGraphVector2f size;
    size.setValue(0.0, iaVector2f(2.0f, 3.0f));

    iaKeyFrameGraphf emission;
    emission.setValue(0.0f, 5.0f);

    iaKeyFrameGraphVector2f startOrientation;
    startOrientation.setValue(0.0f, iaVector2f(0.0f, 2.0f * static_cast<float32>(M_PI)));

    iNodeParticleSystem *particleSystem = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
    _particleSystemIDs.push_back(particleSystem->getID());

    iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                       {IGOR_RESOURCE_PARAM_GENERATE, true},
                       {IGOR_RESOURCE_PARAM_SHADER_MATERIAL, _shaderMaterial},
                       {IGOR_RESOURCE_PARAM_TEXTURE0, iResourceManager::getInstance().loadResource<iTexture>("example_texture_particleKreuzHerzPikKaro")}});
    particleSystem->setMaterial(iResourceManager::getInstance().loadResource<iMaterial>(param));

    particleSystem->setMaxParticleCount(300);
    particleSystem->setTextureTiling(2, 2);
    particleSystem->setColorGradient(colorGradient);
    particleSystem->setStartVelocityGradient(velocity);
    particleSystem->setStartOrientationGradient(startOrientation);
    particleSystem->setStartAgeGradient(maxAge);
    particleSystem->setStartSizeGradient(size);
    particleSystem->setEmissionGradient(emission);
    getScene()->getRoot()->insertNode(particleSystem);
    particleSystem->start();

    // create an mesh emitter with the shape of a cos wave
    iNodeEmitter *emitter = iNodeManager::getInstance().createNode<iNodeEmitter>();
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
    iNodeTransform *emitterTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    _waveEmitterTransformID = emitterTransform->getID();
    emitterTransform->translate(-60, 15, -15);
    emitterTransform->insertNode(emitter);
    getScene()->getRoot()->insertNode(emitterTransform);
}

void Particles::createFireParticleSystem()
{
    iaKeyFrameGraphColor4f colors;
    colors.setValue(1.0f, iaColor4f(1.0f, 1.0f, 1.0f, 0.0f));
    colors.setValue(0.4f, iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
    colors.setValue(0.2f, iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
    colors.setValue(0.0f, iaColor4f(0.0f, 0.0f, 0.0f, 0.0f));

    iaKeyFrameGraphVector2f maxAge;
    maxAge.setValue(0.0f, iaVector2f(0.2f, 0.5f));

    iaKeyFrameGraphf emission;
    emission.setValue(0.0f, 2.0f);

    iaKeyFrameGraphVector2f velocity;
    velocity.setValue(0.0f, iaVector2f(0.4f, 0.7f));

    iaKeyFrameGraphVector2f startSize;
    startSize.setValue(0.0f, iaVector2f(3.0f, 4.0f));

    iNodeParticleSystem *particleSystem = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
    _particleSystemIDs.push_back(particleSystem->getID());

    iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                       {IGOR_RESOURCE_PARAM_GENERATE, true},
                       {IGOR_RESOURCE_PARAM_SHADER_MATERIAL, _shaderMaterial},
                       {IGOR_RESOURCE_PARAM_TEXTURE0, iResourceManager::getInstance().loadResource<iTexture>("example_texture_particleFire")}});
    particleSystem->setMaterial(iResourceManager::getInstance().loadResource<iMaterial>(param));
    particleSystem->setTileIncrement(0.8);
    particleSystem->setMaxParticleCount(200);
    particleSystem->setTextureTiling(4, 4);
    particleSystem->setColorGradient(colors);
    particleSystem->setEmissionGradient(emission);
    particleSystem->setStartAgeGradient(maxAge);
    particleSystem->setStartSizeGradient(startSize);
    particleSystem->setStartVelocityGradient(velocity);
    particleSystem->setVortexTorque(0.2f, 0.5f);
    particleSystem->setVorticityConfinement(0.05f);
    particleSystem->setVortexRange(10.0f, 15.0f);
    particleSystem->setVortexToParticleRate(0.05f);
    getScene()->getRoot()->insertNode(particleSystem);
    particleSystem->start();

    iNodeEmitter *emitter = iNodeManager::getInstance().createNode<iNodeEmitter>();
    particleSystem->setEmitter(emitter->getID());
    emitter->setEmitterType(iEmitterType::Disc);
    emitter->setSize(3.0f);

    iNodeTransform *transform = iNodeManager::getInstance().createNode<iNodeTransform>();
    transform->translate(-2, -4, 40);
    transform->insertNode(emitter);
    getScene()->getRoot()->insertNode(transform);
}

void Particles::createFontainParticleSystem()
{
    iaKeyFrameGraphColor4f colors;
    colors.setValue(1.0f, iaColor4f(0.0f, 0.2f, 1.0f, 1.0f));
    colors.setValue(0.5f, iaColor4f(0.0f, 0.8f, 1.0f, 1.0f));
    colors.setValue(0.0f, iaColor4f(0.0f, 0.8f, 1.0f, 0.0f));

    iaKeyFrameGraphVector2f maxAge;
    maxAge.setValue(0.0f, iaVector2f(4.5f, 5.0f));
    maxAge.setValue(1.0f, iaVector2f(1.5f, 2.0f));
    maxAge.setValue(2.0f, iaVector2f(4.5f, 5.0f));

    iaKeyFrameGraphf emission;
    emission.setValue(0.0, 3);

    iaKeyFrameGraphVector2f velocity;
    velocity.setValue(0.0f, iaVector2f(0.4f, 0.5f));
    velocity.setValue(1.0f, iaVector2f(0.6f, 0.6f));
    velocity.setValue(2.0f, iaVector2f(0.4f, 0.5f));

    // using negative lift here so it's basically weight
    iaKeyFrameGraphVector2f lift;
    lift.setValue(0.0f, iaVector2f(-0.002f, -0.002f));

    iaKeyFrameGraphVector2f startSize;
    startSize.setValue(0.0f, iaVector2f(0.5f, 0.8f));

    iNodeParticleSystem *particleSystem = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
    _particleSystemIDs.push_back(particleSystem->getID());

    iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                       {IGOR_RESOURCE_PARAM_GENERATE, true},
                       {IGOR_RESOURCE_PARAM_SHADER_MATERIAL, _shaderMaterial},
                       {IGOR_RESOURCE_PARAM_TEXTURE0, iResourceManager::getInstance().loadResource<iTexture>("example_texture_particleTrail")}});
    particleSystem->setMaterial(iResourceManager::getInstance().loadResource<iMaterial>(param));
    particleSystem->setMaxParticleCount(800);
    particleSystem->setColorGradient(colors);
    particleSystem->setEmissionGradient(emission);
    particleSystem->setStartAgeGradient(maxAge);
    particleSystem->setVelocityOriented();
    particleSystem->setStartSizeGradient(startSize);
    particleSystem->setStartVelocityGradient(velocity);
    particleSystem->setStartLiftGradient(lift);
    particleSystem->setAirDrag(0.985f);
    getScene()->getRoot()->insertNode(particleSystem);
    particleSystem->start();

    iNodeEmitter *emitter = iNodeManager::getInstance().createNode<iNodeEmitter>();
    particleSystem->setEmitter(emitter->getID());
    emitter->setEmitterType(iEmitterType::Disc);
    emitter->setSize(1);

    iNodeTransform *transform = iNodeManager::getInstance().createNode<iNodeTransform>();
    transform->translate(-30, 12, 40);
    transform->rotate(-0.12 * M_PI, iaAxis::Z);
    transform->insertNode(emitter);
    getScene()->getRoot()->insertNode(transform);
}

void Particles::createRingParticleSystem()
{
    iaKeyFrameGraphColor4f ringColors;
    ringColors.setValue(1.0f, iaColor4f(0.0f, 1.0f, 1.0f, 1.0f));
    ringColors.setValue(0.66f, iaColor4f(1.0f, 0.0f, 1.0f, 1.0f));
    ringColors.setValue(0.33f, iaColor4f(0.0f, 0.0f, 1.0f, 1.0f));
    ringColors.setValue(0.0f, iaColor4f(0.0f, 0.0f, 0.0f, 0.0f));

    iaKeyFrameGraphVector2f maxAge;
    maxAge.setValue(0.0f, iaVector2f(4.0f, 4.0f));

    iaKeyFrameGraphf emission;
    emission.setValue(0.0f, 10.0f);

    iaKeyFrameGraphVector2f startSize;
    startSize.setValue(0.0f, iaVector2f(2.0f, 2.5f));
    startSize.setValue(1.0f, iaVector2f(0.1f, 0.5f));
    startSize.setValue(2.0f, iaVector2f(2.0f, 2.5f));
    startSize.setValue(3.0f, iaVector2f(0.1f, 0.5f));
    startSize.setValue(4.0f, iaVector2f(2.0f, 2.5f));

    iaKeyFrameGraphVector2f startOrientationRate;
    startOrientationRate.setValue(0.0f, iaVector2f(-0.1f, 0.1f));

    iNodeParticleSystem *circleParticleSystem = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
    _particleSystemIDs.push_back(circleParticleSystem->getID());
    circleParticleSystem->setMaxParticleCount(500);
    iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                       {IGOR_RESOURCE_PARAM_GENERATE, true},
                       {IGOR_RESOURCE_PARAM_SHADER_MATERIAL, _shaderMaterial},
                       {IGOR_RESOURCE_PARAM_TEXTURE0, iResourceManager::getInstance().loadResource<iTexture>("example_texture_particleGem")},
                       {IGOR_RESOURCE_PARAM_TEXTURE1, iResourceManager::getInstance().loadResource<iTexture>("example_texture_octave_1")}});
    circleParticleSystem->setMaterial(iResourceManager::getInstance().loadResource<iMaterial>(param));
    circleParticleSystem->setColorGradient(ringColors);
    circleParticleSystem->setEmissionGradient(emission);
    circleParticleSystem->setStartAgeGradient(maxAge);
    circleParticleSystem->setStartSizeGradient(startSize);
    circleParticleSystem->setStartOrientationRateGradient(startOrientationRate);
    getScene()->getRoot()->insertNode(circleParticleSystem);
    circleParticleSystem->start();

    iNodeEmitter *circleEmitter = iNodeManager::getInstance().createNode<iNodeEmitter>();
    circleParticleSystem->setEmitter(circleEmitter->getID());
    circleEmitter->setEmitterType(iEmitterType::Circle);
    circleEmitter->setSize(10.0f);

    iNodeTransform *circleEmitterTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    _circleEmitterTransformID = circleEmitterTransform->getID();
    circleEmitterTransform->translate(-30, 0, 40);
    circleEmitterTransform->rotate(-0.5 * M_PI, iaAxis::Z);
    circleEmitterTransform->insertNode(circleEmitter);
    getScene()->getRoot()->insertNode(circleEmitterTransform);
}

void Particles::createSmokeParticleSystem()
{
    iaKeyFrameGraphVector2f startOrientation;
    startOrientation.setValue(0.0f, iaVector2f(0.0f, 2.0f * static_cast<float32>(M_PI)));

    iaKeyFrameGraphVector2f startOrientationRate;
    startOrientationRate.setValue(0.0f, iaVector2f(-0.05f, 0.05f));

    iaKeyFrameGraphColor4f smokeGradient;
    smokeGradient.setValue(1.0f, iaColor4f(1.0f, 1.0f, 1.0f, 0.0f));
    smokeGradient.setValue(0.5f, iaColor4f(1.0f, 1.0f, 1.0f, 0.6f));
    smokeGradient.setValue(0.2f, iaColor4f(1.0f, 1.0f, 1.0f, 0.5f));
    smokeGradient.setValue(0.0f, iaColor4f(1.0f, 1.0f, 1.0f, 0.0f));

    iaKeyFrameGraphVector2f smokeSize;
    smokeSize.setValue(0.0f, iaVector2f(3.0f, 6.0f));

    iaKeyFrameGraphVector2f smokemaxAge;
    smokemaxAge.setValue(0.0f, iaVector2f(7.0f, 10.0f));

    iaKeyFrameGraphf emission;
    emission.setValue(0.0f, 1.0f);

    iaKeyFrameGraphVector2f lift;
    lift.setValue(0.0f, iaVector2f(0.0007f, 0.0015f));

    iaKeyFrameGraphf sizeScale;
    sizeScale.setValue(0.0f, 1.0f);
    sizeScale.setValue(10.0f, 10.0f);

    iNodeParticleSystem *particleSystem = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
    _particleSystemIDs.push_back(particleSystem->getID());
    iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                       {IGOR_RESOURCE_PARAM_GENERATE, true},
                       {IGOR_RESOURCE_PARAM_SHADER_MATERIAL, _shaderMaterial},
                       {IGOR_RESOURCE_PARAM_TEXTURE0, iResourceManager::getInstance().loadResource<iTexture>("example_texture_particle_dot")},
                       {IGOR_RESOURCE_PARAM_TEXTURE1, iResourceManager::getInstance().loadResource<iTexture>("example_texture_octave_1")},
                       {IGOR_RESOURCE_PARAM_TEXTURE2, iResourceManager::getInstance().loadResource<iTexture>("example_texture_octave_2")}});
    particleSystem->setMaterial(iResourceManager::getInstance().loadResource<iMaterial>(param));
    particleSystem->setMaxParticleCount(500);
    particleSystem->setVortexTorque(0.5f, 0.7f);
    particleSystem->setStartOrientationGradient(startOrientation);
    particleSystem->setStartOrientationRateGradient(startOrientationRate);
    particleSystem->setVorticityConfinement(0.05f);
    particleSystem->setVortexRange(20.0f, 40.0f);
    particleSystem->setVortexToParticleRate(0.1f);
    particleSystem->setStartSizeGradient(smokeSize);
    particleSystem->setSizeScaleGradient(sizeScale);
    particleSystem->setColorGradient(smokeGradient);
    particleSystem->setStartAgeGradient(smokemaxAge);
    particleSystem->setEmissionGradient(emission);
    particleSystem->setStartLiftGradient(lift);
    getScene()->getRoot()->insertNode(particleSystem);
    particleSystem->start();

    iNodeEmitter *smokeEmitter = iNodeManager::getInstance().createNode<iNodeEmitter>();
    particleSystem->setEmitter(smokeEmitter->getID());
    smokeEmitter->setEmitterType(iEmitterType::Sphere);
    smokeEmitter->setSize(6.0f);

    iNodeTransform *smokeEmitterTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    smokeEmitterTransform->translate(35, 0, 30);
    smokeEmitterTransform->insertNode(smokeEmitter);
    getScene()->getRoot()->insertNode(smokeEmitterTransform);
}

void Particles::createDotParticleSystem()
{
    iaKeyFrameGraphf emission;
    emission.setValue(0.0f, 60.0f);
    emission.setValue(0.2f, 0.0f);

    iaKeyFrameGraphColor4f colorGradient;
    colorGradient.setValue(1.0f, iaColor4f(1.0f, 1.0f, 1.0f, 0.0f));
    colorGradient.setValue(0.8f, iaColor4f(0.8f, 0.8f, 1.0f, 1.0f));
    colorGradient.setValue(0.5f, iaColor4f(0.3f, 0.3f, 0.5f, 1.0f));
    colorGradient.setValue(0.0f, iaColor4f(0.3f, 0.3f, 0.0f, 0.0f));

    iaKeyFrameGraphVector2f velocity;
    velocity.setValue(0.0f, iaVector2f(0.1f, 0.3f));

    iaKeyFrameGraphVector2f maxAge;
    maxAge.setValue(0.0f, iaVector2f(1.5f, 2.0f));

    iaKeyFrameGraphVector2f size;
    size.setValue(0.0f, iaVector2f(1.0f, 1.5f));

    iNodeParticleSystem *particleSystem = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
    _particleSystemIDs.push_back(particleSystem->getID());
    iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                       {IGOR_RESOURCE_PARAM_GENERATE, true},
                       {IGOR_RESOURCE_PARAM_SHADER_MATERIAL, _shaderMaterial},
                       {IGOR_RESOURCE_PARAM_TEXTURE0, iResourceManager::getInstance().loadResource<iTexture>("example_texture_particleTrail")}});
    particleSystem->setMaterial(iResourceManager::getInstance().loadResource<iMaterial>(param));
    particleSystem->setMaxParticleCount(500);
    particleSystem->setColorGradient(colorGradient);
    particleSystem->setStartVelocityGradient(velocity);
    particleSystem->setStartAgeGradient(maxAge);
    particleSystem->setVelocityOriented();
    particleSystem->setStartSizeGradient(size);
    particleSystem->setEmissionGradient(emission);
    particleSystem->start();

    iaKeyFrameGraphf emission2;
    emission2.setValue(0.0f, 10.0f);
    emission2.setValue(0.5f, 0.0f);

    iaKeyFrameGraphColor4f colorGradient2;
    colorGradient2.setValue(1.0f, iaColor4f(1.0f, 1.0f, 1.0f, 0.0f));
    colorGradient2.setValue(0.4f, iaColor4f(1.0f, 1.0f, 1.0f, 0.5f));
    colorGradient2.setValue(0.1f, iaColor4f(1.0f, 1.0f, 1.0f, 0.25f));
    colorGradient2.setValue(0.0f, iaColor4f(1.0f, 1.0f, 1.0f, 0.0f));

    iaKeyFrameGraphVector2f velocity2;
    velocity2.setValue(0.0f, iaVector2f(0.05f, 0.3f));

    iaKeyFrameGraphVector2f maxAge2;
    maxAge2.setValue(0.0f, iaVector2f(0.5f, 1.0f));

    iaKeyFrameGraphVector2f size2;
    size2.setValue(0.0f, iaVector2f(1.0f, 2.5f));

    iaKeyFrameGraphf sizeScale2;
    sizeScale2.setValue(0.0f, 1.0f);
    sizeScale2.setValue(3.0f, 20.0f);

    iaKeyFrameGraphVector2f startOrientation;
    startOrientation.setValue(0.0f, iaVector2f(0.0f, 2.0f * static_cast<float32>(M_PI)));

    iaKeyFrameGraphVector2f startOrientationRate;
    startOrientationRate.setValue(0.0f, iaVector2f(-0.01f, 0.01f));

    iNodeParticleSystem *particleSystem2 = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
    _particleSystemIDs.push_back(particleSystem2->getID());
    iParameters param2({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                       {IGOR_RESOURCE_PARAM_GENERATE, true},
                       {IGOR_RESOURCE_PARAM_SHADER_MATERIAL, _shaderMaterial},
                       {IGOR_RESOURCE_PARAM_TEXTURE0, iResourceManager::getInstance().loadResource<iTexture>("example_texture_particleFlame")},
                       {IGOR_RESOURCE_PARAM_TEXTURE1, iResourceManager::getInstance().loadResource<iTexture>("example_texture_octave_1")},
                       {IGOR_RESOURCE_PARAM_TEXTURE2, iResourceManager::getInstance().loadResource<iTexture>("example_texture_octave_2")}});
    particleSystem2->setMaterial(iResourceManager::getInstance().loadResource<iMaterial>(param2));
    particleSystem->setMaxParticleCount(500);
    particleSystem2->setColorGradient(colorGradient2);
    particleSystem2->setStartVelocityGradient(velocity2);
    particleSystem2->setStartAgeGradient(maxAge2);
    particleSystem2->setStartOrientationGradient(startOrientation);
    particleSystem2->setStartOrientationRateGradient(startOrientationRate);
    particleSystem2->setStartSizeGradient(size2);
    particleSystem2->setSizeScaleGradient(sizeScale2);
    particleSystem2->setEmissionGradient(emission2);
    particleSystem2->start();

    iNodeEmitter *dotEmitter = iNodeManager::getInstance().createNode<iNodeEmitter>();
    particleSystem->setEmitter(dotEmitter->getID());
    particleSystem2->setEmitter(dotEmitter->getID());
    dotEmitter->setEmitterType(iEmitterType::Point);

    iNodeTransform *dotEmitterTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    dotEmitterTransform->translate(20, 0, 20);

    getScene()->getRoot()->insertNode(particleSystem);
    getScene()->getRoot()->insertNode(particleSystem2);

    dotEmitterTransform->insertNode(dotEmitter);
    getScene()->getRoot()->insertNode(dotEmitterTransform);
}

void Particles::onDeinit()
{
    // stop light animation
    if (_animationTimingHandle)
    {
        delete _animationTimingHandle;
        _animationTimingHandle = nullptr;
    }
}

void Particles::onEvent(iEvent &event)
{
    // first call example base
    ExampleBase::onEvent(event);

    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(Particles::onKeyDown));
    event.dispatch<iEventMouseMove>(IGOR_BIND_EVENT_FUNCTION(Particles::onMouseMoveEvent));
    event.dispatch<iEventMouseWheel>(IGOR_BIND_EVENT_FUNCTION(Particles::onMouseWheelEvent));
}

bool Particles::onKeyDown(iEventKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::Space:
        iApplication::getInstance().pause(!iApplication::getInstance().isPaused());
        return true;

    case iKeyCode::R:
        for (auto particleSystemID : _particleSystemIDs)
        {
            iNodeParticleSystem *circleParticleSystem = static_cast<iNodeParticleSystem *>(iNodeManager::getInstance().getNode(particleSystemID));
            if (circleParticleSystem != nullptr)
            {
                circleParticleSystem->reset();
            }
        }
        return true;
    }

    return false;
}

bool Particles::onMouseMoveEvent(iEventMouseMove &event)
{
    if (iMouse::getInstance().getLeftButton())
    {
        const auto from = event.getLastPosition();
        const auto to = event.getPosition();

        iNodeTransform *cameraPitch = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraPitch));
        iNodeTransform *cameraHeading = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraHeading));

        if (cameraPitch != nullptr &&
            cameraHeading != nullptr)
        {
            cameraPitch->rotate((to._y - from._y) * 0.005f, iaAxis::X);
            cameraHeading->rotate((from._x - to._x) * 0.005f, iaAxis::Y);

            iMouse::getInstance().setCenter();
        }

        return true;
    }

    return false;
}

bool Particles::onMouseWheelEvent(iEventMouseWheel &event)
{
    iNodeTransform *camTranslation = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraTranslation));
    if (camTranslation != nullptr)
    {
        if (event.getWheelDelta() < 0)
        {
            camTranslation->translate(0, 0, 10);
        }
        else
        {
            camTranslation->translate(0, 0, -10);
        }
    }

    return true;
}

void Particles::onUpdate(const iaTime &time)
{
    iNodeTransform *transform = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_circleEmitterTransformID));
    if (transform != nullptr)
    {
        transform->rotate(0.1, iaAxis::Z);
        transform->rotate(0.04, iaAxis::X);
        transform->rotate(0.06, iaAxis::Y);
    }
}

iaString Particles::getHelpString()
{
    iaString help = "\n"
                    "[Space] Toggle main loop\n"
                    "[R] reset particle systems\n";

    iaString result = ExampleBase::getHelpString();
    result += help;

    return result;
}
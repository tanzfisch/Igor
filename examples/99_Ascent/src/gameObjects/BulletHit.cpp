// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "BulletHit.h"

#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodePhysics.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/resources/model/iModel.h>
#include <igor/scene/iScene.h>
#include <igor/physics/iPhysics.h>
#include <igor/physics/iPhysicsBody.h>
#include <igor/physics/iPhysicsCollision.h>
#include <igor/scene/nodes/iNodeParticleSystem.h>
#include <igor/scene/nodes/iNodeEmitter.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/material/iMaterial.h>
using namespace igor;

#include <iaux/data/iaString.h>
using namespace iaux;

BulletHit::BulletHit(iScenePtr scene, const iaMatrixd &matrix)
    : GameObject(Fraction::None, GameObjectType::None)
{
    setHealth(100.0);
    setDamage(0.0);
    setShieldDamage(0.0);

    iaKeyFrameGraphColor4f colorGradient;
    colorGradient.setValue(0.0, iaColor4f(1.0, 1.0, 0.1, 1));
    colorGradient.setValue(0.5, iaColor4f(0.9, 0.5, 0.1, 0.7));
    colorGradient.setValue(1.0, iaColor4f(0.3, 0.3, 0.3, 0));

    iaKeyFrameGraphVector2f velocity;
    velocity.setValue(0.0, iaVector2f(0.2, 0.6));

    iaKeyFrameGraphVector2f visibility;
    visibility.setValue(0.0, iaVector2f(0.1, 0.2));

    iaKeyFrameGraphVector2f size;
    size.setValue(0.0, iaVector2f(0.3, 0.5));

    iaKeyFrameGraphf emission;
    emission.setValue(0.0, 20);
    emission.setValue(0.1, 0);

    iNodeParticleSystem *particleSystem = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
    _particleSystemNodeID = particleSystem->getID();
    particleSystem->setLoop(false);
    particleSystem->setMaterial(iMaterialResourceFactory_old::getInstance().getMaterialID("Particles"));
    particleSystem->setTextureA("particleTrail.png");
    particleSystem->setColorGradient(colorGradient);
    particleSystem->setStartVelocityGradient(velocity);
    particleSystem->setVelocityOriented();
    particleSystem->setStartAgeGradient(visibility);
    particleSystem->setStartSizeGradient(size);
    particleSystem->setEmissionGradient(emission);
    particleSystem->setPeriodTime(3.0);
    particleSystem->start();

    iNodeEmitter *emitter = iNodeManager::getInstance().createNode<iNodeEmitter>();
    _emitterNodeID = emitter->getID();
    emitter->setEmitterType(iEmitterType::Point);
    particleSystem->setEmitter(_emitterNodeID);

    iNodeTransform *transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();
    transformNode->setMatrix(matrix);
    _pos = matrix._pos;

    scene->getRoot()->insertNodeAsync(particleSystem);

    transformNode->insertNode(emitter);
    scene->getRoot()->insertNodeAsync(transformNode);
}

BulletHit::~BulletHit()
{
    iNodeManager::getInstance().destroyNodeAsync(_particleSystemNodeID);
}

void BulletHit::hitBy(uint64 entityID)
{
    // nothing to do
}

iaVector3d BulletHit::getCurrentPos()
{
    return _pos;
}

void BulletHit::handle()
{
    iNodeParticleSystem *particleSystem = static_cast<iNodeParticleSystem *>(iNodeManager::getInstance().getNode(_particleSystemNodeID));
    if (particleSystem != nullptr)
    {
        if (particleSystem->isFinished())
        {
            kill();
        }
    }
}

#include "DigEffect.h"

#include <igor/graphics/scene/nodes/iNodeManager.h>
#include <igor/graphics/scene/nodes/iNodeTransform.h>
#include <igor/graphics/scene/nodes/iNodePhysics.h>
#include <igor/graphics/scene/nodes/iNodeModel.h>
#include <igor/resources/model/iModel.h>
#include <igor/graphics/scene/iScene.h>
#include <igor/physics/iPhysics.h>
#include <igor/physics/iPhysicsBody.h>
#include <igor/physics/iPhysicsCollision.h>
#include <igor/graphics/scene/nodes/iNodeParticleSystem.h>
#include <igor/graphics/scene/nodes/iNodeEmitter.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/material/iMaterial.h>
using namespace Igor;

#include <iaux/data/iaString.h>
using namespace IgorAux;

DigEffect::DigEffect(iScene *scene, const iaMatrixd &matrix)
    : GameObject(Fraction::None, GameObjectType::None)
{
    setHealth(100.0);
    setDamage(0.0);
    setShieldDamage(0.0);

    iaGradientf emission;
    emission.setValue(0.0, 5);
    emission.setValue(0.1, 0);

    iaGradientColor4f colorGradient;
    colorGradient.setValue(0.0, iaColor4f(0.6, 0.6, 0.6, 0));
    colorGradient.setValue(0.0, iaColor4f(0.6, 0.6, 0.6, 0.5));
    colorGradient.setValue(0.5, iaColor4f(0.6, 0.6, 0.6, 0.5));
    colorGradient.setValue(1.0, iaColor4f(0.6, 0.6, 0.6, 0));

    iaGradientVector2f velocity;
    velocity.setValue(0.0, iaVector2f(0.01, 0.02));

    iaGradientVector2f visibility;
    visibility.setValue(0.0, iaVector2f(0.5, 1.5));

    iaGradientVector2f size;
    size.setValue(0.0, iaVector2f(3.0, 4.0));

    iNodeParticleSystem *particleSystem = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
    _particleSystemNodeID = particleSystem->getID();
    particleSystem->setMaterial(iMaterialResourceFactory::getInstance().getMaterialID("PMat"));
    particleSystem->setTextureA("particleSmoke.png");
    particleSystem->setTextureB("octave1.png");
    particleSystem->setTextureC("octave2.png");
    particleSystem->setLoop(false);
    particleSystem->setColorGradient(colorGradient);
    particleSystem->setStartVelocityGradient(velocity);
    particleSystem->setStartVisibleTimeGradient(visibility);
    particleSystem->setStartSizeGradient(size);
    particleSystem->setEmissionGradient(emission);
    particleSystem->setPeriodTime(4.0);
    particleSystem->start();

    iNodeEmitter *emitter = iNodeManager::getInstance().createNode<iNodeEmitter>();
    emitter->setEmitterType(iEmitterType::Sphere);
    emitter->setSize(5);
    particleSystem->setEmitter(emitter->getID());

    iNodeTransform *transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();
    _transformNodeID = transformNode->getID();
    transformNode->setMatrix(matrix);
    _pos = matrix._pos;

    scene->getRoot()->insertNode(particleSystem);

    transformNode->insertNode(emitter);
    scene->getRoot()->insertNode(transformNode);
}

DigEffect::~DigEffect()
{
    iNodeManager::getInstance().destroyNodeAsync(_particleSystemNodeID);
    iNodeManager::getInstance().destroyNodeAsync(_transformNodeID);
}

void DigEffect::hitBy(uint64 entityID)
{
    // nothing to do
}

iaVector3d DigEffect::getCurrentPos()
{
    return _pos;
}

void DigEffect::handle()
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
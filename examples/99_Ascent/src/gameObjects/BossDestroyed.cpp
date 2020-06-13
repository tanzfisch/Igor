#include "BossDestroyed.h"

#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodePhysics.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/scene/nodes/iNodeParticleSystem.h>
#include <igor/scene/nodes/iNodeEmitter.h>
#include <igor/resources/model/iModel.h>
#include <igor/scene/iScene.h>
#include <igor/physics/iPhysics.h>
#include <igor/physics/iPhysicsBody.h>
#include <igor/physics/iPhysicsCollision.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/material/iMaterial.h>
using namespace igor;

#include <iaux/data/iaString.h>
using namespace iaux;

BossDestroyed::BossDestroyed(iScene *scene, const iaMatrixd &matrix)
    : GameObject(Fraction::None, GameObjectType::None)
{
    setHealth(100.0);
    setDamage(0.0);
    setShieldDamage(0.0);

    iaGradientf emission;
    emission.setValue(0.0, 0);
    emission.setValue(0.2, 0);
    emission.setValue(0.3, 80);
    emission.setValue(0.4, 0);

    iaGradientColor4f colorGradient;
    colorGradient.setValue(0.0, iaColor4f(0.1, 0.8, 0.9, 1));
    colorGradient.setValue(0.5, iaColor4f(0.2, 0.7, 1.0, 0.7));
    colorGradient.setValue(1.0, iaColor4f(0.3, 0.3, 0.7, 0));

    iaGradientVector2f velocity;
    velocity.setValue(0.0, iaVector2f(0.8, 1.2));

    iaGradientVector2f visibility;
    visibility.setValue(0.0, iaVector2f(1.0, 3.0));

    iaGradientVector2f size;
    size.setValue(0.0, iaVector2f(1.4, 2.0));

    iNodeParticleSystem *particleSystem = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
    _traceNodeID = particleSystem->getID();
    particleSystem->setMaterial(iMaterialResourceFactory::getInstance().getMaterialID("PMat"));
    particleSystem->setTextureA("particleTrail.png");
    particleSystem->setLoop(false);
    particleSystem->setColorGradient(colorGradient);
    particleSystem->setStartVelocityGradient(velocity);
    particleSystem->setStartVisibleTimeGradient(visibility);
    particleSystem->setVelocityOriented();
    particleSystem->setStartSizeGradient(size);
    particleSystem->setEmissionGradient(emission);
    particleSystem->setPeriodTime(5.0);
    particleSystem->start();

    iaGradientf emission2;
    emission2.setValue(0.0, 0);
    emission2.setValue(0.2, 50);
    emission2.setValue(0.3, 0);

    iaGradientColor4f colorGradient2;
    colorGradient2.setValue(0.0, iaColor4f(0.25, 0.25, 0.25, 0));
    colorGradient2.setValue(0.1, iaColor4f(0.25, 0.25, 0.25, 0.25));
    colorGradient2.setValue(1.0, iaColor4f(0.25, 0.25, 0.25, 0));

    iaGradientVector2f velocity2;
    velocity2.setValue(0.0, iaVector2f(0.1, 0.9));

    iaGradientVector2f visibility2;
    visibility2.setValue(0.0, iaVector2f(12.0, 15.0));

    iaGradientVector2f size2;
    size2.setValue(0.0, iaVector2f(3.0, 5.0));

    iaGradientf sizeScale2;
    sizeScale2.setValue(0.0, 1.0);
    sizeScale2.setValue(20.0, 10.0);

    iaGradientVector2f startOrientation2;
    startOrientation2.setValue(0.0, iaVector2f(0.0, 2.0 * M_PI));

    iaGradientVector2f startOrientationRate2;
    startOrientationRate2.setValue(0.0, iaVector2f(-0.04, 0.04));

    iNodeParticleSystem *particleSystem2 = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
    _smokeNodeID = particleSystem2->getID();
    particleSystem2->setMaterial(iMaterialResourceFactory::getInstance().getMaterialID("PMat"));
    particleSystem2->setTextureA("particleSmoke.png");
    particleSystem2->setTextureB("octave1.png");
    particleSystem2->setTextureC("octave2.png");
    particleSystem2->setLoop(false);
    particleSystem2->setColorGradient(colorGradient2);
    particleSystem2->setStartVelocityGradient(velocity2);
    particleSystem2->setStartVisibleTimeGradient(visibility2);
    particleSystem2->setStartOrientationGradient(startOrientation2);
    particleSystem2->setStartOrientationRateGradient(startOrientationRate2);
    particleSystem2->setStartSizeGradient(size2);
    particleSystem2->setSizeScaleGradient(sizeScale2);
    particleSystem2->setEmissionGradient(emission2);
    particleSystem2->setAirDrag(0.95);
    particleSystem2->setPeriodTime(15.0);
    particleSystem2->start();

    iaGradientf emission3;
    emission3.setValue(0.0, 50);
    emission3.setValue(0.1, 0);

    iaGradientColor4f colorGradient3;
    colorGradient3.setValue(0.0, iaColor4f(1.0, 1.0, 1.0, 0));
    colorGradient3.setValue(0.1, iaColor4f(1.0, 1.0, 1.0, 1));
    colorGradient3.setValue(0.7, iaColor4f(1.0, 1.0, 1.0, 1));
    colorGradient3.setValue(1.0, iaColor4f(1.0, 1.0, 1.0, 0));

    iaGradientVector2f velocity3;
    velocity3.setValue(0.0, iaVector2f(0.3, 0.5));

    iaGradientVector2f visibility3;
    visibility3.setValue(0.0, iaVector2f(0.2, 0.4));

    iaGradientVector2f size3;
    size3.setValue(0.0, iaVector2f(2.0, 4.0));

    iaGradientVector2f startOrientation3;
    startOrientation3.setValue(0.0, iaVector2f(0.0, 2.0 * M_PI));

    iaGradientVector2f startOrientationRate3;
    startOrientationRate3.setValue(0.0, iaVector2f(-0.1, 0.1));

    iNodeParticleSystem *particleSystem3 = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
    _fireBallNodeID = particleSystem3->getID();
    particleSystem3->setMaterial(iMaterialResourceFactory::getInstance().getMaterialID("PMat"));
    particleSystem3->setTextureA("particleFlame.png");
    particleSystem3->setTextureB("octave1.png");
    particleSystem3->setTextureC("octave2.png");
    particleSystem3->setLoop(false);
    particleSystem3->setColorGradient(colorGradient3);
    particleSystem3->setStartVelocityGradient(velocity3);
    particleSystem3->setStartVisibleTimeGradient(visibility3);
    particleSystem3->setStartOrientationGradient(startOrientation3);
    particleSystem3->setStartOrientationRateGradient(startOrientationRate3);
    particleSystem3->setAirDrag(0.98);
    particleSystem3->setStartSizeGradient(size3);
    particleSystem3->setEmissionGradient(emission3);
    particleSystem3->setPeriodTime(5.0);
    particleSystem3->start();

    iNodeEmitter *emitter = iNodeManager::getInstance().createNode<iNodeEmitter>();
    emitter->setEmitterType(iEmitterType::Sphere);
    emitter->setSize(1);
    particleSystem->setEmitter(emitter->getID());
    particleSystem2->setEmitter(emitter->getID());
    particleSystem3->setEmitter(emitter->getID());

    iNodeTransform *transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();
    _transformNodeID = transformNode->getID();
    transformNode->setMatrix(matrix);
    _pos = matrix._pos;

    scene->getRoot()->insertNode(particleSystem2);
    scene->getRoot()->insertNode(particleSystem3);
    scene->getRoot()->insertNode(particleSystem);

    transformNode->insertNode(emitter);
    scene->getRoot()->insertNode(transformNode);
}

BossDestroyed::~BossDestroyed()
{
    iNodeManager::getInstance().destroyNodeAsync(_fireBallNodeID);
    iNodeManager::getInstance().destroyNodeAsync(_smokeNodeID);
    iNodeManager::getInstance().destroyNodeAsync(_traceNodeID);
    iNodeManager::getInstance().destroyNodeAsync(_transformNodeID);
}

void BossDestroyed::hitBy(uint64 entityID)
{
    // nothing to do
}

iaVector3d BossDestroyed::getCurrentPos()
{
    return _pos;
}

void BossDestroyed::handle()
{
    iNodeParticleSystem *particleSystem1 = static_cast<iNodeParticleSystem *>(iNodeManager::getInstance().getNode(_fireBallNodeID));
    iNodeParticleSystem *particleSystem2 = static_cast<iNodeParticleSystem *>(iNodeManager::getInstance().getNode(_smokeNodeID));
    iNodeParticleSystem *particleSystem3 = static_cast<iNodeParticleSystem *>(iNodeManager::getInstance().getNode(_traceNodeID));

    if (particleSystem1 != nullptr &&
        particleSystem2 != nullptr &&
        particleSystem3 != nullptr)
    {
        if (particleSystem1->isFinished() &&
            particleSystem2->isFinished() &&
            particleSystem3->isFinished())
        {
            kill();
        }
    }
}
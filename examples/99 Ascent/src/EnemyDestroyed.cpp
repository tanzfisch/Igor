#include "EnemyDestroyed.h"

#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iNodePhysics.h>
#include <iNodeModel.h>
#include <iModel.h>
#include <iScene.h>
#include <iPhysics.h>
#include <iPhysicsBody.h>
#include <iPhysicsCollision.h>
#include <iNodeParticleSystem.h>
#include <iNodeEmitter.h>
#include <iMaterialResourceFactory.h>
#include <iMaterial.h>
using namespace Igor;

#include <iaString.h>
using namespace IgorAux;

#include "EntityManager.h"

EnemyDestroyed::EnemyDestroyed(iScene* scene, const iaMatrixf& matrix)
    : Entity(Fraction::None, EntityType::None)
{
    setHealth(100.0);
    setDamage(0.0);
    setShieldDamage(0.0);

    iGradientui emission;
    emission.insertValue(0.0, 0);
    emission.insertValue(0.2, 0);
    emission.insertValue(0.3, 80);
    emission.insertValue(0.4, 0);

    iGradientColor4f colorGradient;
    colorGradient.insertValue(0.0, iaColor4f(1.0, 1.0, 0.1, 1));
    colorGradient.insertValue(0.5, iaColor4f(0.9, 0.5, 0.1, 0.7));
    colorGradient.insertValue(1.0, iaColor4f(0.3, 0.3, 0.3, 0));

    iGradientVector2f velocity;
    velocity.insertValue(0.0, iaVector2f(0.1, 0.2));

    iGradientVector2f visibility;
    visibility.insertValue(0.0, iaVector2f(0.6, 1.0));

    iGradientVector2f size;
    size.insertValue(0.0, iaVector2f(0.3, 0.5));

    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
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
    particleSystem->setPeriodTime(3.0);
    particleSystem->start();

    iGradientui emission2;
    emission2.insertValue(0.0, 0);
    emission2.insertValue(0.2, 50);
    emission2.insertValue(0.3, 0);

    iGradientColor4f colorGradient2;
    colorGradient2.insertValue(0.0, iaColor4f(0.25, 0.25, 0.25, 0));
    colorGradient2.insertValue(0.1, iaColor4f(0.25, 0.25, 0.25, 0.25));
    colorGradient2.insertValue(1.0, iaColor4f(0.25, 0.25, 0.25, 0));

    iGradientVector2f velocity2;
    velocity2.insertValue(0.0, iaVector2f(0.01, 0.8));

    iGradientVector2f visibility2;
    visibility2.insertValue(0.0, iaVector2f(2.0, 2.5));

    iGradientVector2f size2;
    size2.insertValue(0.0, iaVector2f(1.0, 2.0));

    iGradientf sizeScale2;
    sizeScale2.insertValue(0.0, 1.0);
    sizeScale2.insertValue(3.0, 4.0);

    iGradientVector2f startOrientation2;
    startOrientation2.insertValue(0.0, iaVector2f(0.0, 2.0 * M_PI));

    iGradientVector2f startOrientationRate2;
    startOrientationRate2.insertValue(0.0, iaVector2f(-0.04, 0.04));

    iNodeParticleSystem* particleSystem2 = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
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
    particleSystem2->setPeriodTime(3.0);
    particleSystem2->start();

    iGradientui emission3;
    emission3.insertValue(0.0, 50);
    emission3.insertValue(0.1, 0);

    iGradientColor4f colorGradient3;
    colorGradient3.insertValue(0.0, iaColor4f(1.0, 1.0, 1.0, 0));
    colorGradient3.insertValue(0.1, iaColor4f(1.0, 1.0, 1.0, 1));
    colorGradient3.insertValue(0.7, iaColor4f(1.0, 1.0, 1.0, 1));
    colorGradient3.insertValue(1.0, iaColor4f(1.0, 1.0, 1.0, 0));

    iGradientVector2f velocity3;
    velocity3.insertValue(0.0, iaVector2f(0.3, 0.5));

    iGradientVector2f visibility3;
    visibility3.insertValue(0.0, iaVector2f(0.2, 0.4));

    iGradientVector2f size3;
    size3.insertValue(0.0, iaVector2f(1.0, 2.0));

    iGradientVector2f startOrientation3;
    startOrientation3.insertValue(0.0, iaVector2f(0.0, 2.0 * M_PI));

    iGradientVector2f startOrientationRate3;
    startOrientationRate3.insertValue(0.0, iaVector2f(-0.1, 0.1));

    iNodeParticleSystem* particleSystem3 = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
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
    particleSystem3->setPeriodTime(3.0);
    particleSystem3->start();

    iNodeEmitter* emitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
    emitter->setType(iEmitterType::Sphere);
    emitter->setSize(1);
    particleSystem->setEmitter(emitter->getID());
    particleSystem2->setEmitter(emitter->getID());
    particleSystem3->setEmitter(emitter->getID());

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _transformNodeID = transformNode->getID();
    transformNode->setMatrix(matrix);
    _pos = matrix._pos;

    scene->getRoot()->insertNode(particleSystem2);
    scene->getRoot()->insertNode(particleSystem3);
    scene->getRoot()->insertNode(particleSystem);

    transformNode->insertNode(emitter);
    scene->getRoot()->insertNode(transformNode);
}

EnemyDestroyed::~EnemyDestroyed()
{
    iNodeFactory::getInstance().destroyNodeAsync(_fireBallNodeID);
    iNodeFactory::getInstance().destroyNodeAsync(_smokeNodeID);
    iNodeFactory::getInstance().destroyNodeAsync(_traceNodeID);
    iNodeFactory::getInstance().destroyNodeAsync(_transformNodeID);
}

void EnemyDestroyed::hitBy(uint64 entityID)
{
    // nothing to do
}

iaVector3f EnemyDestroyed::updatePos()
{
    return _pos;
}

void EnemyDestroyed::handle()
{
    iNodeParticleSystem* particleSystem1 = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(_fireBallNodeID));
    iNodeParticleSystem* particleSystem2 = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(_smokeNodeID));
    iNodeParticleSystem* particleSystem3 = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(_traceNodeID));

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
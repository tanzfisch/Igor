#include "MuzzleFlash.h"

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

MuzzleFlash::MuzzleFlash(iScene* scene, uint32 emitterID)
    : Entity(Fraction::None, EntityType::None)
{
    setHealth(100.0);
    setDamage(0.0);
    setShieldDamage(0.0);

    iGradientColor4f colorGradient;
    colorGradient.insertValue(0.0, iaColor4f(1.0, 1.0, 1.0, 0));
    colorGradient.insertValue(0.1, iaColor4f(1.0, 1.0, 1.0, 1));
    colorGradient.insertValue(1.0, iaColor4f(1.0, 1.0, 1.0, 0));

    iGradientVector2f velocity;
    velocity.insertValue(0.0, iaVector2f(0.0, 0.0));

    iGradientVector2f visibility;
    visibility.insertValue(0.0, iaVector2f(0.1, 0.2));

    iGradientVector2f size;
    size.insertValue(0.0, iaVector2f(0.1, 0.1));

    iGradientf sizeScale;
    sizeScale.insertValue(0.0, 1.0);
    sizeScale.insertValue(0.2, 5.0);

    iGradientui emission;
    emission.insertValue(0.0, 1);
    emission.insertValue(0.1, 0);

    iNodeParticleSystem* particleSystem1 = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
    _particleSystemNodeID1 = particleSystem1->getID();
    particleSystem1->setLoop(false);
    particleSystem1->setMaterial(iMaterialResourceFactory::getInstance().getMaterialID("PMat"));
    particleSystem1->setTextureA("particleFlash.png");
    particleSystem1->setColorGradient(colorGradient);
    particleSystem1->setStartVelocityGradient(velocity);
    particleSystem1->setStartVisibleTimeGradient(visibility);
    particleSystem1->setStartSizeGradient(size);
    particleSystem1->setSizeScaleGradient(sizeScale);
    particleSystem1->setEmissionGradient(emission);
    particleSystem1->setPeriodTime(3.0);
    particleSystem1->start();
    particleSystem1->setEmitter(emitterID);

    iGradientColor4f colorGradient2;
    colorGradient2.insertValue(0.0, iaColor4f(0.3, 0.3, 0.3, 0));
    colorGradient2.insertValue(0.1, iaColor4f(0.3, 0.3, 0.3, 0.5));
    colorGradient2.insertValue(1.0, iaColor4f(0.3, 0.3, 0.3, 0));

    iGradientVector2f velocity2;
    velocity2.insertValue(0.0, iaVector2f(0.0, 0.005));

    iGradientVector2f visibility2;
    visibility2.insertValue(0.0, iaVector2f(0.4, 0.8));

    iGradientVector2f size2;
    size2.insertValue(0.0, iaVector2f(0.01, 0.03));

    iGradientf sizeScale2;
    sizeScale2.insertValue(0.0, 1.0);
    sizeScale2.insertValue(1.0, 5.0);

    iGradientui emission2;
    emission2.insertValue(0.0, 10);
    emission2.insertValue(0.1, 0);

    iNodeParticleSystem* particleSystem2 = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
    _particleSystemNodeID2 = particleSystem2->getID();
    particleSystem2->setLoop(false);
    particleSystem2->setMaterial(iMaterialResourceFactory::getInstance().getMaterialID("PMat"));
    particleSystem2->setTextureA("particleSmoke.png");
    particleSystem2->setColorGradient(colorGradient2);
    particleSystem2->setStartVelocityGradient(velocity2);
    particleSystem2->setStartVisibleTimeGradient(visibility2);
    particleSystem2->setStartSizeGradient(size2);
    particleSystem2->setSizeScaleGradient(sizeScale2);
    particleSystem2->setEmissionGradient(emission2);
    particleSystem2->setAirDrag(0.99);
    particleSystem2->setPeriodTime(3.0);
    particleSystem2->start();
    particleSystem2->setEmitter(emitterID);

    iNode* emitterNode = iNodeFactory::getInstance().getNode(emitterID);
    if (emitterNode != nullptr)
    {
        emitterNode->insertNode(particleSystem1);
        emitterNode->insertNode(particleSystem2);

        iaMatrixf matrix;
        emitterNode->calcWorldTransformation(matrix);
        _pos = matrix._pos;
    }
}

MuzzleFlash::~MuzzleFlash()
{
    iNodeFactory::getInstance().destroyNodeAsync(_particleSystemNodeID1);
    iNodeFactory::getInstance().destroyNodeAsync(_particleSystemNodeID2);
}

void MuzzleFlash::hitBy(uint64 entityID)
{
    // nothing to do
}

iaVector3f MuzzleFlash::updatePos()
{
    return _pos;
}

void MuzzleFlash::handle()
{
    iNodeParticleSystem* particleSystem1 = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(_particleSystemNodeID1));
    iNodeParticleSystem* particleSystem2 = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(_particleSystemNodeID2));
    if (particleSystem1 != nullptr &&
        particleSystem2 != nullptr)
    {
        if (particleSystem1->isFinished() &&
            particleSystem2->isFinished())
        {
            kill();
        }
    }
}

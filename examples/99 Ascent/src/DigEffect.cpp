#include "DigEffect.h"

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

DigEffect::DigEffect(iScene* scene, const iaMatrixd& matrix)
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

    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
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

	iNodeEmitter* emitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
	emitter->setEmitterType(iEmitterType::Sphere);
    emitter->setSize(5);
	particleSystem->setEmitter(emitter->getID());

	iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _transformNodeID = transformNode->getID();
	transformNode->setMatrix(matrix);
	_pos = matrix._pos;

	scene->getRoot()->insertNode(particleSystem);
    
    transformNode->insertNode(emitter);
	scene->getRoot()->insertNode(transformNode);
}

DigEffect::~DigEffect()
{
    iNodeFactory::getInstance().destroyNodeAsync(_particleSystemNodeID);
    iNodeFactory::getInstance().destroyNodeAsync(_transformNodeID);
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
    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(_particleSystemNodeID));

	if (particleSystem != nullptr)
	{
		if (particleSystem->isFinished())
		{
			kill();
		}
	}
}
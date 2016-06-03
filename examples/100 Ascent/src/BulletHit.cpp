#include "BulletHit.h"

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

BulletHit::BulletHit(iScene* scene, const iaMatrixf& matrix)
    : Entity(Fraction::None, EntityType::None)
{
    setHealth(100.0);
    setDamage(0.0);
    setShieldDamage(0.0);

	iGradientColor4f colorGradient;
	colorGradient.insertValue(0.0, iaColor4f(1.0, 1.0, 0.1, 1));
    colorGradient.insertValue(0.5, iaColor4f(0.9, 0.5, 0.1, 0.7));
	colorGradient.insertValue(1.0, iaColor4f(0.3, 0.3, 0.3, 0));

	iGradientVector2f velocity;
	velocity.insertValue(0.0, iaVector2f(0.2, 0.7));

	iGradientVector2f visibility;
	visibility.insertValue(0.0, iaVector2f(0.1, 0.2));

	iGradientVector2f size;
	size.insertValue(0.0, iaVector2f(0.5, 0.9));

	iGradientui emission;
	emission.insertValue(0.0, 20);
    emission.insertValue(0.1, 0);

	iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
	_particleSystemNodeID = particleSystem->getID();
	particleSystem->setLoop(false);
	particleSystem->setMaterial(iMaterialResourceFactory::getInstance().getMaterialID("PMat"));
	particleSystem->setTextureA("particleTrail.png");
	particleSystem->setColorGradient(colorGradient);
	particleSystem->setStartVelocityGradient(velocity);
    particleSystem->setVelocityOriented();
	particleSystem->setStartVisibleTimeGradient(visibility);
	particleSystem->setStartSizeGradient(size);
	particleSystem->setEmissionGradient(emission);
	particleSystem->setPeriodTime(3.0);
	particleSystem->start();

	iNodeEmitter* emitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
	_emitterNodeID = emitter->getID();
	emitter->setType(iEmitterType::Point);
	particleSystem->setEmitter(_emitterNodeID);

	iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	transformNode->setMatrix(matrix);
	_pos = matrix._pos;

	scene->getRoot()->insertNode(particleSystem);
    transformNode->insertNode(emitter);
	scene->getRoot()->insertNode(transformNode);
}

BulletHit::~BulletHit()
{
	if (_particleSystemNodeID != iNode::INVALID_NODE_ID)
	{
		iNodeFactory::getInstance().destroyNode(_particleSystemNodeID);
	}

	if (_transformNodeID != iNode::INVALID_NODE_ID)
	{
		iNodeFactory::getInstance().destroyNode(_transformNodeID);
	}
}

void BulletHit::hitBy(uint64 entityID)
{
	// nothing to do
}

iaVector3f BulletHit::updatePos()
{
	return _pos;
}

void BulletHit::handle()
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

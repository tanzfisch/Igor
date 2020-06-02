#include "BulletHit.h"

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

BulletHit::BulletHit(iScene *scene, const iaMatrixd &matrix)
	: GameObject(Fraction::None, GameObjectType::None)
{
	setHealth(100.0);
	setDamage(0.0);
	setShieldDamage(0.0);

	iaGradientColor4f colorGradient;
	colorGradient.setValue(0.0, iaColor4f(1.0, 1.0, 0.1, 1));
	colorGradient.setValue(0.5, iaColor4f(0.9, 0.5, 0.1, 0.7));
	colorGradient.setValue(1.0, iaColor4f(0.3, 0.3, 0.3, 0));

	iaGradientVector2f velocity;
	velocity.setValue(0.0, iaVector2f(0.2, 0.6));

	iaGradientVector2f visibility;
	visibility.setValue(0.0, iaVector2f(0.1, 0.2));

	iaGradientVector2f size;
	size.setValue(0.0, iaVector2f(0.3, 0.5));

	iaGradientf emission;
	emission.setValue(0.0, 20);
	emission.setValue(0.1, 0);

	iNodeParticleSystem *particleSystem = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
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

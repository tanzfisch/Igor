#include "Bullet.h"

#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodePhysics.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/resources/model/iModel.h>
#include <igor/scene/iScene.h>
#include <igor/physics/iPhysics.h>
#include <igor/physics/iPhysicsBody.h>
#include <igor/physics/iPhysicsCollision.h>
#include <igor/scene/nodes/iNodeEmitter.h>
#include <igor/scene/nodes/iNodeParticleSystem.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/entities/iEntityManager.h>
using namespace igor;

#include <iaux/data/iaString.h>
using namespace iaux;

#include "BulletHit.h"
#include "../Ascent.h"

Bullet::Bullet(iScene *scene, const iaVector3d &addForce, const iaMatrixd &matrix, Fraction fraction)
	: GameObject(fraction, GameObjectType::Weapon)
{
	_scene = scene;

	_force = matrix._depth;
	_force.negate();
	_force.normalize();
	_force *= 0.75;
	_force += addForce;

	setHealth(100.0);
	setShield(100.0);
	setDamage(50.0);
	setShieldDamage(20.0);

	iNodeTransform *transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();
	transformNode->setMatrix(matrix);
	_transformNodeID = transformNode->getID();

	iaGradientColor4f colorGradient;
	colorGradient.setValue(0.0, iaColor4f(0.0, 1.0, 1.0, 1));
	colorGradient.setValue(0.5, iaColor4f(0.0, 0.4, 1.0, 0.7));
	colorGradient.setValue(1.0, iaColor4f(0.0, 0.0, 0.0, 0));

	iaGradientVector2f velocity;
	velocity.setValue(0.0, iaVector2f(0.0, 1.0));

	iaGradientVector2f visibility;
	visibility.setValue(0.0, iaVector2f(0.1, 0.2));

	iaGradientVector2f size;
	size.setValue(0.0, iaVector2f(0.1, 0.2));

	iaGradientf emission;
	emission.setValue(0.0, 2);

	iNodeParticleSystem *particleSystem = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
	_particleSystemNodeID = particleSystem->getID();
	particleSystem->setLoop(true);
	particleSystem->setMaterial(iMaterialResourceFactory::getInstance().getMaterialID("PMat"));
	particleSystem->setTextureA("particleDot.png");
	particleSystem->setColorGradient(colorGradient);
	particleSystem->setStartVelocityGradient(velocity);
	particleSystem->setStartVisibleTimeGradient(visibility);
	particleSystem->setStartSizeGradient(size);
	particleSystem->setEmissionGradient(emission);
	particleSystem->setPeriodTime(1.0);
	particleSystem->setAirDrag(0.0);
	particleSystem->start();

	iNodeEmitter *emitter = iNodeManager::getInstance().createNode<iNodeEmitter>();
	emitter->setEmitterType(iEmitterType::Disc);
	emitter->setSize(0.0);
	particleSystem->setEmitter(emitter->getID());

	iNodeTransform *emitterTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	emitterTransform->rotate(-0.5 * M_PI, iaAxis::X);

	iaMatrixd offset;
	iNodePhysics *physicsNode = iNodeManager::getInstance().createNode<iNodePhysics>();
	physicsNode->addSphere(0.1, offset);
	physicsNode->finalizeCollision();
	physicsNode->setMass(0.001);
	physicsNode->setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Bullet::onApplyForceAndTorque));
	physicsNode->setMaterial(Ascent::_bulletMaterialID);
	physicsNode->setUserData(getID());

	_scene->getRoot()->insertNode(particleSystem);

	_scene->getRoot()->insertNode(transformNode);
	transformNode->insertNode(emitterTransform);
	emitterTransform->insertNode(emitter);
	transformNode->insertNode(physicsNode);
}

Bullet::~Bullet()
{
	iNodeManager::getInstance().destroyNodeAsync(_transformNodeID);
	iNodeManager::getInstance().destroyNodeAsync(_particleSystemNodeID);
}

void Bullet::hitBy(uint64 entityID)
{
	bool killit = false;
	GameObject *gameObject = static_cast<GameObject *>(iEntityManager::getInstance().getEntity(entityID));
	if (gameObject != nullptr)
	{
		if (gameObject->getFraction() != getFraction())
		{
			killit = true;
		}
	}
	else
	{
		killit = true;
	}

	if (killit)
	{
		iNodeTransform *transformNode = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_transformNodeID));
		if (transformNode != nullptr)
		{
			iaMatrixd matrix;
			transformNode->getMatrix(matrix);
			BulletHit *bulletHit = new BulletHit(_scene, matrix);
		}

		kill();
	}
}

iaVector3d Bullet::getCurrentPos()
{
	iaVector3d result;
	iNodeTransform *transformNode = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_transformNodeID));
	if (transformNode != nullptr)
	{
		iaMatrixd matrix;
		transformNode->getMatrix(matrix);
		result = matrix._pos;
	}
	return result;
}

void Bullet::handle()
{
	float32 health = getHealth() - 0.1;
	if (health <= 0.0)
	{
		health = 0;
		kill();
	}
	setHealth(health);

	// making sure bullets don't leave the scene
	const iaVector3d center(10000, 10000, 10000);
	if (getCurrentPos().distance(center) > 5000)
	{
		kill();
	}
}

void Bullet::onApplyForceAndTorque(iPhysicsBody *body, float32 timestep)
{
	body->setForce(_force);
}

#include "Bullet.h"

#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iNodePhysics.h>
#include <iNodeModel.h>
#include <iModel.h>
#include <iScene.h>
#include <iPhysics.h>
#include <iPhysicsBody.h>
#include <iPhysicsCollision.h>
#include <iNodeEmitter.h>
#include <iNodeParticleSystem.h>
#include <iMaterialResourceFactory.h>
using namespace Igor;

#include <iaString.h>
using namespace IgorAux;

#include "EntityManager.h"
#include "BulletHit.h"

Bullet::Bullet(iScene* scene, const iaVector3f& addForce, const iaMatrixf& matrix, Fraction fraction)
	: Entity(fraction, EntityType::Weapon)
{
	_scene = scene;

	_force = matrix._depth;
	_force.negate();
	_force.normalize();
	_force *= 7.5;
	_force += addForce;

	setHealth(100.0);
	setShield(100.0);
	setDamage(20.0);
	setShieldDamage(10.0);

	iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	transformNode->setMatrix(matrix);
	_transformNodeID = transformNode->getID();

	iGradientColor4f colorGradient;
	colorGradient.insertValue(0.0, iaColor4f(0.0, 1.0, 1.0, 1));
	colorGradient.insertValue(0.5, iaColor4f(0.0, 0.4, 1.0, 0.7));
	colorGradient.insertValue(1.0, iaColor4f(0.0, 0.0, 0.0, 0));

	iGradientVector2f velocity;
	velocity.insertValue(0.0, iaVector2f(0.0, 1.0));

	iGradientVector2f visibility;
	visibility.insertValue(0.0, iaVector2f(0.1, 0.2));

	iGradientVector2f size;
	size.insertValue(0.0, iaVector2f(0.1, 0.2));

	iGradientui emission;
	emission.insertValue(0.0, 2);

	iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
	_particleSystemNodeID = particleSystem->getID();
	particleSystem->setLoop(true);
	particleSystem->setMaterial(iMaterialResourceFactory::getInstance().getMaterialID("PMat"));
	particleSystem->setTextureA("particleDot.png");
	particleSystem->setColorGradient(colorGradient);
	particleSystem->setStartVelocityGradient(velocity);
	particleSystem->setStartVisibleTimeGradient(visibility);
	particleSystem->setStartSizeGradient(size);
	particleSystem->setEmissionGradient(emission);
	particleSystem->setAirDrag(0.0);
	particleSystem->start();

	iNodeEmitter* emitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
	emitter->setType(iEmitterType::Disc);
	emitter->setSize(0.0);
	particleSystem->setEmitter(emitter->getID());

	iNodeTransform* emitterTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	emitterTransform->rotate(-0.5 * M_PI, iaAxis::X);

	iaMatrixf offset;
	iNodePhysics* physicsNode = static_cast<iNodePhysics*>(iNodeFactory::getInstance().createNode(iNodeType::iNodePhysics));
	physicsNode->addSphere(0.5, offset);
	physicsNode->finalizeCollision();
	physicsNode->setMass(0.01);
	physicsNode->setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Bullet::onApplyForceAndTorque));
	physicsNode->setMaterial(EntityManager::getInstance().getBulletMaterialID());
	physicsNode->setUserData(&_id);

	_scene->getRoot()->insertNode(particleSystem);

	_scene->getRoot()->insertNode(transformNode);
	transformNode->insertNode(emitterTransform);
	emitterTransform->insertNode(emitter);
	transformNode->insertNode(physicsNode);
}

Bullet::~Bullet()
{
	iNodeFactory::getInstance().destroyNodeAsync(_transformNodeID);
	iNodeFactory::getInstance().destroyNodeAsync(_particleSystemNodeID);
}

void Bullet::hitBy(uint64 entityID)
{
	bool killit = false;
	Entity* entity = EntityManager::getInstance().getEntity(entityID);
	if (entity != nullptr)
	{
		if (entity->getFraction() != getFraction())
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
		iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
		if (transformNode != nullptr)
		{
			iaMatrixf matrix;
			transformNode->getMatrix(matrix);
			BulletHit* bulletHit = new BulletHit(_scene, matrix);
		}

		kill();
	}
}

iaVector3f Bullet::updatePos()
{
	iaVector3f result;
	iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
	if (transformNode != nullptr)
	{
		iaMatrixf matrix;
		transformNode->getMatrix(matrix);
		result = matrix._pos;
	}
	return result;
}

void Bullet::handle()
{
	setHealth(getHealth() - 1);
	float32 damage = getDamage() - 0.1;
	if (damage < 0.0)
	{
		damage = 0;
	}
	setDamage(damage);

	float32 shieldDamage = getShieldDamage() - 0.1;
	if (shieldDamage < 0.0)
	{
		shieldDamage = 0.0;
	}
	setShieldDamage(shieldDamage);
}

void Bullet::onApplyForceAndTorque(iPhysicsBody* body, float32 timestep, int threadIndex)
{
	body->setForce(_force);
}

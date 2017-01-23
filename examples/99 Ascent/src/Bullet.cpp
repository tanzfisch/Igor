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

Bullet::Bullet()
    : GameObject(GameObjectKind::Weapon)
{

}

Entity* Bullet::createInstance()
{
    return static_cast<Entity*>(new Bullet());
}

void Bullet::setForce(const iaVector3d& force)
{
    _force = force;
}

void Bullet::init()
{
	setHealth(100.0);
	setShield(100.0);
	setDamage(20.0);
	setShieldDamage(10.0);

	iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformNode->translate(_sphere._center);
	_transformNodeID = transformNode->getID();

/*	iaGradientColor4f colorGradient;
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
	particleSystem->start();*/

	iNodeEmitter* emitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
    _emitterNodeID = emitter->getID();
	emitter->setType(iEmitterType::Disc);
	emitter->setSize(0.0);

	iNodeTransform* emitterTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	emitterTransform->rotate(-0.5 * M_PI, iaAxis::X);

	iaMatrixd offset;
	iNodePhysics* physicsNode = static_cast<iNodePhysics*>(iNodeFactory::getInstance().createNode(iNodeType::iNodePhysics));
	physicsNode->addSphere(0.5, offset);
	physicsNode->finalizeCollision();
	physicsNode->setMass(0.01);
	physicsNode->setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Bullet::onApplyForceAndTorque));
	physicsNode->setMaterial(iPhysics::getInstance().getMaterialID("bullet"));
	physicsNode->setUserData(reinterpret_cast<const void*>(getID()));


    iNodeModel* particleSystem = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    _particleSystemNodeID = particleSystem->getID();
    particleSystem->setModel("BulletBlue.ompf");
    particleSystem->registerModelLoadedDelegate(iModelLoadedDelegate(this, &Bullet::onEffectLoaded));
    GameObject::_scene->getRoot()->insertNode(particleSystem);

    transformNode->insertNode(emitterTransform);
    emitterTransform->insertNode(emitter);
    transformNode->insertNode(physicsNode);

	_scene->getRoot()->insertNodeAsync(particleSystem);
	_scene->getRoot()->insertNodeAsync(transformNode);
}

void Bullet::onEffectLoaded()
{
    iNodeModel* muzzleFlashNode = static_cast<iNodeModel*>(iNodeFactory::getInstance().getNode(_particleSystemNodeID));
    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(muzzleFlashNode->getChild("Effect"));
    particleSystem->setEmitter(_emitterNodeID);
    particleSystem->start();
}

void Bullet::deinit()
{
	iNodeFactory::getInstance().destroyNodeAsync(_transformNodeID);
	iNodeFactory::getInstance().destroyNodeAsync(_particleSystemNodeID);
}

void Bullet::hitBy(uint64 entityID)
{
	bool killit = false;
    GameObject* gameObject = static_cast<GameObject*>(EntityManager::getInstance().getEntity(entityID));
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
		iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
		if (transformNode != nullptr)
		{
			iaMatrixd matrix;
			transformNode->getMatrix(matrix);
			//BulletHit* bulletHit = new BulletHit(_scene, matrix);
		}

		kill();
	}
}

iaVector3d Bullet::updatePos()
{
	iaVector3d result;
	iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
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
    float32 health = getHealth() - 1;
    if (health < 0.0)
    {
        health = 0;
        kill();
    }
	setHealth(health);


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

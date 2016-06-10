#include "BossEnemy.h"

#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iNodePhysics.h>
#include <iNodeModel.h>
#include <iModel.h>
#include <iScene.h>
#include <iPhysics.h>
#include <iPhysicsBody.h>
#include <iPhysicsJoint.h>
#include <iPhysicsCollision.h>
#include <iTimer.h>
using namespace Igor;

#include <iaString.h>
#include <iaConvert.h>
using namespace IgorAux;

#include "EntityManager.h"
#include "Turret.h"
#include "EnemyDestroyed.h"
#include "VoxelTerrainGenerator.h"

BossEnemy::BossEnemy(iScene* scene, const iaMatrixf& matrix, uint64 playerID)
    : Entity(Fraction::Red, EntityType::Vehicle)
{
    _playerID = playerID;
    _scene = scene;

    setHealth(100.0);
    setShield(0.0);

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformNode->setMatrix(matrix);
    _transformNodeID = transformNode->getID();

    iNodeTransform* bodyTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    bodyTransform->translate(0, 0, 0);
    iNodeTransform* bodyScale = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    bodyScale->scale(3, 3, 3);

    iNodeModel* bodyModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    bodyModel->setModel("crate.ompf", nullptr);

    iaMatrixf offset;
    iNodePhysics* physicsNode = static_cast<iNodePhysics*>(iNodeFactory::getInstance().createNode(iNodeType::iNodePhysics));
    _physicsNodeID = physicsNode->getID();
    physicsNode->addBox(3.0, 3.0, 3.0, offset);
    physicsNode->finalizeCollision();
    physicsNode->setMass(0);
    physicsNode->setMaterial(EntityManager::getInstance().getEntityMaterialID());
    physicsNode->setUserData(&_id);

    _scene->getRoot()->insertNode(transformNode);
    transformNode->insertNode(bodyTransform);
    transformNode->insertNode(physicsNode);
    bodyTransform->insertNode(bodyScale);
    bodyScale->insertNode(bodyModel);

    iNodeTransform* turretATransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    turretATransform->translate(0, 1.5, 0);
    transformNode->insertNode(turretATransform);
    Turret* turretA = new Turret(_scene, turretATransform, getFraction(), _playerID);
    _turretAID = turretA->getID();

	iNodeTransform* turretBTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	turretBTransform->rotate(M_PI * 0.5, iaAxis::Z);
	turretBTransform->translate(0, 1.5, 0);
	transformNode->insertNode(turretBTransform);
	Turret* turretB = new Turret(_scene, turretBTransform, getFraction(), _playerID);
	_turretBID = turretB->getID();

	iNodeTransform* turretCTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	turretCTransform->rotate(M_PI * -0.5, iaAxis::Z);
	turretCTransform->translate(0, 1.5, 0);
	transformNode->insertNode(turretCTransform);
	Turret* turretC = new Turret(_scene, turretCTransform, getFraction(), _playerID);
	_turretCID = turretC->getID();

	iNodeTransform* turretDTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	turretDTransform->rotate(M_PI * 1.0, iaAxis::Z);
	turretDTransform->translate(0, 1.5, 0);
	transformNode->insertNode(turretDTransform);
	Turret* turretD = new Turret(_scene, turretDTransform, getFraction(), _playerID);
	_turretDID = turretD->getID();

	iNodeTransform* turretETransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	turretETransform->rotate(M_PI * 0.5, iaAxis::X);
	turretETransform->translate(0, 1.5, 0);
	transformNode->insertNode(turretETransform);
	Turret* turretE = new Turret(_scene, turretETransform, getFraction(), _playerID);
	_turretEID = turretE->getID();

	iNodeTransform* turretFTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	turretFTransform->rotate(M_PI * -0.5, iaAxis::X);
	turretFTransform->translate(0, 1.5, 0);
	transformNode->insertNode(turretFTransform);
	Turret* turretF = new Turret(_scene, turretFTransform, getFraction(), _playerID);
	_turretFID = turretF->getID();
}

BossEnemy::~BossEnemy()
{
    Entity* turretA = EntityManager::getInstance().getEntity(_turretAID);
    if (turretA != nullptr) 
    { 
        turretA->kill(); 
    }
	
    Entity* turretB = EntityManager::getInstance().getEntity(_turretBID);
	if (turretB != nullptr) 
    { 
        turretB->kill(); 
    }

	Entity* turretC = EntityManager::getInstance().getEntity(_turretCID);
	if (turretC != nullptr) 
    { 
        turretC->kill(); 
    }

	Entity* turretD = EntityManager::getInstance().getEntity(_turretDID);
	if (turretD != nullptr) 
    { 
        turretD->kill(); 
    }

	Entity* turretE = EntityManager::getInstance().getEntity(_turretEID);
	if (turretE != nullptr) 
    { 
        turretE->kill(); 
    }

	Entity* turretF = EntityManager::getInstance().getEntity(_turretFID);
	if (turretF != nullptr) 
    { 
        turretF->kill(); 
    }

    iNodeFactory::getInstance().destroyNodeAsync(_transformNodeID);
}

void BossEnemy::hitBy(uint64 entityID)
{
    Entity* target = EntityManager::getInstance().getEntity(entityID);
    if (target->getFraction() != getFraction())
    {
        float32 shield = getShield();
        float32 health = getHealth();

        shield -= target->getShieldDamage();
        
        if (shield <= 0)
        {
            shield = 0;

            health -= target->getDamage();
            if (health <= 0)
            {
                health = 0;
            }
        }

        setShield(shield);
        setHealth(health);
    }

    if (getHealth() <= 0.0)
    {
        iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
        if (transformNode != nullptr)
        {
            iaMatrixf matrix;
            transformNode->getMatrix(matrix);
            EnemyDestroyed* effect = new EnemyDestroyed(_scene, matrix);
        }
    }
}

iaVector3f BossEnemy::updatePos()
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

void BossEnemy::handle()
{
    // nothing to do
}


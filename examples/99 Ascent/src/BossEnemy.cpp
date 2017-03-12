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
#include <iEntityManager.h>
using namespace Igor;

#include <iaString.h>
#include <iaConvert.h>
using namespace IgorAux;

#include "Turret.h"
#include "BossDestroyed.h"
#include "VoxelTerrainGenerator.h"
#include "Ascent.h"

BossEnemy::BossEnemy(iScene* scene, const iaMatrixd& matrix, uint64 playerID)
    : GameObject(Fraction::Red, GameObjectType::Vehicle)
{
    _playerID = playerID;
    _scene = scene;

    setHealth(500.0);
    setShield(200.0);

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformNode->setMatrix(matrix);
    _transformNodeID = transformNode->getID();

    iNodeTransform* bodyTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    bodyTransform->translate(0, 0, 0);
    iNodeTransform* bodyScale = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    bodyScale->scale(3, 3, 3);

    iNodeModel* bodyModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    bodyModel->setModel("crate.ompf", nullptr);

    iaMatrixd offset;
    iNodePhysics* physicsNode = static_cast<iNodePhysics*>(iNodeFactory::getInstance().createNode(iNodeType::iNodePhysics));
    _physicsNodeID = physicsNode->getID();
    physicsNode->addBox(3.0, 3.0, 3.0, offset);
    physicsNode->finalizeCollision();
    physicsNode->setMass(0);
    physicsNode->setMaterial(Ascent::_entityMaterialID);
    physicsNode->setUserData(reinterpret_cast<const void*>(getID()));

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
    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
    if (transformNode != nullptr)
    {
        iaMatrixd matrix;
        transformNode->getMatrix(matrix);
        BossDestroyed* effect = new BossDestroyed(_scene, matrix);
    }

    GameObject* turretA = static_cast<GameObject*>(iEntityManager::getInstance().getEntity(_turretAID));
    if (turretA != nullptr) 
    { 
        turretA->kill(); 
    }
	
    GameObject* turretB = static_cast<GameObject*>(iEntityManager::getInstance().getEntity(_turretBID));
	if (turretB != nullptr) 
    { 
        turretB->kill(); 
    }

    GameObject* turretC = static_cast<GameObject*>(iEntityManager::getInstance().getEntity(_turretCID));
	if (turretC != nullptr) 
    { 
        turretC->kill(); 
    }

    GameObject* turretD = static_cast<GameObject*>(iEntityManager::getInstance().getEntity(_turretDID));
	if (turretD != nullptr) 
    { 
        turretD->kill(); 
    }

    GameObject* turretE = static_cast<GameObject*>(iEntityManager::getInstance().getEntity(_turretEID));
	if (turretE != nullptr) 
    { 
        turretE->kill(); 
    }

    GameObject* turretF = static_cast<GameObject*>(iEntityManager::getInstance().getEntity(_turretFID));
	if (turretF != nullptr) 
    { 
        turretF->kill(); 
    }

    iNodeFactory::getInstance().destroyNodeAsync(_transformNodeID);
}

void BossEnemy::hitBy(uint64 entityID)
{
    GameObject* entity = static_cast<GameObject*>(iEntityManager::getInstance().getEntity(entityID));
    if (entity != nullptr &&
        entity->getFraction() != getFraction())
    {
        float32 shield = getShield();
        float32 health = getHealth();

        shield -= entity->getShieldDamage();
        
        if (shield <= 0)
        {
            shield = 0;

            health -= entity->getDamage();
            if (health <= 0)
            {
                health = 0;
            }
        }

        setShield(shield);
        setHealth(health);

        if (getHealth() <= 0)
        {
            kill();
        }
    }
}

iaVector3d BossEnemy::getCurrentPos()
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

void BossEnemy::handle()
{
    // nothing to do
}


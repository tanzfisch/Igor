#include "BossEnemy.h"

#include <iNodeManager.h>
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
#include "Ascent.h"

BossEnemy::BossEnemy(iScene* scene, iVoxelTerrain* voxelTerrain, const iaMatrixd& matrix, uint64 playerID)
    : GameObject(Fraction::Red, GameObjectType::Vehicle)
{
    _playerID = playerID;
    _scene = scene;
    _voxelTerrain = voxelTerrain;

    setHealth(500.0);
    setShield(200.0);

    iNodeTransform* transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();
    transformNode->setMatrix(matrix);
    _transformNodeID = transformNode->getID();

    iNodeTransform* bodyTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    bodyTransform->translate(0, 0, 0);
    iNodeTransform* bodyScale = iNodeManager::getInstance().createNode<iNodeTransform>();
    bodyScale->scale(3, 3, 3);

    iNodeModel* bodyModel = iNodeManager::getInstance().createNode<iNodeModel>();
    bodyModel->setModel("crate.ompf");

    iaMatrixd offset;
    iNodePhysics* physicsNode = iNodeManager::getInstance().createNode<iNodePhysics>();
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

    iNodeTransform* turretATransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    turretATransform->translate(0, 1.5, 0);
    transformNode->insertNode(turretATransform);
    Turret* turretA = new Turret(_scene, turretATransform, _voxelTerrain, getFraction(), _playerID);
    _turretAID = turretA->getID();

	iNodeTransform* turretBTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	turretBTransform->rotate(M_PI * 0.5, iaAxis::Z);
	turretBTransform->translate(0, 1.5, 0);
	transformNode->insertNode(turretBTransform);
	Turret* turretB = new Turret(_scene, turretBTransform, _voxelTerrain, getFraction(), _playerID);
	_turretBID = turretB->getID();

	iNodeTransform* turretCTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	turretCTransform->rotate(M_PI * -0.5, iaAxis::Z);
	turretCTransform->translate(0, 1.5, 0);
	transformNode->insertNode(turretCTransform);
	Turret* turretC = new Turret(_scene, turretCTransform, _voxelTerrain, getFraction(), _playerID);
	_turretCID = turretC->getID();

	iNodeTransform* turretDTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	turretDTransform->rotate(M_PI * 1.0, iaAxis::Z);
	turretDTransform->translate(0, 1.5, 0);
	transformNode->insertNode(turretDTransform);
	Turret* turretD = new Turret(_scene, turretDTransform, _voxelTerrain, getFraction(), _playerID);
	_turretDID = turretD->getID();

	iNodeTransform* turretETransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	turretETransform->rotate(M_PI * 0.5, iaAxis::X);
	turretETransform->translate(0, 1.5, 0);
	transformNode->insertNode(turretETransform);
	Turret* turretE = new Turret(_scene, turretETransform, _voxelTerrain, getFraction(), _playerID);
	_turretEID = turretE->getID();

	iNodeTransform* turretFTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	turretFTransform->rotate(M_PI * -0.5, iaAxis::X);
	turretFTransform->translate(0, 1.5, 0);
	transformNode->insertNode(turretFTransform);
	Turret* turretF = new Turret(_scene, turretFTransform, _voxelTerrain, getFraction(), _playerID);
	_turretFID = turretF->getID();
}

BossEnemy::~BossEnemy()
{
    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeManager::getInstance().getNode(_transformNodeID));
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

    iNodeManager::getInstance().destroyNodeAsync(_transformNodeID);
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
    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeManager::getInstance().getNode(_transformNodeID));
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


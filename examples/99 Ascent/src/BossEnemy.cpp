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
#include "BossDestroyed.h"
#include "VoxelTerrainGenerator.h"

iaString BossEnemy::TYPE_NAME("BossEnemy");

BossEnemy::BossEnemy()
    : GameObject(GameObjectKind::Vehicle)
{
}

BossEnemy::~BossEnemy()
{
    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
    if (transformNode != nullptr)
    {
        iaMatrixd matrix;
        transformNode->getMatrix(matrix);
        // TODO BossDestroyed* effect = new BossDestroyed(_scene, matrix);
    }

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

void BossEnemy::setTargetID(uint64 targetID)
{
    _targetID = targetID;

    Turret* turretA = static_cast<Turret*>(EntityManager::getInstance().getEntity(_turretAID));
    if (turretA != nullptr)
    {
        turretA->setTargetID(_targetID);
    }

    Turret* turretB = static_cast<Turret*>(EntityManager::getInstance().getEntity(_turretBID));
    if (turretB != nullptr)
    {
        turretB->setTargetID(_targetID);
    }

    Turret* turretC = static_cast<Turret*>(EntityManager::getInstance().getEntity(_turretCID));
    if (turretC != nullptr)
    {
        turretC->setTargetID(_targetID);
    }

    Turret* turretD = static_cast<Turret*>(EntityManager::getInstance().getEntity(_turretDID));
    if (turretD != nullptr)
    {
        turretD->setTargetID(_targetID);
    }

    Turret* turretE = static_cast<Turret*>(EntityManager::getInstance().getEntity(_turretEID));
    if (turretE != nullptr)
    {
        turretE->setTargetID(_targetID);
    }

    Turret* turretF = static_cast<Turret*>(EntityManager::getInstance().getEntity(_turretFID));
    if (turretF != nullptr)
    {
        turretF->setTargetID(_targetID);
    }
}

void BossEnemy::setPosition(const iaVector3d& position)
{
    Entity::setPosition(position);

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
    if (transformNode != nullptr)
    {
        iaMatrixd matrix;
        transformNode->getMatrix(matrix);
        matrix._pos = position;
        transformNode->setMatrix(matrix);
    }
}

void BossEnemy::init()
{
    setHealth(500.0);
    setShield(200.0);

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformNode->translate(_sphere._center);
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
    physicsNode->setMaterial(iPhysics::getInstance().getMaterialID("entity"));
    physicsNode->setUserData(reinterpret_cast<const void*>(getID()));

    _scene->getRoot()->insertNode(transformNode);
    transformNode->insertNode(bodyTransform);
    transformNode->insertNode(physicsNode);
    bodyTransform->insertNode(bodyScale);
    bodyScale->insertNode(bodyModel);

    iNodeTransform* turretATransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    turretATransform->translate(0, 1.5, 0);
    transformNode->insertNode(turretATransform);
    Turret* turretA = static_cast<Turret*>(EntityManager::getInstance().createEntity(Turret::TYPE_NAME));
    turretA->setParentNode(turretATransform);
    turretA->setFraction(getFraction());
    turretA->setTargetID(_targetID);
    _turretAID = turretA->getID();

    iNodeTransform* turretBTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    turretBTransform->rotate(M_PI * 0.5, iaAxis::Z);
    turretBTransform->translate(0, 1.5, 0);
    transformNode->insertNode(turretBTransform);
    Turret* turretB = static_cast<Turret*>(EntityManager::getInstance().createEntity(Turret::TYPE_NAME));
    turretB->setParentNode(turretBTransform);
    turretB->setFraction(getFraction());
    turretB->setTargetID(_targetID);
    _turretBID = turretB->getID();

    iNodeTransform* turretCTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    turretCTransform->rotate(M_PI * -0.5, iaAxis::Z);
    turretCTransform->translate(0, 1.5, 0);
    transformNode->insertNode(turretCTransform);
    Turret* turretC = static_cast<Turret*>(EntityManager::getInstance().createEntity(Turret::TYPE_NAME));
    turretC->setParentNode(turretCTransform);
    turretC->setFraction(getFraction());
    turretC->setTargetID(_targetID);
    _turretCID = turretC->getID();

    iNodeTransform* turretDTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    turretDTransform->rotate(M_PI * 1.0, iaAxis::Z);
    turretDTransform->translate(0, 1.5, 0);
    transformNode->insertNode(turretDTransform);
    Turret* turretD = static_cast<Turret*>(EntityManager::getInstance().createEntity(Turret::TYPE_NAME));
    turretD->setParentNode(turretDTransform);
    turretD->setFraction(getFraction());
    turretD->setTargetID(_targetID);
    _turretDID = turretD->getID();

    iNodeTransform* turretETransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    turretETransform->rotate(M_PI * 0.5, iaAxis::X);
    turretETransform->translate(0, 1.5, 0);
    transformNode->insertNode(turretETransform);
    Turret* turretE = static_cast<Turret*>(EntityManager::getInstance().createEntity(Turret::TYPE_NAME));
    turretE->setParentNode(turretETransform);
    turretE->setFraction(getFraction());
    turretE->setTargetID(_targetID);
    _turretEID = turretE->getID();

    iNodeTransform* turretFTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    turretFTransform->rotate(M_PI * -0.5, iaAxis::X);
    turretFTransform->translate(0, 1.5, 0);
    transformNode->insertNode(turretFTransform);
    Turret* turretF = static_cast<Turret*>(EntityManager::getInstance().createEntity(Turret::TYPE_NAME));
    turretF->setParentNode(turretFTransform);
    turretF->setFraction(getFraction());
    turretF->setTargetID(_targetID);
    _turretFID = turretF->getID();
}

void BossEnemy::deinit()
{

}

Entity* BossEnemy::createInstance()
{
    return new BossEnemy();
}

void BossEnemy::hitBy(uint64 entityID)
{
    GameObject* gameObject = static_cast<GameObject*>(EntityManager::getInstance().getEntity(entityID));
    if (gameObject != nullptr &&
        gameObject->getFraction() != getFraction())
    {
        float32 shield = getShield();
        float32 health = getHealth();

        shield -= gameObject->getShieldDamage();
        
        if (shield <= 0)
        {
            shield = 0;

            health -= gameObject->getDamage();
            if (health <= 0)
            {
                health = 0;
            }
        }

        setShield(shield);
        setHealth(health);
    }
}

/*iaVector3d BossEnemy::updatePos()
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
}*/

void BossEnemy::handle()
{
    // nothing to do
}


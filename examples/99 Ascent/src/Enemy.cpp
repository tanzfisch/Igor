#include "Enemy.h"

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
#include <iVoxelTerrain.h>
using namespace Igor;

#include <iaString.h>
#include <iaConvert.h>
using namespace IgorAux;

#include "Turret.h"
#include "EnemyDestroyed.h"
#include "Ascent.h"

Enemy::Enemy(iScene* scene, iVoxelTerrain* voxelTerrain, const iaMatrixd& matrix, uint64 playerID)
    : GameObject(Fraction::Red, GameObjectType::Vehicle)
{
    _playerID = playerID;
    _scene = scene;
    _voxelTerrain = voxelTerrain;

    setHealth(100.0);
    setShield(50.0);

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformNode->setMatrix(matrix);
    _transformNodeID = transformNode->getID();

    iNodeTransform* bodyScale = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    bodyScale->scale(1,0.25,1);
    iNodeModel* bodyModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    bodyModel->setModel("crate.ompf");

    iaMatrixd offset;
    iNodePhysics* physicsNode = static_cast<iNodePhysics*>(iNodeFactory::getInstance().createNode(iNodeType::iNodePhysics));
    physicsNode->addBox(1,0.5,1, offset);
    physicsNode->finalizeCollision();
    physicsNode->setMass(10);
    physicsNode->setMaterial(Ascent::_entityMaterialID);
    physicsNode->setUserData(reinterpret_cast<const void*>(getID()));
    physicsNode->setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Enemy::onApplyForceAndTorque));
    physicsNode->setAngularDamping(iaVector3d(10000, 10000, 10000));
    physicsNode->setLinearDamping(100);

    bodyScale->insertNode(bodyModel);
    transformNode->insertNode(bodyScale);
    transformNode->insertNode(physicsNode);

    iNodeTransform* turretATransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    turretATransform->translate(0, 0.125, 0);
    transformNode->insertNode(turretATransform);
    Turret* turretA = new Turret(_scene, turretATransform, _voxelTerrain, getFraction(), _playerID);
    _turretAID = turretA->getID();

    iNodeTransform* turretBTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    turretBTransform->rotate(M_PI, iaAxis::Z);
    turretBTransform->translate(0, 0.125, 0);
    transformNode->insertNode(turretBTransform);
    Turret* turretB = new Turret(_scene, turretBTransform, _voxelTerrain, getFraction(), _playerID);
    _turretBID = turretB->getID();

    _scene->getRoot()->insertNodeAsync(transformNode);
}

Enemy::~Enemy()
{
    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
    if (transformNode != nullptr)
    {
        iaMatrixd matrix;
        transformNode->getMatrix(matrix);
        EnemyDestroyed* effect = new EnemyDestroyed(_scene, matrix);
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

    iNodeFactory::getInstance().destroyNodeAsync(_transformNodeID);
}

void Enemy::hitBy(uint64 entityID)
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

iaVector3d Enemy::getCurrentPos()
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

void Enemy::handle()
{
    if (_idleCounter > 0)
    {
        _idleCounter--;
    }
    else
    {
        const float32 detectionDistance = 40;
        const float32 approachDistance = 20;
        bool upperGunActive = true;

        GameObject* identifiedTarget = static_cast<GameObject*>(iEntityManager::getInstance().getEntity(_playerID));

        _force.set(0, 0, 0);

        if (identifiedTarget != nullptr)
        {
            iaVector3d targetPos = identifiedTarget->getSphere()._center;
            iaVector3I outside;
            iaVector3I inside;

            iaVector3d dir = targetPos;
            dir -= getSphere()._center;
            float64 distance = dir.length();

            if(distance < detectionDistance && 
                distance > approachDistance)
            {
                _force = dir;
                _force.normalize();
                _force *= 200;
            }
        }
        else
        {
            _idleCounter = rand() % 5 + 50;
        }
    }
}

void Enemy::onApplyForceAndTorque(iPhysicsBody* body, float32 timestep)
{  
    body->setForce(_force);
}
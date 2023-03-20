// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "Enemy.h"

#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodePhysics.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/resources/model/iModel.h>
#include <igor/scene/iScene.h>
#include <igor/physics/iPhysics.h>
#include <igor/physics/iPhysicsBody.h>
#include <igor/physics/iPhysicsJoint.h>
#include <igor/physics/iPhysicsCollision.h>
#include <igor/system/iTimer.h>
#include <igor/entities/iEntityManager.h>
#include <igor/terrain/iVoxelTerrain.h>
using namespace igor;

#include <iaux/data/iaString.h>
#include <iaux/data/iaConvert.h>
using namespace iaux;

#include "Turret.h"
#include "EnemyDestroyed.h"
#include "../Ascent.h"

Enemy::Enemy(iScenePtr scene, iVoxelTerrain *voxelTerrain, const iaMatrixd &matrix, uint64 playerID)
    : GameObject(Fraction::Red, GameObjectType::Vehicle)
{
    _playerID = playerID;
    _scene = scene;
    _voxelTerrain = voxelTerrain;

    setHealth(100.0);
    setShield(50.0);

    iNodeTransform *transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();
    transformNode->setMatrix(matrix);
    _transformNodeID = transformNode->getID();

    iNodeTransform *bodyScale = iNodeManager::getInstance().createNode<iNodeTransform>();
    bodyScale->scale(1, 0.25, 1);
    iNodeModel *bodyModel = iNodeManager::getInstance().createNode<iNodeModel>();
    bodyModel->setModel("cube.ompf");

    iaMatrixd offset;
    iNodePhysics *physicsNode = iNodeManager::getInstance().createNode<iNodePhysics>();
    physicsNode->addBox(1, 0.5, 1, offset);
    physicsNode->finalizeCollision();
    physicsNode->setMass(10);
    physicsNode->setMaterial(Ascent::_entityMaterialID);
    physicsNode->setUserData(getID());
    physicsNode->setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Enemy::onApplyForceAndTorque));
    physicsNode->setAngularDamping(iaVector3d(10000, 10000, 10000));
    physicsNode->setLinearDamping(100);

    bodyScale->insertNode(bodyModel);
    transformNode->insertNode(bodyScale);
    transformNode->insertNode(physicsNode);

    iNodeTransform *turretATransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    turretATransform->translate(0, 0.125, 0);
    transformNode->insertNode(turretATransform);
    Turret *turretA = new Turret(_scene, turretATransform, _voxelTerrain, getFraction(), _playerID);
    _turretAID = turretA->getID();

    iNodeTransform *turretBTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    turretBTransform->rotate(M_PI, iaAxis::Z);
    turretBTransform->translate(0, 0.125, 0);
    transformNode->insertNode(turretBTransform);
    Turret *turretB = new Turret(_scene, turretBTransform, _voxelTerrain, getFraction(), _playerID);
    _turretBID = turretB->getID();

    _scene->getRoot()->insertNodeAsync(transformNode);
}

Enemy::~Enemy()
{
    iNodeTransform *transformNode = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_transformNodeID));
    if (transformNode != nullptr)
    {
        iaMatrixd matrix;
        transformNode->getMatrix(matrix);
        EnemyDestroyed *effect = new EnemyDestroyed(_scene, matrix);
    }

    GameObject *turretA = static_cast<GameObject *>(iEntityManager::getInstance().getEntity(_turretAID));
    if (turretA != nullptr)
    {
        turretA->kill();
    }

    GameObject *turretB = static_cast<GameObject *>(iEntityManager::getInstance().getEntity(_turretBID));
    if (turretB != nullptr)
    {
        turretB->kill();
    }

    iNodeManager::getInstance().destroyNodeAsync(_transformNodeID);
}

void Enemy::hitBy(uint64 entityID)
{
    GameObject *entity = static_cast<GameObject *>(iEntityManager::getInstance().getEntity(entityID));
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
    iNodeTransform *transformNode = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_transformNodeID));
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

        GameObject *identifiedTarget = static_cast<GameObject *>(iEntityManager::getInstance().getEntity(_playerID));

        _force.set(0, 0, 0);

        if (identifiedTarget != nullptr)
        {
            iaVector3d targetPos = identifiedTarget->getSphere()._center;
            iaVector3I outside;
            iaVector3I inside;

            iaVector3d dir = targetPos;
            dir -= getSphere()._center;
            float64 distance = dir.length();

            if (distance < detectionDistance &&
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

void Enemy::onApplyForceAndTorque(iPhysicsBody *body, float32 timestep)
{
    body->setForce(_force);
}
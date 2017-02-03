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
using namespace Igor;

#include <iaString.h>
#include <iaConvert.h>
using namespace IgorAux;

#include "EntityManager.h"
#include "Turret.h"
#include "EnemyDestroyed.h"
#include "VoxelTerrainGenerator.h"

Enemy::Enemy(iScene* scene, const iaMatrixd& matrix, uint64 playerID)
    : Entity(Fraction::Red, EntityType::Vehicle)
{
    _playerID = playerID;
    _scene = scene;

    setHealth(100.0);
    setShield(50.0);

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformNode->setMatrix(matrix);
    _transformNodeID = transformNode->getID();

    iNodeTransform* bodyScale = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    bodyScale->scale(1,0.25,1);
    iNodeModel* bodyModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    bodyModel->setModel("crate.ompf", nullptr);

    iaMatrixd offset;
    iNodePhysics* physicsNode = static_cast<iNodePhysics*>(iNodeFactory::getInstance().createNode(iNodeType::iNodePhysics));
    physicsNode->addBox(1,0.5,1, offset);
    physicsNode->finalizeCollision();
    physicsNode->setMass(10);
    physicsNode->setMaterial(EntityManager::getInstance().getEntityMaterialID());
    physicsNode->setUserData(&_id);
    physicsNode->setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Enemy::onApplyForceAndTorque));
    physicsNode->setAngularDamping(iaVector3d(10000, 10000, 10000));
    physicsNode->setLinearDamping(100);

    _scene->getRoot()->insertNode(transformNode);
    bodyScale->insertNode(bodyModel);
    transformNode->insertNode(bodyScale);
    transformNode->insertNode(physicsNode);

    iNodeTransform* turretATransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    turretATransform->translate(0, 0.125, 0);
    transformNode->insertNode(turretATransform);
    Turret* turretA = new Turret(_scene, turretATransform, getFraction(), _playerID);
    _turretAID = turretA->getID();

    iNodeTransform* turretBTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    turretBTransform->rotate(M_PI, iaAxis::Z);
    turretBTransform->translate(0, 0.125, 0);
    transformNode->insertNode(turretBTransform);
    Turret* turretB = new Turret(_scene, turretBTransform, getFraction(), _playerID);
    _turretBID = turretB->getID();
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

    iNodeFactory::getInstance().destroyNodeAsync(_transformNodeID);
}

void Enemy::hitBy(uint64 entityID)
{
    Entity* entity = EntityManager::getInstance().getEntity(entityID);
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
    }
}

iaVector3d Enemy::updatePos()
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
    if (!VoxelTerrainGenerator::getInstance().isInstantiated())
    {
        return;
    }

    if (_idleCounter > 0)
    {
        _idleCounter--;
    }
    else
    {
        const float32 detectionDistance = 40;
        const float32 approachDistance = 20;
        bool upperGunActive = true;

        Entity* identifiedTarget = EntityManager::getInstance().getEntity(_playerID);

        _force.set(0, 0, 0);

        if (identifiedTarget != nullptr)
        {
            iaVector3d targetPos = identifiedTarget->getSphere()._center;
            iaVector3I outside;
            iaVector3I inside;

            iaVector3d dir = targetPos;
            dir -= getSphere()._center;
            float64 distance = dir.length();

            iaVector3I center;
            iaVector3I pos;
            iaConvert::convert(getSphere()._center, center);
            iaConvert::convert(targetPos, pos);

            VoxelTerrainGenerator::getInstance().castRay(center, pos, outside, inside);

            iaVector3d out;
            iaConvert::convert(outside, out);

            float64 distanceToWall = out.distance(getSphere()._center) + 5.0;
            if(distanceToWall > distance &&
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
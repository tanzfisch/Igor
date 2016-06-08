#include "StaticEnemy.h"

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

StaticEnemy::StaticEnemy(iScene* scene, const iaMatrixf& matrix, uint64 playerID)
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
    bodyScale->scale(1, 2, 1);

    iNodeModel* bodyModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    bodyModel->setModel("crate.ompf", nullptr);

    iaMatrixf offset;
    iNodePhysics* physicsNode = static_cast<iNodePhysics*>(iNodeFactory::getInstance().createNode(iNodeType::iNodePhysics));
    _physicsNodeID = physicsNode->getID();
    physicsNode->addSphere(2.0, offset);
    physicsNode->finalizeCollision();
    physicsNode->setMass(0);
    physicsNode->setMaterial(EntityManager::getInstance().getEntityMaterialID());
    physicsNode->setUserData(&_id);
    physicsNode->setAngularDamping(iaVector3f(10000, 10000, 10000));
    physicsNode->setLinearDamping(100);

    _scene->getRoot()->insertNode(transformNode);
    transformNode->insertNode(bodyTransform);
    bodyTransform->insertNode(bodyScale);
    bodyScale->insertNode(bodyModel);

    iNodeTransform* turretATransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    turretATransform->translate(0, 1.0, 0);
    transformNode->insertNode(turretATransform);
    Turret* turretA = new Turret(_scene, turretATransform, getFraction(), _playerID);
    _turretID = turretA->getID();
}

StaticEnemy::~StaticEnemy()
{
    Entity* turretA = EntityManager::getInstance().getEntity(_turretID);
    if (turretA != nullptr)
    {
        turretA->kill();
    }

    iNodeFactory::getInstance().destroyNode(_transformNodeID);
}

void StaticEnemy::hitBy(uint64 entityID)
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

iaVector3f StaticEnemy::updatePos()
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

void StaticEnemy::handle()
{
    // nothing to do
}


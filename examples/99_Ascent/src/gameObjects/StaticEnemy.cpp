#include "StaticEnemy.h"

#include <igor/graphics/scene/nodes/iNodeManager.h>
#include <igor/graphics/scene/nodes/iNodeTransform.h>
#include <igor/graphics/scene/nodes/iNodePhysics.h>
#include <igor/graphics/scene/nodes/iNodeModel.h>
#include <igor/resources/model/iModel.h>
#include <igor/graphics/scene/iScene.h>
#include <igor/physics/iPhysics.h>
#include <igor/physics/iPhysicsBody.h>
#include <igor/physics/iPhysicsJoint.h>
#include <igor/physics/iPhysicsCollision.h>
#include <igor/os/iTimer.h>
#include <igor/entities/iEntityManager.h>
using namespace Igor;

#include <iaux/data/iaString.h>
#include <iaux/data/iaConvert.h>
using namespace IgorAux;

#include "Turret.h"
#include "EnemyDestroyed.h"
#include "../Ascent.h"

StaticEnemy::StaticEnemy(iScene *scene, iVoxelTerrain *voxelTerrain, const iaMatrixd &matrix, uint64 playerID)
    : GameObject(Fraction::Red, GameObjectType::Vehicle)
{
    _playerID = playerID;
    _scene = scene;
    _voxelTerrain = voxelTerrain;

    setHealth(300.0);
    setShield(100.0);

    iNodeTransform *transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();
    transformNode->setMatrix(matrix);
    _transformNodeID = transformNode->getID();

    iNodeTransform *bodyTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    bodyTransform->translate(0, 0, 0);
    iNodeTransform *bodyScale = iNodeManager::getInstance().createNode<iNodeTransform>();
    bodyScale->scale(1, 2, 1);

    iNodeModel *bodyModel = iNodeManager::getInstance().createNode<iNodeModel>();
    bodyModel->setModel("crate.ompf");

    iaMatrixd offset;
    iNodePhysics *physicsNode = iNodeManager::getInstance().createNode<iNodePhysics>();
    _physicsNodeID = physicsNode->getID();
    physicsNode->addBox(1, 2, 1, offset);
    physicsNode->finalizeCollision();
    physicsNode->setMass(0);
    physicsNode->setMaterial(Ascent::_entityMaterialID);
    physicsNode->setUserData(getID());

    _scene->getRoot()->insertNode(transformNode);
    transformNode->insertNode(bodyTransform);
    transformNode->insertNode(physicsNode);
    bodyTransform->insertNode(bodyScale);
    bodyScale->insertNode(bodyModel);

    iNodeTransform *turretATransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    turretATransform->translate(0, 1.0, 0);
    transformNode->insertNode(turretATransform);
    Turret *turretA = new Turret(_scene, turretATransform, _voxelTerrain, getFraction(), _playerID);
    _turretID = turretA->getID();
}

StaticEnemy::~StaticEnemy()
{
    GameObject *turretA = static_cast<GameObject *>(iEntityManager::getInstance().getEntity(_turretID));
    if (turretA != nullptr)
    {
        turretA->kill();
    }

    iNodeManager::getInstance().destroyNodeAsync(_transformNodeID);
}

void StaticEnemy::hitBy(uint64 entityID)
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
    }

    if (getHealth() <= 0.0)
    {
        iNodeTransform *transformNode = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_transformNodeID));
        if (transformNode != nullptr)
        {
            iaMatrixd matrix;
            transformNode->getMatrix(matrix);
            EnemyDestroyed *effect = new EnemyDestroyed(_scene, matrix);
        }
    }

    if (getHealth() <= 0)
    {
        kill();
    }
}

iaVector3d StaticEnemy::getCurrentPos()
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

void StaticEnemy::handle()
{
    // nothing to do
}

// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include "Granade.h"

#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodePhysics.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/resources/model/iModel.h>
#include <igor/scene/iScene.h>
#include <igor/physics/iPhysics.h>
#include <igor/physics/iPhysicsBody.h>
#include <igor/physics/iPhysicsCollision.h>
#include <igor/entities/iEntityManager.h>
using namespace igor;

#include <iaux/data/iaString.h>
using namespace iaux;

#include "../Ascent.h"

Granade::Granade(iScenePtr scene, const iaMatrixd &matrix, Fraction fraction)
    : GameObject(fraction, GameObjectType::Weapon)
{
    _scene = scene;
    _parentFraction = fraction;

    _force = matrix._depth;
    _force.negate();
    _force._y += 0.5;

    _force.normalize();
    _force *= 3;

    iaMatrixd startMatrix;
    startMatrix = matrix;
    startMatrix._pos -= matrix._depth * 5;

    setHealth(100.0);
    setShield(100.0);
    setDamage(100.0);
    setShieldDamage(0.0);

    iNodeTransform *transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();
    transformNode->setMatrix(startMatrix);
    _transformNodeID = transformNode->getID();

    iNodeModel *bulletModel = iNodeManager::getInstance().createNode<iNodeModel>();
    bulletModel->setModel("cube.ompf");

    iaMatrixd offset;
    iNodePhysics *physicsNode = iNodeManager::getInstance().createNode<iNodePhysics>();
    physicsNode->addSphere(1, offset);
    physicsNode->finalizeCollision();
    physicsNode->setMass(0.1);
    physicsNode->setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Granade::onApplyForceAndTorque));
    physicsNode->setUserData(getID());
    physicsNode->setMaterial(Ascent::_bulletMaterialID);

    _scene->getRoot()->insertNode(transformNode);
    transformNode->insertNode(bulletModel);
    transformNode->insertNode(physicsNode);
}

Granade::~Granade()
{
    iNodeManager::getInstance().destroyNodeAsync(_transformNodeID);
}

void Granade::hitBy(uint64 entityID)
{
    kill();
}

iaVector3d Granade::getCurrentPos()
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

void Granade::handle()
{
    std::vector<uint64> detectedEntities;
    iSphered detectionSphere;
    detectionSphere._center.set(getSphere()._center._x, getSphere()._center._y, getSphere()._center._z);
    detectionSphere._radius = 1.0; // todo size of bullet granade etc.

    iEntityManager::getInstance().getEntities(detectionSphere, detectedEntities);

    if (detectedEntities.size() > 0)
    {
        for (auto entityID : detectedEntities)
        {
            if (entityID != getID())
            {
                GameObject *target = static_cast<GameObject *>(iEntityManager::getInstance().getEntity(entityID));
                if (target->getFraction() != getFraction())
                {
                    float32 targetHealth = target->getHealth();
                    float32 targetShield = target->getShield();
                    calcDamage(targetShield, targetHealth);
                    target->setHealth(targetHealth);
                    target->setShield(targetShield);

                    kill();
                    break;
                }
            }
        }
    }

    //setHealth(getHealth() - 0.5);
}

void Granade::calcDamage(float32 &shield, float32 &health)
{
    health -= 10;
    if (health < 0)
    {
        health = 0;
    }
}

void Granade::onApplyForceAndTorque(iPhysicsBody *body, float32 timestep)
{
    body->setForce(_force);
}
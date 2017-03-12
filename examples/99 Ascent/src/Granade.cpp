#include "Granade.h"

#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iNodePhysics.h>
#include <iNodeModel.h>
#include <iModel.h>
#include <iScene.h>
#include <iPhysics.h>
#include <iPhysicsBody.h>
#include <iPhysicsCollision.h>
#include <iEntityManager.h>
using namespace Igor;

#include <iaString.h>
using namespace IgorAux;

#include "Ascent.h"

Granade::Granade(iScene* scene, const iaMatrixd& matrix, Fraction fraction)
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

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformNode->setMatrix(startMatrix);
    _transformNodeID = transformNode->getID();

    iNodeModel* bulletModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    bulletModel->setModel("cube.ompf", nullptr);

    iaMatrixd offset;
    iNodePhysics* physicsNode = static_cast<iNodePhysics*>(iNodeFactory::getInstance().createNode(iNodeType::iNodePhysics));
    physicsNode->addSphere(1, offset);
    physicsNode->finalizeCollision();
    physicsNode->setMass(0.1);
    physicsNode->setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Granade::onApplyForceAndTorque));
    physicsNode->setUserData(reinterpret_cast<const void*>(getID()));
    physicsNode->setMaterial(Ascent::_bulletMaterialID);

    _scene->getRoot()->insertNode(transformNode);
    transformNode->insertNode(bulletModel);
    transformNode->insertNode(physicsNode);
}

Granade::~Granade()
{
    iNodeFactory::getInstance().destroyNodeAsync(_transformNodeID);
}

void Granade::hitBy(uint64 entityID)
{
    kill();
}

iaVector3d Granade::getCurrentPos()
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

void Granade::handle()
{
    vector<uint64> detectedEntities;
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
                GameObject* target = static_cast<GameObject*>(iEntityManager::getInstance().getEntity(entityID));
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

void Granade::calcDamage(float32& shield, float32& health)
{
    health -= 10;
    if (health < 0)
    {
        health = 0;
    }
}

void Granade::onApplyForceAndTorque(iPhysicsBody* body, float32 timestep)
{
    float32 Ixx;
    float32 Iyy;
    float32 Izz;
    float32 mass;
    iaVector3d force = _force;

 //   iPhysics::getInstance().getMassMatrix(static_cast<void*>(body->getNewtonBody()), mass, Ixx, Iyy, Izz);
    //force += iaVector3f(0.0f, -mass * static_cast<float32>(__IGOR_GRAVITY__), 0.0f);

    body->setForce(force);

  //  _force *= 0.98;
}
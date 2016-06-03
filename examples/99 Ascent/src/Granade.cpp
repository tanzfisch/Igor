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
using namespace Igor;

#include <iaString.h>
using namespace IgorAux;

#include "EntityManager.h"

Granade::Granade(iScene* scene, const iaMatrixf& matrix, Fraction fraction)
    : Entity(fraction, EntityType::Weapon)
{
    _scene = scene;
    _parentFraction = fraction;

    _force = matrix._depth;
    _force.negate();
    _force._y += 0.5;

    _force.normalize();
    _force *= 3;

    iaMatrixf startMatrix;
    startMatrix = matrix;
    startMatrix._pos -= matrix._depth * 5;

    setHealth(100.0);
    setShield(100.0);
    setDamage(100.0);
    setShieldDamage(100.0);

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformNode->setMatrix(startMatrix);
    _transformNodeID = transformNode->getID();

    iNodeModel* bulletModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    bulletModel->setModel("cube.ompf", nullptr);

    iaMatrixf offset;
    iNodePhysics* physicsNode = static_cast<iNodePhysics*>(iNodeFactory::getInstance().createNode(iNodeType::iNodePhysics));
    physicsNode->addSphere(1, offset);
    physicsNode->finalizeCollision();
    physicsNode->setMass(0.1);
    physicsNode->setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Granade::onApplyForceAndTorque));
    physicsNode->setUserData(&_id);
    physicsNode->setMaterial(EntityManager::getInstance().getBulletMaterialID());

    _scene->getRoot()->insertNode(transformNode);
    transformNode->insertNode(bulletModel);
    transformNode->insertNode(physicsNode);
}

Granade::~Granade()
{
    iNodeFactory::getInstance().destroyNode(_transformNodeID);
}

void Granade::hitBy(uint64 entityID)
{
    kill();
}

iaVector3f Granade::updatePos()
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

void Granade::handle()
{
    vector<uint64> detectedEntities;
    iSphered detectionSphere;
    detectionSphere._center.set(getSphere()._center._x, getSphere()._center._y, getSphere()._center._z);
    detectionSphere._radius = 1.0; // todo size of bullet granade etc.

    EntityManager::getInstance().getEntities(detectionSphere, detectedEntities);

    if (detectedEntities.size() > 0)
    {
        for (auto entityID : detectedEntities)
        {
            if (entityID != getID())
            {
                Entity* target = EntityManager::getInstance().getEntity(entityID);
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

void Granade::onApplyForceAndTorque(iPhysicsBody* body, float32 timestep, int threadIndex)
{
    float32 Ixx;
    float32 Iyy;
    float32 Izz;
    float32 mass;
    iaVector3f force = _force;

 //   iPhysics::getInstance().getMassMatrix(static_cast<void*>(body->getNewtonBody()), mass, Ixx, Iyy, Izz);
    //force += iaVector3f(0.0f, -mass * static_cast<float32>(__IGOR_GRAVITY__), 0.0f);

    body->setForce(force);

  //  _force *= 0.98;
}
#include "Enemy.h"

#include <iNodeFactory.h>
#include <iNodeTransform.h>
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

Enemy::Enemy(iScene* scene, const iaVector3f& pos)
    : Entity(2)
{
    _scene = scene;

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _transformNodeID = transformNode->getID();

    iNodeModel* playerModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    playerModel->setModel("crate.ompf");

    transformNode->insertNode(playerModel);
    transformNode->translate(pos);
    _scene->getRoot()->insertNode(transformNode);

    iaMatrixf offset;
    iPhysicsCollision* collisionBox = iPhysics::getInstance().createBox(1, 1, 1, offset);
    iPhysicsBody* body = iPhysics::getInstance().createBody(collisionBox);
    body->setMass(10);
    body->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Enemy::onApplyForceAndTorque));

    iPhysics::getInstance().bindTransformNode(body, transformNode);


    _bodyID = body->getID();
    iPhysics::getInstance().destroyCollision(collisionBox);
}

Enemy::~Enemy()
{

}

void Enemy::handle()
{
    uint64 targetID = 0;
    vector<uint64> detectedEntities;
    iSphered detectionSphere;
    detectionSphere._center.set(getSphere()._center._x, getSphere()._center._y, getSphere()._center._z);
    detectionSphere._radius = 2;

    EntityManager::getInstance().getEntities(detectionSphere, detectedEntities);

    if (detectedEntities.size() > 0)
    {
        for (auto entityID : detectedEntities)
        {
            if (entityID != getID())
            {
                targetID = entityID;
                break;
            }
        }

        if (targetID != 0)
        {
            Entity* target = EntityManager::getInstance().getEntity(targetID);
            iaVector3f targetPos = target->getSphere()._center;
            iaVector3f dir = getSphere()._center;
            dir -= targetPos;
            dir.negate();
            dir.normalize();

            iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
            if (body != nullptr)
            {
                iaVector3f force = dir * 200;
                body->setForce(force);
            }
        }
    }

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
    if (transformNode != nullptr)
    {
        iaMatrixf matrix;
        transformNode->getMatrix(matrix);
        _sphere._center = matrix._pos;
        syncPosition();
    }
}

void Enemy::onApplyForceAndTorque(iPhysicsBody* body, float32 timestep, int threadIndex)
{
    iaVector3f force;
    float32 Ixx;
    float32 Iyy;
    float32 Izz;
    float32 mass;

    //iPhysics::getInstance().getMassMatrixFromBody(static_cast<void*>(body->getNewtonBody()), mass, Ixx, Iyy, Izz);
    //force.set(0.0f, -mass * static_cast<float32>(__IGOR_GRAVITY__), 0.0f);
    force = body->getForce();

    iaVector3<float32> velocity;
    iPhysics::getInstance().getVelocity(body->getNewtonBody(), velocity);
    velocity.negate();
    force += (velocity / (1.0 / iPhysics::getSimulationRate())) * 0.5;

    iPhysics::getInstance().setForce(static_cast<void*>(body->getNewtonBody()), force);
}
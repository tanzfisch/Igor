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

    iaMatrixf bodyMatrix;
    bodyMatrix.translate(pos);

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformNode->setMatrix(bodyMatrix);
    _transformNodeID = transformNode->getID();

    iNodeModel* playerModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    playerModel->setModel("crate.ompf");

    iaMatrixf offset;
    iPhysicsCollision* collisionBox = iPhysics::getInstance().createBox(1, 1, 1, offset);
    iPhysicsBody* body = iPhysics::getInstance().createBody(collisionBox);
    _bodyID = body->getID();
    body->setMass(10);
    body->setMatrix(bodyMatrix);
    body->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Enemy::onApplyForceAndTorque));
    iPhysics::getInstance().createUpVectorJoint(body, iaVector3f(0, 1, 0));
    iPhysics::getInstance().createUpVectorJoint(body, iaVector3f(1, 0, 0));
    iPhysics::getInstance().destroyCollision(collisionBox);

    transformNode->insertNode(playerModel);
    _scene->getRoot()->insertNode(transformNode);

    iPhysics::getInstance().bindTransformNode(body, transformNode);

    _sphere._center = bodyMatrix._pos;
    syncPosition();
}

Enemy::~Enemy()
{

}

void Enemy::handle()
{
    if (_idleCounter > 0)
    {
        _idleCounter--;
    }
    else
    {
        Entity* identifiedTarget = nullptr;
        vector<uint64> detectedEntities;
        iSphered detectionSphere;
        detectionSphere._center.set(getSphere()._center._x, getSphere()._center._y, getSphere()._center._z);
        detectionSphere._radius = 10;

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
                        identifiedTarget = target;
                        break;
                    }
                }
            }

            iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
            if (body != nullptr)
            {
                if (identifiedTarget != nullptr)
                {
                    iaVector3f targetPos = identifiedTarget->getSphere()._center;
                    iaVector3f dir = getSphere()._center;
                    dir -= targetPos;
                    dir.negate();
                    dir.normalize();

                    iaVector3f force = dir * 200;
                    body->setForce(force);
                }
                else
                {
                    _idleCounter = rand() % 100;
                    body->setForce(iaVector3f());
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

    iaVector3f velocity = body->getVelocity();
    velocity.negate();
    force += (velocity / (1.0 / iPhysics::getSimulationRate())) * 0.5;

    iPhysics::getInstance().setForce(static_cast<void*>(body->getNewtonBody()), force);
}
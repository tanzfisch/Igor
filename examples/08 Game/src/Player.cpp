#include "Player.h"

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

Player::Player(iScene* scene, const iaVector3f& pos)
    : Entity(1)
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
    body->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Player::onApplyForceAndTorque));
    iPhysics::getInstance().bindTransformNode(body, transformNode);

    _bodyID = body->getID();
    iPhysics::getInstance().destroyCollision(collisionBox);
}

Player::~Player()
{

}

void Player::handle()
{
    iaVector3f force;
    const float32 speed = 1000;

    if (_up)
    {
        force += iaVector3f(0,0,-speed);
    }
    if (_down)
    {
        force += iaVector3f(0, 0, speed);
    }
    if (_left)
    {
        force += iaVector3f(-speed, 0, 0);
    }
    if (_right)
    {
        force += iaVector3f(speed, 0, 0);
    }

    iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
    if (body != nullptr)
    {
        body->setForce(force);
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

void Player::startUp()
{
    _up = true;
}

void Player::stopUp()
{
    _up = false;
}

void Player::startDown()
{
    _down = true;
}

void Player::stopDown()
{
    _down = false;
}

void Player::startLeft()
{
    _left = true;
}

void Player::stopLeft()
{
    _left = false;
}

void Player::startRight()
{
    _right = true;
}

void Player::stopRight()
{
    _right = false;
}


void Player::onApplyForceAndTorque(iPhysicsBody* body, float32 timestep, int threadIndex)
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
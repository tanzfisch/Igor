#include "Player.h"

#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iNodeModel.h>
#include <iModel.h>
#include <iScene.h>
#include <iPhysics.h>
#include <iPhysicsBody.h>
#include <iPhysicsCollision.h>
#include <iNodeCamera.h>
using namespace Igor;

#include <iaConsole.h>
#include <iaString.h>
using namespace IgorAux;

Player::Player(iScene* scene, const iaVector3f& pos)
    : Entity(1)
{
    _scene = scene;

    iaMatrixf bodyMatrix;
    bodyMatrix.translate(pos);

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformNode->setMatrix(bodyMatrix);
    _transformNodeID = transformNode->getID();

    iNodeModel* playerModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    playerModel->setModel("cat.ompf");

    iNodeCamera* camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
    iNodeTransform* cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    iNodeTransform* cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    iNodeTransform* cameraDistance = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));

    // camera
    cameraHeading->insertNode(cameraPitch);
    cameraPitch->insertNode(cameraDistance);
    cameraDistance->insertNode(camera);

    cameraPitch->rotate(-45.0f / 180.0f * M_PI, iaAxis::X);
    cameraDistance->translate(0, 0, 15);

    transformNode->insertNode(cameraHeading);

    iaMatrixf offset;
    iPhysicsCollision* collisionBox = iPhysics::getInstance().createBox(1, 1, 1, offset);
    iPhysicsBody* body = iPhysics::getInstance().createBody(collisionBox);
    _bodyID = body->getID();
    body->setMass(10);
    body->setMatrix(bodyMatrix);
    body->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Player::onApplyForceAndTorque));
    body->setLinearDamping(200);
    body->setAngularDamping(iaVector3f(200,200,200));
//    iPhysics::getInstance().createUpVectorJoint(body, iaVector3f(0, 1, 0));
    //iPhysics::getInstance().createUpVectorJoint(body, iaVector3f(1, 0, 0));
    iPhysics::getInstance().destroyCollision(collisionBox);

    transformNode->insertNode(playerModel);
    _scene->getRoot()->insertNode(transformNode);

    iPhysics::getInstance().bindTransformNode(body, transformNode);

    camera->makeCurrent();

    _sphere._center = bodyMatrix._pos;
    syncPosition();
}

Player::~Player()
{

}

void Player::handle()
{
    const float32 speed = 1000;

    _force.set(0, 0, 0);

    if (_up)
    {
        _force += iaVector3f(0,0,-speed);
    }
    if (_down)
    {
        _force += iaVector3f(0, 0, speed);
    }
    if (_left)
    {
        _force += iaVector3f(-speed, 0, 0);
    }
    if (_right)
    {
        _force += iaVector3f(speed, 0, 0);
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
    body->setForce(_force);
}
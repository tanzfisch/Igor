// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "Player.h"

#include <iPhysics.h>
#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iNodeCamera.h>
#include <iNodeLODTrigger.h>
#include <iScene.h>
#include <iNodeTransformControl.h>
#include <iEntityTransformControl.h>
#include <iMouse.h>
#include <iEntityFactory.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

Player::Player(iScene* scene)
{
    _scene = scene;

    _orientationNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraNode = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
    _camLODTriggerNode = static_cast<iNodeLODTrigger*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLODTrigger));
    _camLODTriggerNode->setName("camLODTrigger");

    scene->getRoot()->insertNode(_transformNode);
    _transformNode->insertNode(_orientationNode);
    _orientationNode->insertNode(_cameraNode);
    _cameraNode->insertNode(_camLODTriggerNode);
    _cameraNode->makeCurrent();

    _playerEntity = iEntityFactory::getInstance().createEntity();
    _playerEntity->setPosition(iaVector3f(10000, 10000, 10000));

    iaMatrixf offset;
    offset.translate(0, -0.5, 0);
    iPhysicsCollision* bottomSphere = iPhysics::getInstance().createSphere(1, offset);
    offset.translate(0, 0.5, 0);
    iPhysicsCollision* centerSphere = iPhysics::getInstance().createSphere(1, offset);
    offset.translate(0, 0.5, 0);
    iPhysicsCollision* topSphere = iPhysics::getInstance().createSphere(1, offset);

    vector<iPhysicsCollision*> collisions;
    collisions.push_back(bottomSphere);
    collisions.push_back(centerSphere);
    collisions.push_back(topSphere);

    iPhysicsCollision* fullBody = iPhysics::getInstance().createCompound(collisions);

    _playerBody = iPhysics::getInstance().createBody(fullBody);

    _playerEntity->setBody(_playerBody);
    _playerEntity->getBody()->setMass(5);
    _playerEntity->setTransformNode(_transformNode);
    _entityID = _playerEntity->getID();

    iPhysicsJoint* upVectorJoint = iPhysics::getInstance().createUpVectorJoint(_playerBody, iaVector3f(0, 1, 0));
    iPhysicsJoint* leftVectorJoint = iPhysics::getInstance().createUpVectorJoint(_playerBody, iaVector3f(1, 0, 0));

    _playerBody->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Player::onApplyForceAndTorque));

    /*offset.identity();
    offset.translate(0, -1, 0);
    iPhysicsBody* floor = iPhysics::getInstance().createBody(iPhysics::getInstance().createBox(1000, 2, 1000, offset));
    iaMatrixf position;
    position.translate(10000, 10000 - 5, 10000);
    floor->updateMatrix(position);/**/

    setKeyBinding(iKeyCode::Q, iTransformDirection::Up);
    setKeyBinding(iKeyCode::W, iTransformDirection::Foreward);
    setKeyBinding(iKeyCode::E, iTransformDirection::Down);
    setKeyBinding(iKeyCode::A, iTransformDirection::Left);
    setKeyBinding(iKeyCode::S, iTransformDirection::Backward);
    setKeyBinding(iKeyCode::D, iTransformDirection::Right);
    setKeyBinding(iKeyCode::MouseLeft, iTransformDirection::MouseHV);

    setFactor(iTransformDirection::Up, 1000);
    setFactor(iTransformDirection::Foreward, 1000);
    setFactor(iTransformDirection::Down, 1000);
    setFactor(iTransformDirection::Left, 1000);
    setFactor(iTransformDirection::Backward, 1000);
    setFactor(iTransformDirection::Right, 1000);

    iMouse::getInstance().registerMouseMoveFullDelegate(MouseMoveFullDelegate(this, &Player::onMouseMove));
}

void Player::getMatrix(iaMatrixf& matrix) const
{
    _cameraNode->getWorldMatrix(matrix);
}

Player::~Player()
{
    iMouse::getInstance().unregisterMouseMoveFullDelegate(MouseMoveFullDelegate(this, &Player::onMouseMove));
    _playerBody->unregisterForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Player::onApplyForceAndTorque));
    iEntityFactory::getInstance().destroyEntity(_entityID);
}

void Player::printPosition()
{
    iaMatrixf matrix;
    _cameraNode->getWorldMatrix(matrix);
    con_endl("cam pos: " << matrix._pos);
}

void Player::onApplyForceAndTorque(iPhysicsBody* body, float32 timestep, int threadIndex)
{
    float32 Ixx;
    float32 Iyy;
    float32 Izz;
    float32 mass;
    iaVector3f force;
    iaVector3f angularForce;

    if (body == _playerBody) // not sure if this can even fail
    {
        //iPhysics::getInstance().getMassMatrixFromBody(static_cast<void*>(body->getNewtonBody()), mass, Ixx, Iyy, Izz);
        //force.set(0.0f, -mass * static_cast<float32>(IGOR_GRAVITY), 0.0f);
        force += _playerEntity->getForce();

        iaVector3<float32> velocity;
        iPhysics::getInstance().getVelocity(body->getNewtonBody(), velocity);
        velocity.negate();
        force += (velocity / (1.0 / iPhysics::getSimulationRate())) * 0.9;

        iPhysics::getInstance().setForce(static_cast<void*>(body->getNewtonBody()), force);
    }
}

iNodeLODTrigger* Player::getLODTrigger()
{
    return _camLODTriggerNode;
}

void Player::onHandle()
{
    iEntity* entity = iEntityFactory::getInstance().getEntity(_entityID);
    if (entity != nullptr)
    {
        iaMatrixf matrix;
        entity->getMatrix(matrix);

        iaVector3f resultingForce;
        iaVector3f resultingAngularForce;

        if (_activeDir[iTransformDirection::RotateLeft])
        {
            _heading.rotate(_factorDir[iTransformDirection::RotateLeft], iaAxis::Y);
            resultingAngularForce._y += 1.0f * _factorDir[iTransformDirection::RotateLeft];
        }

        if (_activeDir[iTransformDirection::RotateRight])
        {
            _heading.rotate(-_factorDir[iTransformDirection::RotateRight], iaAxis::Y);
            resultingAngularForce._y -= 1.0f * _factorDir[iTransformDirection::RotateRight];
        }

        if (_activeDir[iTransformDirection::RotateUp])
        {
            _pitch.rotate(_factorDir[iTransformDirection::RotateUp], iaAxis::X);
            resultingAngularForce._x += 1.0f * _factorDir[iTransformDirection::RotateUp];
        }

        if (_activeDir[iTransformDirection::RotateDown])
        {
            _pitch.rotate(-_factorDir[iTransformDirection::RotateDown], iaAxis::X);
            resultingAngularForce._x -= 1.0f * _factorDir[iTransformDirection::RotateDown];
        }

        if (_activeDir[iTransformDirection::RollLeft])
        {
            _roll.rotate(_factorDir[iTransformDirection::RollLeft], iaAxis::Z);
            resultingAngularForce._z += 1.0f * _factorDir[iTransformDirection::RollLeft];
        }

        if (_activeDir[iTransformDirection::RollRight])
        {
            _roll.rotate(-_factorDir[iTransformDirection::RollRight], iaAxis::Z);
            resultingAngularForce._z -= 1.0f * _factorDir[iTransformDirection::RollRight];
        }

        iaMatrixf orientation = _heading;
        orientation *= _pitch;
        orientation *= _roll;

        _orientationNode->setMatrix(orientation);

        if (_activeDir[iTransformDirection::Foreward])
        {
            iaVector3f foreward = orientation._depth;
            foreward._y = 0;
            if (foreward.length2() > 0)
            {
                foreward.negate();
                foreward.normalize();
                foreward *= _factorDir[iTransformDirection::Foreward];
                resultingForce += foreward;
            }
        }

        if (_activeDir[iTransformDirection::Backward])
        {
            iaVector3f backward = orientation._depth;
            backward._y = 0;
            if (backward.length2() > 0)
            {
                backward.normalize();
                backward *= _factorDir[iTransformDirection::Backward];
                resultingForce += backward;
            }
        }

        if (_activeDir[iTransformDirection::Left])
        {
            iaVector3f left = orientation._right;
            left._y = 0;
            if (left.length2() > 0)
            {
                left.negate();
                left.normalize();
                left *= _factorDir[iTransformDirection::Left];
                resultingForce += left;
            }
        }

        if (_activeDir[iTransformDirection::Right])
        {
            iaVector3f right = orientation._right;
            right._y = 0;
            if (right.length2() > 0)
            {
                right.normalize();
                right *= _factorDir[iTransformDirection::Right];
                resultingForce += right;
            }
        }

        if (_activeDir[iTransformDirection::Up])
        {
            iaVector3f up(0, 1, 0);
            up *= _factorDir[iTransformDirection::Up];
            resultingForce += up;
        }

        if (_activeDir[iTransformDirection::Down])
        {
            iaVector3f down(0, -1, 0);
            down *= _factorDir[iTransformDirection::Down];
            resultingForce += down;
        }

        entity->setForce(resultingForce);
    }
}

void Player::onMouseMove(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window)
{
    if (_activeDir[iTransformDirection::MouseHV])
    {
        _pitchAngle += (y1 - y2) * _factorDir[iTransformDirection::MouseHV];

        if (_pitchAngle > M_PI * 0.495)
        {
            _pitchAngle = M_PI * 0.495;
        }

        if (_pitchAngle < -(M_PI * 0.495))
        {
            _pitchAngle = -(M_PI * 0.495);
        }

        _pitch.identity();
        _pitch.rotate(_pitchAngle, iaAxis::X);

        _headingAngle += (x1 - x2) * _factorDir[iTransformDirection::MouseHV];
        _heading.identity();
        _heading.rotate(_headingAngle, iaAxis::Y);

        iMouse::getInstance().setCenter(true);
    }
}


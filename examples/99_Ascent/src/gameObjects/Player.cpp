// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "Player.h"

#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodePhysics.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/resources/model/iModel.h>
#include <igor/scene/iScene.h>
#include <igor/physics/iPhysics.h>
#include <igor/physics/iPhysicsBody.h>
#include <igor/physics/iPhysicsJoint.h>
#include <igor/physics/iPhysicsCollision.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeLODTrigger.h>
#include <igor/renderer/iView.h>
#include <igor/system/iWindow.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/scene/nodes/iNodeEmitter.h>
#include <igor/system/iTimer.h>
#include <igor/entities/iEntityManager.h>
using namespace igor;

#include <iaux/system/iaConsole.h>
#include <iaux/data/iaString.h>
using namespace iaux;

#include "Bullet.h"
#include "Granade.h"
#include "DigEffect.h"
#include "MuzzleFlash.h"
#include "../Ascent.h"

Player::Player(iScenePtr scene, iView *view, const iaMatrixd &matrix)
    : GameObject(Fraction::Blue, GameObjectType::Vehicle)
{
    _scene = scene;

    setHealth(2000.0);
    setShield(3000.0);
    setDamage(1.0);
    setShieldDamage(1.0);

    iNodeTransform *transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();
    transformNode->setMatrix(matrix);
    _transformNodeID = transformNode->getID();

    iNodeTransform *transformCam = iNodeManager::getInstance().createNode<iNodeTransform>();
    _transformCamNodeID = transformCam->getID();

    iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    _cameraNodeID = camera->getID();
    iNodeLODTrigger *lodTrigger = iNodeManager::getInstance().createNode<iNodeLODTrigger>();
    _lodTriggerID = lodTrigger->getID();

    iaMatrixd offset;
    iNodePhysics *physicsNode = iNodeManager::getInstance().createNode<iNodePhysics>();
    _physicsNodeID = physicsNode->getID();
    physicsNode->addSphere(1, offset);
    physicsNode->finalizeCollision();
    physicsNode->setMass(10);
    physicsNode->setMaterial(Ascent::_entityMaterialID);
    physicsNode->setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Player::onApplyForceAndTorque));
    physicsNode->setUserData(getID());
    physicsNode->setAngularDamping(iaVector3d(100000, 100000, 100000));
    physicsNode->setLinearDamping(500);

    iNodeTransform *transformRecoilLeftGun = iNodeManager::getInstance().createNode<iNodeTransform>();
    _transformRecoilLeftGun = transformRecoilLeftGun->getID();

    iNodeTransform *transformRecoilRightGun = iNodeManager::getInstance().createNode<iNodeTransform>();
    _transformRecoilRightGun = transformRecoilRightGun->getID();

    iNodeTransform *transformLeftGun = iNodeManager::getInstance().createNode<iNodeTransform>();
    transformLeftGun->translate(-0.5, -0.4, -0.75);
    transformLeftGun->scale(0.1, 0.1, 1);
    iNodeModel *leftgun = iNodeManager::getInstance().createNode<iNodeModel>();
    leftgun->setModel("crate.ompf");

    iNodeTransform *transformRightGun = iNodeManager::getInstance().createNode<iNodeTransform>();
    transformRightGun->translate(0.5, -0.4, -0.75);
    transformRightGun->scale(0.1, 0.1, 1);
    iNodeModel *rightgun = iNodeManager::getInstance().createNode<iNodeModel>();
    rightgun->setModel("crate.ompf");

    iNodeTransform *transformLeftGunEmitter = iNodeManager::getInstance().createNode<iNodeTransform>();
    transformLeftGunEmitter->translate(-0.5, -0.4, -1.25);

    iNodeTransform *transformRightGunEmitter = iNodeManager::getInstance().createNode<iNodeTransform>();
    transformRightGunEmitter->translate(0.5, -0.4, -1.25);

    iNodeEmitter *emitterLeftGun = iNodeManager::getInstance().createNode<iNodeEmitter>();
    _emitterLeftGunNodeID = emitterLeftGun->getID();
    emitterLeftGun->setEmitterType(iEmitterType::Point);

    iNodeEmitter *emitterRightGun = iNodeManager::getInstance().createNode<iNodeEmitter>();
    _emitterRightGunNodeID = emitterRightGun->getID();
    emitterRightGun->setEmitterType(iEmitterType::Point);

    transformNode->insertNode(transformRecoilLeftGun);
    transformNode->insertNode(transformRecoilRightGun);

    transformRecoilLeftGun->insertNode(transformLeftGun);
    transformRecoilRightGun->insertNode(transformRightGun);

    transformRecoilLeftGun->insertNode(transformLeftGunEmitter);
    transformRecoilRightGun->insertNode(transformRightGunEmitter);

    transformLeftGun->insertNode(leftgun);
    transformRightGun->insertNode(rightgun);

    transformLeftGunEmitter->insertNode(emitterLeftGun);
    transformRightGunEmitter->insertNode(emitterRightGun);

    transformCam->insertNode(camera);
    transformNode->insertNode(transformCam);
    transformNode->insertNode(physicsNode);
    camera->insertNode(lodTrigger);
    _scene->getRoot()->insertNode(transformNode);

    view->setCurrentCamera(_cameraNodeID);

    _materialSolid = iMaterialResourceFactory_old::getInstance().createMaterial("Solid");
    iMaterialResourceFactory_old::getInstance().getMaterial(_materialSolid)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory_old::getInstance().getMaterial(_materialSolid)->setRenderState(iRenderState::Blend, iRenderStateValue::On);

    _primaryWeaponTime = iTimer::getInstance().getTime();
}

Player::~Player()
{
    iNodeManager::getInstance().destroyNodeAsync(_transformNodeID);

    con_endl("player dead");
}

void Player::hitBy(uint64 entityID)
{
    return;

    GameObject *gameObject = static_cast<GameObject *>(iEntityManager::getInstance().getEntity(entityID));
    if (gameObject != nullptr &&
        gameObject->getFraction() != getFraction())
    {
        float32 shield = getShield();
        float32 health = getHealth();

        shield -= gameObject->getShieldDamage();

        if (shield <= 0)
        {
            shield = 0;

            health -= gameObject->getDamage();
            if (health <= 0)
            {
                health = 0;
            }
        }

        setShield(shield);
        setHealth(health);

        if (getHealth() <= 0)
        {
            kill();
        }
    }
}

iaVector3I Player::getGunPointPosition()
{

    return iaVector3I();
}

void Player::shootSecondaryWeapon(iView &view, const iaVector3d &screenCoordinates)
{
    iNodeTransform *transformationNode = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_transformNodeID));
    if (transformationNode != nullptr)
    {
        iaMatrixd matrix;
        transformationNode->getMatrix(matrix);
        matrix._pos = getSphere()._center;

        Granade *bullet = new Granade(_scene, matrix, getFraction());
    }
}

void Player::shootPrimaryWeapon(iView &view, const iaVector3d &screenCoordinates)
{
    iaTime currentTime = iTimer::getInstance().getTime();
    if (currentTime > _primaryWeaponTime + iaTime::fromMilliseconds(100))
    {
        iNodeTransform *transformNode = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_transformNodeID));
        if (transformNode != nullptr)
        {
            iaMatrixd worldMatrix;
            transformNode->calcWorldTransformation(worldMatrix);

            iaVector3d pos = view.unProject(screenCoordinates, worldMatrix);

            iaVector3d topScreen = screenCoordinates;
            topScreen._y -= 1;
            iaVector3d top = view.unProject(topScreen, worldMatrix);
            top -= pos;

            iaVector3d depthScreen = screenCoordinates;
            depthScreen._z = -1;
            iaVector3d depth = view.unProject(depthScreen, worldMatrix);
            depth -= pos;

            iaMatrixd matrix;

            matrix.grammSchmidt(depth, top);
            matrix._pos = pos;

            matrix = worldMatrix * matrix;

            iaMatrixd offsetLeft = matrix;
            offsetLeft.translate(-0.5, -0.4, -1.5);

            iaMatrixd offsetRight = matrix;
            offsetRight.translate(0.5, -0.4, -1.5);

            new Bullet(_scene, _force * 0.00001, offsetLeft, getFraction());
            new Bullet(_scene, _force * 0.00001, offsetRight, getFraction());

            new MuzzleFlash(_scene, _emitterLeftGunNodeID);
            new MuzzleFlash(_scene, _emitterRightGunNodeID);

            iNodeTransform *transformRecoilLeftGun = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_transformRecoilLeftGun));
            iNodeTransform *transformRecoilRightGun = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_transformRecoilRightGun));

            if (transformRecoilLeftGun != nullptr &&
                transformRecoilRightGun != nullptr)
            {
                iaMatrixd matrix;
                transformRecoilLeftGun->getMatrix(matrix);
                matrix._pos += iaVector3d(0, 0, 0.25);
                transformRecoilLeftGun->setMatrix(matrix);

                transformRecoilRightGun->getMatrix(matrix);
                matrix._pos += iaVector3d(0, 0, 0.25);
                transformRecoilRightGun->setMatrix(matrix);
            }
        }

        _primaryWeaponTime = currentTime;
    }
}

uint32 Player::getLODTriggerID()
{
    return _lodTriggerID;
}

iaVector3d Player::getCurrentPos()
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

void Player::drawReticle(iWindowPtr window)
{
    iaVector3f weaponPos(window->getClientWidth() * 0.5, window->getClientHeight() * 0.5, 0);

    float32 scale = 0.001 * window->getClientWidth();

    iRenderer::getInstance().setMaterial(_materialSolid);
    // TODO iRenderer::getInstance().setLineWidth(1 * scale);

    // TODO iRenderer::getInstance().setColor(iaColor4f(1, 0, 0, 1));
    // TODO iRenderer::getInstance().drawLine(weaponPos + iaVector3f(-10 * scale, 0, 0), weaponPos + iaVector3f(10 * scale, 0, 0));
    // TODO iRenderer::getInstance().drawLine(weaponPos + iaVector3f(0, -10 * scale, 0), weaponPos + iaVector3f(0, 10 * scale, 0));
}

void Player::handle()
{
    float32 speed = 500;

    const float32 offsetIncrease = 0.1;
    iaMatrixd matrix;
    iaVector3d resultingForce;

    iNodeTransform *transformationNode = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_transformNodeID));
    transformationNode->getMatrix(matrix);

    if (_forward)
    {
        iaVector3d foreward = matrix._depth;
        foreward.negate();
        foreward.normalize();
        foreward *= speed;
        resultingForce += foreward;
    }

    if (_backward)
    {
        iaVector3d backward = matrix._depth;
        backward.normalize();
        backward *= speed;
        resultingForce += backward;
    }

    if (_left)
    {
        iaVector3d left = matrix._right;
        left.negate();
        left.normalize();
        left *= speed;
        resultingForce += left;
    }

    if (_right)
    {
        iaVector3d right = matrix._right;
        right.normalize();
        right *= speed;
        resultingForce += right;
    }

    if (_up)
    {
        iaVector3d up = matrix._top;
        up.normalize();
        up *= speed;
        resultingForce += up;
    }

    if (_down)
    {
        iaVector3d down = matrix._top;
        down.negate();
        down.normalize();
        down *= speed;
        resultingForce += down;
    }

    if (_rollLeft)
    {
        _torque._z = 50.0;
    }
    else if (_rollRight)
    {
        _torque._z = -50.0;
    }
    else
    {
        _torque._z = 0.0;
    }

    _force = resultingForce;

    iNodeTransform *transformCam = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_transformCamNodeID));
    if (transformCam != nullptr)
    {
        iaMatrixd camMatrix;
        iaMatrixd transformInvMatrix = matrix;
        transformInvMatrix.invert();
        transformCam->getMatrix(camMatrix);
        iaVector3d camForce = transformInvMatrix * resultingForce;
        camForce -= transformInvMatrix._pos;
        camForce *= 0.00004;
        camMatrix._pos += camForce;
        camMatrix._pos *= 0.9;
        transformCam->setMatrix(camMatrix);
    }

    iNodeTransform *transformRecoilLeftGun = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_transformRecoilLeftGun));
    iNodeTransform *transformRecoilRightGun = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_transformRecoilRightGun));

    if (transformRecoilLeftGun != nullptr &&
        transformRecoilRightGun != nullptr)
    {
        iaMatrixd matrix;
        transformRecoilLeftGun->getMatrix(matrix);
        matrix._pos *= 0.95;
        transformRecoilLeftGun->setMatrix(matrix);

        transformRecoilRightGun->getMatrix(matrix);
        matrix._pos *= 0.95;
        transformRecoilRightGun->setMatrix(matrix);
    }
}

void Player::rotate(float32 heading, float32 pitch)
{
    iNodeTransform *transformationNode = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_transformNodeID));
    iaMatrixd matrix;

    transformationNode->getMatrix(matrix);
    matrix._pos.set(0, 0, 0);

    if (_fastTurn)
    {
        _torque.set(pitch * 700.0, heading * 700.0, _torque._z);
    }
    else
    {
        _torque.set(pitch * 400.0, heading * 400.0, _torque._z);
    }
    _torque = matrix * _torque;
}

void Player::startFastTurn()
{
    _fastTurn = true;
}

void Player::stopFastTurn()
{
    _fastTurn = false;
}

void Player::startForward()
{
    _forward = true;
}

void Player::stopForward()
{
    _forward = false;
}

void Player::startBackward()
{
    _backward = true;
}

void Player::stopBackward()
{
    _backward = false;
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

void Player::startRollLeft()
{
    _rollLeft = true;
}

void Player::stopRollLeft()
{
    _rollLeft = false;
}

void Player::startRollRight()
{
    _rollRight = true;
}

void Player::stopRollRight()
{
    _rollRight = false;
}

void Player::onApplyForceAndTorque(iPhysicsBody *body, float32 timestep)
{
    body->setForce(_force);
    body->setTorque(_torque);
}
#include "Player.h"

#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iNodePhysics.h>
#include <iNodeModel.h>
#include <iModel.h>
#include <iScene.h>
#include <iPhysics.h>
#include <iPhysicsBody.h>
#include <iPhysicsJoint.h>
#include <iPhysicsCollision.h>
#include <iNodeCamera.h>
#include <iNodeLODTrigger.h>
#include <iRenderer.h>
#include <iView.h>
#include <iWindow.h>
#include <iMaterialResourceFactory.h>
#include <iNodeEmitter.h>
#include <iTimer.h>
#include <iEntityFactory.h>
using namespace Igor;

#include <iaMatrix.h>
#include <iaConsole.h>
#include <iaString.h>
using namespace IgorAux;

//#include "Bullet.h"
//#include "Granade.h"
//#include "EntityManager.h"
#include "VoxelTerrainGenerator.h"
//#include "DigEffect.h"
//#include "MuzzleFlash.h"

iaString Player::TYPE_NAME("Player");

void Player::init()
{
    _scene = iEntityFactory::getInstance().getScene();

    /*setHealth(200.0);
    setShield(300.0);
    setDamage(1.0);
    setShieldDamage(1.0);*/

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _transformNodeID = transformNode->getID();

    iNodeTransform* transformCam = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _transformCamNodeID = transformCam->getID();

    iNodeCamera* camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
    _cameraNodeID = camera->getID();
    iNodeLODTrigger* lodTrigger = static_cast<iNodeLODTrigger*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLODTrigger));
    _lodTriggerID = lodTrigger->getID();

    iaMatrixd offset;
    iNodePhysics* physicsNode = static_cast<iNodePhysics*>(iNodeFactory::getInstance().createNode(iNodeType::iNodePhysics));
    _physicsNodeID = physicsNode->getID();
    physicsNode->addSphere(1, offset);
    physicsNode->finalizeCollision();
    physicsNode->setMass(10);
    // physicsNode->setMaterial(EntityManager::getInstance().getEntityMaterialID());
    physicsNode->setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Player::onApplyForceAndTorque));
    physicsNode->setUserData(reinterpret_cast<const void*>(getID()));
    physicsNode->setAngularDamping(iaVector3d(100000, 100000, 100000));
    physicsNode->setLinearDamping(500);

    iNodeTransform* transformRecoilLeftGun = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _transformRecoilLeftGun = transformRecoilLeftGun->getID();

    iNodeTransform* transformRecoilRightGun = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _transformRecoilRightGun = transformRecoilRightGun->getID();

    iNodeTransform* transformLeftGun = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformLeftGun->translate(-0.5, -0.4, -0.75);
    transformLeftGun->scale(0.1, 0.1, 1);
    iNodeModel* leftgun = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    leftgun->setModel("crate.ompf", nullptr);

    iNodeTransform* transformRightGun = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformRightGun->translate(0.5, -0.4, -0.75);
    transformRightGun->scale(0.1, 0.1, 1);
    iNodeModel* rightgun = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    rightgun->setModel("crate.ompf", nullptr);

    iNodeTransform* transformLeftGunEmitter = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformLeftGunEmitter->translate(-0.5, -0.4, -1.25);

    iNodeTransform* transformRightGunEmitter = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformRightGunEmitter->translate(0.5, -0.4, -1.25);

    iNodeEmitter* emitterLeftGun = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
    _emitterLeftGunNodeID = emitterLeftGun->getID();
    emitterLeftGun->setType(iEmitterType::Point);

    iNodeEmitter* emitterRightGun = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
    _emitterRightGunNodeID = emitterRightGun->getID();
    emitterRightGun->setType(iEmitterType::Point);

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

    camera->makeCurrent();

    _primaryWeaponTime = iTimer::getInstance().getTime();
}

void Player::deinit()
{
    iNodeFactory::getInstance().destroyNodeAsync(_transformNodeID);

    con_endl("player dead");
}


iEntity* Player::createInstance()
{
    return new Player();
}

void Player::hitBy(uint64 entityID)
{
/*    Entity* entity = EntityManager::getInstance().getEntity(entityID);
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
    }*/
}

iaVector3I Player::getGunPointPosition()
{
    iNodeCamera* camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().getNode(_cameraNodeID));
    if (camera != nullptr)
    {
        iaMatrixd modelMatrix;
        camera->getWorldMatrix(modelMatrix);

        iaVector3d dir(modelMatrix._depth._x, modelMatrix._depth._y, modelMatrix._depth._z);
        dir.negate();
        dir *= 100.0f;
        iaVector3d from(modelMatrix._pos._x, modelMatrix._pos._y, modelMatrix._pos._z);
        iaVector3d to(from);
        to += dir;

        if (from._x > 0 &&
            from._y > 0 &&
            from._z > 0 &&
            to._x > 0 &&
            to._y > 0 &&
            to._z > 0)
        {
            iaVector3I f(static_cast<int64>(from._x + 0.5), static_cast<int64>(from._y + 0.5), static_cast<int64>(from._z + 0.5));
            iaVector3I t(static_cast<int64>(to._x + 0.5), static_cast<int64>(to._y + 0.5), static_cast<int64>(to._z + 0.5));
            iaVector3I outside;
            iaVector3I inside;

            VoxelTerrainGenerator::getInstance().castRay(f, t, outside, inside);

            return outside;
        }
    }
}

void Player::dig(uint64 toolSize, uint8 toolDensity)
{
    iaVector3I center = getGunPointPosition();
    if (center.length2() > 0)
    {
        int64 toolRadius = toolSize / 2;
        int64 toolRadiusQuadric = toolRadius * toolRadius;

        iaVector3I modifyFrom(center);
        modifyFrom._x -= toolRadius;
        modifyFrom._y -= toolRadius;
        modifyFrom._z -= toolRadius;

        iaVector3I modifyTo(center);
        modifyTo._x += toolRadius;
        modifyTo._y += toolRadius;
        modifyTo._z += toolRadius;

        iaMatrixd effectMatrix;
        effectMatrix.translate(center._x, center._y, center._z);
        //new DigEffect(_scene, effectMatrix);

        iaVector3I pos;

        for (int x = modifyFrom._x; x <= modifyTo._x; ++x)
        {
            for (int y = modifyFrom._y; y <= modifyTo._y; ++y)
            {
                for (int z = modifyFrom._z; z <= modifyTo._z; ++z)
                {
                    pos.set(x, y, z);

                    if (center.distance2(pos) < toolRadiusQuadric)
                    {
                        VoxelTerrainGenerator::getInstance().setVoxelDensity(pos, toolDensity);
                    }
                }
            }
        }

        modifyFrom._x -= 32;
        modifyFrom._y -= 32;
        modifyFrom._z -= 32;
        modifyTo._x += 32;
        modifyTo._y += 32;
        modifyTo._z += 32;

        modifyFrom /= 32;
        modifyTo /= 32;

        iaVector3I tilePosition = center / 32;

        for (int x = modifyFrom._x; x <= modifyTo._x; ++x)
        {
            for (int y = modifyFrom._y; y <= modifyTo._y; ++y)
            {
                for (int z = modifyFrom._z; z <= modifyTo._z; ++z)
                {
                    iaVector3I pos(x, y, z);
                    VoxelTerrainGenerator::getInstance().refreshTile(pos);
                }
            }
        }
    }
}

void Player::shootSecondaryWeapon(iView& view, const iaVector3d& screenCoordinates)
{
/*    iNodeTransform* transformationNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
    if (transformationNode != nullptr)
    {
        iaMatrixd matrix;
        transformationNode->getMatrix(matrix);
        matrix._pos = getSphere()._center;

        Granade* bullet = new Granade(_scene, matrix, getFraction());
    }*/
}

void Player::shootPrimaryWeapon(iView& view, const iaVector3d& screenCoordinates)
{
    float64 currentTime = iTimer::getInstance().getTime();
    if (currentTime > _primaryWeaponTime + 100)
    {
        iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
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
            offsetLeft.translate(-0.5, -0.4, -1.0);

            iaMatrixd offsetRight = matrix;
            offsetRight.translate(0.5, -0.4, -1.0);

/*            new Bullet(_scene, _force * 0.001, offsetLeft, getFraction());
            new Bullet(_scene, _force * 0.001, offsetRight, getFraction());

            new MuzzleFlash(_scene, _emitterLeftGunNodeID);
            new MuzzleFlash(_scene, _emitterRightGunNodeID);*/

            iNodeTransform* transformRecoilLeftGun = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformRecoilLeftGun));
            iNodeTransform* transformRecoilRightGun = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformRecoilRightGun));

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

void Player::setPosition(const iaVector3d& position)
{
    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
    if (transformNode != nullptr)
    {
        iaMatrixd matrix;
        transformNode->getMatrix(matrix);
        matrix._pos = position;
        transformNode->setMatrix(matrix);

        updatePosition();
    }
}

void Player::updatePosition()
{
    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
    if (transformNode != nullptr)
    {
        iaMatrixd matrix;
        transformNode->getMatrix(matrix);
        _position = matrix._pos;
    }
}

void Player::handle()
{
    float32 speed = 500;

    const float32 offsetIncrease = 0.1;
    iaMatrixd matrix;
    iaVector3d resultingForce;

    iNodeTransform* transformationNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
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

    iNodeTransform* transformCam = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformCamNodeID));
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

    iNodeTransform* transformRecoilLeftGun = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformRecoilLeftGun));
    iNodeTransform* transformRecoilRightGun = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformRecoilRightGun));

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
    iNodeTransform* transformationNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
    if (transformationNode != nullptr)
    {
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

void Player::onApplyForceAndTorque(iPhysicsBody* body, float32 timestep, int threadIndex)
{
    body->setForce(_force);
    body->setTorque(_torque);
}
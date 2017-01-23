#include "Turret.h"

#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iNodeModel.h>
#include <iModel.h>
#include <iScene.h>
#include <iPhysics.h>
#include <iTimer.h>
using namespace Igor;

#include <iaString.h>
#include <iaConvert.h>
using namespace IgorAux;

#include "EntityManager.h"
#include "Bullet.h"
#include "Granade.h"
#include "VoxelTerrainGenerator.h"

iaString Turret::TYPE_NAME("Turret");

Turret::Turret()
    : GameObject(GameObjectKind::None)
{
}

Turret::~Turret()
{
    iNodeFactory::getInstance().destroyNodeAsync(_turretNodeID);
}

Entity* Turret::createInstance()
{
    return new Turret();
}

void Turret::setParentNode(iNodeTransform* parent)
{
    _parentNodeID = parent->getID();
}

void Turret::setTargetID(uint64 targetID)
{
    _targetID = targetID;
}

void Turret::updatePosition()
{
    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_parentNodeID));
    if (transformNode != nullptr)
    {
        iaMatrixd matrix;
        transformNode->calcWorldTransformation(matrix);
        _sphere._center = matrix._pos;
    }
}

void Turret::init()
{
    setHealth(100.0);
    setShield(100.0);
    setDamage(10.0);
    setShieldDamage(10.0);

    iNode* parent = iNodeFactory::getInstance().getNode(_parentNodeID);
    if (parent != nullptr)
    {
        iNodeModel* turret = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        _turretNodeID = turret->getID();
        turret->setModel("turret.ompf", nullptr);

        parent->insertNode(turret);

        _time = iTimer::getInstance().getTime();
    }
    else
    {
        con_err("can't initialize");
    }
}

void Turret::deinit()
{

}

void Turret::hitBy(uint64 entityID)
{
}

void Turret::handle()
{
    updatePosition();

    if (!_initilized)
    {
        iNodeModel* turret = static_cast<iNodeModel*>(iNodeFactory::getInstance().getNode(_turretNodeID));
        if (turret != nullptr)
        {
            if (turret->isLoaded())
            {
                if (turret->hasChildren())
                {
                    iNodeTransform* platformTransform = static_cast<iNodeTransform*>(turret->getChild("platformTransform"));
                    if (platformTransform != nullptr)
                    {
                        _platformID = platformTransform->getID();

                        iNodeTransform* towerTransform = static_cast<iNodeTransform*>(platformTransform->getChild("towerTransform"));
                        if (towerTransform != nullptr)
                        {
                            _headingID = towerTransform->getID();

                            iNodeTransform* gunTransform = static_cast<iNodeTransform*>(towerTransform->getChild("gunTransform"));
                            if (gunTransform != nullptr)
                            {
                                _pitchID = gunTransform->getID();
                            }
                        }
                    }
                }

                _initilized = true;
            }
            else
            {
                return;
            }
        }
    }

    const float32 detectionDistance = 60;
    const float32 fireDistance = 55;
    bool fired = false;
    bool canFire = false;

    Entity* identifiedTarget = EntityManager::getInstance().getEntity(_targetID);

    if (identifiedTarget != nullptr)
    {
        iaVector3d targetPos = identifiedTarget->getSphere()._center;
        iaVector3d dir = targetPos;
        dir -= getSphere()._center;
        float32 distance = dir.length();

        if (distance < detectionDistance)
        {
            iNodeTransform* platform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_platformID));
            if (platform != nullptr)
            {
                iaMatrixd world;
                platform->setTransformationDirty(); // todo workaround because dirty flags don't get set right :-(
                platform->calcWorldTransformation(world);

                iaVector3d worldPos = world._pos;
                world._pos.set(0,0,0);

                iaMatrixd worldInverse;
                worldInverse = world;
                worldInverse.invert();
                iaVector3d localdir = worldInverse * dir;

                iaVector3d upVector(0, 1, 0);
                upVector = world * upVector;

                iNodeTransform* headingNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_headingID));
                iNodeTransform* pitchNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_pitchID));

                float32 heading = localdir.angleXZ();
                headingNode->identity();
                headingNode->rotate(heading - (M_PI * 0.5f), iaAxis::Y);

                float32 pitch = (M_PI * 0.5f) - localdir.angle(iaVector3d(0, 1, 0));
                if (pitch > 0)
                {
                    pitchNode->identity();
                    pitchNode->rotate(pitch, iaAxis::X);
                    canFire = true;
                }

                iaVector3I outside, inside;
                iaVector3I center;
                iaVector3I pos;
                iaConvert::convert(getSphere()._center, center);
                iaConvert::convert(targetPos, pos);

                VoxelTerrainGenerator::getInstance().castRay(center, pos, outside, inside);

                iaVector3d out;
                iaConvert::convert(outside, out);
                float32 distanceToWall = out.distance(getSphere()._center) + 5;

                if (canFire && 
                    distanceToWall > distance &&
                    distance < fireDistance)
                {
                    if (_time + 1000 < iTimer::getInstance().getTime())
                    {
                        iaMatrixd matrixOrientation;
                        matrixOrientation._depth = dir;
                        matrixOrientation._depth.negate();
                        matrixOrientation._depth.normalize();
                        matrixOrientation._top = upVector;
                        matrixOrientation._top.normalize();
                        matrixOrientation._right = matrixOrientation._top % matrixOrientation._depth;
                        matrixOrientation._right.normalize();
                        matrixOrientation._top = matrixOrientation._depth % matrixOrientation._right;
                        matrixOrientation._top.normalize();

                        matrixOrientation._pos = worldPos;
                        matrixOrientation._pos -= matrixOrientation._depth * 2.0;

                        iaVector3d bulletForce = matrixOrientation._depth * -0.75;

                        Bullet* bullet = static_cast<Bullet*>(EntityManager::getInstance().createEntity("Bullet"));
                        bullet->setFraction(getFraction());
                        bullet->setForce(bulletForce);
                        bullet->setPosition(matrixOrientation._pos);

                        _time = iTimer::getInstance().getTime();

                        fired = true;
                    }
                }
            }
        }
    }
}

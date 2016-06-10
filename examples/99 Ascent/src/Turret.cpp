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

Turret::Turret(iScene* scene, iNodeTransform* parent, Fraction fraction, uint64 playerID)
    : Entity(fraction, EntityType::None)
{
    _playerID = playerID;
    _scene = scene;

    setHealth(100.0);
    setShield(100.0);
    setDamage(10.0);
    setShieldDamage(10.0);

    iNodeModel* turret = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    _turretNodeID = turret->getID();
    turret->setModel("turret.ompf", nullptr);

    _parentNodeID = parent->getID();
    parent->insertNode(turret);

    _time = iTimer::getInstance().getTime();
}

Turret::~Turret()
{
    iNodeFactory::getInstance().destroyNodeAsync(_turretNodeID);
}

void Turret::hitBy(uint64 entityID)
{
}

iaVector3f Turret::updatePos()
{
    iaVector3f result;
    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_parentNodeID));
    if (transformNode != nullptr)
    {
        iaMatrixf matrix;
        transformNode->calcWorldTransformation(matrix);
        result = matrix._pos;
    }
    return result;
}

void Turret::handle()
{
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

    Entity* identifiedTarget = EntityManager::getInstance().getEntity(_playerID);

    if (identifiedTarget != nullptr)
    {
        iaVector3f targetPos = identifiedTarget->getSphere()._center;
        iaVector3f dir = targetPos;
        dir -= getSphere()._center;
        float32 distance = dir.length();

        if (distance < detectionDistance)
        {
            iNodeTransform* platform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_platformID));
            if (platform != nullptr)
            {
                iaMatrixf world;
                platform->setTransformationDirty(); // todo workaround because dirty flags don't get set right :-(
                platform->calcWorldTransformation(world);

                iaVector3f worldPos = world._pos;
                world._pos.set(0,0,0);

                iaMatrixf worldInverse;
                worldInverse = world;
                worldInverse.invert();
                iaVector3f localdir = worldInverse * dir;

                iaVector3f upVector(0, 1, 0);
                upVector = world * upVector;

                iNodeTransform* headingNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_headingID));
                iNodeTransform* pitchNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_pitchID));

                float32 heading = localdir.angleXZ();
                headingNode->identity();
                headingNode->rotate(heading - (M_PI * 0.5f), iaAxis::Y);

                float32 pitch = (M_PI * 0.5f) - localdir.angle(iaVector3f(0, 1, 0));
                if (pitch > 0)
                {
                    pitchNode->identity();
                    pitchNode->rotate(pitch, iaAxis::X);
                    canFire = true;
                }

                iaVector3I outside, inside;
                VoxelTerrainGenerator::getInstance().castRay(iaConvert::convert3I(getSphere()._center), iaConvert::convert3I(targetPos), outside, inside);

                float32 distanceToWall = iaConvert::convert3f(outside).distance(getSphere()._center) + 5;

                if (canFire && 
                    distanceToWall > distance &&
                    distance < fireDistance)
                {
                    if (_time + 1000 < iTimer::getInstance().getTime())
                    {
                        iaMatrixf matrixOrientation;
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

                        Bullet* bullet = new Bullet(_scene, iaVector3f(), matrixOrientation, getFraction());
                        _time = iTimer::getInstance().getTime();

                        fired = true;
                    }
                }
            }
        }
    }
}

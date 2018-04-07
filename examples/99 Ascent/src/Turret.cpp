#include "Turret.h"

#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iNodeModel.h>
#include <iModel.h>
#include <iScene.h>
#include <iPhysics.h>
#include <iTimer.h>
#include <iEntityManager.h>
using namespace Igor;

#include <iaString.h>
#include <iaConvert.h>
using namespace IgorAux;

#include "Bullet.h"
#include "Granade.h"
#include "VoxelTerrainGenerator.h"

Turret::Turret(iScene* scene, iNodeTransform* parent, Fraction fraction, uint64 playerID)
    : GameObject(fraction, GameObjectType::None)
{
    _playerID = playerID;
    _scene = scene;

    setHealth(100.0);
    setShield(100.0);
    setDamage(10.0);
    setShieldDamage(10.0);

    iNodeModel* turret = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    _turretNodeID = turret->getID();
    turret->setModel("turret.ompf", iResourceCacheMode::Cache, nullptr);
    turret->registerModelReadyDelegate(iModelReadyDelegate(this, &Turret::onModelReady));

    _parentNodeID = parent->getID();
    parent->insertNode(turret);

    _time = iTimer::getInstance().getApplicationTime();
}

void Turret::onModelReady(uint64 modelNodeID)
{
    con_assert(modelNodeID == _turretNodeID, "impossible");

    iNodeModel* turret = static_cast<iNodeModel*>(iNodeFactory::getInstance().getNode(_turretNodeID));
    if (turret != nullptr)
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
                        _initilized = true;
                    }
                }
            }
        }
    }
}

Turret::~Turret()
{
    if (_turretNodeID != iNode::INVALID_NODE_ID)
    {
        iNodeFactory::getInstance().destroyNodeAsync(_turretNodeID);
    }
}

void Turret::hitBy(uint64 entityID)
{
}

iaVector3d Turret::getCurrentPos()
{
    iaVector3d result;
    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_parentNodeID));
    if (transformNode != nullptr)
    {
        iaMatrixd matrix;
        transformNode->calcWorldTransformation(matrix);
        result = matrix._pos;
    }
    return result;
}

void Turret::handle()
{
    if (_initilized)
    {
        const float32 detectionDistance = 60;
        const float32 fireDistance = 55;
        bool fired = false;
        bool canFire = false;

        GameObject* identifiedTarget = static_cast<GameObject*>(iEntityManager::getInstance().getEntity(_playerID));

        if (identifiedTarget != nullptr)
        {
            iaVector3d targetPos = identifiedTarget->getSphere()._center;
            iaVector3d dir = targetPos;
            dir -= getSphere()._center;
            float32 distance = dir.length();
            
            if (distance > 0 && 
                distance < detectionDistance)
            {
                iNodeTransform* platform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_platformID));
                if (platform != nullptr)
                {
                    iaMatrixd world;
                    platform->setTransformationDirty(); // todo workaround because dirty flags don't get set right :-(
                    platform->calcWorldTransformation(world);

                    iaVector3d worldPos = world._pos;
                    world._pos.set(0, 0, 0);

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
                        if (_time + 1000 < iTimer::getInstance().getApplicationTime())
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

                            Bullet* bullet = new Bullet(_scene, iaVector3d(), matrixOrientation, getFraction());
                            _time = iTimer::getInstance().getApplicationTime();

                            fired = true;
                        }
                    }
                }
            }
        }
    }
}

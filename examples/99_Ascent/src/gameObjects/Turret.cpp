// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Turret.h"

#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/resources/model/iModel.h>
#include <igor/scene/iScene.h>
#include <igor/physics/iPhysics.h>
#include <igor/system/iTimer.h>
#include <igor/entities/iEntityManager.h>
#include <igor/terrain/iVoxelTerrain.h>
#include <igor/scene/traversal/iNodeVisitorSearchName.h>
using namespace igor;

#include <iaux/data/iaString.h>
#include <iaux/data/iaConvert.h>
using namespace iaux;

#include "Bullet.h"
#include "Granade.h"

Turret::Turret(iScenePtr scene, iNodeTransform *parent, iVoxelTerrain *voxelTerrain, Fraction fraction, uint64 playerID)
    : GameObject(fraction, GameObjectType::None)
{
    _playerID = playerID;
    _scene = scene;
    _voxelTerrain = voxelTerrain;

    setHealth(100.0);
    setShield(100.0);
    setDamage(10.0);
    setShieldDamage(10.0);

    iNodeModel *turret = iNodeManager::getInstance().createNode<iNodeModel>();
    _turretNodeID = turret->getID();
    turret->setModel("turret.ompf", iResourceCacheMode::Cache, nullptr);
    turret->registerModelReadyDelegate(iModelReadyDelegate(this, &Turret::onModelReady));

    _parentNodeID = parent->getID();
    parent->insertNode(turret);

    _time = iTimer::getInstance().getTime();
}

void Turret::onModelReady(uint64 modelNodeID)
{
    con_assert(modelNodeID == _turretNodeID, "impossible");

    iNodeModel *turret = static_cast<iNodeModel *>(iNodeManager::getInstance().getNode(_turretNodeID));
    if (turret != nullptr)
    {
        iNodeVisitorSearchName searchName;

        std::vector<uint64> nodeIDs = searchName.find(turret, "platform");
        if (!nodeIDs.empty())
        {
            _platformID = nodeIDs.front();
        }

        nodeIDs = searchName.find(turret, "tower");
        if (!nodeIDs.empty())
        {
            _headingID = nodeIDs.front();
        }

        nodeIDs = searchName.find(turret, "gun");
        if (!nodeIDs.empty())
        {
            _pitchID = nodeIDs.front();
        }

        if (_platformID != iNode::INVALID_NODE_ID &&
            _headingID != iNode::INVALID_NODE_ID &&
            _pitchID != iNode::INVALID_NODE_ID)
        {
            _initilized = true;
        }
    }
}

Turret::~Turret()
{
    if (_turretNodeID != iNode::INVALID_NODE_ID)
    {
        iNodeManager::getInstance().destroyNodeAsync(_turretNodeID);
    }
}

void Turret::hitBy(uint64 entityID)
{
}

iaVector3d Turret::getCurrentPos()
{
    iaVector3d result;
    iNodeTransform *transformNode = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_parentNodeID));
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

        GameObject *identifiedTarget = static_cast<GameObject *>(iEntityManager::getInstance().getEntity(_playerID));

        if (identifiedTarget != nullptr)
        {
            iaVector3d targetPos = identifiedTarget->getSphere()._center;
            iaVector3d dir = targetPos;
            dir -= getSphere()._center;
            float32 distance = dir.length();

            if (distance > 0 &&
                distance < detectionDistance)
            {
                iNodeTransform *platform = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_platformID));
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

                    iNodeTransform *headingNode = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_headingID));
                    iNodeTransform *pitchNode = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_pitchID));

                    float32 heading = localdir.angleXZ();
                    headingNode->identity();

                    headingNode->rotate(heading - (M_PI * 0.5f), iaAxis::Y);

                    float32 pitch = (M_PI * 0.5f) - localdir.angle(iaVector3d(0.0, 1.0, 0.0));
                    if (pitch > 0)
                    {
                        pitchNode->identity();
                        pitchNode->rotate(pitch, iaAxis::X);
                        canFire = true;
                    }

                    if (canFire &&
                        distance < fireDistance &&
                        _time + iaTime::fromSeconds(1) < iTimer::getInstance().getTime())
                    {
                        // check line of sight
                        iaVector3I outside, inside;
                        iaVector3I center = getSphere()._center.convert<int64>();
                        iaVector3I pos = targetPos.convert<int64>();

                        if (_voxelTerrain->castRay(center, pos, outside, inside))
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

                            Bullet *bullet = new Bullet(_scene, iaVector3d(), matrixOrientation, getFraction());
                            _time = iTimer::getInstance().getGameTime();

                            fired = true;
                        }
                    }
                }
            }
        }
    }
}

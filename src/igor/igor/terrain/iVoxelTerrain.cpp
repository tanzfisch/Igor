// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/terrain/iVoxelTerrain.h>

#include <igor/scene/nodes/iNodeLODTrigger.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/system/iApplication.h>
#include <igor/terrain/data/iVoxelBlock.h>
#include <igor/threading/iTaskManager.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeLODSwitch.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/scene/iScene.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/iResourceManager.h>
#include <igor/physics/iPhysics.h>
#include <igor/physics/iPhysicsBody.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/model/iModelFactory.h>
#include <igor/generation/iContouringCubes.h>
#include <igor/generation/iPerlinNoise.h>
#include <igor/data/iIntersection.h>
#include <igor/terrain/tasks/iTaskGenerateVoxels.h>
#include <igor/terrain/tasks/iTaskVoxelTerrain.h>
#include <igor/terrain/operations/iVoxelOperationBox.h>
#include <igor/terrain/operations/iVoxelOperationSphere.h>
#include <igor/renderer/iRenderer.h>

#include <iaux/data/iaConvert.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

// uncomment next line for voxel terrain debug fixed lod trigger position
// #define DEBUG_VOXEL_TERRAIN_FIX_POSITION

// uncomment next line for voxel terrain debug fixed lod trigger height
// #define DEBUG_VOXEL_TERRAIN_FIX_HEIGHT 10000

namespace igor
{

    iaVector3I childOffsetPosition[8] =
        {
            iaVector3I(0, 0, 0),
            iaVector3I(1, 0, 0),
            iaVector3I(1, 0, 1),
            iaVector3I(0, 0, 1),
            iaVector3I(0, 1, 0),
            iaVector3I(1, 1, 0),
            iaVector3I(1, 1, 1),
            iaVector3I(0, 1, 1)};

    iVoxelTerrain::iVoxelTerrain(iVoxelTerrainGenerateDelegate generateVoxelsDelegate,
                                 iVoxelTerrainPlacePropsDelegate placePropsDelegate,
                                 uint32 lodCount,
                                 uint32 voxelBlockSetupDistance,
                                 const iaVector3I &maxDiscoveryBoundaries)
    {
        con_assert_sticky(lodCount >= 2, "lod count out of range");
        con_assert_sticky(lodCount <= 11, "lod count out of range");
        con_assert_sticky(voxelBlockSetupDistance >= 2, "voxel block setup distance out of range");

        con_assert_sticky(maxDiscoveryBoundaries._x > 0, "discovery boundaries out of range");
        con_assert_sticky(maxDiscoveryBoundaries._y > 0, "discovery boundaries out of range");
        con_assert_sticky(maxDiscoveryBoundaries._z > 0, "discovery boundaries out of range");

        _maxDiscoveryBoundaries = maxDiscoveryBoundaries;
        _placePropsDelegate = placePropsDelegate;
        _generateVoxelsDelegate = generateVoxelsDelegate;
        _lowestLOD = lodCount - 1;
        _voxelBlockSetupDistance = voxelBlockSetupDistance;
        _voxelBlockDiscoveryDistance = _voxelBlockSetupDistance + 2;

        init();
    }

    iVoxelTerrain::~iVoxelTerrain()
    {
        deinit();
    }

    void iVoxelTerrain::modify(const iaSphereI &sphere, uint8 density)
    {
        iVoxelOperationSphere *voxelOperationBox = new iVoxelOperationSphere(sphere, density);
        _operationsQueueMutex.lock();
        _operationsQueue.push_back(std::shared_ptr<iVoxelOperation>(voxelOperationBox));
        _operationsQueueMutex.unlock();
    }

    void iVoxelTerrain::modify(const iAABoxI &box, uint8 density)
    {
        iVoxelOperationBox *voxelOperationBox = new iVoxelOperationBox(box, density);
        _operationsQueueMutex.lock();
        _operationsQueue.push_back(std::shared_ptr<iVoxelOperation>(voxelOperationBox));
        _operationsQueueMutex.unlock();
    }

    void iVoxelTerrain::setScene(iScenePtr scene)
    {
        con_assert(scene != nullptr, "zero pointer");
        con_assert(_rootNode == nullptr, "already initialized");

        if (scene != nullptr &&
            _rootNode == nullptr)
        {
            _rootNode = iNodeManager::getInstance().createNode<iNode>();
            scene->getRoot()->insertNode(_rootNode);

            iModelFactory::registerModelDataIO("igor.vtg", &iVoxelTerrainMeshGenerator::createInstance);

            _voxelTerrainTask = iTaskManager::getInstance().addTask(new iTaskVoxelTerrain(this));
        }
    }

    void iVoxelTerrain::setNodeActiveAsync(iNodePtr node, bool active)
    {
        iNodeManager::iAction action;
        action._action = active ? iNodeManager::iActionType::Activate : iNodeManager::iActionType::Deactivate;
        action._nodeA = node->getID();

        _actionQueue.push_back(action);
    }

    void iVoxelTerrain::insertNodeAsync(iNodePtr src, iNodePtr dst)
    {
        iNodeManager::iAction action;
        action._action = iNodeManager::iActionType::Insert;
        action._nodeA = src->getID();
        action._nodeB = dst->getID();

        _actionQueue.push_back(action);
    }

    iTargetMaterialPtr iVoxelTerrain::getTargetMaterial() const
    {
        return _targetMaterial;
    }

    void iVoxelTerrain::setPhysicsMaterialID(uint64 materialID)
    {
        _physicsMaterialID = materialID;
    }

    uint64 iVoxelTerrain::getPhysicsMaterialID() const
    {
        return _physicsMaterialID;
    }

    void iVoxelTerrain::removeNodeAsync(iNodePtr src, iNodePtr dst)
    {
        iNodeManager::iAction action;
        action._action = iNodeManager::iActionType::Remove;
        action._nodeA = src->getID();
        action._nodeB = dst->getID();

        _actionQueue.push_back(action);
    }

    void iVoxelTerrain::destroyNodeAsync(uint64 nodeID)
    {
        con_assert(nodeID != iNode::INVALID_NODE_ID, "invalid node id");

        iNodeManager::iAction action;
        action._action = iNodeManager::iActionType::Destroy;
        action._nodeA = nodeID;

        _actionQueue.push_back(action);
    }

    void iVoxelTerrain::init()
    {
        std::unordered_map<iaVector3I, iVoxelBlock *, iVectorHasher, iVectorEqualFn> voxelBlocks;

        for (int i = 0; i < _lowestLOD + 1; ++i)
        {
            _voxelBlocks.push_back(voxelBlocks);
        }

        // set up terrain material
        _terrainMaterial = iRenderer::getInstance().getDefaultMaterial();

        // set up terrain target material
        _targetMaterial = iTargetMaterial::create();
        _targetMaterial->setAmbient(iaColor3f(0.7f, 0.7f, 0.7f));
        _targetMaterial->setDiffuse(iaColor3f(0.9f, 0.9f, 0.9f));
        _targetMaterial->setSpecular(iaColor3f(0.1f, 0.1f, 0.1f));
        _targetMaterial->setEmissive(iaColor3f(0.05f, 0.05f, 0.05f));
        _targetMaterial->setShininess(100.0f);
    }

    void iVoxelTerrain::deinit()
    {
        con_info("shutdown iVoxelTerrain ...");

        auto voxelTerrainTask = iTaskManager::getInstance().getTask(_voxelTerrainTask);
        if (voxelTerrainTask != nullptr)
        {
            voxelTerrainTask->abort();
        }

        iModelFactory::unregisterModelDataIO("igor.vtg");
        _targetMaterial = nullptr;
    }

    void iVoxelTerrain::setLODTrigger(uint32 lodTriggerID)
    {
        _lodTrigger = lodTriggerID;
    }

    void iVoxelTerrain::setMaterial(const iMaterialPtr &material)
    {
        _terrainMaterial = material;
    }

    iMaterialPtr iVoxelTerrain::getMaterial() const
    {
        return _terrainMaterial;
    }

    void iVoxelTerrain::update()
    {
        deleteBlocks();

        iNodeLODTrigger *lodTrigger = static_cast<iNodeLODTrigger *>(iNodeManager::getInstance().getNode(_lodTrigger));
        if (lodTrigger != nullptr)
        {
            iaVector3d pos = lodTrigger->getWorldPosition();
            if (pos.length2() < 1.0f) // TODO why?
            {
                return;
            }

#ifdef DEBUG_VOXEL_TERRAIN_FIX_POSITION
            pos.set(9986, 310, 8977);
#endif

#ifdef DEBUG_VOXEL_TERRAIN_FIX_HEIGHT
            pos._y = DEBUG_VOXEL_TERRAIN_FIX_HEIGHT;
#endif
            iaVector3I observerPosition = pos.convert<int64>();

            discoverBlocks(observerPosition);
            updateBlocks(observerPosition);
        }

        applyVoxelOperations();

        // apply all actions at once so they will be synced with next frame
        iNodeManager::getInstance().applyActionsAsync(_actionQueue);
        _actionQueue.clear();
    }

    void iVoxelTerrain::applyVoxelOperations()
    {
        _operationsQueueMutex.lock();
        const auto operations = std::move(_operationsQueue);
        _operationsQueue.clear();
        _operationsQueueMutex.unlock();

        auto iterOp = operations.begin();
        while (iterOp != operations.end())
        {
            applyVoxelOperation((*iterOp));
            iterOp++;
        }
    }

    void iVoxelTerrain::applyVoxelOperation(std::shared_ptr<iVoxelOperation> voxelOperation)
    {
        iAABoxI boundings;
        voxelOperation->getBoundings(boundings);

        iaVector3I min = boundings._center;
        min -= boundings._halfWidths;
        iaVector3I max = boundings._center;
        max += boundings._halfWidths;

        int64 lodFactor = static_cast<int64>(pow(2, _lowestLOD));
        int64 worldBlockSize = _voxelBlockSize * lodFactor;
        min /= worldBlockSize;
        max /= worldBlockSize;

        if (min._x < 0)
        {
            min._x = 0;
        }

        if (min._y < 0)
        {
            min._y = 0;
        }

        if (min._z < 0)
        {
            min._z = 0;
        }

        if (max._x > _maxDiscoveryBoundaries._x)
        {
            min._x = _maxDiscoveryBoundaries._x;
        }

        if (min._y > _maxDiscoveryBoundaries._y)
        {
            min._y = _maxDiscoveryBoundaries._y;
        }

        if (min._z > _maxDiscoveryBoundaries._z)
        {
            min._z = _maxDiscoveryBoundaries._z;
        }

        auto &voxelBlocks = _voxelBlocks[_lowestLOD];
        iaVector3I voxelBlockPosition;

        for (int64 voxelBlockX = min._x; voxelBlockX <= max._x; ++voxelBlockX)
        {
            for (int64 voxelBlockY = min._y; voxelBlockY <= max._y; ++voxelBlockY)
            {
                for (int64 voxelBlockZ = min._z; voxelBlockZ <= max._z; ++voxelBlockZ)
                {
                    voxelBlockPosition.set(voxelBlockX, voxelBlockY, voxelBlockZ);
                    iVoxelBlock *voxelBlock = nullptr;
                    auto blockIter = voxelBlocks.find(voxelBlockPosition);
                    if (blockIter != voxelBlocks.end())
                    {
                        voxelBlock = (*blockIter).second;
                    }
                    else
                    {
                        voxelBlock = createVoxelBlock(_lowestLOD, voxelBlockPosition, 0);
                    }

                    voxelBlock->_voxelOperations.push_back(voxelOperation);
                }
            }
        }
    }

    void iVoxelTerrain::updateBlocks(const iaVector3I &observerPosition)
    {
        auto &voxelBlocks = _voxelBlocks[_lowestLOD];

        for (auto block : voxelBlocks)
        {
            update(block.second, observerPosition);
        }

        for (auto block : voxelBlocks)
        {
            updateVisibility(block.second);
        }
    }

    void iVoxelTerrain::deleteBlocks()
    {
        auto iter = _voxelBlocksToDelete.begin();
        while (iter != _voxelBlocksToDelete.end())
        {
            if (canBeDeleted((*iter)))
            {
                deleteBlock((*iter));
                iter = _voxelBlocksToDelete.erase(iter);
            }
            else
            {
                iter++;
            }
        }
    }

    bool iVoxelTerrain::canBeDeleted(iVoxelBlock *voxelBlock)
    {
        for (int i = 0; i < 8; ++i)
        {
            if (voxelBlock->_children[i] != iVoxelBlock::INVALID_VOXELBLOCKID)
            {
                return false;
            }
        }

        if (voxelBlock->_state == iVoxelBlockState::GeneratingMesh)
        {
            finalizeMesh(voxelBlock);
        }

        if (voxelBlock->_transformNodeIDQueued != iNode::INVALID_NODE_ID)
        {
            return false;
        }

        if (voxelBlock->_voxelGenerationTaskID != iTask::INVALID_TASK_ID)
        {
            return false;
        }

        return true;
    }

    void iVoxelTerrain::deleteBlock(iVoxelBlock *voxelBlock)
    {
        auto parent = _voxelBlocksMap.find(voxelBlock->_parent);
        if (parent != _voxelBlocksMap.end())
        {
            for (int i = 0; i < 8; ++i)
            {
                if ((*parent).second->_children[i] == voxelBlock->_id)
                {
                    (*parent).second->_children[i] = iVoxelBlock::INVALID_VOXELBLOCKID;
                }
            }
        }

        if (voxelBlock->_transformNodeIDCurrent != iNode::INVALID_NODE_ID)
        {
            destroyNodeAsync(voxelBlock->_transformNodeIDCurrent);
        }

        auto block = _voxelBlocksMap.find(voxelBlock->_id);
        if (block != _voxelBlocksMap.end())
        {
            _voxelBlocksMap.erase(block);
        }

        /*if (voxelBlock->_voxelData != nullptr)
        {
            delete voxelBlock->_voxelData;
        }

        if (voxelBlock->_voxelBlockInfo != nullptr)
        {
            delete voxelBlock->_voxelBlockInfo;
        }

        delete voxelBlock;*/
    }

    void iVoxelTerrain::setNeighboursDirty(iVoxelBlock *voxelBlock)
    {
        voxelBlock->_dirtyNeighbours = true;

        for (int i = 0; i < 6; ++i)
        {
            auto neighbour = _voxelBlocksMap.find(voxelBlock->_neighbours[i]);
            if (neighbour != _voxelBlocksMap.end())
            {
                (*neighbour).second->_dirtyNeighbours = true;
            }
            else
            {
                voxelBlock->_neighbours[i] = iVoxelBlock::INVALID_VOXELBLOCKID;
            }
        }
    }

    iVoxelBlock *iVoxelTerrain::createVoxelBlock(uint32 lod, iaVector3I positionInLOD, uint8 childAdress)
    {
        con_assert(lod >= 0 && lod <= _lowestLOD, "lod out of range");

        iVoxelBlock *result = new iVoxelBlock();

        result->_positionInLOD = positionInLOD;
        result->_positionInLOD += childOffsetPosition[childAdress];

        result->_lod = lod;
        result->_size = static_cast<uint16>(_voxelBlockSize * pow(2, lod));
        result->_childAddress = childAdress;

        for (int i = 0; i < 8; ++i)
        {
            result->_children[i] = iVoxelBlock::INVALID_VOXELBLOCKID;
        }

        for (int i = 0; i < 6; ++i)
        {
            result->_neighbours[i] = iVoxelBlock::INVALID_VOXELBLOCKID;
        }

        _voxelBlocks[lod][result->_positionInLOD] = result;
        _voxelBlocksMap[_nextVoxelBlockID] = result;
        result->_id = _nextVoxelBlockID++;

        return result;
    }

    void iVoxelTerrain::discoverBlocks(const iaVector3I &observerPosition)
    {
        int64 lodFactor = static_cast<int64>(pow(2, _lowestLOD));
        int64 worldBlockSize = _voxelBlockSize * lodFactor;

        if (_dirtyDiscovery || observerPosition.distance2(_lastDiscoveryPosition) > worldBlockSize * worldBlockSize)
        {
            _dirtyDiscovery = false;
            _lastDiscoveryPosition = observerPosition;

            iaVector3I center = observerPosition;
            center /= worldBlockSize;

            iaVector3I start(center);
            start._x -= _voxelBlockDiscoveryDistance;
            start._y -= _voxelBlockDiscoveryDistance;
            start._z -= _voxelBlockDiscoveryDistance;

            iaVector3I stop(center);
            stop._x += _voxelBlockDiscoveryDistance;
            stop._y += _voxelBlockDiscoveryDistance;
            stop._z += _voxelBlockDiscoveryDistance;

            if (start._x < 0)
            {
                start._x = 0;
            }

            if (start._y < 0)
            {
                start._y = 0;
            }

            if (start._z < 0)
            {
                start._z = 0;
            }

            auto voxelBlocksToDelete = _voxelBlocks[_lowestLOD];
            auto &voxelBlocks = _voxelBlocks[_lowestLOD];
            iaVector3I voxelBlockPosition;

            for (int64 voxelBlockX = start._x; voxelBlockX < stop._x; ++voxelBlockX)
            {
                for (int64 voxelBlockY = start._y; voxelBlockY < stop._y; ++voxelBlockY)
                {
                    for (int64 voxelBlockZ = start._z; voxelBlockZ < stop._z; ++voxelBlockZ)
                    {
                        voxelBlockPosition.set(voxelBlockX, voxelBlockY, voxelBlockZ);

                        auto blockIter = voxelBlocks.find(voxelBlockPosition);
                        if (blockIter == voxelBlocks.end())
                        {
                            createVoxelBlock(_lowestLOD, voxelBlockPosition, 0);
                        }
                        else
                        {
                            auto blockToDeleteIter = voxelBlocksToDelete.find(voxelBlockPosition);
                            if (blockToDeleteIter != voxelBlocksToDelete.end())
                            {
                                voxelBlocksToDelete.erase(blockToDeleteIter);
                            }
                        }
                    }
                }
            }

            for (auto iter : voxelBlocksToDelete)
            {
                collectBlocksToDelete(iter.second, _voxelBlocksToDelete);
            }
        }
    }

    void iVoxelTerrain::collectBlocksToDelete(iVoxelBlock *currentBlock, std::vector<iVoxelBlock *> &dst)
    {
        detachNeighbours(currentBlock);

        auto foundBlock = _voxelBlocks[currentBlock->_lod].find(currentBlock->_positionInLOD);
        if (foundBlock != _voxelBlocks[currentBlock->_lod].end())
        {
            _voxelBlocks[currentBlock->_lod].erase(foundBlock);
            dst.push_back(currentBlock);

            for (int i = 0; i < 8; ++i)
            {
                auto child = _voxelBlocksMap.find(currentBlock->_children[i]);

                if (child != _voxelBlocksMap.end())
                {
                    collectBlocksToDelete((*child).second, dst);
                }
            }
        }
    }

    void iVoxelTerrain::setInRange(iVoxelBlock *voxelBlock, bool inRange)
    {
        if (voxelBlock->_inRange != inRange)
        {
            voxelBlock->_inRange = inRange;
            setNeighboursDirty(voxelBlock);
        }
    }

    void iVoxelTerrain::detachNeighbours(iVoxelBlock *voxelBlock)
    {
        auto neighbour = _voxelBlocksMap.find(voxelBlock->_neighbours[0]);
        if (neighbour != _voxelBlocksMap.end())
        {
            (*neighbour).second->_neighbours[1] = iVoxelBlock::INVALID_VOXELBLOCKID;
        }
        voxelBlock->_neighbours[0] = iVoxelBlock::INVALID_VOXELBLOCKID;

        neighbour = _voxelBlocksMap.find(voxelBlock->_neighbours[1]);
        if (neighbour != _voxelBlocksMap.end())
        {
            (*neighbour).second->_neighbours[0] = iVoxelBlock::INVALID_VOXELBLOCKID;
        }
        voxelBlock->_neighbours[1] = iVoxelBlock::INVALID_VOXELBLOCKID;

        neighbour = _voxelBlocksMap.find(voxelBlock->_neighbours[2]);
        if (neighbour != _voxelBlocksMap.end())
        {
            (*neighbour).second->_neighbours[3] = iVoxelBlock::INVALID_VOXELBLOCKID;
        }
        voxelBlock->_neighbours[2] = iVoxelBlock::INVALID_VOXELBLOCKID;

        neighbour = _voxelBlocksMap.find(voxelBlock->_neighbours[3]);
        if (neighbour != _voxelBlocksMap.end())
        {
            (*neighbour).second->_neighbours[2] = iVoxelBlock::INVALID_VOXELBLOCKID;
        }
        voxelBlock->_neighbours[3] = iVoxelBlock::INVALID_VOXELBLOCKID;

        neighbour = _voxelBlocksMap.find(voxelBlock->_neighbours[4]);
        if (neighbour != _voxelBlocksMap.end())
        {
            (*neighbour).second->_neighbours[5] = iVoxelBlock::INVALID_VOXELBLOCKID;
        }
        voxelBlock->_neighbours[4] = iVoxelBlock::INVALID_VOXELBLOCKID;

        neighbour = _voxelBlocksMap.find(voxelBlock->_neighbours[5]);
        if (neighbour != _voxelBlocksMap.end())
        {
            (*neighbour).second->_neighbours[4] = iVoxelBlock::INVALID_VOXELBLOCKID;
        }
        voxelBlock->_neighbours[5] = iVoxelBlock::INVALID_VOXELBLOCKID;
    }

    void iVoxelTerrain::attachNeighbours(iVoxelBlock *voxelBlock)
    {
        auto voxelBlocks = _voxelBlocks[voxelBlock->_lod];
        iaVector3I neighbourPos;

        if (voxelBlock->_neighbours[0] == iVoxelBlock::INVALID_VOXELBLOCKID)
        {
            neighbourPos = voxelBlock->_positionInLOD;
            neighbourPos._x += 1;
            auto neighbour = voxelBlocks.find(neighbourPos);
            if (neighbour != voxelBlocks.end())
            {
                voxelBlock->_neighbours[0] = (*neighbour).second->_id;
                (*neighbour).second->_neighbours[1] = voxelBlock->_id;
            }
        }

        if (voxelBlock->_neighbours[1] == iVoxelBlock::INVALID_VOXELBLOCKID)
        {
            neighbourPos = voxelBlock->_positionInLOD;
            neighbourPos._x -= 1;
            auto neighbour = voxelBlocks.find(neighbourPos);
            if (neighbour != voxelBlocks.end())
            {
                voxelBlock->_neighbours[1] = (*neighbour).second->_id;
                (*neighbour).second->_neighbours[0] = voxelBlock->_id;
            }
        }

        if (voxelBlock->_neighbours[2] == iVoxelBlock::INVALID_VOXELBLOCKID)
        {
            neighbourPos = voxelBlock->_positionInLOD;
            neighbourPos._y += 1;
            auto neighbour = voxelBlocks.find(neighbourPos);
            if (neighbour != voxelBlocks.end())
            {
                voxelBlock->_neighbours[2] = (*neighbour).second->_id;
                (*neighbour).second->_neighbours[3] = voxelBlock->_id;
            }
        }

        if (voxelBlock->_neighbours[3] == iVoxelBlock::INVALID_VOXELBLOCKID)
        {
            neighbourPos = voxelBlock->_positionInLOD;
            neighbourPos._y -= 1;
            auto neighbour = voxelBlocks.find(neighbourPos);
            if (neighbour != voxelBlocks.end())
            {
                voxelBlock->_neighbours[3] = (*neighbour).second->_id;
                (*neighbour).second->_neighbours[2] = voxelBlock->_id;
            }
        }

        if (voxelBlock->_neighbours[4] == iVoxelBlock::INVALID_VOXELBLOCKID)
        {
            neighbourPos = voxelBlock->_positionInLOD;
            neighbourPos._z += 1;
            auto neighbour = voxelBlocks.find(neighbourPos);
            if (neighbour != voxelBlocks.end())
            {
                voxelBlock->_neighbours[4] = (*neighbour).second->_id;
                (*neighbour).second->_neighbours[5] = voxelBlock->_id;
            }
        }

        if (voxelBlock->_neighbours[5] == iVoxelBlock::INVALID_VOXELBLOCKID)
        {
            neighbourPos = voxelBlock->_positionInLOD;
            neighbourPos._z -= 1;
            auto neighbour = voxelBlocks.find(neighbourPos);
            if (neighbour != voxelBlocks.end())
            {
                voxelBlock->_neighbours[5] = (*neighbour).second->_id;
                (*neighbour).second->_neighbours[4] = voxelBlock->_id;
            }
        }
    }

    void iVoxelTerrain::update(iVoxelBlock *voxelBlock, iaVector3I observerPosition)
    {
        if (voxelBlock->_state == iVoxelBlockState::Empty)
        {
            if (voxelBlock->_voxelOperations.size() > 0)
            {
                voxelBlock->_voxelData = new iVoxelData();
                voxelBlock->_voxelData->setClearValue(0);
                voxelBlock->_voxelBlockInfo->_voxelData = voxelBlock->_voxelData;
                voxelBlock->_voxelData->initData(voxelBlock->_voxelBlockInfo->_size, voxelBlock->_voxelBlockInfo->_size, voxelBlock->_voxelBlockInfo->_size);

                voxelBlock->_voxelBlockInfo->_transition = true;
                voxelBlock->_state = iVoxelBlockState::GeneratingVoxel;
            }
            else
            {
                return;
            }
        }

        // distance in tiles of current lod in all three dimensions
        iaVector3I distance = observerPosition;
        distance /= voxelBlock->_size;
        distance -= voxelBlock->_positionInLOD;
        distance._x = abs(distance._x);
        distance._y = abs(distance._y);
        distance._z = abs(distance._z);

        bool inRange = false;

        if (voxelBlock->_lod == _lowestLOD)
        {
            if (distance._x <= _voxelBlockDiscoveryDistance && distance._y <= _voxelBlockDiscoveryDistance && distance._z <= _voxelBlockDiscoveryDistance)
            {
                inRange = true;
            }
        }

        if (voxelBlock->_children[0] != iVoxelBlock::INVALID_VOXELBLOCKID)
        {
            bool childrenInRange = false;

            if (distance._x <= 1 && distance._y <= 1 && distance._z <= 1)
            {
                childrenInRange = true;
            }

            for (int i = 0; i < 8; ++i)
            {
                iVoxelBlock *child = _voxelBlocksMap[voxelBlock->_children[i]];
                setInRange(child, childrenInRange);
            }
        }

        if (voxelBlock->_lod == _lowestLOD)
        {
            setInRange(voxelBlock, inRange);
        }

        switch (voxelBlock->_state)
        {
        case iVoxelBlockState::Initial:
            if (distance._x <= _voxelBlockSetupDistance && distance._y <= _voxelBlockSetupDistance && distance._z <= _voxelBlockSetupDistance)
            {
                voxelBlock->_state = iVoxelBlockState::Setup;
            }
            break;

        case iVoxelBlockState::Setup:
        {
            if (voxelBlock->_voxelBlockInfo == nullptr)
            {
                voxelBlock->_voxelData = new iVoxelData();
                voxelBlock->_voxelData->setClearValue(0);

                voxelBlock->_voxelBlockInfo = new iVoxelBlockInfo();
                voxelBlock->_voxelBlockInfo->_size = _voxelBlockSize + _voxelBlockOverlap;
                voxelBlock->_voxelBlockInfo->_positionInLOD = voxelBlock->_positionInLOD;
                voxelBlock->_voxelBlockInfo->_voxelData = voxelBlock->_voxelData;
                voxelBlock->_voxelBlockInfo->_lodOffset = iContouringCubes::calcLODOffset(voxelBlock->_lod);
                voxelBlock->_voxelBlockInfo->_lod = voxelBlock->_lod;

                // lower lods need higher priority to build
                uint32 priority = _lowestLOD - voxelBlock->_lod + 1;

                iTaskGenerateVoxels *task = new iTaskGenerateVoxels(voxelBlock->_voxelBlockInfo, priority, _generateVoxelsDelegate);
                voxelBlock->_voxelGenerationTaskID = iTaskManager::getInstance().addTask(task);
            }

            voxelBlock->_state = iVoxelBlockState::GeneratingVoxel;
        }
        break;

        case iVoxelBlockState::GeneratingVoxel:
        {
            iTask *task = iTaskManager::getInstance().getTask(voxelBlock->_voxelGenerationTaskID);
            if (task == nullptr)
            {
                voxelBlock->_voxelGenerationTaskID = iTask::INVALID_TASK_ID;

                if (!voxelBlock->_voxelBlockInfo->_transition)
                {
                    delete voxelBlock->_voxelData;
                    voxelBlock->_voxelData = nullptr;
                    voxelBlock->_voxelBlockInfo->_voxelData = nullptr;
                    voxelBlock->_state = iVoxelBlockState::Empty;
                }
                else
                {
                    if (voxelBlock->_lod != 0)
                    {
                        if (voxelBlock->_children[0] == 0)
                        {
                            uint32 childLOD = voxelBlock->_lod - 1;
                            iaVector3I positionInLOD = voxelBlock->_positionInLOD;
                            positionInLOD *= 2;
                            iVoxelBlock *child = createVoxelBlock(childLOD, positionInLOD, 0);
                            voxelBlock->_children[0] = child->_id;
                            child->_parent = voxelBlock->_id;

                            child = createVoxelBlock(childLOD, positionInLOD, 1);
                            voxelBlock->_children[1] = child->_id;
                            child->_parent = voxelBlock->_id;

                            child = createVoxelBlock(childLOD, positionInLOD, 2);
                            voxelBlock->_children[2] = child->_id;
                            child->_parent = voxelBlock->_id;

                            child = createVoxelBlock(childLOD, positionInLOD, 3);
                            voxelBlock->_children[3] = child->_id;
                            child->_parent = voxelBlock->_id;

                            child = createVoxelBlock(childLOD, positionInLOD, 4);
                            voxelBlock->_children[4] = child->_id;
                            child->_parent = voxelBlock->_id;

                            child = createVoxelBlock(childLOD, positionInLOD, 5);
                            voxelBlock->_children[5] = child->_id;
                            child->_parent = voxelBlock->_id;

                            child = createVoxelBlock(childLOD, positionInLOD, 6);
                            voxelBlock->_children[6] = child->_id;
                            child->_parent = voxelBlock->_id;

                            child = createVoxelBlock(childLOD, positionInLOD, 7);
                            voxelBlock->_children[7] = child->_id;
                            child->_parent = voxelBlock->_id;

                            //   4-----5
                            //  /|    /|
                            // 7-----6 |
                            // | 0---|-1
                            // |/    |/
                            // 3-----2

                            _voxelBlocksMap[voxelBlock->_children[0]]->_neighbours[0] = voxelBlock->_children[1];
                            _voxelBlocksMap[voxelBlock->_children[1]]->_neighbours[1] = voxelBlock->_children[0];

                            _voxelBlocksMap[voxelBlock->_children[3]]->_neighbours[0] = voxelBlock->_children[2];
                            _voxelBlocksMap[voxelBlock->_children[2]]->_neighbours[1] = voxelBlock->_children[3];

                            _voxelBlocksMap[voxelBlock->_children[4]]->_neighbours[0] = voxelBlock->_children[4];
                            _voxelBlocksMap[voxelBlock->_children[5]]->_neighbours[1] = voxelBlock->_children[5];

                            _voxelBlocksMap[voxelBlock->_children[7]]->_neighbours[0] = voxelBlock->_children[6];
                            _voxelBlocksMap[voxelBlock->_children[6]]->_neighbours[1] = voxelBlock->_children[7];

                            _voxelBlocksMap[voxelBlock->_children[0]]->_neighbours[2] = voxelBlock->_children[4];
                            _voxelBlocksMap[voxelBlock->_children[4]]->_neighbours[3] = voxelBlock->_children[0];

                            _voxelBlocksMap[voxelBlock->_children[1]]->_neighbours[2] = voxelBlock->_children[5];
                            _voxelBlocksMap[voxelBlock->_children[5]]->_neighbours[3] = voxelBlock->_children[1];

                            _voxelBlocksMap[voxelBlock->_children[2]]->_neighbours[2] = voxelBlock->_children[6];
                            _voxelBlocksMap[voxelBlock->_children[6]]->_neighbours[3] = voxelBlock->_children[2];

                            _voxelBlocksMap[voxelBlock->_children[3]]->_neighbours[2] = voxelBlock->_children[7];
                            _voxelBlocksMap[voxelBlock->_children[7]]->_neighbours[3] = voxelBlock->_children[3];

                            _voxelBlocksMap[voxelBlock->_children[0]]->_neighbours[4] = voxelBlock->_children[3];
                            _voxelBlocksMap[voxelBlock->_children[3]]->_neighbours[5] = voxelBlock->_children[0];

                            _voxelBlocksMap[voxelBlock->_children[1]]->_neighbours[4] = voxelBlock->_children[2];
                            _voxelBlocksMap[voxelBlock->_children[2]]->_neighbours[5] = voxelBlock->_children[1];

                            _voxelBlocksMap[voxelBlock->_children[4]]->_neighbours[4] = voxelBlock->_children[7];
                            _voxelBlocksMap[voxelBlock->_children[7]]->_neighbours[5] = voxelBlock->_children[4];

                            _voxelBlocksMap[voxelBlock->_children[5]]->_neighbours[4] = voxelBlock->_children[6];
                            _voxelBlocksMap[voxelBlock->_children[6]]->_neighbours[5] = voxelBlock->_children[5];

                            for (int i = 0; i < 8; ++i)
                            {
                                attachNeighbours(_voxelBlocksMap[voxelBlock->_children[i]]);
                            }
                        }
                    }
                    else
                    {
                        if (voxelBlock->_mutationCounter == 0)
                        {
                            iVoxelBlockPropsInfo info;

                            info._min = voxelBlock->_positionInLOD;
                            info._min *= _voxelBlockSize;

                            info._max = info._min;
                            info._max._x += _voxelBlockSize;
                            info._max._y += _voxelBlockSize;
                            info._max._z += _voxelBlockSize;

                            iTaskPropsOnVoxels *task = new iTaskPropsOnVoxels(info, 0, _placePropsDelegate);
                            iTaskManager::getInstance().addTask(task);
                        }
                    }

                    if (voxelBlock->_lod == _lowestLOD)
                    {
                        voxelBlock->_state = iVoxelBlockState::Ready; // can not generate mesh for lowest lod
                    }
                    else
                    {
                        if (voxelBlock->_voxelOperations.size() > 0)
                        {
                            voxelBlock->_state = iVoxelBlockState::Ready;
                        }
                        else
                        {
                            voxelBlock->_state = iVoxelBlockState::GeneratingMesh;
                        }
                    }
                }
            }
        }
        break;

        case iVoxelBlockState::GeneratingMesh:
            updateMesh(voxelBlock);
            break;

        case iVoxelBlockState::Ready:
        {
            if (voxelBlock->_inRange)
            {
                if (voxelBlock->_dirtyNeighbours)
                {
                    uint8 neighborsLOD = calcLODTransition(voxelBlock);
                    if (voxelBlock->_neighboursLOD != neighborsLOD)
                    {
                        voxelBlock->_neighboursLOD = neighborsLOD;
                        voxelBlock->_dirty = true;
                    }

                    voxelBlock->_dirtyNeighbours = false;
                }
            }

            if (voxelBlock->_voxelOperations.size() > 0)
            {
                for (auto op : voxelBlock->_voxelOperations)
                {
                    op->apply(voxelBlock);
                }

                voxelBlock->_dirty = true;

                if (voxelBlock->_children[0] != iVoxelBlock::INVALID_VOXELBLOCKID)
                {
                    int64 lodFactor = static_cast<int64>(pow(2, voxelBlock->_lod - 1));
                    int64 halfVoxelBlockSize = (iVoxelTerrain::_voxelBlockSize + iVoxelTerrain::_voxelBlockOverlap) / 2 * lodFactor;

                    for (int i = 0; i < 8; ++i)
                    {
                        iVoxelBlock *child = _voxelBlocksMap[voxelBlock->_children[i]];

                        iAABoxI box;
                        box._halfWidths.set(halfVoxelBlockSize, halfVoxelBlockSize, halfVoxelBlockSize);
                        box._center = child->_positionInLOD * iVoxelTerrain::_voxelBlockSize;
                        box._center *= lodFactor;
                        box._center._x += halfVoxelBlockSize;
                        box._center._y += halfVoxelBlockSize;
                        box._center._z += halfVoxelBlockSize;

                        iAABoxI opbox;
                        for (auto op : voxelBlock->_voxelOperations)
                        {
                            op->getBoundings(opbox);
                            if (iIntersection::intersects(opbox, box))
                            {
                                child->_voxelOperations.push_back(op);
                            }
                        }
                    }
                }

                voxelBlock->_voxelOperations.clear();
            }

            if (voxelBlock->_dirty && voxelBlock->_lod != _lowestLOD)
            {
                iNodeModel *modelNode = static_cast<iNodeModel *>(iNodeManager::getInstance().getNode(voxelBlock->_modelNodeIDCurrent));
                if (modelNode == nullptr || (modelNode != nullptr &&
                                             modelNode->isValid()))
                {
                    voxelBlock->_transformNodeIDQueued = iNode::INVALID_NODE_ID;
                    voxelBlock->_state = iVoxelBlockState::GeneratingMesh;

                    voxelBlock->_dirty = false;
                }
            }
        }
        break;
        }

        if (voxelBlock->_children[0] != iVoxelBlock::INVALID_VOXELBLOCKID)
        {
            for (int i = 0; i < 8; ++i)
            {
                iVoxelBlock *child = _voxelBlocksMap[voxelBlock->_children[i]];
                update(child, observerPosition);
            }
        }
    }

    bool iVoxelTerrain::updateVisibility(iVoxelBlock *voxelBlock)
    {
        if (voxelBlock->_state == iVoxelBlockState::Empty)
        {
            return true;
        }

        bool childrenVisible = false;
        bool blockVisible = voxelBlock->_inRange;
        bool meshVisible = blockVisible;

        if (voxelBlock->_children[0] != iVoxelBlock::INVALID_VOXELBLOCKID)
        {
            childrenVisible = true;
            for (int i = 0; i < 8; ++i)
            {
                if (!updateVisibility(_voxelBlocksMap[voxelBlock->_children[i]]))
                {
                    childrenVisible = false;
                }
            }

            if (!childrenVisible)
            {
                for (int i = 0; i < 8; ++i)
                {
                    iVoxelBlock *child = _voxelBlocksMap[voxelBlock->_children[i]];
                    if (child->_modelNodeIDCurrent != iNode::INVALID_NODE_ID)
                    {
                        iNodeModel *modelNode = static_cast<iNodeModel *>(iNodeManager::getInstance().getNode(child->_modelNodeIDCurrent));
                        if (modelNode != nullptr &&
                            modelNode->isValid())
                        {
                            iNodeTransform *transformNode = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(child->_transformNodeIDCurrent));
                            if (transformNode != nullptr)
                            {
                                setNodeActiveAsync(transformNode, false);
                            }
                        }
                    }
                }
            }
            else
            {
                meshVisible = false;
            }
        }

        if (voxelBlock->_modelNodeIDCurrent != iNode::INVALID_NODE_ID)
        {
            iNodeModel *modelNode = static_cast<iNodeModel *>(iNodeManager::getInstance().getNode(voxelBlock->_modelNodeIDCurrent));
            if (modelNode != nullptr &&
                modelNode->isValid())
            {
                iNodePtr group = static_cast<iNodeMesh *>(modelNode->getChild("group"));
                if (group != nullptr)
                {
                    const auto &children = group->getChildren();
                    if (!children.empty())
                    {
                        iNodeMesh *meshNode = static_cast<iNodeMesh *>(children[0]);
                        meshNode->setVisible(meshVisible);
                    }
                }

                iNodeTransform *transformNode = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(voxelBlock->_transformNodeIDCurrent));
                if (transformNode != nullptr)
                {
                    setNodeActiveAsync(transformNode, meshVisible);
                }
            }
            else
            {
                meshVisible = false;
            }
        }
        else
        {
            meshVisible = false;
        }

        return (meshVisible || childrenVisible);
    }

#define HIGHER_NEIGHBOR_LOD_XPOSITIVE 0x20
#define HIGHER_NEIGHBOR_LOD_XNEGATIVE 0x10
#define HIGHER_NEIGHBOR_LOD_YPOSITIVE 0x08
#define HIGHER_NEIGHBOR_LOD_YNEGATIVE 0x04
#define HIGHER_NEIGHBOR_LOD_ZPOSITIVE 0x02
#define HIGHER_NEIGHBOR_LOD_ZNEGATIVE 0x01

    uint8 iVoxelTerrain::calcLODTransition(iVoxelBlock *voxelBlock)
    {
        uint8 result = 0;

        if (voxelBlock->_lod >= _lowestLOD)
        {
            return result;
        }

        auto neighbour = _voxelBlocksMap.find(voxelBlock->_neighbours[0]);
        if (neighbour != _voxelBlocksMap.end())
        {
            if (!(*neighbour).second->_inRange)
            {
                result |= HIGHER_NEIGHBOR_LOD_XPOSITIVE;
            }
        }
        else
        {
            result |= HIGHER_NEIGHBOR_LOD_XPOSITIVE;
        }

        neighbour = _voxelBlocksMap.find(voxelBlock->_neighbours[1]);
        if (neighbour != _voxelBlocksMap.end())
        {
            if (!(*neighbour).second->_inRange)
            {
                result |= HIGHER_NEIGHBOR_LOD_XNEGATIVE;
            }
        }
        else
        {
            result |= HIGHER_NEIGHBOR_LOD_XNEGATIVE;
        }

        neighbour = _voxelBlocksMap.find(voxelBlock->_neighbours[2]);
        if (neighbour != _voxelBlocksMap.end())
        {
            if (!(*neighbour).second->_inRange)
            {
                result |= HIGHER_NEIGHBOR_LOD_YPOSITIVE;
            }
        }
        else
        {
            result |= HIGHER_NEIGHBOR_LOD_YPOSITIVE;
        }

        neighbour = _voxelBlocksMap.find(voxelBlock->_neighbours[3]);
        if (neighbour != _voxelBlocksMap.end())
        {
            if (!(*neighbour).second->_inRange)
            {
                result |= HIGHER_NEIGHBOR_LOD_YNEGATIVE;
            }
        }
        else
        {
            result |= HIGHER_NEIGHBOR_LOD_YNEGATIVE;
        }

        neighbour = _voxelBlocksMap.find(voxelBlock->_neighbours[4]);
        if (neighbour != _voxelBlocksMap.end())
        {
            if (!(*neighbour).second->_inRange)
            {
                result |= HIGHER_NEIGHBOR_LOD_ZPOSITIVE;
            }
        }
        else
        {
            result |= HIGHER_NEIGHBOR_LOD_ZPOSITIVE;
        }

        neighbour = _voxelBlocksMap.find(voxelBlock->_neighbours[5]);
        if (neighbour != _voxelBlocksMap.end())
        {
            if (!(*neighbour).second->_inRange)
            {
                result |= HIGHER_NEIGHBOR_LOD_ZNEGATIVE;
            }
        }
        else
        {
            result |= HIGHER_NEIGHBOR_LOD_ZNEGATIVE;
        }

        return result;
    }

    void iVoxelTerrain::updateMesh(iVoxelBlock *voxelBlock)
    {
        if (voxelBlock->_transformNodeIDQueued == iNode::INVALID_NODE_ID)
        {
            if (voxelBlock->_voxelData != nullptr)
            {
                auto parent = _voxelBlocksMap.find(voxelBlock->_parent);
                if (parent != _voxelBlocksMap.end())
                {
                    iaString tileName = iaString::toString(voxelBlock->_positionInLOD._x);
                    tileName += ":";
                    tileName += iaString::toString(voxelBlock->_positionInLOD._y);
                    tileName += ":";
                    tileName += iaString::toString(voxelBlock->_positionInLOD._z);
                    tileName += ":";
                    tileName += iaString::toString(voxelBlock->_lod);
                    tileName += ":";
                    tileName += iaString::toString((uint32)voxelBlock->_mutationCounter++);

                    iNodeTransform *transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();
                    iaVector3d transform = voxelBlock->_positionInLOD.convert<float64>();
                    transform *= voxelBlock->_size;
                    transformNode->translate(transform);

                    iaVector3I voxelOffsetToNextLOD(childOffsetPosition[voxelBlock->_childAddress]);
                    voxelOffsetToNextLOD *= 16;

                    iVoxelData* voxelData = new iVoxelData();
                    voxelBlock->_voxelData->getCopy(*voxelData);

                    iVoxelData* voxelDataNextLOD = new iVoxelData();
                    (*parent).second->_voxelData->getCopy(*voxelDataNextLOD);

                    iParameters parameters({
                        {"alias", tileName},
                        {"type", iaString("model")},
                        {"subType", iaString("igor.vtg")},
                        {"cacheMode", iResourceCacheMode::Free},
                        {"quiet", true},
                        {"joinVertices", true},
                        {"material", _terrainMaterial},
                        {"voxelOffsetToNextLOD", voxelOffsetToNextLOD},
                        {"voxelData", voxelData},
                        {"voxelDataNextLOD", voxelDataNextLOD},
                        {"lod", voxelBlock->_lod},
                        {"neighboursLOD", voxelBlock->_neighboursLOD},
                        {"targetMaterial", _targetMaterial},
                        {"physicsMaterialID", _physicsMaterialID}
                    });
                    iModelPtr model = iResourceManager::getInstance().requestResource<iModel>(parameters);

                    iNodeModel *modelNode = iNodeManager::getInstance().createNode<iNodeModel>();
                    modelNode->setModel(model);

                    transformNode->insertNode(modelNode);
                    insertNodeAsync(_rootNode, transformNode);

                    voxelBlock->_transformNodeIDQueued = transformNode->getID();
                    voxelBlock->_modelNodeIDQueued = modelNode->getID();

                    voxelBlock->_dirty = false;
                }
            }
        }
        else
        {
            finalizeMesh(voxelBlock);
        }
    }

    void iVoxelTerrain::finalizeMesh(iVoxelBlock *voxelBlock)
    {
        static int count = 0;
        iNodeModel *modelNode = static_cast<iNodeModel *>(iNodeManager::getInstance().getNode(voxelBlock->_modelNodeIDQueued));
        if (modelNode != nullptr &&
            modelNode->isValid())
        {
            if (voxelBlock->_transformNodeIDCurrent != iNode::INVALID_NODE_ID)
            {
                destroyNodeAsync(voxelBlock->_transformNodeIDCurrent);
            }

            voxelBlock->_transformNodeIDCurrent = voxelBlock->_transformNodeIDQueued;
            voxelBlock->_modelNodeIDCurrent = voxelBlock->_modelNodeIDQueued;
            voxelBlock->_modelNodeIDQueued = iNode::INVALID_NODE_ID;

            voxelBlock->_state = iVoxelBlockState::Ready;
        }
    }

    uint8 iVoxelTerrain::getVoxelDensity(iaVector3I pos)
    {
        uint8 result = 0;

        iaVector3I voxelBlockPos(pos);
        voxelBlockPos /= _voxelBlockSize;

        iVoxelBlock *block = nullptr;
        const auto &voxelBlocksLOD0 = _voxelBlocks[0];
        auto blockIter = voxelBlocksLOD0.find(voxelBlockPos);
        if (blockIter != voxelBlocksLOD0.end())
        {
            auto block = (*blockIter).second;
            if (block->_voxelData != nullptr &&
                block->_voxelData->hasData())
            {
                iaVector3I voxelRelativePos(pos);

                voxelRelativePos._x = voxelRelativePos._x % static_cast<int64>(_voxelBlockSize);
                voxelRelativePos._y = voxelRelativePos._y % static_cast<int64>(_voxelBlockSize);
                voxelRelativePos._z = voxelRelativePos._z % static_cast<int64>(_voxelBlockSize);

                result = block->_voxelData->getVoxelDensity(voxelRelativePos);
            }
        }

        return result;
    }

    bool iVoxelTerrain::castRay(const iaVector3I &from, const iaVector3I &to, iaVector3I &outside, iaVector3I &inside)
    {
        iaVector3I u(from);
        iaVector3I delta(to);
        delta -= from;
        iaVector3I step;

        (delta._x > 0) ? step._x = 1 : step._x = -1;
        (delta._y > 0) ? step._y = 1 : step._y = -1;
        (delta._z > 0) ? step._z = 1 : step._z = -1;

        if (delta._x < 0)
            delta._x = -delta._x;
        if (delta._y < 0)
            delta._y = -delta._y;
        if (delta._z < 0)
            delta._z = -delta._z;

        int64 dist = (delta._x > delta._y) ? delta._x : delta._y;
        dist = (dist > delta._z) ? dist : delta._z;

        iaVector3I err(delta._x, delta._y, delta._z);

        outside = u;

        for (int i = 0; i < dist; i++)
        {
            if (getVoxelDensity(iaVector3I(u._x, u._y, u._z)) != 0)
            {
                inside = u;
                return true;
            }

            outside = u;

            err += delta;

            if (err._x > dist)
            {
                err._x -= dist;
                u._x += step._x;
            }

            if (err._y > dist)
            {
                err._y -= dist;
                u._y += step._y;
            }

            if (err._z > dist)
            {
                err._z -= dist;
                u._z += step._z;
            }
        }

        return false;
    }
} // namespace igor
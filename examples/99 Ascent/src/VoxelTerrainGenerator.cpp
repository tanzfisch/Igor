#include "VoxelTerrainGenerator.h"

#include <iNodeLODTrigger.h>
#include <iNodeFactory.h>
#include <iApplication.h>
#include <iVoxelData.h>
#include <iTaskManager.h>
#include <iModelResourceFactory.h>
#include <iNodeTransform.h>
#include <iNodeLODSwitch.h>
#include <iNodeModel.h>
#include <iNodeMesh.h>
#include <iScene.h>
#include <iMaterialResourceFactory.h>
#include <iMaterial.h>
#include <iPhysics.h>
#include <iPhysicsBody.h>
#include <iMesh.h>
#include <iStatistics.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

#include "TaskGenerateVoxels.h"

VoxelTerrainGenerator::VoxelTerrainGenerator()
{

}

VoxelTerrainGenerator::~VoxelTerrainGenerator()
{
    deinit();
}

void VoxelTerrainGenerator::setScene(iScene* scene)
{
    con_assert(scene != nullptr, "zero pointer");
    _scene = scene;

    init();
}

void VoxelTerrainGenerator::init()
{
    iApplication::getInstance().registerApplicationHandleDelegate(iApplicationHandleDelegate(this, &VoxelTerrainGenerator::onHandle));

    iModelResourceFactory::getInstance().registerModelDataIO("vtg", &VoxelTerrainMeshGenerator::createInstance);

    // set up terrain material
    _terrainMaterialID = iMaterialResourceFactory::getInstance().createMaterial("TerrainMaterial");
    iMaterialResourceFactory::getInstance().getMaterial(_terrainMaterialID)->addShaderSource("ascent_terrain.vert", iShaderObjectType::Vertex);
    iMaterialResourceFactory::getInstance().getMaterial(_terrainMaterialID)->addShaderSource("ascent_terrain_directional_light.frag", iShaderObjectType::Fragment);
    iMaterialResourceFactory::getInstance().getMaterial(_terrainMaterialID)->compileShader();
    iMaterialResourceFactory::getInstance().getMaterial(_terrainMaterialID)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
	//iMaterialResourceFactory::getInstance().getMaterial(_terrainMaterialID)->getRenderStateSet().setRenderState(iRenderState::CullFace, iRenderStateValue::Off);
	//iMaterialResourceFactory::getInstance().getMaterial(_terrainMaterialID)->getRenderStateSet().setRenderState(iRenderState::Wireframe, iRenderStateValue::On);

    iTaskManager::getInstance().registerTaskFinishedDelegate(iTaskFinishedDelegate(this, &VoxelTerrainGenerator::onTaskFinished));
}

void VoxelTerrainGenerator::deinit()
{
    con_endl("shutdown VoxelTerrainGenerator ...");

    iModelResourceFactory::getInstance().unregisterModelDataIO("vtg");

    iApplication::getInstance().unregisterApplicationHandleDelegate(iApplicationHandleDelegate(this, &VoxelTerrainGenerator::onHandle));

    con_endl("waiting for some tasks ...");
    while (_runningTasks.size() > 0)
    {
        _sleep(1000);
    }

    iTaskManager::getInstance().unregisterTaskFinishedDelegate(iTaskFinishedDelegate(this, &VoxelTerrainGenerator::onTaskFinished));

    con_endl("clear blocks ...");
    for (auto blockIter : _voxelBlocks)
    {
        if (blockIter.second != nullptr)
        {
            if (blockIter.second->_generatedVoxels &&
                blockIter.second->_voxelData != nullptr)
            {
                delete blockIter.second->_voxelData;
            }

            delete blockIter.second;
        }
    }
    _voxelBlocks.clear();

    con_endl("clear tiles ...");
    for (auto tile : _tileDataSets)
    {
        if (tile.second._transformNodeID != iNode::INVALID_NODE_ID)
        {
            iNodeFactory::getInstance().destroyNodeAsync(tile.second._transformNodeID);
        }

        for(auto id : tile.second._destroyNodeIDs)
        {
            iNodeFactory::getInstance().destroyNodeAsync(id);
        }
    }

    con_endl("... done");
}

void VoxelTerrainGenerator::setLODTrigger(uint32 lodTriggerID)
{
    _lodTrigger = lodTriggerID;
}

uint8 VoxelTerrainGenerator::getVoxelDensity(iaVector3I pos)
{
    uint8 result = 0;

    iaVector3I voxelBlock(pos);
    voxelBlock /= _voxelBlockSize;

    iaVector3I voxelRelativePos(pos);

    voxelRelativePos._x = voxelRelativePos._x % static_cast<int64>(_voxelBlockSize);
    voxelRelativePos._y = voxelRelativePos._y % static_cast<int64>(_voxelBlockSize);
    voxelRelativePos._z = voxelRelativePos._z % static_cast<int64>(_voxelBlockSize);

    VoxelBlock* block = nullptr;

    auto blockIter = _voxelBlocks.find(voxelBlock);
    if (blockIter != _voxelBlocks.end())
    {
        block = _voxelBlocks[voxelBlock];
        if (block->_generatedVoxels)
        {
            result = block->_voxelData->getVoxelDensity(voxelRelativePos);
        }
    }

    return result;
}

void VoxelTerrainGenerator::setVoxelDensity(iaVector3I voxelBlock, iaVector3I voxelRelativePos, uint8 density)
{
    auto blockIter = _voxelBlocks.find(voxelBlock);
    if (blockIter != _voxelBlocks.end())
    {
        VoxelBlock* block = _voxelBlocks[voxelBlock];
        if (block->_generatedVoxels)
        {
            block->_voxelData->setVoxelDensity(voxelRelativePos, density);
        }
    }
}

void VoxelTerrainGenerator::setVoxelDensity(iaVector3I pos, uint8 density)
{
    iaVector3I voxelBlock(pos);
    voxelBlock /= _voxelBlockSize;

    iaVector3I voxelRelativePos(pos);

    voxelRelativePos._x = voxelRelativePos._x % static_cast<int64>(_voxelBlockSize);
    voxelRelativePos._y = voxelRelativePos._y % static_cast<int64>(_voxelBlockSize);
    voxelRelativePos._z = voxelRelativePos._z % static_cast<int64>(_voxelBlockSize);

    VoxelBlock* block = nullptr;

    auto blockIter = _voxelBlocks.find(voxelBlock);
    if (blockIter != _voxelBlocks.end())
    {
        block = _voxelBlocks[voxelBlock];
        if (block->_generatedVoxels)
        {
            block->_voxelData->setVoxelDensity(voxelRelativePos, density);
        }
    }

    if (voxelRelativePos._x < _voxelBlockOverlap)
    {
        iaVector3I voxelBlockX(voxelBlock);
        voxelBlockX._x--;
        iaVector3I voxelRelativePosX(voxelRelativePos);
        voxelRelativePosX._x += _voxelBlockSize;
        setVoxelDensity(voxelBlockX, voxelRelativePosX, density);
    }

    if (voxelRelativePos._y < _voxelBlockOverlap)
    {
        iaVector3I voxelBlockY(voxelBlock);
        voxelBlockY._y--;
        iaVector3I voxelRelativePosY(voxelRelativePos);
        voxelRelativePosY._y += _voxelBlockSize;
        setVoxelDensity(voxelBlockY, voxelRelativePosY, density);
    }

    if (voxelRelativePos._z < _voxelBlockOverlap)
    {
        iaVector3I voxelBlockZ(voxelBlock);
        voxelBlockZ._z--;
        iaVector3I voxelRelativePosZ(voxelRelativePos);
        voxelRelativePosZ._z += _voxelBlockSize;
        setVoxelDensity(voxelBlockZ, voxelRelativePosZ, density);
    }

    if (voxelRelativePos._x < _voxelBlockOverlap &&
        voxelRelativePos._y < _voxelBlockOverlap)
    {
        iaVector3I voxelBlockXY(voxelBlock);
        voxelBlockXY._x--;
        voxelBlockXY._y--;
        iaVector3I voxelRelativePosXY(voxelRelativePos);
        voxelRelativePosXY._x += _voxelBlockSize;
        voxelRelativePosXY._y += _voxelBlockSize;
        setVoxelDensity(voxelBlockXY, voxelRelativePosXY, density);
    }

    if (voxelRelativePos._x < _voxelBlockOverlap &&
        voxelRelativePos._z < _voxelBlockOverlap)
    {
        iaVector3I voxelBlockXZ(voxelBlock);
        voxelBlockXZ._x--;
        voxelBlockXZ._z--;
        iaVector3I voxelRelativePosXZ(voxelRelativePos);
        voxelRelativePosXZ._x += _voxelBlockSize;
        voxelRelativePosXZ._z += _voxelBlockSize;
        setVoxelDensity(voxelBlockXZ, voxelRelativePosXZ, density);
    }

    if (voxelRelativePos._y < _voxelBlockOverlap &&
        voxelRelativePos._z < _voxelBlockOverlap)
    {
        iaVector3I voxelBlockYZ(voxelBlock);
        voxelBlockYZ._y--;
        voxelBlockYZ._z--;
        iaVector3I voxelRelativePosYZ(voxelRelativePos);
        voxelRelativePosYZ._y += _voxelBlockSize;
        voxelRelativePosYZ._z += _voxelBlockSize;
        setVoxelDensity(voxelBlockYZ, voxelRelativePosYZ, density);
    }

    if (voxelRelativePos._x < _voxelBlockOverlap &&
        voxelRelativePos._y < _voxelBlockOverlap &&
        voxelRelativePos._z < _voxelBlockOverlap)
    {
        iaVector3I voxelBlockXYZ(voxelBlock);
        voxelBlockXYZ._x--;
        voxelBlockXYZ._y--;
        voxelBlockXYZ._z--;
        iaVector3I voxelRelativePosXYZ(voxelRelativePos);
        voxelRelativePosXYZ._x += _voxelBlockSize;
        voxelRelativePosXYZ._y += _voxelBlockSize;
        voxelRelativePosXYZ._z += _voxelBlockSize;
        setVoxelDensity(voxelBlockXYZ, voxelRelativePosXYZ, density);
    }
}

void VoxelTerrainGenerator::castRay(const iaVector3I& from, const iaVector3I& to, iaVector3I& outside, iaVector3I& inside)
{
    iaVector3I u(from);
    iaVector3I delta(to);
    delta -= from;
    iaVector3I step;

    (delta._x > 0) ? step._x = 1 : step._x = -1;
    (delta._y > 0) ? step._y = 1 : step._y = -1;
    (delta._z > 0) ? step._z = 1 : step._z = -1;

    if (delta._x < 0) delta._x = -delta._x;
    if (delta._y < 0) delta._y = -delta._y;
    if (delta._z < 0) delta._z = -delta._z;

    int64 dist = (delta._x>delta._y) ? delta._x : delta._y;
    dist = (dist > delta._z) ? dist : delta._z;

    iaVector3I err(delta._x, delta._y, delta._z);

    outside = u;

    for (int i = 0; i < dist; i++)
    {
        if (getVoxelDensity(iaVector3I(u._x, u._y, u._z)) != 0)
        {
            inside = u;
            return;
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
}

#define array3D(x, y, z, f) ((x) + (f) * ((y) + (f) * (z)))

void VoxelTerrainGenerator::refreshTile(iaVector3I tilepos)
{
    auto tileIter = _tileDataSets.find(tilepos);
    if (tileIter != _tileDataSets.end())
    {
        (*tileIter).second._destroyNodeIDs.push_back((*tileIter).second._transformNodeID);
        (*tileIter).second._modelNodeID = iNode::INVALID_NODE_ID;
        (*tileIter).second._transformNodeID = iNode::INVALID_NODE_ID;
    }
}

void VoxelTerrainGenerator::handleMeshTiles(iVoxelData* voxelData, const iaVector3I& blockPos, iNodeLODTrigger* lodTrigger, const iaVector3I& lodTriggerPos)
{
    iaVector3I tilePos = blockPos;
    TileInformation tileInformation;
    tileInformation._materialID = _terrainMaterialID;
    tileInformation._voxelData = voxelData;
    tileInformation._lodTriggerID = lodTrigger->getID();

    for (int tileX = 0; tileX < _tilesCountPerVoxelBlock; ++tileX)
    {
        for (int tileY = 0; tileY < _tilesCountPerVoxelBlock; ++tileY)
        {
            for (int tileZ = 0; tileZ < _tilesCountPerVoxelBlock; ++tileZ)
            {
                tileInformation._relativePos.set(tileX, tileY, tileZ);
                tileInformation._relativePos *= _tileSize;

                tilePos = blockPos;
                tilePos += tileInformation._relativePos;

                iaVector3I tileIndexPos = tilePos;
                tileIndexPos /= _tileSize;

                float32 distance = lodTriggerPos.distance2(tilePos);

                auto tileIter = _tileDataSets.find(tileIndexPos);
                if (tileIter == _tileDataSets.end())
                {
                    TileData tileData;
                    tileData._transformNodeID = iNode::INVALID_NODE_ID;
                    tileData._modelNodeID = iNode::INVALID_NODE_ID;
                    _tileDataSets[tileIndexPos] = tileData;

                    tileIter = _tileDataSets.find(tileIndexPos);
                }

                if (distance < _tileCreationDistance &&
                    (*tileIter).second._transformNodeID == iNode::INVALID_NODE_ID)
                {
                    tileInformation._width = _tileSize + _tileOverlap;
                    tileInformation._depth = _tileSize + _tileOverlap;
                    tileInformation._height = _tileSize + _tileOverlap;

                    iModelDataInputParameter* inputParam = new iModelDataInputParameter(); // will be deleted by iModel
                    inputParam->_identifier = "vtg";
                    inputParam->_joinVertexes = true;
                    inputParam->_needsRenderContext = false;
                    inputParam->_modelSourceType = iModelSourceType::Generated;
                    inputParam->_loadPriority = static_cast<uint32>(distance);
                    inputParam->_parameters.setData(reinterpret_cast<const char*>(&tileInformation), sizeof(TileInformation));

                    iaString tileName = iaString::itoa(tilePos._x);
                    tileName += ":";
                    tileName += iaString::itoa(tilePos._y);
                    tileName += ":";
                    tileName += iaString::itoa(tilePos._z);
                    tileName += ":";
                    tileName += iaString::itoa((*tileIter).second._mutationCounter++);

                    iNodeTransform* transform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
                    transform->translate(tilePos._x, tilePos._y, tilePos._z);

                    iNodeLODSwitch* lodSwitch = static_cast<iNodeLODSwitch*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLODSwitch));
                    lodSwitch->addTrigger(lodTrigger);

                    iNodeModel* modelNode = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
                    modelNode->setModel(tileName, inputParam);

                    transform->insertNode(lodSwitch);
                    lodSwitch->insertNode(modelNode);

                    lodSwitch->setThresholds(modelNode, 0.0f, _tileVisualDistance);
                    _scene->getRoot()->insertNode(transform);

                    (*tileIter).second._transformNodeID = transform->getID();
                    (*tileIter).second._modelNodeID = modelNode->getID();
                }

                if (!(*tileIter).second._destroyNodeIDs.empty())
                {
                    bool destroy = false;
                    iNodeModel* modelNode = static_cast<iNodeModel*>(iNodeFactory::getInstance().getNode((*tileIter).second._modelNodeID));
                    if (modelNode != nullptr)
                    {
                        if (modelNode->isLoaded())
                        {
                            destroy = true;
                        }
                    }
                    else
                    {
                        destroy = true;
                    }

                    if (destroy)
                    {
                        for (auto id : (*tileIter).second._destroyNodeIDs)
                        {
                            iNode* node = iNodeFactory::getInstance().getNode(id);
                            if (node != nullptr)
                            {
                                iNodeFactory::getInstance().destroyNodeAsync(node);
                            }
                        }

                        (*tileIter).second._destroyNodeIDs.clear();
                    }
                }

                if (distance > _tileDestructionDistance)
                {
                    if ((*tileIter).second._transformNodeID != iNode::INVALID_NODE_ID)
                    {
                        iNodeFactory::getInstance().destroyNodeAsync((*tileIter).second._transformNodeID);
                    }

                    if (!(*tileIter).second._destroyNodeIDs.empty())
                    {
                        for (auto id : (*tileIter).second._destroyNodeIDs)
                        {
                            iNodeFactory::getInstance().destroyNodeAsync(id);
                        }

                        (*tileIter).second._destroyNodeIDs.clear();
                    }

                    _tileDataSets.erase(tileIter);
                }
            }
        }
    }
}

void VoxelTerrainGenerator::registerVoxelDataGeneratedDelegate(VoxelDataGeneratedDelegate voxelDataGeneratedDelegate)
{
    _dataGeneratedEvent.append(voxelDataGeneratedDelegate);
}

void VoxelTerrainGenerator::unregisterVoxelDataGeneratedDelegate(VoxelDataGeneratedDelegate voxelDataGeneratedDelegate)
{
    _dataGeneratedEvent.remove(voxelDataGeneratedDelegate);
}

void VoxelTerrainGenerator::onHandle()
{
    handleVoxelBlocks();
}

void VoxelTerrainGenerator::onTaskFinished(uint64 taskID)
{
    _mutexRunningTasks.lock();
    auto task = find(_runningTasks.begin(), _runningTasks.end(), taskID);
    if (task != _runningTasks.end())
    {
        _runningTasks.erase(task);
    }
    _mutexRunningTasks.unlock();
}

bool VoxelTerrainGenerator::loading()
{
    bool result;
    
    _mutexRunningTasks.lock();
    result = _runningTasks.size() ? true : false;
    _mutexRunningTasks.unlock();

    return result;
}

void VoxelTerrainGenerator::handleVoxelBlocks()
{
    iaVector3I currentVoxelBlock;
    VoxelBlock* block = nullptr;

    iNodeLODTrigger* lodTrigger = static_cast<iNodeLODTrigger*>(iNodeFactory::getInstance().getNode(_lodTrigger));
    if (lodTrigger != nullptr)
    {
        iaVector3d pos = lodTrigger->getWorldPosition();
        iaVector3I lodTriggerPos(pos._x, pos._y, pos._z);

        iaVector3I center(lodTriggerPos._x, lodTriggerPos._y, lodTriggerPos._z);
        center /= _voxelBlockSize;

        iaVector3I start(center);
        start._x -= _voxelBlockScanDistance;
        start._y -= _voxelBlockScanDistance;
        start._z -= _voxelBlockScanDistance;

        iaVector3I stop(center);
        stop._x += _voxelBlockScanDistance;
        stop._y += _voxelBlockScanDistance;
        stop._z += _voxelBlockScanDistance;

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

        for (int64 voxelBlockX = start._x; voxelBlockX < stop._x; ++voxelBlockX)
        {
            for (int64 voxelBlockY = start._y; voxelBlockY < stop._y; ++voxelBlockY)
            {
                for (int64 voxelBlockZ = start._z; voxelBlockZ < stop._z; ++voxelBlockZ)
                {
                    currentVoxelBlock.set(voxelBlockX, voxelBlockY, voxelBlockZ);

                    auto blockIter = _voxelBlocks.find(currentVoxelBlock);
                    if (blockIter == _voxelBlocks.end())
                    {
                        block = new VoxelBlock();
                        _voxelBlocks[currentVoxelBlock] = block;
                    }
                    else
                    {
                        block = _voxelBlocks[currentVoxelBlock];
                    }

                    iaVector3I blockPos(voxelBlockX, voxelBlockY, voxelBlockZ);
                    blockPos *= _voxelBlockSize;

                    if (block->_voxelData == nullptr)
                    {
                        iaVector3I blockCenterPos = blockPos;
                        blockCenterPos._x += _voxelBlockSize / 2;
                        blockCenterPos._y += _voxelBlockSize / 2;
                        blockCenterPos._z += _voxelBlockSize / 2;

                        float32 distance = lodTriggerPos.distance2(blockCenterPos);
                        if (distance < _voxelBlockCreationDistance)
                        {
                            block->_voxelData = new iVoxelData();
                            block->_voxelData->setClearValue(0);
                            block->_offset = blockPos;
                            block->_size.set(_voxelBlockSize + _voxelBlockOverlap, _voxelBlockSize + _voxelBlockOverlap, _voxelBlockSize + _voxelBlockOverlap);

                            TaskGenerateVoxels* task = new TaskGenerateVoxels(block, static_cast<uint32>(distance * 0.9));
                            _mutexRunningTasks.lock();
                            _runningTasks.push_back(task->getID());
                            _mutexRunningTasks.unlock();
                            iTaskManager::getInstance().addTask(task);
                        }
                    }
                    else if (block->_generatedVoxels)
                    {
                        handleMeshTiles(block->_voxelData, blockPos, lodTrigger, lodTriggerPos);

                        if (!block->_generatedEnemies)
                        {
                            iaVector3I blockMax = blockPos;
                            blockMax._x += _voxelBlockSize;
                            blockMax._y += _voxelBlockSize;
                            blockMax._z += _voxelBlockSize;
                            _dataGeneratedEvent(blockPos, blockMax);
                            block->_generatedEnemies = true;
                        }
                    }
                }
            }
        }
    }
}

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

VoxelTerrainGenerator::VoxelTerrainGenerator(iScene* scene)
{
    con_assert(scene != nullptr, "zero pointer");

    _scene = scene;

    init();
}

VoxelTerrainGenerator::~VoxelTerrainGenerator()
{
    deinit();
}

void VoxelTerrainGenerator::init()
{
    _handleSectionID = iStatistics::getInstance().registerSection("VTG", iaColor4f(0, 0, 0, 1));

    iApplication::getInstance().registerApplicationHandleDelegate(ApplicationHandleDelegate(this, &VoxelTerrainGenerator::onHandle));

    iModelResourceFactory::getInstance().registerModelDataIO("vtg", &VoxelTerrainMeshGenerator::createInstance);

    // set up terrain material
    _terrainMaterialID = iMaterialResourceFactory::getInstance().createMaterial("TerrainMaterial");
    iMaterialResourceFactory::getInstance().getMaterial(_terrainMaterialID)->addShaderSource("voxel_example_terrain.vert", iShaderObjectType::Vertex);
    iMaterialResourceFactory::getInstance().getMaterial(_terrainMaterialID)->addShaderSource("voxel_example_terrain_directional_light.frag", iShaderObjectType::Fragment);
    iMaterialResourceFactory::getInstance().getMaterial(_terrainMaterialID)->compileShader();
    iMaterialResourceFactory::getInstance().getMaterial(_terrainMaterialID)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
}

void VoxelTerrainGenerator::deinit()
{
    iStatistics::getInstance().unregisterSection(_handleSectionID);

    iApplication::getInstance().unregisterApplicationHandleDelegate(ApplicationHandleDelegate(this, &VoxelTerrainGenerator::onHandle));

    for (auto blockIter : _voxelBlocks)
    {
        if (blockIter.second != nullptr)
        {
            if (blockIter.second->_voxelData != nullptr)
            {
                delete blockIter.second->_voxelData;
            }

            delete blockIter.second;
        }
    }
    _voxelBlocks.clear();
}

void VoxelTerrainGenerator::setLODTrigger(iNodeLODTrigger* lodTrigger)
{
    con_assert(lodTrigger != nullptr, "zero pointer");

    if (lodTrigger != nullptr)
    {
        _lodTrigger = lodTrigger->getID();
    }
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

    if (voxelRelativePos._x <= _voxelBlockOverlap)
    {
        iaVector3I voxelBlockX(voxelBlock);
        voxelBlockX._x--;
        iaVector3I voxelRelativePosX(voxelRelativePos);
        voxelRelativePosX._x += _voxelBlockSize;
        setVoxelDensity(voxelBlockX, voxelRelativePosX, density);
    }

    if (voxelRelativePos._y <= _voxelBlockOverlap)
    {
        iaVector3I voxelBlockY(voxelBlock);
        voxelBlockY._y--;
        iaVector3I voxelRelativePosY(voxelRelativePos);
        voxelRelativePosY._y += _voxelBlockSize;
        setVoxelDensity(voxelBlockY, voxelRelativePosY, density);
    }

    if (voxelRelativePos._z <= _voxelBlockOverlap)
    {
        iaVector3I voxelBlockZ(voxelBlock);
        voxelBlockZ._z--;
        iaVector3I voxelRelativePosZ(voxelRelativePos);
        voxelRelativePosZ._z += _voxelBlockSize;
        setVoxelDensity(voxelBlockZ, voxelRelativePosZ, density);
    }

    if (voxelRelativePos._x <= _voxelBlockOverlap &&
        voxelRelativePos._y <= _voxelBlockOverlap)
    {
        iaVector3I voxelBlockXY(voxelBlock);
        voxelBlockXY._x--;
        voxelBlockXY._y--;
        iaVector3I voxelRelativePosXY(voxelRelativePos);
        voxelRelativePosXY._x += _voxelBlockSize;
        voxelRelativePosXY._y += _voxelBlockSize;
        setVoxelDensity(voxelBlockXY, voxelRelativePosXY, density);
    }

    if (voxelRelativePos._x <= _voxelBlockOverlap &&
        voxelRelativePos._z <= _voxelBlockOverlap)
    {
        iaVector3I voxelBlockXZ(voxelBlock);
        voxelBlockXZ._x--;
        voxelBlockXZ._z--;
        iaVector3I voxelRelativePosXZ(voxelRelativePos);
        voxelRelativePosXZ._x += _voxelBlockSize;
        voxelRelativePosXZ._z += _voxelBlockSize;
        setVoxelDensity(voxelBlockXZ, voxelRelativePosXZ, density);
    }

    if (voxelRelativePos._y <= _voxelBlockOverlap &&
        voxelRelativePos._z <= _voxelBlockOverlap)
    {
        iaVector3I voxelBlockYZ(voxelBlock);
        voxelBlockYZ._y--;
        voxelBlockYZ._z--;
        iaVector3I voxelRelativePosYZ(voxelRelativePos);
        voxelRelativePosYZ._y += _voxelBlockSize;
        voxelRelativePosYZ._z += _voxelBlockSize;
        setVoxelDensity(voxelBlockYZ, voxelRelativePosYZ, density);
    }

    if (voxelRelativePos._x <= _voxelBlockOverlap &&
        voxelRelativePos._y <= _voxelBlockOverlap &&
        voxelRelativePos._z <= _voxelBlockOverlap)
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

const iaVector3I VoxelTerrainGenerator::castRay(const iaVector3I& from, const iaVector3I& to)
{
    iaVector3I result;

    iaVector3I u(from);
    iaVector3I lastU;
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

    bool inside = false;

    lastU = u;

    for (int i = 0; i < dist; i++)
    {
        if (getVoxelDensity(iaVector3I(u._x, u._y, u._z)) != 0)
        {
            return lastU;
        }

        lastU = u;

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

    return result;
}

#define array3D(x, y, z, f) ((x) + (f) * ((y) + (f) * (z)))

void VoxelTerrainGenerator::refreshTile(iaVector3I tilepos)
{
    auto tileIter = _tileDataSets.find(tilepos);
    if (tileIter != _tileDataSets.end())
    {
        iNodeFactory::getInstance().destroyNode((*tileIter).second._transformNodeID);
        _tileDataSets.erase(tileIter);
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
                    if (distance < _tileCreationDistance)
                    {
                        tileInformation._width = _tileSize + _tileOverlap;
                        tileInformation._depth = _tileSize + _tileOverlap;
                        tileInformation._height = _tileSize + _tileOverlap;

                        iModelDataInputParameter* inputParam = new iModelDataInputParameter(); // will be deleted in VoxelTerrainMeshGenerator
                        inputParam->_identifier = "vtg";
                        inputParam->_joinVertexes = true;
                        inputParam->_needsRenderContext = false;
                        inputParam->_passToPhysics = true;
                        inputParam->_modelSourceType = iModelSourceType::Generated;
                        inputParam->_loadPriority = static_cast<uint32>(distance);
                        inputParam->_parameters.setData(reinterpret_cast<const char*>(&tileInformation), sizeof(TileInformation));

                        iaString tileName = iaString::itoa(tilePos._x);
                        tileName += ":";
                        tileName += iaString::itoa(tilePos._y);
                        tileName += ":";
                        tileName += iaString::itoa(tilePos._z);

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

                        TileData tileData;
                        tileData._transformNodeID = transform->getID();
                        tileData._modelNodeID = modelNode->getID();
                        tileData._physicsGenerated = false;
                        tileData._bodyID = 0;
                        _tileDataSets[tileIndexPos] = tileData;

                        return; // start only one per frame (todo use a queue)
                    }
                }
                else
                {
                    if (distance > _tileDestructionDistance)
                    {
                        iNodeFactory::getInstance().destroyNode((*tileIter).second._transformNodeID);
                        _tileDataSets.erase(tileIter);
                    }
                }
            }
        }
    }
}

void VoxelTerrainGenerator::onHandle()
{
    iStatistics::getInstance().beginSection(_handleSectionID);
    handleVoxelBlocks();
    iStatistics::getInstance().endSection(_handleSectionID);
}

void VoxelTerrainGenerator::handleVoxelBlocks()
{
    iaVector3I currentVoxelBlock;

    iNodeLODTrigger* lodTrigger = static_cast<iNodeLODTrigger*>(iNodeFactory::getInstance().getNode(_lodTrigger));
    if (lodTrigger != nullptr)
    {
        iaVector3f pos = lodTrigger->getWorldPosition();
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
                    VoxelBlock* block = nullptr;
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

                    iaVector3I blockCenterPos = blockPos;
                    blockCenterPos._x += _voxelBlockSize / 2;
                    blockCenterPos._y += _voxelBlockSize / 2;
                    blockCenterPos._z += _voxelBlockSize / 2;

                    float32 distance = lodTriggerPos.distance2(blockCenterPos);
                    if (distance < _voxelBlockcCreationDistance)
                    {
                        if (block->_voxelData == nullptr)
                        {
                            block->_voxelData = new iVoxelData();
                            block->_offset = blockPos;
                            block->_size.set(_voxelBlockSize + _voxelBlockOverlap, _voxelBlockSize + _voxelBlockOverlap, _voxelBlockSize + _voxelBlockOverlap);

                            iTaskManager::getInstance().addTask(new TaskGenerateVoxels(block, static_cast<uint32>(distance * 0.9)));
                        }
                    }

                    if (block->_generatedVoxels)
                    {
                        handleMeshTiles(block->_voxelData, blockPos, lodTrigger, lodTriggerPos);
                    }
                }
            }
        }
    }
}

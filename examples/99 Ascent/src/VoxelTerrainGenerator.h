#ifndef __VOXELTERRAINGENERATOR__
#define __VOXELTERRAINGENERATOR__

#include <iNode.h>
using namespace Igor;

#include <iaEvent.h>
#include <iaVector3.h>
#include <iaSingleton.h>
using namespace IgorAux;

#include <unordered_map>
using namespace std;

#include "TaskGenerateVoxels.h"
#include "VoxelTerrainMeshGenerator.h"

namespace Igor
{
    class iNodeLODTrigger;
    class iVoxelData;
    class iScene;
}

iaEVENT(VoxelDataGeneratedEvent, VoxelDataGeneratedDelegate, void, (const iaVector3I& min, const iaVector3I& max), (min, max));

class VoxelTerrainGenerator : public iaSingleton<VoxelTerrainGenerator>
{

    friend class iaSingleton<VoxelTerrainGenerator>;

    class VectorHasher
    {
    public:
        size_t operator() (iaVector3I const& key) const
        {
            return (key._x << 1) ^ (key._y << 2) ^ (key._y << 3);
        }
    };

    class VectorEqualFn
    {
    public:
        bool operator() (iaVector3I const& t1, iaVector3I const& t2) const
        {
            return t1 == t2;
        };
    };

    struct TileData
    {
        uint32 _transformNodeID = iNode::INVALID_NODE_ID;
        uint32 _modelNodeID = iNode::INVALID_NODE_ID;
        vector<uint32> _destroyNodeIDs;
        uint32 _mutationCounter = 0;
    };

public:

    /*! sets lod trigger node to monitor
    */
    void setLODTrigger(uint32 lodTriggerID);

    void castRay(const iaVector3I& from, const iaVector3I& to, iaVector3I& outside, iaVector3I& inside);

    void setVoxelDensity(iaVector3I pos, uint8 density);
    uint8 getVoxelDensity(iaVector3I pos);

    void refreshTile(iaVector3I tilepos);

    bool loading();

    void registerVoxelDataGeneratedDelegate(VoxelDataGeneratedDelegate voxelDataGeneratedDelegate);
    void unregisterVoxelDataGeneratedDelegate(VoxelDataGeneratedDelegate voxelDataGeneratedDelegate);

    void setScene(iScene* scene);

private:

    VoxelDataGeneratedEvent _dataGeneratedEvent;

    /*! list with ids of currently running tasks
    */
    vector<uint64> _runningTasks;

    /*! tile IDs
    */
    unordered_map<iaVector3I, TileData, VectorHasher, VectorEqualFn> _tileDataSets;

    /*! the voxel data
    */
    unordered_map<iaVector3I, VoxelBlock*, VectorHasher, VectorEqualFn> _voxelBlocks;

    /*! all 3 dimensions
    */
    static const int64 _tilesCountPerVoxelBlock = 4;

    /*! size of a tile
    */
    static constexpr float32 _tileSize = 32;

    /*! size of a voxel block
    */
    static constexpr float32 _voxelBlockSize = _tileSize * _tilesCountPerVoxelBlock;

    /*! tile overlap
    */
    static constexpr float32 _tileOverlap = 4;

    /*! overlapping of voxel blocks
    */
    static constexpr float32 _voxelBlockOverlap = 5;

    static constexpr float32 _tileVisualDistance = 300.0f;
    static const int64 _tileCreationDistance = 200 * 200;
    static const int64 _tileDestructionDistance = 400 * 400;

    static const int64 _voxelBlockCreationDistance = 300 * 300;
    static const int64 _voxelBlockScanDistance = 2;

    /*! scene
    */
    iScene* _scene = nullptr;

    iScene* _effectScene = nullptr;

    /*! terrain material id
    */
    uint32 _terrainMaterialID = 0;
    
    /*! lod trigger node id
    */
    uint32 _lodTrigger = iNode::INVALID_NODE_ID;

    /*! called per frame
    */
    void onHandle();

    void setVoxelDensity(iaVector3I voxelBlock, iaVector3I voxelRelativePos, uint8 density);

    void handleVoxelBlocks();
    void handleMeshTiles(iVoxelData* voxelData, const iaVector3I& blockPos, iNodeLODTrigger* lodTrigger, const iaVector3I& lodTriggerPos);

    /*! called when task was finished

    \param taskID id of finished task
    */
    void onTaskFinished(uint64 taskID);

    /*! init
    */
    void init();

    /*! deinit
    */
    void deinit();

    /*! init
    */
    VoxelTerrainGenerator();

    /*! deinit
    */
    ~VoxelTerrainGenerator();

};

#endif
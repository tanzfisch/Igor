//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2014-2015 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it   
// under the terms of the GNU Lesser General Public License as published by  
// the Free Software Foundation; either version 3 of the License, or (at   
// your option) any later version.                                           
// 
// This library is distributed in the hope that it will be useful,           
// but WITHOUT ANY WARRANTY; without even the implied warranty of            
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
// Lesser General Public License for more details.                           
// 
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
// 
// contact: martinloga@gmx.de  

#ifndef __VOXELTERRAINGENERATOR__
#define __VOXELTERRAINGENERATOR__

#include <iNode.h>
using namespace Igor;

#include <iaVector3.h>
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

class VoxelTerrainGenerator
{

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
        bool _physicsGenerated = false;
        uint64 _bodyID = 0;
    };

public:

    /*! init
    */
    VoxelTerrainGenerator(iScene* scene);

    /*! deinit
    */
    ~VoxelTerrainGenerator();

    /*! sets lod trigger node to monitor
    */
    void setLODTrigger(iNodeLODTrigger* lodTrigger);

    const iaVector3I castRay(const iaVector3I& from, const iaVector3I& to);

    void setVoxelDensity(iaVector3I pos, uint8 density);
    uint8 getVoxelDensity(iaVector3I pos);

    void refreshTile(iaVector3I tilepos);

private:

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

    /*! all 3 dimensions
    */
    static const int64 _tileCreationDistance = 500 * 500;
    static constexpr float32 _tileVisualDistance = 500.0f;
    static const int64 _tileDestructionDistance = 800 * 800;
    static const int64 _physicCreationDistance = 50 * 50;
    static const int64 _voxelBlockcCreationDistance = 300 * 300;
    static const int64 _voxelBlockScanDistance = 7;

    uint32 _handleSectionID = 0;

    /*! scene
    */
    iScene* _scene = nullptr;

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

    /*! init
    */
    void init();

    /*! deinit
    */
    void deinit();

};

#endif
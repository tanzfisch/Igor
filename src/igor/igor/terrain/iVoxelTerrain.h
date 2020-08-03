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
// (c) Copyright 2012-2020 by Martin Loga
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
// contact: igorgameengine@protonmail.com

#ifndef __IGOR_VOXELTERRAINGENERATOR_H__
#define __IGOR_VOXELTERRAINGENERATOR_H__

#include <igor/scene/nodes/iNode.h>
#include <igor/scene/nodes/iNodeManager.h>

#include <igor/terrain/tasks/iTaskGenerateVoxels.h>
#include <igor/terrain/tasks/iTaskPropsOnVoxels.h>
#include <igor/terrain/iVoxelTerrainMeshGenerator.h>
#include <igor/terrain/data/iVoxelBlock.h>

#include <igor/terrain/operations/iVoxelOperation.h>
#include <igor/data/iAABox.h>

#include <iaux/system/iaEvent.h>
#include <iaux/math/iaVector3.h>
#include <iaux/system/iaMutex.h>
using namespace iaux;

#include <unordered_map>
#include <queue>

namespace igor
{

    class iNodeLODTrigger;
    class iVoxelData;
    class iScene;
    class iTargetMaterial;

    /*! voxel terrain class
    */
    class Igor_API iVoxelTerrain
    {

        friend class iTaskVoxelTerrain;

        /*! voxel block adress hasher
        */
        class iVectorHasher
        {
        public:
            size_t operator()(iaVector3I const &key) const
            {
                return (key._x << 1) ^ (key._y << 2) ^ (key._z << 3);
            }
        };

        /*! voxel block position comparsion
        */
        class iVectorEqualFn
        {
        public:
            bool operator()(iaVector3I const &t1, iaVector3I const &t2) const
            {
                return t1 == t2;
            };
        };

    public:
        /*! block quibic size

        \bug can't be changed right now
        */
        static const int32 _voxelBlockSize = 32;

        /*! block overlap

        \bug can't be changed right now
        */
        static const int32 _voxelBlockOverlap = 2;

        /*! initializes voxel terrain

        \param generateVoxelsDelegate callback to generate voxel data
        \param lodCount count of level of detail allowed range is 2-11 (right now the lowest LOD is not visible)
        \param voxelBlockSetupDistance distance in blocks of the lowest level of detail to be generated and visible when in range
        \param maxDiscoveryBoundaries optionally adding boundaries to the discovery area. Values are in voxel block coordinates of the lowest LOD
        */
        iVoxelTerrain(iVoxelTerrainGenerateDelegate generateVoxelsDelegate,
                      iVoxelTerrainPlacePropsDelegate placePropsDelegate,
                      uint32 lodCount = 11,
                      uint32 voxelBlockSetupDistance = 4,
                      const iaVector3I &maxDiscoveryBoundaries = iaVector3I(100000, 100000, 100000));

        /*! deinit
        */
        ~iVoxelTerrain();

        /*! sets the scene and initializes the terrain

        \param scene the scene to put the terrain in
        */
        void setScene(iScenePtr scene);

        /*! sets lod trigger node to monitor

        \param lodTriggerID the lod trigger's id
        */
        void setLODTrigger(uint32 lodTriggerID);

        /*! sets material ID

        \param materialID the material ID to use
        */
        void setMaterialID(uint64 materialID);

        /*! \returns terrain material ID
        */
        uint64 getMaterialID() const;

        /*! sets physics material ID

        \param materialID the material ID to use
        */
        void setPhysicsMaterialID(uint64 materialID);

        /*! \returns terrain material ID
        */
        uint64 getPhysicsMaterialID() const;

        /*! \returns target material
        */
        iTargetMaterial *getTargetMaterial();

        /*! modifies voxel data by manipulating a box area

        \param box the defined box area to manipulate
        \param density the density to set within the box area
        */
        void modify(const iAABoxI &box, uint8 density);

        /*! modifies voxel data by manipulating a sphere area

        \param sphere the defined area to manipulate
        \param density the density to set within the box area
        */
        void modify(const iSphereI &sphere, uint8 density);

        /*! casts ray to voxels to detect intersection

        \param from from where the ray is cast
        \param to to where the ray is cast to
        \param[out] outside if there is an intersection this is the last voxel right before
        \param[out] inside if there is an intersection this is the first voxel with density greater zero
        \returns true if there was an intersection
        */
        bool castRay(const iaVector3I &from, const iaVector3I &to, iaVector3I &outside, iaVector3I &inside);

        /*! \returns voxel density at given position
        \param pos the given position
        */
        uint8 getVoxelDensity(iaVector3I pos);

    private:
        /*! the discovery boundaries
        */
        iaVector3I _maxDiscoveryBoundaries;

        /*! lowest allowed lod
        */
        uint32 _lowestLOD = 0;

        /*! voxel block discovery distance in blocks
        */
        int64 _voxelBlockDiscoveryDistance = 0;

        /*! voxel block setup distance in blocks
        */
        int64 _voxelBlockSetupDistance = 0;

        /*! phyics material ID
        */
        uint64 _physicsMaterialID = 0;

        /*! target material for given tile
        */
        iTargetMaterial *_targetMaterial = nullptr;

        /*! voxel operations queue
        */
        std::vector<std::shared_ptr<iVoxelOperation>> _operationsQueue;

        /*! voxel operations queue mutex
        */
        iaMutex _operationsQueueMutex;

        /*! delegate registered by application to generate voxel data
        */
        iVoxelTerrainGenerateDelegate _generateVoxelsDelegate;

        /*! props placement delegate
        */
        iVoxelTerrainPlacePropsDelegate _placePropsDelegate;

        /*! queue of actions
        */
        std::vector<iNodeManager::iAction> _actionQueue;

        /*! mutex to protect action queue
        */
        iaMutex _mutexActionQueue;

        /*! the voxel data
        */
        std::vector<std::unordered_map<iaVector3I, iVoxelBlock *, iVectorHasher, iVectorEqualFn>> _voxelBlocks;

        /*! map of voxel block IDs to voxel blocks
        */
        std::map<uint64, iVoxelBlock *> _voxelBlocksMap;

        /*! next voxel block id
        */
        uint64 _nextVoxelBlockID = 1;

        /*! map of voxel blocks that have to be deleted
        */
        std::vector<iVoxelBlock *> _voxelBlocksToDelete;

        /*! keep observer position since last discovery
        */
        iaVector3I _lastDiscoveryPosition;

        /*! dirty flag if it is time for a rediscovery
        */
        bool _dirtyDiscovery = true;

        /*! root node of terrain
        */
        iNodePtr _rootNode = nullptr;

        /*! terrain material id
        */
        uint64 _terrainMaterialID = 0;

        /*! lod trigger node id
        */
        uint32 _lodTrigger = iNode::INVALID_NODE_ID;

        void collectBlocksToDelete(iVoxelBlock *currentBlock, std::vector<iVoxelBlock *> &dst);

        void setNeighboursDirty(iVoxelBlock *voxelBlock);
        void setInRange(iVoxelBlock *voxelBlock, bool inRange);

        iVoxelBlock *createVoxelBlock(uint32 lod, iaVector3I parentPositionInLOD, uint8 childAdress);

        void deleteBlocks();
        void deleteBlock(iVoxelBlock *voxelBlock);
        bool canBeDeleted(iVoxelBlock *voxelBlock);

        /*! discovers if there are unknown blocks of lowest LOD near by

        \param observerPosition current observer position
        */
        void discoverBlocks(const iaVector3I &observerPosition);

        /*! updates blocks

        \param observerPosition current observer position
        */
        void updateBlocks(const iaVector3I &observerPosition);

        /*! finds and attaches neighbours of a block

        \param voxelBlock the block to find neighbour for
        */
        void attachNeighbours(iVoxelBlock *voxelBlock);

        /*! detaches the blocks neighbours

        \param voxelBlock the block to detach
        */
        void detachNeighbours(iVoxelBlock *voxelBlock);

        /*! main handle callback
        */
        void update();

        void update(iVoxelBlock *voxelBlock, iaVector3I observerPosition);

        void applyVoxelOperations();
        void applyVoxelOperation(std::shared_ptr<iVoxelOperation> voxelOperation);

        bool updateVisibility(iVoxelBlock *voxelBlock);

        void updateMesh(iVoxelBlock *voxelBlock);
        void finalizeMesh(iVoxelBlock *voxelBlock);

        void setNodeActiveAsync(iNodePtr node, bool active);
        void insertNodeAsync(iNodePtr src, iNodePtr dst);
        void removeNodeAsync(iNodePtr src, iNodePtr dst);
        void destroyNodeAsync(uint64 nodeID);

        uint8 calcLODTransition(iVoxelBlock *voxelBlock);

        /*! init
        */
        void init();

        /*! deinit
        */
        void deinit();
    };

} // namespace igor

#endif // __IGOR_VOXELTERRAINGENERATOR_H__

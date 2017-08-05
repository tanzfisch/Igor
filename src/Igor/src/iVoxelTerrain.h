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
// (c) Copyright 2012-2017 by Martin Loga
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

#ifndef __iVOXELTERRAINGENERATOR__
#define __iVOXELTERRAINGENERATOR__

#include <iNode.h>
#include <iNodeFactory.h>

#include <iTaskGenerateVoxels.h>
#include <iVoxelTerrainMeshGenerator.h>
#include <iVoxelBlock.h>
#include <iVoxelOperation.h>

#include <iaEvent.h>
#include <iaVector3.h>
#include <iaMutex.h>
using namespace IgorAux;

#include <unordered_map>
#include <queue>
using namespace std;

namespace Igor
{

    class iNodeLODTrigger;
    class iVoxelData;
    class iScene;

    iaEVENT(iVoxelDataGeneratedEvent, iVoxelDataGeneratedDelegate, void, (const iaVector3I& min, const iaVector3I& max), (min, max));

    /*!
        \todo should not be a singleton
    */
    class Igor_API iVoxelTerrain
    {

        friend class iTaskVoxelTerrain;

        /*! voxel block adress hasher
        */
        class iVectorHasher
        {
        public:
            size_t operator() (iaVector3I const& key) const
            {
                return (key._x << 1) ^ (key._y << 2) ^ (key._z << 3);
            }
        };

        /*! voxel block position comparsion
        */
        class iVectorEqualFn
        {
        public:
            bool operator() (iaVector3I const& t1, iaVector3I const& t2) const
            {
                return t1 == t2;
            };
        };

    public:

        /*! lowest allowed lod

        more than 10 might not work
        */
        static const uint32 _lowestLOD = 10;

        /*! voxel block discovery distance
        */
        static const int64 _voxelBlockDiscoveryDistance = 4;

        /*! voxel block setup distance
        */
        static const int64 _voxelBlockSetupDistance = 2;

        /*! block quibic size

        can't be changed right now
        */
        static const int32 _voxelBlockSize = 32;

        /*! block overlap

        can't be changed right now
        */
        static const int32 _voxelBlockOverlap = 2;

        /*! init
        */
        iVoxelTerrain(iGenerateVoxelsDelegate generateVoxelsDelegate);

        /*! deinit
        */
        ~iVoxelTerrain();

        /*! sets the scene and initializes the terrain

        \param scene the scene to put the terrain in
        */
        void setScene(iScene* scene);

        /*! sets lod trigger node to monitor

        \param lodTriggerID the lod trigger's id
        */
        void setLODTrigger(uint32 lodTriggerID);

        /*! \returns terrain material ID
        */
        uint64 getMaterial() const;

        /*! modifies voxel data by manipulating a box area

        \param box the defined box area to manipulate
        \param density the density to set within the box area
        */
        void modify(const iAABoxI& box, uint8 density);

    private:

        vector<shared_ptr<iVoxelOperation>> _operationsQueue;

        iaMutex _operationsQueueMutex;

        /*! delegate registered by application to generate voxel data
        */
        iGenerateVoxelsDelegate _generateVoxelsDelegate;

        /*! queue of actions
        */
        vector<iNodeFactory::iAction> _actionQueue;

        /*! mutex to protect action queue
        */
        iaMutex _mutexActionQueue;

        /*! performance section discover blocks
        */
        uint32 _discoverBlocksSection = 0;

        /*! performance section delete blocks
        */
        uint32 _deleteBlocksSection = 0;

        /*! performance section apply actions
        */
        uint32 _applyActionsSection = 0;

        /*! performance section update blocks
        */
        uint32 _updateBlocksSection = 0;

        /*! performance section update visibility blocks
        */
        uint32 _updateVisBlocksSection = 0;

        /*! performance section total handle
        */
        uint32 _totalSection = 0;

        /*! the voxel data
        */
        vector<unordered_map<iaVector3I, iVoxelBlock*, iVectorHasher, iVectorEqualFn>> _voxelBlocks;

        /*! map of voxel block IDs to voxel blocks
        */
        map<uint64, iVoxelBlock*> _voxelBlocksMap;

        /*! next voxel block id
        */
        uint64 _nextVoxelBlockID = 1;

        /*! map of voxel blocks that have to be deleted
        */
        vector<iVoxelBlock*> _voxelBlocksToDelete;

        /*! keep observer position since last discovery
        */
        iaVector3I _lastDiscoveryPosition;

        /*! dirty flag if it is time for a rediscovery
        */
        bool _dirtyDiscovery = true;

        /*! root node of terrain
        */
        iNode* _rootNode = nullptr;

        /*! terrain material id
        */
        uint64 _terrainMaterialID = 0;

        /*! lod trigger node id
        */
        uint32 _lodTrigger = iNode::INVALID_NODE_ID;

        void collectBlocksToDelete(iVoxelBlock* currentBlock, vector<iVoxelBlock*>& dst);

        void setNeighboursDirty(iVoxelBlock* voxelBlock);
        void setInRange(iVoxelBlock* voxelBlock, bool inRange);

        iVoxelBlock* createVoxelBlock(uint32 lod, iaVector3I parentPositionInLOD, uint8 childAdress);

        void deleteBlocks();
        void deleteBlock(iVoxelBlock* voxelBlock);
        bool canBeDeleted(iVoxelBlock* voxelBlock);

        /*! discovers if there are unknown blocks of lowest LOD near by

        \param observerPosition current observer position
        */
        void discoverBlocks(const iaVector3I& observerPosition);

        /*! updates blocks

        \param observerPosition current observer position
        */
        void updateBlocks(const iaVector3I& observerPosition);

        /*! finds and attaches neighbours of a block

        \param voxelBlock the block to find neighbour for
        */
        void attachNeighbours(iVoxelBlock* voxelBlock);

        /*! detaches the blocks neighbours

        \param voxelBlock the block to detach
        */
        void detachNeighbours(iVoxelBlock* voxelBlock);

        /*! main handle callback
        */
        void update();

        void update(iVoxelBlock* voxelBlock, iaVector3I observerPosition);

        void applyVoxelOperations();
        void applyVoxelOperation(shared_ptr<iVoxelOperation> voxelOperation);

        bool updateVisibility(iVoxelBlock* voxelBlock);

        void updateMesh(iVoxelBlock* voxelBlock);
        void finalizeMesh(iVoxelBlock* voxelBlock);

        void setNodeActiveAsync(iNode* node, bool active);
        void insertNodeAsync(iNode* src, iNode* dst);
        void removeNodeAsync(iNode* src, iNode* dst);
        void destroyNodeAsync(uint64 nodeID);

        uint8 calcLODTransition(iVoxelBlock* voxelBlock);

        /*! init
        */
        void init(iScene* scene);

        /*! deinit
        */
        void deinit();

    };

}

#endif

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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_VOXELBLOCK__
#define __IGOR_VOXELBLOCK__

#include <igor/terrain/data/iVoxelData.h>
#include <igor/threading/iTaskManager.h>
#include <igor/scene/nodes/iNode.h>
#include <igor/threading/tasks/iTask.h>

#include <iaux/math/iaVector3.h>
using namespace iaux;

#include <memory>

namespace igor
{
    class iScene;

    /*! state which a voxel block is in
    */
    enum class iVoxelBlockState : uint8
    {
        Initial,
        Setup,
        GeneratingVoxel,
        GeneratingMesh,
        Ready,
        Empty
    };

    struct iVoxelBlockInfo;
    class iVoxelOperation;

    /*! voxel block (or tile) or specific LOD
    */
    struct iVoxelBlock
    {

    public:
        /*! invalid voxel block id definition
        */
        static const uint64 INVALID_VOXELBLOCKID = 0;

        /*! block ID
        */
        uint64 _id = INVALID_VOXELBLOCKID;

        /*! bit mask with current neighbors LOD settings
        */
        uint8 _neighboursLOD = 0;

        /*! if dirty the tile has to be regenerated
        */
        bool _dirty = true;

        /*! if true mesh is in visible range (and SHOULD be visible)

        but can be actually not rendered because e.g. the mesh is not ready yet
        */
        bool _inRange = false;

        /*! if true neighbours changed and we might have to regenerate the mesh
        */
        bool _dirtyNeighbours = true;

        /*! id of voxel generation task

        there is only one at a time needed
        */
        uint64 _voxelGenerationTaskID = iTask::INVALID_TASK_ID;

        /*! id to transform node to control if a tile is in the scene and therefore visible
        */
        uint64 _transformNodeIDCurrent = iNode::INVALID_NODE_ID;

        /*! id to generated model currently in use
        */
        uint64 _modelNodeIDCurrent = iNode::INVALID_NODE_ID;

        /*! temporary transform node id to control where we have to regenerate a new tile or not
        */
        uint64 _transformNodeIDQueued = iNode::INVALID_NODE_ID;

        /*! temporary id of node so we can tell if it was already loaded
        */
        uint64 _modelNodeIDQueued = iNode::INVALID_NODE_ID;

        /*! every time the tile changes this counter goes up so Igor can tell the difference between the models before and after
        */
        uint16 _mutationCounter = 0;

        /*! index position of block relative to parent
        */
        uint8 _childAddress;

        /*! blocks position as index in corresponding LOD
        */
        iaVector3I _positionInLOD;

        /*! level of detail of this block
        */
        uint32 _lod = 0;

        /*! world size of block
        */
        uint16 _size = 0;

        /*! current state of the block
        */
        iVoxelBlockState _state = iVoxelBlockState::Initial;

        /*! the actual voxel data
        */
        iVoxelData *_voxelData = nullptr;

        /*! voxel block info needed to generated voxel data
        */
        iVoxelBlockInfo *_voxelBlockInfo = nullptr;

        /*! pointer to parenting block
        */
        uint64 _parent = INVALID_VOXELBLOCKID;

        /*! indexes to children
        */
        uint64 _children[8];

        /*! indexes to neighbour in same LOD
        */
        uint64 _neighbours[6];

        /*! voxel operation on this block
        */
        std::vector<std::shared_ptr<iVoxelOperation>> _voxelOperations;
    };

} // namespace igor

#endif
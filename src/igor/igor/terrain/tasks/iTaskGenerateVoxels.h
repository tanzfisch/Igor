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

#ifndef __IGOR_TASKGENERATEVOXELS__
#define __IGOR_TASKGENERATEVOXELS__

#include <igor/threading/tasks/iTask.h>

#include <iaux/data/iaSphere.h>
#include <iaux/math/iaVector3.h>
#include <iaux/system/iaSlot.h>
using namespace iaux;

#include <vector>

namespace igor
{
    class iVoxelData;

    /*! voxel block information

    contains information needed to be able to generate the voxel data for specified voxel block
    */
    struct iVoxelBlockInfo
    {
        /*! absolute position of voxel block in LOD voxel coordinates
        */
        iaVector3I _positionInLOD;

        /*! level of detail dependent offset to absolute grid
        */
        iaVector3f _lodOffset;

        /*! level of detail of this block
        */
        uint32 _lod;

        /*! size of voxel block in voxel
        */
        uint64 _size;

        /*! the destination voxel data structure
        */
        iVoxelData *_voxelData = nullptr;

        /*! true if the generated voxel data contains an air to solid transition
        */
        bool _transition = false;
    };

    /*! callback to generate voxel data

    \param voxelBlockInfo contains all information to generate the voxels
    */
    IGOR_SLOT(iVoxelTerrainGenerateDelegate, void, (iVoxelBlockInfo * voxelBlockInfo), (voxelBlockInfo));

    // TODO add callback onAfterMeshGeneration ?

    /*! task to generate voxels.

    the actual voxel generation happens in the callback function to be implemented by application
    */
    class iTaskGenerateVoxels : public iTask
    {

    public:
        /*! initializes member variables

        \param voxelBlockInfo the voxel block to generate the data for
        \param priority the priority to run this task with
        \param generateVoxelsDelegate the delegate to do the actual work
        \param voxelDataGeneratedDelegate the delegate to be called after generating the voxels
        */
        iTaskGenerateVoxels(iVoxelBlockInfo *voxelBlockInfo, uint32 priority, iVoxelTerrainGenerateDelegate generateVoxelsDelegate);

        /*! does nothing
        */
        virtual ~iTaskGenerateVoxels() = default;

    protected:
        /*! runs the task
        */
        void run();

    private:
        /*! delegate that generated the voxels
        */
        iVoxelTerrainGenerateDelegate _generateVoxelsDelegate;

        /*! the data to work with
        */
        iVoxelBlockInfo *_voxelBlockInfo = nullptr;
    };
} // namespace igor

#endif

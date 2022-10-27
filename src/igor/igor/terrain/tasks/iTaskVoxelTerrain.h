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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __IGOR_TASKVOXELTERRAIN__
#define __IGOR_TASKVOXELTERRAIN__

#include <igor/threading/tasks/iTask.h>

namespace igor
{

    class iVoxelTerrain;

    /*! voxel terrain task running the main loop of one voxel terrain instance
    */
    class iTaskVoxelTerrain : public iTask
    {

    public:
        /*! initializes member variables

        \param window window connected to render context
        */
        iTaskVoxelTerrain(iVoxelTerrain *voxelTerrain);

        /*! does nothing
        */
        virtual ~iTaskVoxelTerrain() = default;

    protected:
        /*! runs the task
        */
        void run();

    private:
        /*! the data to work with
        */
        iVoxelTerrain *_voxelTerrain = nullptr;
    };

} // namespace igor

#endif

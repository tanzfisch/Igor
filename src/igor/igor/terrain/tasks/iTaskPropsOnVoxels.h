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

#ifndef __IGOR_TASKPROPSONVOXELS__
#define __IGOR_TASKPROPSONVOXELS__

#include <igor/threading/tasks/iTask.h>

#include <iaux/data/iaSphere.h>
#include <iaux/math/iaVector3.h>
#include <iaux/system/iaEvent.h>
using namespace iaux;

#include <vector>

namespace igor
{
    class iVoxelData;

    struct iVoxelBlockPropsInfo
    {
        iaVector3I _min;
        iaVector3I _max;
    };

    /*! callback for placing props on terrain
    */
    typedef iaDelegate<void, iVoxelBlockPropsInfo> iVoxelTerrainPlacePropsDelegate;

    class iTaskPropsOnVoxels : public iTask
    {

    public:
        iTaskPropsOnVoxels(iVoxelBlockPropsInfo voxelBlockPropsInfo, uint32 priority, iVoxelTerrainPlacePropsDelegate placePropsDelegate);

        /*! does nothing
        */
        virtual ~iTaskPropsOnVoxels() = default;

    protected:
        /*! runs the task
        */
        void run();

    private:
        /*! delegate that place the props
        */
        iVoxelTerrainPlacePropsDelegate _placePropsDelegate;

        /*! the data to work with
        */
        iVoxelBlockPropsInfo _voxelBlockPropsInfo;
    };
} // namespace igor

#endif

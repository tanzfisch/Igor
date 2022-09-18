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

#ifndef __iVOXELOPERATIONSPHERE__
#define __iVOXELOPERATIONSPHERE__

#include <igor/terrain/operations/iVoxelOperation.h>

#include <iaux/data/iaSphere.h>

namespace igor
{

    /*! voxel operation that set the values of an area specified by a sphere
    */
    class iVoxelOperationSphere : public iVoxelOperation
    {

    public:
        /*! ctor initializes members

        \param box the box to manipulate
        \param density the new density to set
        */
        iVoxelOperationSphere(const iSphereI &sphere, uint8 density);

        /*! applies the operation within a specified voxel block

        \param voxelBlock the current voxel block to manipulate
        */
        void apply(iVoxelBlock *voxelBlock);

        /*! returns boundings of this operation

        \param boundings the param to return the operation boundings with
        */
        void getBoundings(iAABoxI &boundings);

    private:
        /*! sphere that defines wich area to set the new value
        */
        iSphereI _sphere;

        /*! the new density to set
        */
        uint8 _density = 0;
    };

} // namespace igor

#endif
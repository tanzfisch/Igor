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

#ifndef __iVOXELOPERATION__
#define __iVOXELOPERATION__

#include <iAABox.h>

namespace Igor
{

    struct iVoxelBlock;

    /*! base class for voxel operations on a voxel terrain
    */
    class iVoxelOperation
    {

    public:

        /*! applies operation on voxel terrain data

        \param voxelBlock the voxel data block to apply operation to
        */
        virtual void apply(iVoxelBlock* voxelBlock) = 0;

        /*! returns bounding box of operation

        \param[out] boundings returning bounding box of the magnitude of this operation
        */
        virtual void getBoundings(iAABoxI& boundings) = 0;

    };

}

#endif
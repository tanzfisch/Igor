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

#ifndef __TASKGENERATEVOXELS__
#define __TASKGENERATEVOXELS__

#include <iTask.h>
using namespace Igor;

#include <iaVector3.h>
using namespace IgorAux;

namespace Igor
{
    class iVoxelData;
}

struct VoxelBlock
{
    bool _generatedVoxels = false;
    iaVector3I _offset;
    iaVector3i _size;
    iVoxelData* _voxelData = nullptr;
};

class TaskGenerateVoxels : public iTask
{

public:

    /*! initializes member variables

    \param window window connected to render context
    */
    TaskGenerateVoxels(VoxelBlock* voxelBlock, uint32 priority);

    /*! does nothing
    */
    virtual ~TaskGenerateVoxels() = default;

protected:

    /*! runs the task
    */
    void run();

private:

    /*! the data to work with
    */
    VoxelBlock* _voxelBlock = nullptr;

};

#endif
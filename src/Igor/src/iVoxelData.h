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
// (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __iVOXELDATACHUNK__
#define __iVOXELDATACHUNK__

#include <iDefines.h>
#include <iaString.h>
#include <iaRLE.h>
using namespace IgorAux;

#include <iaVector3.h>

#include <vector>
using namespace std;

namespace Igor
{

    struct iVoxelPole
    {
        iaRLE<uint8, uint8> _density;
        iaRLE<uint8, uint8> _material;
    };

	class Igor_API iVoxelData
    {

    public:

        /*! does nothing
        */
        iVoxelData() = default;

        /*! release memory
        */
        ~iVoxelData();

        /*! specifies size of voxel data and allocates memory

        \param width width of voxel grid in voxels in x direction
        \param height height of voxel grid in voxels in y direction
        \param depth depth of voxel grid in voxels in z direction

        !!! ATTENTION this method must be called before any other method is called
        */
        void initData(int64 width, int64 height, int64 depth);

        /*! does same as initData but keeps the preset width height and depht
        */
        void clear();

        /*! \returns width of voxel data
        */
        int64 getWidth();

        /*! \returns depth of voxel data
        */
        int64 getDepth();

        /*! \returns height of voxel data
        */
        int64 getHeight();
 
        /*! sets density of specified voxel

        \param pos the voxel to change the density
        \param density the new density
        */
        void setVoxelDensity(iaVector3I pos, uint8 density);

        /*! \returns density of specified voxel

        \param pos the voxel to get the density from
        */
        uint8 getVoxelDensity(iaVector3I pos);

        void setVoxelMaterial(iaVector3I pos, uint8 material);
        uint8 getVoxelMaterial(iaVector3I pos);

        iaRLE<uint8, uint8>& getDensityBlocks(int64 xDir, int64 zDir);
        iaRLE<uint8, uint8>& getMaterialBlocks(int64 xDir, int64 zDir);

        void setVoxelLine(iaVector3I pos1, iaVector3I pos2, uint8 density);
        vector<uint8> getVoxelLine(iaVector3I pos1, iaVector3I pos2);

        bool getIntersection(iaVector3I pos, iaVector3I dir, iaVector3I &returnPos, uint8 &returnValue);

        void setClearValue(uint8 clearValue);
        uint8 getClearValue() const;

    private:

        /*! the data
        */
		iVoxelPole* _data = nullptr;

		int64 _width = 0;
		int64 _depth = 0;
		int64 _height = 0;

        uint8 _clearValue = 0;
		uint8 _nonSolidValue = 0;

        void freeMemory();

	};

}

#endif

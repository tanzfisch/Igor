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

#ifndef __iSIMPLECUBES__
#define __iSIMPLECUBES__

#include <iVoxelData.h>
#include <iMeshBuilder.h>
#include <iaVector3.h>

#include <vector>


namespace Igor
{

    /*! Implements a simplified versiopn of the marching cubes algorythm to turn voxel data in to mesh data.
    */
	class Igor_API iSimpleCubes
	{

    public:

        iSimpleCubes();

        void setVoxelData(iVoxelData* voxelData);

        void compile(iMeshBuilder& meshBuilder, iaVector3I pos, iaVector3I volume);

	private:

		iVoxelData* _voxelData = nullptr;

		std::vector<iVoxelBlock>::iterator _ait;
		std::vector<iVoxelBlock>::iterator _bit;
		std::vector<iVoxelBlock>::iterator _cit;
		std::vector<iVoxelBlock>::iterator _dit;

        uint8 _a1Density = 0;
        uint8 _a2Density = 0;
        uint8 _b1Density = 0;
        uint8 _b2Density = 0;
        uint8 _c1Density = 0;
        uint8 _c2Density = 0;
        uint8 _d1Density = 0;
        uint8 _d2Density = 0;

		uint64 _aitPos = 0;
        uint64 _bitPos = 0;
        uint64 _citPos = 0;
        uint64 _ditPos = 0;

		iaVector3I _cubePosition;
        
		iaVector3I _cubeStartPosition;
		iaVector3I _marchingVolume;

		void generateGeometry(iMeshBuilder& meshBuilder, uint8 value);
        uint8 getNextCube();
		void startGettingCubes(iaVector3I &start_position);

	};

};

#endif
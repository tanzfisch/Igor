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

#ifndef __iCONTOURINGCUBES__
#define __iCONTOURINGCUBES__

#include <iVoxelData.h>
#include <iMeshBuilder.h>

#include <iaVector3.h>
#include <iaRLE.h>
using namespace IgorAux;

#include <vector>
using namespace std;

namespace Igor
{

    /*! an algorythm inspired by dual contouring and marching cubes
    */
	class Igor_API iContouringCubes
	{

        /*! pole (as in vertically arranged voxels) iterator
        */
        struct DensityPoleIterator
        {
            /*! current density block in pole
            */
            vector<iaRLEBlock<uint8, uint8>>::const_iterator _currentDensityBlock;

            /*! current density position as index
            */
            uint64 _currentDensityBlockPosition;

            /*! current density block in pole
            */
            vector<iaRLEBlock<uint8, uint8>>::const_iterator _currentMaterialBlock;

            /*! current density position as index
            */
            uint64 _currentMaterialBlockPosition;
        };

    public:

        /*! initialize members
        */
        iContouringCubes();

        /*! sets voxel data to be used

        \param voxelData the voxel data
        */
        void setVoxelData(iVoxelData* voxelData);

        /*! compile mesh out of voxel data

        \param pos location in voxel data to be compiled
        \param volume the size of the are to be compiled
        \returns mesh
        */
        shared_ptr<iMesh> compile(iaVector3I pos, iaVector3I volume);

	private:

        /*! pointer to voxel data
        */
		iVoxelData* _voxelData = nullptr;

        /*! current poles (3 times 3) for iterating through the voxel data
        */
        vector<DensityPoleIterator> _currentPoles;

        /*! density cache
        */
        float32 _density[3*3*3];

        /*! material cache
        */
        uint8 _material[3*3*3];

        /*! current interation position
        */
		iaVector3I _cubePosition;

        /*! start position within the pole
        */
		iaVector3I _cubeStartPosition;

        /*! keeps a list of all triangles to the corresponding material id
        */
        map<uint32, vector<uint32>> _trianglesToKeep;

        /*! meshbuilder to work with
        */
        iMeshBuilder _meshBuilder;

        /*! calculates vertex position allong iso surface

        \param density0
        \param density1
        \param density2
        \param density3
        \param density4
        \param density5
        \param density6
        \param density7
        \param pos out value vertex position
        \param normal out value vertex normal
        */
        iaVector3f calculateVertex(float32 density0, float32 density1, float32 density2, float32 density3, float32 density4, float32 density5, float32 density6, float32 density7);

        /*! creates triangles for the resulting mesh

        \param keepTriangles if the triangles created are to keep
        */
		void generateGeometry(bool keepTriangles);

        /*! climbs up the pole
        */
        void climb();

        /*! initializes start position on bottom of pole at specified position

        \param startPosition defines which poles to use next
        */
		void getPoles(iaVector3I &startPosition);

	};

};

#endif
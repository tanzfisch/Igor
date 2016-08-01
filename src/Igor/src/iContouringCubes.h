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
        struct DensityPole
        {
            /*! current density pole
            */
            iaRLE<uint8, uint8>* _currentDensityPole = nullptr;
            
            /*! current density pole
            */
            iaRLE<uint8, uint8>* _currentMaterialPole = nullptr;
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
		shared_ptr<iMesh> compile(iaVector3I pos, iaVector3I volume, uint32 lod, uint32 neighbors = 0);

	private:

        /*! pointer to voxel data
        */
		iVoxelData* _voxelData = nullptr;

        /*! current poles (3 times 3) for iterating through the voxel data
        */
        vector<DensityPole> _currentPoles;

        /*! density cache
        */
        uint8 _density[3 * 3 * 3];

		iaVector3f _vertexPositions[2 * 2 * 2];

		iaVector3f _vertexNormals[2 * 2 * 2];

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

		/*! model lod
		*/
		uint32 _lod = 0;

		/*! model scale
		*/
        float64 _scale = 0;

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
        void calculateVertex(uint8 density0, uint8 density1, uint8 density2, uint8 density3, uint8 density4, uint8 density5, uint8 density6, uint8 density7, iaVector3f& vertex, iaVector3f& normal);

        /*! creates triangles for the resulting mesh

        \param keepTriangles if the triangles created are to keep
        */
		void generateGeometry(const iaVector3f& transformedCubePosition, const uint8* density, bool keepTriangles, uint32 neighborLODs);

        /*! climbs up the pole
        */
        void climb();
		void climbVoxel();
		void climbPositionAndNormals();

        /*! initializes start position on bottom of pole at specified position

        \param startPosition defines which poles to use next
        */
		void startClimb(iaVector3I &startPosition);

	};

};

#endif
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

#ifndef __IGOR_CONTOURINGCUBES__
#define __IGOR_CONTOURINGCUBES__

#include <igor/terrain/data/iVoxelBlock.h>
#include <igor/resources/mesh/iMeshBuilder.h>

#include <iaux/math/iaVector3.h>
#include <iaux/data/iaRLE.h>
using namespace iaux;

#include <vector>

namespace igor
{

    /*! an algorythm inspired by dual contouring and marching cubes
    */
    class IGOR_API iContouringCubes
    {

        /*! pole (as in vertically arranged voxels) iterator
        */
        struct DensityPole
        {
            /*! current density pole
            */
            iaRLE<uint8, uint8> *_currentDensityPole = nullptr;
        };

    public:
        /*! initialize members
        */
        iContouringCubes();

        /*! sets voxel data to be used

        \param voxelData the voxel data
        */
        void setVoxelData(iVoxelData *voxelData);

        /*! sets voxel data of next LOD level (lower resolution)

        voxel data to next LOD is optional

        \param voxelData the voxel data of the next LOD
        */
        void setVoxelDataNextLOD(iVoxelData *voxelData);

        /*! sets offset to next (lower resolution) lod voxel data

        \param offset offset to next lod voxel data
        */
        void setNextLODVoxelOffset(const iaVector3I &offset);

        /*! compile mesh out of voxel data

        \param pos location in voxel data to be compiled
        \param volume the size of the are to be compiled
        \returns mesh
        */
        iMeshPtr compile(iaVector3I pos, iaVector3I volume, uint32 lod = 0, uint32 neighbors = 0);

        /*! calculates scale based on LOD

        \param lod the lod to cal the scale for
        \returns the scale of the given lod 
        \todo create a table
        */
        static float64 calcLODScale(uint32 lod);

        /*! calculates the lod offset based on lod

        \param lod given lod
        \returns offset for given lod
        \todo create a table
        */
        static iaVector3f calcLODOffset(uint32 lod);

    private:
        /*! pointer to voxel data
        */
        iVoxelData *_voxelData = nullptr;

        /*! pointer to voxel data of next LOD with lesser resolution
        */
        iVoxelData *_voxelDataNextLOD = nullptr;

        /*! current poles (3 times 3) for iterating through the voxel data
        */
        std::vector<DensityPole> _currentPoles;

        /*! density cache
        */
        uint8 _density[3 * 3 * 3];

        /*! next lod density cache
        */
        uint8 _nextLODDensity[3 * 3 * 3];

        /*! mixed los density cache
        */
        uint8 _mixedDensity[3 * 3 * 3];

        /*! next lod offset
        */
        iaVector3I _nextLODVoxelOffset;

        /*! vertices of next lod voxel data
        */
        iaVector3f _vertexPositionsNextLOD[27];

        /*! visible flags for vertices of next lod voxel data
        */
        bool _vertexPositionsNextLODVisible[27];

        /*! current interation position
        */
        iaVector3I _cubePosition;

        /*! start position within the pole
        */
        iaVector3I _cubeStartPosition;

        /*! keeps a list of all triangles to the corresponding material id
        */
        std::map<uint32, std::vector<uint32>> _trianglesToKeep;

        /*! meshbuilder to work with
        */
        iMeshBuilder _meshBuilder;

        /*! model scale
		*/
        uint32 _lod = 0;

        /*! scale of current lod voxel data
        */
        float64 _scale = 0;

        /*! scale of next level lod voxel data
        */
        float64 _scaleNextLOD = 0;

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
        bool calculateVertex(uint8 density0, uint8 density1, uint8 density2, uint8 density3, uint8 density4, uint8 density5, uint8 density6, uint8 density7, iaVector3f &vertex);

        /*! adds a triangle to mesh

        \param a first index of triangle
        \param b second index of triangle
        \param c third index of triangle
        \param keep 
        */
        void addCheckedTriangle(uint32 a, uint32 b, uint32 c, bool keep);

        void calculateNextLOD();

        iaVector3f _offset;
        iaVector3f _offsetNextLOD;

        /*! creates triangles for the resulting mesh

        \param keepTriangles if the triangles created are to keep
        */
        void generateGeometry(bool keepTriangles, uint32 neighborLODs);

        /*! climbs up the pole
        */
        void climb();

        /*! initializes start position on bottom of pole at specified position

        \param startPosition defines which poles to use next
        */
        void startClimb(iaVector3I &startPosition);
    };

}; // namespace igor

#endif
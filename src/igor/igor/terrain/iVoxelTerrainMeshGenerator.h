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

#ifndef __IGOR_GENERATORTERRAIN__
#define __IGOR_GENERATORTERRAIN__

#include <igor/resources/model/loader/iModelDataIO.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/material/iTargetMaterial.h>

#include <iaux/math/iaVector3.h>
#include <iaux/math/iaRandomNumberGenerator.h>
using namespace iaux;

namespace igor
{
    class iContouringCubes;
    class iVoxelData;
    class iMeshBuilder;

    /*! tile information package to be able to generate a cetain tile
    */
    struct iVoxelTerrainTileInformation
    {
        /*! level of details
        */
        uint32 _lod = 0;

        /*! voxel data of current tile
        */
        iVoxelData *_voxelData = nullptr;

        /*! voxel data of current tile but from next lower LOD
        */
        iVoxelData *_voxelDataNextLOD = nullptr;

        /*! offset to next LOD in real world coordinates
        */
        iaVector3I _voxelOffsetToNextLOD;

        /*! material of tile
        */
        iMaterialPtr _material;

        /*! target material for given tile
        */
        iTargetMaterialPtr _targetMaterial;

        /*! physics material of terrain tile
        */
        uint64 _physicsMaterialID = 0;

        /*! neighbors LOD flags
        */
        uint32 _neighboursLOD = 0;
    };

    /*! voxel terrain mesh generator
    */
    class iVoxelTerrainMeshGenerator : public iModelDataIO
    {

    public:
        /*! generates terrain tiles

        !!! ATTENTION consumes and deletes "parameter"

        \param filename usually the file name but we are not using it here because wedon't load but generate data
        \return parameter tile parameters
        */
        iNodePtr importData(const iaString &filename, iModelDataInputParameterPtr parameter);

        /*! initialize members
        */
        iVoxelTerrainMeshGenerator();

        /*! does nothing
        */
        virtual ~iVoxelTerrainMeshGenerator() = default;

        /*! creates an instance of this class

        \returns new instance
        */
        static iModelDataIO *createInstance();
    };

} // namespace igor

#endif

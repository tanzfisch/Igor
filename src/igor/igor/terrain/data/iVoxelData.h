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

#ifndef __IGOR_VOXELDATACHUNK__
#define __IGOR_VOXELDATACHUNK__

#include <igor/iDefines.h>

#include <iaux/data/iaString.h>
#include <iaux/data/iaRLE.h>
#include <iaux/math/iaVector3.h>
using namespace iaux;

#include <vector>

namespace igor
{

    struct iVoxelPole
    {
        iaRLE<uint8, uint8> _density;
        iaRLE<uint8, uint8> _material;
    };

    /*!
     Density is an 8 bit value where 0 means outside of the body and every value >0 lieas within the body.
     But a value of 1 actually means a density of zero too only that it will generate a body with zero volume.
     This way we can generate slopes down to a volume of zero or walls with a thickness of zero.
    */
    class IGOR_API iVoxelData
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

        \todo why are we not doing it in the ctor?
        */
        void initData(int64 width, int64 height, int64 depth);

        void getCopy(iVoxelData &dst);

        /*! does same as initData but keeps the preset width height and depht
        */
        void clear();

        /*! \returns width of voxel data
        */
        int64 getWidth() const;

        /*! \returns depth of voxel data
        */
        int64 getDepth() const;

        /*! \returns height of voxel data
        */
        int64 getHeight() const;

        /*! sets density of specified voxel

        \param pos the voxel to change the density
        \param density the new density (see explanation above)
        */
        void setVoxelDensity(iaVector3I pos, uint8 density);

        /*! \returns density of specified voxel

        \param pos the voxel to get the density from
        */
        uint8 getVoxelDensity(iaVector3I pos);

        void setVoxelMaterial(iaVector3I pos, uint8 material);
        uint8 getVoxelMaterial(iaVector3I pos);

        iaRLE<uint8, uint8> &getDensityPole(int64 xDir, int64 zDir);
        iaRLE<uint8, uint8> &getMaterialPole(int64 xDir, int64 zDir);

        /*! sets a line of voxels to a target density

		\param pos1 the from pos
		\param pos2 the to pos
		\param density the density to set
		*/
        void setVoxelLine(iaVector3I pos1, iaVector3I pos2, uint8 density);

        /*! reads a line of densities from the voxels

        \param pos1 the from position of the line
        \param pos2 the to position of the line
        \param[out] dst the destination vector

        \todo this was not tested
        */
        void getVoxelLine(iaVector3I pos1, iaVector3I pos2, std::vector<uint8> &dst);

        /*! sets density of specified vertical voxel pole

        \param pos start pos of the pole in voxel coordinates
        \param height height of the pole
        \param density the new density (see explanation above)
        */
        void setVoxelPole(iaVector3I pos, int64 height, uint8 density);

        bool getIntersection(iaVector3I pos, iaVector3I dir, iaVector3I &returnPos, uint8 &returnValue);

        void setClearValue(uint8 clearValue);
        uint8 getClearValue() const;

        bool hasData() const;

    private:
        /*! the data
        */
        iVoxelPole *_data = nullptr;

        int64 _width = 0;
        int64 _depth = 0;
        int64 _height = 0;

        uint8 _clearValue = 0;
        uint8 _nonSolidValue = 0;

        void freeMemory();
    };

} // namespace igor

#endif // __IGOR_VOXELDATACHUNK__

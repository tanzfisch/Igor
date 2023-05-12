// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/terrain/operations/iVoxelOperationSphere.h>

#include <igor/terrain/data/iVoxelBlock.h>
#include <igor/terrain/iVoxelTerrain.h>

#include <algorithm>

namespace igor
{

    iVoxelOperationSphere::iVoxelOperationSphere(const iaSphereI &sphere, uint8 density)
    {
        _sphere = sphere;
        _density = density;
    }

    void iVoxelOperationSphere::getBoundings(iAABoxI &boundings)
    {
        boundings._center = _sphere._center;
        boundings._halfWidths.set(_sphere._radius, _sphere._radius, _sphere._radius);
    }

    void iVoxelOperationSphere::apply(iVoxelBlock *voxelBlock)
    {
        const int64 lodFactor = static_cast<int64>(pow(2, voxelBlock->_lod));

        // get voxel block boundings
        const int64 fullVoxelBlockSize = iVoxelTerrain::_voxelBlockSize + iVoxelTerrain::_voxelBlockOverlap;
        const iaVector3I voxelBlockFrom = voxelBlock->_positionInLOD * iVoxelTerrain::_voxelBlockSize;
        iaVector3I voxelBlockTo = voxelBlockFrom;
        voxelBlockTo._x += fullVoxelBlockSize;
        voxelBlockTo._y += fullVoxelBlockSize;
        voxelBlockTo._z += fullVoxelBlockSize;

        const iaVector3I center = (_sphere._center / lodFactor) - voxelBlockFrom;
        const int64 radius = _sphere._radius / lodFactor;

        // figure out size of manipulated area
        iaVector3I from = center;
        from._x -= radius;
        from._y -= radius;
        from._z -= radius;
        iaVector3I to = center;
        to._x += radius;
        to._y += radius;
        to._z += radius;

        // skip trivial cases
        if (to._x < 0 ||
            to._y < 0 ||
            to._z < 0)
        {
            return;
        }

        if (from._x > fullVoxelBlockSize ||
            from._y > fullVoxelBlockSize ||
            from._z > fullVoxelBlockSize)
        {
            return;
        }

        // clip against voxel block boundings
        from._x = std::max(from._x, (int64)0);
        from._y = std::max(from._y, (int64)0);
        from._z = std::max(from._z, (int64)0);

        to._x = std::min(to._x, fullVoxelBlockSize);
        to._y = std::min(to._y, fullVoxelBlockSize);
        to._z = std::min(to._z, fullVoxelBlockSize);

        const int64 radius2 = radius * radius;

        // iterate though positions
        for (int64 x = from._x; x < to._x; ++x)
        {
            for (int64 y = from._y; y < to._y; ++y)
            {
                for (int64 z = from._z; z < to._z; ++z)
                {
                    iaVector3I pos(x, y, z);
                    if (center.distance(pos) < radius)
                    {
                        voxelBlock->_voxelData->setVoxelDensity(pos, _density);
                    }
                }
            }
        }
    }

} // namespace igor

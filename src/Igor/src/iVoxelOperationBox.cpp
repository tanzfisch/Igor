// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iVoxelOperationBox.h>

#include <iVoxelBlock.h>
#include <iVoxelTerrain.h>

namespace Igor
{

    iVoxelOperationBox::iVoxelOperationBox(const iAABoxI& box, uint8 density)
    {
        _box = box;
        _density = density;
    }

    void iVoxelOperationBox::getBoundings(iAABoxI& boundings)
    {
        boundings = _box;
    }

    void iVoxelOperationBox::apply(iVoxelBlock* voxelBlock)
    {
        iaVector3I from = _box._center;
        from -= _box._halfWidths;
        iaVector3I to = _box._center;
        to += _box._halfWidths;

        int64 lodFactor = pow(2, voxelBlock->_lod);
        from /= lodFactor;
        to /= lodFactor;

        int64 fullVoxelBlockSize = iVoxelTerrain::_voxelBlockSize + iVoxelTerrain::_voxelBlockOverlap;
        iaVector3I voxelBlockFrom = voxelBlock->_positionInLOD * iVoxelTerrain::_voxelBlockSize;
        iaVector3I voxelBlockTo = voxelBlockFrom;
        voxelBlockTo._x += fullVoxelBlockSize;
        voxelBlockTo._y += fullVoxelBlockSize;
        voxelBlockTo._z += fullVoxelBlockSize;

        if (to._x < voxelBlockFrom._x ||
            to._y < voxelBlockFrom._y ||
            to._z < voxelBlockFrom._z)
        {
            return;
        }

        if (from._x > voxelBlockTo._x ||
            from._y > voxelBlockTo._y ||
            from._z > voxelBlockTo._z)
        {
            return;
        }

        from -= voxelBlockFrom;
        to -= voxelBlockFrom;

        if (from._x < 0)
        {
            from._x = 0;
        }
        if (from._y < 0)
        {
            from._y = 0;
        }
        if (from._z < 0)
        {
            from._z = 0;
        }

        if (to._x > fullVoxelBlockSize)
        {
            to._x = fullVoxelBlockSize;
        }
        if (to._y > fullVoxelBlockSize)
        {
            to._y = fullVoxelBlockSize;
        }
        if (to._z > fullVoxelBlockSize)
        {
            to._z = fullVoxelBlockSize;
        }

        int64 poleHeight = to._y - from._y;

        for (int64 x = from._x; x < to._x; ++x)
        {
            for (int64 z = from._z; z < to._z; ++z)
            {
                voxelBlock->_voxelData->setVoxelPole(iaVector3I(x, from._y, z), poleHeight, _density);
            }
        }
    }

}


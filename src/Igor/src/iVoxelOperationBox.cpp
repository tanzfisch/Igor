// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
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
        const int64 lodFactor = static_cast<int64>(pow(2, voxelBlock->_lod));

        // get voxel block boundings
        const int64 fullVoxelBlockSize = iVoxelTerrain::_voxelBlockSize + iVoxelTerrain::_voxelBlockOverlap;
        const iaVector3I voxelBlockFrom = voxelBlock->_positionInLOD * iVoxelTerrain::_voxelBlockSize;
        iaVector3I voxelBlockTo = voxelBlockFrom;
        voxelBlockTo._x += fullVoxelBlockSize;
        voxelBlockTo._y += fullVoxelBlockSize;
        voxelBlockTo._z += fullVoxelBlockSize;

        const iaVector3I center = (_box._center / lodFactor) - voxelBlockFrom;
        const iaVector3I halfWidths = _box._halfWidths / lodFactor;

        // figure out size of manipulated area
        iaVector3I from = center - halfWidths;
        iaVector3I to = center + halfWidths;

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
        from._x = max(from._x, 0);
        from._y = max(from._y, 0);
        from._z = max(from._z, 0);

        to._x = min(to._x, fullVoxelBlockSize);
        to._y = min(to._y, fullVoxelBlockSize);
        to._z = min(to._z, fullVoxelBlockSize);

        // iterate though positions
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


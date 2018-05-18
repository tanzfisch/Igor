// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskGenerateVoxels.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iTaskGenerateVoxels::iTaskGenerateVoxels(iVoxelBlockInfo* voxelBlockInfo, uint32 priority, iGenerateVoxelsDelegate generateVoxelsDelegate, iVoxelDataGeneratedDelegate voxelDataGeneratedDelegate)
        : iTask(nullptr, priority, false)
    {
        _voxelBlockInfo = voxelBlockInfo;
        _generateVoxelsDelegate = generateVoxelsDelegate;
        _voxelDataGeneratedDelegate = voxelDataGeneratedDelegate;
    }

    void iTaskGenerateVoxels::run()
    {
        con_assert(voxelBlockInfo != nullptr, "zero pointer");
        con_assert(voxelBlockInfo->_voxelData != nullptr, "zero pointer");

        if (_voxelBlockInfo != nullptr &&
            _voxelBlockInfo->_voxelData != nullptr)
        {
            _generateVoxelsDelegate(_voxelBlockInfo);
            _voxelDataGeneratedDelegate(_voxelBlockInfo);
        }
    }

}


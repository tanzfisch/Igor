// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskGenerateVoxels.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iTaskGenerateVoxels::iTaskGenerateVoxels(iVoxelBlockInfo* voxelBlockInfo, uint32 priority, iGenerateVoxelsDelegate generateVoxelsDelegate)
        : iTask(nullptr, priority, false)
    {
        con_assert(voxelBlockInfo != nullptr, "zero pointer");
        con_assert(voxelBlockInfo->_voxelData != nullptr, "zero pointer");

        _generateVoxelsDelegate = generateVoxelsDelegate;
        _voxelBlockInfo = voxelBlockInfo;
    }

    void iTaskGenerateVoxels::run()
    {
        _generateVoxelsDelegate(_voxelBlockInfo);
    }

}


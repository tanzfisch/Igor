// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/terrain/tasks/iTaskGenerateVoxels.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iTaskGenerateVoxels::iTaskGenerateVoxels(iVoxelBlockInfo *voxelBlockInfo, uint32 priority, iVoxelTerrainGenerateDelegate generateVoxelsDelegate)
        : iTask(nullptr, priority, false)
    {
        _voxelBlockInfo = voxelBlockInfo;
        _generateVoxelsDelegate = generateVoxelsDelegate;
    }

    void iTaskGenerateVoxels::run()
    {
        con_assert(_voxelBlockInfo != nullptr, "zero pointer");
        con_assert(_voxelBlockInfo->_voxelData != nullptr, "zero pointer");

        if (_voxelBlockInfo != nullptr &&
            _voxelBlockInfo->_voxelData != nullptr)
        {
            _generateVoxelsDelegate(_voxelBlockInfo);
        }
    }

} // namespace igor

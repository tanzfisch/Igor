// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/terrain/tasks/iTaskPropsOnVoxels.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iTaskPropsOnVoxels::iTaskPropsOnVoxels(iVoxelBlockPropsInfo voxelBlockPropsInfo, uint32 priority, iVoxelTerrainPlacePropsDelegate placePropsDelegate)
        : iTask(nullptr, priority, false)
    {
        _voxelBlockPropsInfo = voxelBlockPropsInfo;
        _placePropsDelegate = placePropsDelegate;
    }

    void iTaskPropsOnVoxels::run()
    {
        _placePropsDelegate(_voxelBlockPropsInfo);
    }

} // namespace igor

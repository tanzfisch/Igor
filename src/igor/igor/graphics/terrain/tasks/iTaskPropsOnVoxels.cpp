// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/graphics/terrain/tasks/iTaskPropsOnVoxels.h>

#include <iaux/system/iaConsole.h>
using namespace IgorAux;

namespace Igor
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

} // namespace Igor

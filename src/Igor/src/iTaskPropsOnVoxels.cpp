// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskPropsOnVoxels.h>

#include <iaConsole.h>
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

}


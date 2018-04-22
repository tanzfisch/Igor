// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskVoxelTerrain.h>

#include <iVoxelTerrain.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iTaskVoxelTerrain::iTaskVoxelTerrain(iVoxelTerrain* voxelTerrain)
        : iTask(nullptr, 0, true)
    {
        con_assert(voxelTerrain != nullptr, "zero pointer");

        if (voxelTerrain != nullptr)
        {
            _voxelTerrain = voxelTerrain;
        }
        else
        {
            con_err("zero pointer");
        }
    }

    void iTaskVoxelTerrain::run()
    {
        if (_voxelTerrain != nullptr)
        {
            _voxelTerrain->update();
        }
    }

}


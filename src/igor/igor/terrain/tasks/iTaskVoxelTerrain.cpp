// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/terrain/tasks/iTaskVoxelTerrain.h>

#include <igor/terrain/iVoxelTerrain.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iTaskVoxelTerrain::iTaskVoxelTerrain(iVoxelTerrain *voxelTerrain)
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

} // namespace igor

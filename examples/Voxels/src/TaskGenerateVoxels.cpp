// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "TaskGenerateVoxels.h""

#include <iModelResourceFactory.h>
#include <iTextureResourceFactory.h>
#include <iWindow.h>
#include <iPerlinNoise.h>
#include <iVoxelData.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

TaskGenerateVoxels::TaskGenerateVoxels(VoxelBlock* voxelBlock, uint32 priority)
    : iTask(nullptr, priority, false, false)
{
    con_assert(voxelBlock != nullptr, "zero pointer");
    con_assert(voxelBlock->_voxelData != nullptr, "zero pointer");

    _voxelBlock = voxelBlock;
}

void TaskGenerateVoxels::run()
{
    iPerlinNoise perlinNoise;

    iVoxelData* voxelData = _voxelBlock->_voxelData;
    iaVector3I& offset = _voxelBlock->_offset;
    iaVector3i& size = _voxelBlock->_size;

    voxelData->initData(size._x, size._y, size._z);

     /*  for (int64 x = 50; x < voxelData->getWidth() - 50; ++x)
       {
           for (int64 y = 50; y < voxelData->getHeight() - 50; ++y)
           {
               for (int64 z = 50; z < voxelData->getDepth() - 50; ++z)
               {
                   voxelData->setVoxelDensity(iaVector3I(x, y, z), 255);
               }
           }
       }/**/

    const float64 from = 0.384;
    const float64 to = 0.4;
    float64 factor = 1.0 / (to - from);

    for (int64 x = 0; x < voxelData->getWidth(); ++x)
    {
        for (int64 y = 0; y < voxelData->getHeight(); ++y)
        {
            for (int64 z = 0; z < voxelData->getDepth(); ++z)
            {
                float64 xn = (x + offset._x);
                float64 yn = (y + offset._y);
                float64 zn = (z + offset._z);

                float64 onoff = perlinNoise.getValue(iaVector3d(xn * 0.01, yn * 0.0075, zn * 0.01), 4, 0.5);

                if (onoff <= to)
                {
                    if (onoff >= from)
                    {
                        float64 gradient = 1.0 - ((onoff - from) * factor);
                        voxelData->setVoxelDensity(iaVector3I(x, y, z), (gradient * 254) + 1);
                    }
                    else
                    {
                        voxelData->setVoxelDensity(iaVector3I(x, y, z), 255);
                    }
                }
                else
                {
                    voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                }
            }
        }
    }/**/

    // looks nice for a terrain surface
    /*for (int64 x = 0; x < voxelData->getWidth(); ++x)
    {
        for (int64 z = 0; z < voxelData->getDepth(); ++z)
        {
            int64 xn = (x + offset._x);
            int64 zn = (z + offset._z);
            // there is no offset in y for this demo

            float64 warpedX = perlinNoise.getValue(iaVector3d(xn * 0.1, 0, zn * 0.1), 1, 0.5) * 10.0f;
            float64 warpedZ = perlinNoise.getValue(iaVector3d(xn * 0.1, 0, zn * 0.1), 1, 0.5) * 10.0f;

            float64 height = perlinNoise.getValue(iaVector3d((xn + warpedX) * 0.006, 0, (zn + warpedZ) * 0.006), 7, 0.45) * voxelData->getHeight();
            int64 y = height;
            float32 density = height - static_cast<float32>(y);

            voxelData->setVoxelLine(iaVector3I(x, 0, z), iaVector3I(x, y - 1, z), 255);
            voxelData->setVoxelDensity(iaVector3I(x, y, z), (density * 254.0f) + 1.0f);

            y = height - 50;
            if (y < 3)
            {
                y = 3;
            }

            for (; y < height && y < voxelData->getHeight(); ++y)
            {
                float64 onoff = perlinNoise.getValue(iaVector3d(xn * 0.01, y * 0.01, zn * 0.01), 4, 0.5);

                if (onoff < 0.4)
                {
                    if (onoff < 0.35)
                    {
                        voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                    }
                    else
                    {
                        float64 density = perlinNoise.getValue(iaVector3d(xn * 0.01, y * 0.01, zn * 0.01), 3, 0.5);
                        voxelData->setVoxelDensity(iaVector3I(x, y, z), (density * 254) + 1);
                    }
                }
            }
        }
    }*/

    _voxelBlock->_generatedVoxels = true;
    _isRunning = false;
}


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
    : iTask(nullptr, priority, false, iTaskContext::Default)
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

    /*    for (int64 x = 0; x < voxelData->getWidth() - 0; ++x)
        {
            for (int64 y = 0; y < voxelData->getHeight() - 0; ++y)
            {
                for (int64 z = 0; z < voxelData->getDepth() - 0; ++z)
                {
                    if (x < 40 || x > 80 ||
                        y<40 || y > 80 ||
                        z<40 || z > 80)
                    {
                        voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                    }
                }
            }
        }/**/

    const float64 from = 0.444;
    const float64 to = 0.45;
    float64 factor = 1.0 / (to - from);
    iaVector3I playerStartPos(10000, 9400, 10000);
    const float64 maxHeight = 10000.0f;
    const float64 lowerSurface = 9750.0f;

    for (int64 x = 0; x < voxelData->getWidth(); ++x)
    {
        for (int64 y = 0; y < voxelData->getHeight(); ++y)
        {
            for (int64 z = 0; z < voxelData->getDepth(); ++z)
            {
                float64 yn = (y + offset._y);

                if (yn < maxHeight)
                {
                    float64 xn = (x + offset._x);
                    float64 zn = (z + offset._z);

                    float64 onoff = perlinNoise.getValue(iaVector3d(xn * 0.02, yn * 0.02, zn * 0.02), 4, 0.5);

                    if (onoff <= from)
                    {
                        if (onoff >= to)
                        {
                            float64 gradient = 1.0 - ((onoff - from) * factor);
                            voxelData->setVoxelDensity(iaVector3I(x, y, z), (gradient * 254) + 1);
                        }
                        else
                        {
                            voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                        }
                    }

                    iaVector3I pos(xn, yn, zn);

                    // create a hole were the player starts
                    if (playerStartPos.distance2(pos) < 50 * 50)
                    {
                        voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                    }

                    if (yn >= lowerSurface)
                    {
                        float64 maxHeight = perlinNoise.getValue(iaVector3d(xn * 0.005, 0, zn * 0.005), 6, 0.5) * 250.0;
                        int64 maxInt = maxHeight;

                        if (yn > maxInt + lowerSurface)
                        {
                            if (yn > maxInt + lowerSurface + 1)
                            {
                                voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                            }
                            else
                            {
                                if (voxelData->getVoxelDensity(iaVector3I(x, y, z)) != 0)
                                {
                                    float32 density = maxHeight - static_cast<float32>(maxInt);
                                    voxelData->setVoxelDensity(iaVector3I(x, y, z), (density * 254.0f) + 1.0f);
                                }
                            }
                        }

                    }
                }
                else
                {
                    voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                }
            }
        }
    }/**/

/*
    const float64 from = 0.444;
    const float64 to = 0.45;
    float64 factor = 1.0 / (to - from);
    iaVector3I playerStartPos(10000, 9400, 10000);
    const float64 maxHeight = 10000.0f;
    const float64 lowerSurface = 9750.0f;

    for (int64 x = 0; x < voxelData->getWidth(); ++x)
    {
        for (int64 y = 0; y < voxelData->getHeight(); ++y)
        {
            for (int64 z = 0; z < voxelData->getDepth(); ++z)
            {
                float64 yn = (y + offset._y);

                if (yn < maxHeight)
                {
                    float64 xn = (x + offset._x);
                    float64 zn = (z + offset._z);

                    float64 onoff = perlinNoise.getValue(iaVector3d(xn * 0.02, yn * 0.02, zn * 0.02), 4, 0.5);

                    if (onoff <= from)
                    {
                        if (onoff >= to)
                        {
                            float64 gradient = 1.0 - ((onoff - from) * factor);
                            voxelData->setVoxelDensity(iaVector3I(x, y, z), (gradient * 254) + 1);
                        }
                        else
                        {
                            voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                        }
                    }

                    iaVector3I pos(xn, yn, zn);

                    // create a hole were the player starts
                    if (playerStartPos.distance2(pos) < 50 * 50)
                    {
                        voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                    }

                    if (yn >= lowerSurface)
                    {
                        float64 maxHeight = perlinNoise.getValue(iaVector3d(xn * 0.005, 0, zn * 0.005), 6, 0.5) * 250.0;
                        int64 maxInt = maxHeight;

                        if (yn > maxInt + lowerSurface)
                        {
                            if (yn > maxInt + lowerSurface + 1)
                            {
                                voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                            }
                            else
                            {
                                if (voxelData->getVoxelDensity(iaVector3I(x, y, z)) != 0)
                                {
                                    float32 density = maxHeight - static_cast<float32>(maxInt);
                                    voxelData->setVoxelDensity(iaVector3I(x, y, z), (density * 254.0f) + 1.0f);
                                }
                            }
                        }

                    }
                }
                else
                {
                    voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                }
            }
        }
    }/**/

    _voxelBlock->_generatedVoxels = true;

    finishTask();
}


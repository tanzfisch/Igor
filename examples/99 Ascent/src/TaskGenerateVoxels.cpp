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

__IGOR_INLINE__ float64 metaballFunction(iaVector3f metaballPos, iaVector3f checkPos)
{
    return 1.0 / ((checkPos._x - metaballPos._x) * (checkPos._x - metaballPos._x) + (checkPos._y - metaballPos._y) * (checkPos._y - metaballPos._y) + (checkPos._z - metaballPos._z) * (checkPos._z - metaballPos._z));
}

void TaskGenerateVoxels::run()
{
    iPerlinNoise perlinNoise;
    iaVector3I playerStartPos(10000, 9400, 10000); // TODO

    iVoxelData* voxelData = _voxelBlock->_voxelData;
    iaVector3I& offset = _voxelBlock->_offset;
    iaVector3i& size = _voxelBlock->_size;

    voxelData->initData(size._x, size._y, size._z);

    const float64 from = 0.444;
    const float64 to = 0.45;
    float64 factor = 1.0 / (to - from);

    const float64 fromMeta = 0.017;
    const float64 toMeta = 0.0175;
    float64 factorMeta = 1.0 / (toMeta - fromMeta);

    vector<pair<iaVector3f, float64>> metaballs;
    for (int i = 0; i < 500; ++i)
    {
        metaballs.push_back(pair<iaVector3f, float64>(iaVector3f(playerStartPos._x + (rand() % 300) - 150, playerStartPos._y + (rand() % 300) - 150, playerStartPos._z + (rand() % 300) - 150 - 200), (rand() % 90 + 10) / 100.0));
    }

    for (int64 x = 0; x < voxelData->getWidth() - 0; ++x)
    {
        for (int64 y = 0; y < voxelData->getHeight() - 0; ++y)
        {
            for (int64 z = 0; z < voxelData->getDepth() - 0; ++z)
            {
                // first figure out if a voxel is outside the sphere
                iaVector3f pos(x + offset._x, y + offset._y, z + offset._z);

                float64 distance = 0;
                for (auto metaball : metaballs)
                {
                    distance += metaballFunction(metaball.first, pos) * metaball.second * 1.5;
                }

                if (distance <= toMeta)
                {
                    if (distance >= fromMeta)
                    {
                        float32 denstity = ((distance - fromMeta) * factorMeta);
                        voxelData->setVoxelDensity(iaVector3I(x, y, z), (denstity * 254) + 1);
                    }
                    else
                    {
                        voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                    }
                }

                float64 onoff = perlinNoise.getValue(iaVector3d(pos._x * 0.03, pos._y * 0.03, pos._z * 0.03), 4, 0.5);

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
            }
        }
    }/**/

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

/*    const float64 from = 0.444;
    const float64 to = 0.45;
    float64 factor = 1.0 / (to - from);
    
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


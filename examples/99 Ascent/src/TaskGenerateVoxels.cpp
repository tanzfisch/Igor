#include "TaskGenerateVoxels.h"

#include <iModelResourceFactory.h>
#include <iTextureResourceFactory.h>
#include <iWindow.h>
#include <iPerlinNoise.h>
#include <iVoxelData.h>
#include <iSphere.h>
#include <iTimer.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

int32 TaskGenerateVoxels::_seed = 0;
vector<iSpheref> TaskGenerateVoxels::_metaballs;
vector<iSpheref> TaskGenerateVoxels::_holes;
iaMutex TaskGenerateVoxels::_initMutex;

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

void TaskGenerateVoxels::prepareLevel(iaVector3I playerStartPos)
{
    _initMutex.lock();
    if (_seed == 0)
    {
        int32 seed = iTimer::getInstance().getTime();
        srand(seed);

        // covering the boss
        _metaballs.push_back(iSpheref(iaVector3f(playerStartPos._x + 20, playerStartPos._y, playerStartPos._z - 200), 1.7));
        _metaballs.push_back(iSpheref(iaVector3f(playerStartPos._x - 20, playerStartPos._y, playerStartPos._z - 200), 1.7));
        _metaballs.push_back(iSpheref(iaVector3f(playerStartPos._x, playerStartPos._y + 20, playerStartPos._z - 200), 1.7));
        _metaballs.push_back(iSpheref(iaVector3f(playerStartPos._x, playerStartPos._y - 20, playerStartPos._z - 200), 1.7));
        _metaballs.push_back(iSpheref(iaVector3f(playerStartPos._x, playerStartPos._y, playerStartPos._z - 200 + 20), 1.7));
        _metaballs.push_back(iSpheref(iaVector3f(playerStartPos._x, playerStartPos._y, playerStartPos._z - 200 - 20), 1.7));
        
        // hole where the boss sits
        _holes.push_back(iSpheref(iaVector3f(playerStartPos._x, playerStartPos._y, playerStartPos._z - 200), 1.7));

        // body
        for (int i = 0; i < 10; ++i)
        {
            iaVector3f pos(rand() % 50 - 25, rand() % 50 - 25, rand() % 50 - 25);
            pos.normalize();
            pos *= 20 + (rand() % 20);

            pos._x += playerStartPos._x;
            pos._y += playerStartPos._y;
            pos._z += playerStartPos._z - 200;

            _metaballs.push_back(iSpheref(pos, ((rand() % 90 + 10) / 100.0) * 1.7));
        }

        // body surface crates
        /*for (int i = 0; i < 150; ++i)
        {
            iaVector3f pos(rand() % 50 - 25, rand() % 50 - 25, rand() % 50 - 25);
            pos.normalize();
            pos *= 70 + (rand() % 10);

            pos._x += playerStartPos._x;
            pos._y += playerStartPos._y;
            pos._z += playerStartPos._z - 200;

            _holes.push_back(iSpheref(pos, ((rand() % 90 + 10) / 100.0) * 0.7));
        }*/
        
        _seed = seed;
    }
    _initMutex.unlock();
}

void TaskGenerateVoxels::run()
{
    iaVector3I playerStartPos(10000, 9400, 10000); // TODO

    prepareLevel(playerStartPos);

	iPerlinNoise perlinNoise;

	iVoxelData* voxelData = _voxelBlock->_voxelData;
	iaVector3I& offset = _voxelBlock->_offset;
	iaVector3i& size = _voxelBlock->_size;
    
    voxelData->setClearValue(0);
	voxelData->initData(size._x, size._y, size._z);

    // skip all the voxel blocks that are too far away
    if (iaVector3f(playerStartPos._x, playerStartPos._y, playerStartPos._z - 200).distance(iaVector3f(offset._x + size._x * 0.5, offset._y + size._y * 0.5, offset._z + size._z * 0.5)) < 250)
    {
        const float64 from = 0.444;
        const float64 to = 0.45;
        float64 factor = 1.0 / (to - from);

        const float64 fromMeta = 0.017;
        const float64 toMeta = 0.0175;
        float64 factorMeta = 1.0 / (toMeta - fromMeta);

        for (int64 x = 0; x < voxelData->getWidth() - 0; ++x)
        {
            for (int64 y = 0; y < voxelData->getHeight() - 0; ++y)
            {
                for (int64 z = 0; z < voxelData->getDepth() - 0; ++z)
                {
                    float32 density = 0;

                    // first figure out if a voxel is outside the sphere
                    iaVector3f pos(x + offset._x, y + offset._y, z + offset._z);

                    float64 distance = 0;
                    for (auto metaball : _metaballs)
                    {
                        distance += metaballFunction(metaball._center, pos) * metaball._radius;
                    }

                    if (distance >= toMeta)
                    {
                        density = 1.0;
                    }
                    else
                    {
                        if (distance >= fromMeta)
                        {
                            density = ((distance - fromMeta) * factorMeta);
                        }
                    }

                    distance = 0;
                    for (auto hole : _holes)
                    {
                        distance += metaballFunction(hole._center, pos) * hole._radius;
                    }

                    if (distance >= toMeta)
                    {
                        density = 0.0;
                    }
                    else
                    {
                        if (distance >= fromMeta)
                        {
                            if (density > 0)
                            {
                                density = 1 - ((distance - fromMeta) * factorMeta);
                            }
                        }
                    }

                    float64 onoff = perlinNoise.getValue(iaVector3d(pos._x * 0.04, pos._y * 0.04, pos._z * 0.04), 4, 0.5);

                    if (onoff <= from)
                    {
                        if (onoff >= to)
                        {
                            density = 1.0 - ((onoff - from) * factor);
                        }
                        else
                        {
                            density = 0.0;
                        }
                    }

                    if (density > 1.0) 
                    { 
                        density = 1.0; 
                    }
                    
                    if (density < 0.0) 
                    { 
                        density = 0.0; 
                    }

                    if (density > 0.0)
                    {
                        voxelData->setVoxelDensity(iaVector3I(x, y, z), (density * 254) + 1);
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
                        if (x<60 || x > 70 ||
                            y<60 || y > 70 ||
                            z<60 || z > 70)
                        {
                            voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                        }
                    }
                }
            }/**/
    }

	_voxelBlock->_generatedVoxels = true;
}


#include "TaskGenerateVoxels.h""

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
mutex TaskGenerateVoxels::_initMutex;

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

void TaskGenerateVoxels::prepareLevel()
{
    _initMutex.lock();
    if (_seed == 0)
    {
        iaVector3I playerStartPos(10000, 9400, 10000); // TODO
        int32 seed = iTimer::getInstance().getTime();
        srand(seed);

        for (int i = 0; i < 30; ++i)
        {
            _metaballs.push_back(iSpheref(iaVector3f(playerStartPos._x + (rand() % 100) - 50,
                playerStartPos._y + (rand() % 100) - 50,
                playerStartPos._z + (rand() % 100) - 50 - 200),
                ((rand() % 90 + 10) / 100.0) * 2.6));
        }

        for (int i = 0; i < 7; ++i)
        {
            _holes.push_back(iSpheref(iaVector3f(playerStartPos._x + (rand() % 50) - 25,
                playerStartPos._y + (rand() % 50) - 25,
                playerStartPos._z + (rand() % 50) - 25 - 200),
                ((rand() % 90 + 10) / 100.0) * 0.5));
        }

        // hole where the boss sits
        _holes.push_back(iSpheref(iaVector3f(playerStartPos._x, playerStartPos._y, playerStartPos._z - 200), 1.4));

        _seed = seed;
    }
    _initMutex.unlock();
}

void TaskGenerateVoxels::run()
{
    prepareLevel();

	iPerlinNoise perlinNoise;

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

	for (int64 x = 0; x < voxelData->getWidth() - 0; ++x)
	{
		for (int64 y = 0; y < voxelData->getHeight() - 0; ++y)
		{
			for (int64 z = 0; z < voxelData->getDepth() - 0; ++z)
			{
				// first figure out if a voxel is outside the sphere
				iaVector3f pos(x + offset._x, y + offset._y, z + offset._z);

                // generate some detail noise we will add every where
                float64 detailNoise = perlinNoise.getValue(iaVector3d(pos._x * 0.5, pos._y * 0.5, pos._z * 0.5), 1) - 0.5;

				float64 distance = 0;
				for (auto metaball : _metaballs)
				{
					distance += metaballFunction(metaball._center, pos) * metaball._radius;
				}

				if (distance >= toMeta)
				{
					voxelData->setVoxelDensity(iaVector3I(x, y, z), 255);
				}
				else
				{
					if (distance >= fromMeta)
					{
						float32 denstity = ((distance - fromMeta) * factorMeta) + detailNoise;
                        if (denstity > 1.0) { denstity = 1.0; }
                        if (denstity < 0.0) { denstity = 0.0; }
						voxelData->setVoxelDensity(iaVector3I(x, y, z), (denstity * 254) + 1);
					}
				}

				distance = 0;
				for (auto hole : _holes)
				{
					distance += metaballFunction(hole._center, pos) * hole._radius;
				}

				if (distance >= toMeta)
				{
					voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
				}
				else
				{
					if (distance >= fromMeta)
					{
                        if (voxelData->getVoxelDensity(iaVector3I(x, y, z)) != 0)
                        {
                            float32 denstity = 1 - ((distance - fromMeta) * factorMeta);
                            voxelData->setVoxelDensity(iaVector3I(x, y, z), (denstity * 254) + 1);
                        }
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
					if (x<60 || x > 70 ||
						y<60 || y > 70 ||
						z<60 || z > 70)
					{
						voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
					}
				}
			}
		}/**/

	_voxelBlock->_generatedVoxels = true;

	finishTask();
}


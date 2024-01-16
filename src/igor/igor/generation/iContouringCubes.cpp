// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/generation/iContouringCubes.h>

#include <igor/system/iTimer.h>

#include <iaux/math/iaMatrix2D.h>
using namespace iaux;

#include <cstring>

#define HIGHER_NEIGHBOR_LOD_XPOSITIVE 0x20
#define HIGHER_NEIGHBOR_LOD_XNEGATIVE 0x10
#define HIGHER_NEIGHBOR_LOD_YPOSITIVE 0x08
#define HIGHER_NEIGHBOR_LOD_YNEGATIVE 0x04
#define HIGHER_NEIGHBOR_LOD_ZPOSITIVE 0x02
#define HIGHER_NEIGHBOR_LOD_ZNEGATIVE 0x01

#define FILL_LOD_GAPS
//#define DEV_CUBIFY

namespace igor
{

    /*
    //             4
    //             |   2
    //             |  /
    //             | /
    //             |/
    //   3---------*---------1
    //            /|
    //           / |
    //          /  |
    //         0   |
    //             5

    //    Y
    //    |
    //    |
    //    0---X
    //   /
    //  /
    // Z

    */
    iaVector3f dirs[] =
        {
            iaVector3f(0.0, 0.0, 1.0),  // 0
            iaVector3f(1.0, 0.0, 0.0),  // 1
            iaVector3f(0.0, 0.0, -1.0), // 2
            iaVector3f(-1.0, 0.0, 0.0), // 3
            iaVector3f(0.0, 1.0, 0.0),  // 4
            iaVector3f(0.0, -1.0, 0.0)  // 5
    };

    iContouringCubes::iContouringCubes()
        : _voxelData(0)
    {
        for (int i = 0; i < 9; ++i)
        {
            DensityPole pole;
            _currentPoles.push_back(pole);
        }

        _meshBuilder.setJoinVertices(true);

        for (int i = 0; i < 27; ++i)
        {
            _vertexPositionsNextLODVisible[i] = false;
        }
    }

    /*! helper macro to map density values from 0&1-255 to length 0.0-1.0

    0 -> density is zero therefore it is outside the object
    1 -> is also integpreted als density zero (1-255 -> 0.0-1.0)
    */
#define rescale(value) ((value > 0 ? value - 1 : value) / 254.0)

    void iContouringCubes::addCheckedTriangle(uint32 a, uint32 b, uint32 c, bool keep)
    {
        if (a != b && b != c && c != a)
        {
            uint32 triangleIndex = _meshBuilder.addTriangle(a, b, c);

            if (keep)
            {
                _trianglesToKeep[0].push_back(triangleIndex);
            }
        }
    }

    bool iContouringCubes::calculateVertex(uint8 density0, uint8 density1, uint8 density2, uint8 density3, uint8 density4, uint8 density5, uint8 density6, uint8 density7, iaVector3f &vertex)
    {
        int div = 0;
        iaVector3f calcPos;

        float32 d0 = static_cast<float32>(rescale(static_cast<float32>(density0)));
        float32 d1 = static_cast<float32>(rescale(static_cast<float32>(density1)));
        float32 d2 = static_cast<float32>(rescale(static_cast<float32>(density2)));
        float32 d3 = static_cast<float32>(rescale(static_cast<float32>(density3)));
        float32 d4 = static_cast<float32>(rescale(static_cast<float32>(density4)));
        float32 d5 = static_cast<float32>(rescale(static_cast<float32>(density5)));
        float32 d6 = static_cast<float32>(rescale(static_cast<float32>(density6)));
        float32 d7 = static_cast<float32>(rescale(static_cast<float32>(density7)));

        if (density0 != 0 && density1 == 0)
        {
            calcPos._x += d0;
            div++;
        }

        if (density1 != 0 && density0 == 0)
        {
            calcPos._x += 1 - d1;
            div++;
        }

        if (density0 != 0 && density2 == 0)
        {
            calcPos._z += d0;
            div++;
        }

        if (density2 != 0 && density0 == 0)
        {
            calcPos._z += 1 - d2;
            div++;
        }

        if (density0 != 0 && density4 == 0)
        {
            calcPos._y += d0;
            div++;
        }

        if (density4 != 0 && density0 == 0)
        {
            calcPos._y += 1 - d4;
            div++;
        }

        if (density7 != 0 && density6 == 0)
        {
            calcPos._x += 1 - d7;
            calcPos._y += 1;
            calcPos._z += 1;
            div++;
        }

        if (density6 != 0 && density7 == 0)
        {
            calcPos._x += d6;
            calcPos._y += 1;
            calcPos._z += 1;
            div++;
        }

        if (density7 != 0 && density5 == 0)
        {
            calcPos._x += 1;
            calcPos._y += 1;
            calcPos._z += 1 - d7;
            div++;
        }

        if (density5 != 0 && density7 == 0)
        {
            calcPos._x += 1;
            calcPos._y += 1;
            calcPos._z += d5;
            div++;
        }

        if (density7 != 0 && density3 == 0)
        {
            calcPos._x += 1;
            calcPos._y += 1 - d7;
            calcPos._z += 1;
            div++;
        }

        if (density3 != 0 && density7 == 0)
        {
            calcPos._x += 1;
            calcPos._y += d3;
            calcPos._z += 1;
            div++;
        }

        if (density4 != 0 && density5 == 0)
        {
            calcPos._x += d4;
            calcPos._y += 1;
            div++;
        }

        if (density5 != 0 && density4 == 0)
        {
            calcPos._x += 1 - d5;
            calcPos._y += 1;
            div++;
        }

        if (density5 != 0 && density1 == 0)
        {
            calcPos._x += 1;
            calcPos._y += 1 - d5;
            div++;
        }

        if (density1 != 0 && density5 == 0)
        {
            calcPos._x += 1;
            calcPos._y += d1;
            div++;
        }

        if (density1 != 0 && density3 == 0)
        {
            calcPos._x += 1;
            calcPos._z += d1;
            div++;
        }

        if (density3 != 0 && density1 == 0)
        {
            calcPos._x += 1;
            calcPos._z += 1 - d3;
            div++;
        }

        if (density3 != 0 && density2 == 0)
        {
            calcPos._x += 1 - d3;
            calcPos._z += 1;
            div++;
        }

        if (density2 != 0 && density3 == 0)
        {
            calcPos._x += d2;
            calcPos._z += 1;
            div++;
        }

        if (density2 != 0 && density6 == 0)
        {
            calcPos._y += d2;
            calcPos._z += 1;
            div++;
        }

        if (density6 != 0 && density2 == 0)
        {
            calcPos._y += 1 - d6;
            calcPos._z += 1;
            div++;
        }

        if (density6 != 0 && density4 == 0)
        {
            calcPos._y += 1;
            calcPos._z += 1 - d6;
            div++;
        }

        if (density4 != 0 && density6 == 0)
        {
            calcPos._y += 1;
            calcPos._z += d4;
            div++;
        }

        if (div != 0)
        {
            calcPos /= static_cast<float32>(div);
        }
        else
        {
            calcPos.set(0, 0, 0);
        }

#ifdef DEV_CUBIFY
        calcPos.set(0.5, 0.5, 0.5);
#endif

        vertex = calcPos;

        return div != 0 ? true : false;
    }

    void iContouringCubes::setNextLODVoxelOffset(const iaVector3I &offset)
    {
        _nextLODVoxelOffset = offset;
    }

    /*

    //
    //           ^           ^           ^
    //           |           |           |
    //           |           |           |
    //           0-----------1-----------2 --x
    //       ^  /        ^  /        ^  /
    //       | /         | /         | /
    //       |/          |/          |/
    //       3-----------4-----------5
    //   ^  /        ^  /        ^  /
    //   | /         | /         | /
    //   |/          |/          |/
    //   6-----------7-----------8
    //  /
    // z

    //       y
    //       |
    //       18-----19-----20
    //      /|     /|     /|
    //     21-----22-----23|
    //    /| |   /| |   /| |
    //   24-----25-----26| |
    //   | | 9--|-|-10-|-|-11
    //   | |/|  | |/|  | |/|
    //   | 12---|-13---|-14|
    //   |/| |  |/| |  |/| |
    //   15-----16-----17| |
    //   | | 0--|-|-1--|-|-2--x
    //   | |/   | |/   | |/
    //   | 3----|-4----|-5
    //   |/     |/     |/
    //   6------7------8
    //  /
    // z
    //             4
    //             |   2
    //             |  /
    //             | /
    //             |/
    //   3---------*---------1
    //            /|
    //           / |
    //          /  |
    //         0   |
    //             5

    //    Y
    //    |
    //    |
    //    0---X
    //   /
    //  /
    // Z
    */
    void iContouringCubes::calculateNextLOD()
    {
        // the whole thing here is written very inefficient. fix later please

        iaVector3I blockPos = _cubePosition;
        blockPos._y -= 2;
        blockPos._x = blockPos._x >> 1;
        blockPos._y = blockPos._y >> 1;
        blockPos._z = blockPos._z >> 1;
        blockPos += _nextLODVoxelOffset;

        iaVector3I pos;
        int densityIndexCounter = 0;

        for (int y = 0; y < 3; ++y)
        {
            for (int z = 0; z < 3; ++z)
            {
                for (int x = 0; x < 3; ++x)
                {
                    _nextLODDensity[densityIndexCounter++] = _voxelDataNextLOD->getVoxelDensity(iaVector3I(blockPos._x + x, blockPos._y + y, blockPos._z + z));
                }
            }
        }

        for (int i = 0; i < 27; ++i)
        {
            _vertexPositionsNextLODVisible[i] = false;
            _vertexPositionsNextLOD[i] = iaVector3f();
        }

        iaVector3f position;

        if (calculateVertex(_nextLODDensity[0], _nextLODDensity[1], _nextLODDensity[3], _nextLODDensity[4], _nextLODDensity[9], _nextLODDensity[10], _nextLODDensity[12], _nextLODDensity[13], position))
        {
            _vertexPositionsNextLODVisible[0] = true;
            _vertexPositionsNextLOD[0] = position;
        }

        if (calculateVertex(_nextLODDensity[1], _nextLODDensity[2], _nextLODDensity[4], _nextLODDensity[5], _nextLODDensity[10], _nextLODDensity[11], _nextLODDensity[13], _nextLODDensity[14], position))
        {
            position += dirs[1];
            _vertexPositionsNextLODVisible[2] = true;
            _vertexPositionsNextLOD[2] = position;
        }

        if (calculateVertex(_nextLODDensity[3], _nextLODDensity[4], _nextLODDensity[6], _nextLODDensity[7], _nextLODDensity[12], _nextLODDensity[13], _nextLODDensity[15], _nextLODDensity[16], position))
        {
            position += dirs[0];
            _vertexPositionsNextLODVisible[6] = true;
            _vertexPositionsNextLOD[6] = position;
        }

        if (calculateVertex(_nextLODDensity[4], _nextLODDensity[5], _nextLODDensity[7], _nextLODDensity[8], _nextLODDensity[13], _nextLODDensity[14], _nextLODDensity[16], _nextLODDensity[17], position))
        {
            position += dirs[1];
            position += dirs[0];
            _vertexPositionsNextLODVisible[8] = true;
            _vertexPositionsNextLOD[8] = position;
        }

        if (calculateVertex(_nextLODDensity[9], _nextLODDensity[10], _nextLODDensity[12], _nextLODDensity[13], _nextLODDensity[18], _nextLODDensity[19], _nextLODDensity[21], _nextLODDensity[22], position))
        {
            position += dirs[4];
            _vertexPositionsNextLODVisible[18] = true;
            _vertexPositionsNextLOD[18] = position;
        }

        if (calculateVertex(_nextLODDensity[10], _nextLODDensity[11], _nextLODDensity[13], _nextLODDensity[14], _nextLODDensity[19], _nextLODDensity[20], _nextLODDensity[22], _nextLODDensity[23], position))
        {
            position += dirs[1];
            position += dirs[4];
            _vertexPositionsNextLODVisible[20] = true;
            _vertexPositionsNextLOD[20] = position;
        }

        if (calculateVertex(_nextLODDensity[12], _nextLODDensity[13], _nextLODDensity[15], _nextLODDensity[16], _nextLODDensity[21], _nextLODDensity[22], _nextLODDensity[24], _nextLODDensity[25], position))
        {
            position += dirs[4];
            position += dirs[0];
            _vertexPositionsNextLODVisible[24] = true;
            _vertexPositionsNextLOD[24] = position;
        }

        if (calculateVertex(_nextLODDensity[13], _nextLODDensity[14], _nextLODDensity[16], _nextLODDensity[17], _nextLODDensity[22], _nextLODDensity[23], _nextLODDensity[25], _nextLODDensity[26], position))
        {
            position += dirs[0];
            position += dirs[1];
            position += dirs[4];
            _vertexPositionsNextLODVisible[26] = true;
            _vertexPositionsNextLOD[26] = position;
        }

        if (_vertexPositionsNextLODVisible[0] && _vertexPositionsNextLODVisible[2])
        {
            position = _vertexPositionsNextLOD[0];
            position += _vertexPositionsNextLOD[2];
            position *= 0.5;
            _vertexPositionsNextLODVisible[1] = true;
            _vertexPositionsNextLOD[1] = position;
        }

        if (_vertexPositionsNextLODVisible[0] && _vertexPositionsNextLODVisible[6])
        {
            position = _vertexPositionsNextLOD[0];
            position += _vertexPositionsNextLOD[6];
            position *= 0.5;
            _vertexPositionsNextLODVisible[3] = true;
            _vertexPositionsNextLOD[3] = position;
        }

        if (_vertexPositionsNextLODVisible[2] && _vertexPositionsNextLODVisible[8])
        {
            position = _vertexPositionsNextLOD[2];
            position += _vertexPositionsNextLOD[8];
            position *= 0.5;
            _vertexPositionsNextLODVisible[5] = true;
            _vertexPositionsNextLOD[5] = position;
        }

        if (_vertexPositionsNextLODVisible[6] && _vertexPositionsNextLODVisible[8])
        {
            position = _vertexPositionsNextLOD[6];
            position += _vertexPositionsNextLOD[8];
            position *= 0.5;
            _vertexPositionsNextLODVisible[7] = true;
            _vertexPositionsNextLOD[7] = position;
        }

        if (_vertexPositionsNextLODVisible[0] && _vertexPositionsNextLODVisible[18])
        {
            position = _vertexPositionsNextLOD[0];
            position += _vertexPositionsNextLOD[18];
            position *= 0.5;
            _vertexPositionsNextLODVisible[9] = true;
            _vertexPositionsNextLOD[9] = position;
        }

        if (_vertexPositionsNextLODVisible[2] && _vertexPositionsNextLODVisible[20])
        {
            position = _vertexPositionsNextLOD[2];
            position += _vertexPositionsNextLOD[20];
            position *= 0.5;
            _vertexPositionsNextLODVisible[11] = true;
            _vertexPositionsNextLOD[11] = position;
        }

        if (_vertexPositionsNextLODVisible[8] && _vertexPositionsNextLODVisible[26])
        {
            position = _vertexPositionsNextLOD[8];
            position += _vertexPositionsNextLOD[26];
            position *= 0.5;
            _vertexPositionsNextLODVisible[17] = true;
            _vertexPositionsNextLOD[17] = position;
        }

        if (_vertexPositionsNextLODVisible[6] && _vertexPositionsNextLODVisible[24])
        {
            position = _vertexPositionsNextLOD[6];
            position += _vertexPositionsNextLOD[24];
            position *= 0.5;
            _vertexPositionsNextLODVisible[15] = true;
            _vertexPositionsNextLOD[15] = position;
        }

        if (_vertexPositionsNextLODVisible[18] && _vertexPositionsNextLODVisible[20])
        {
            position = _vertexPositionsNextLOD[18];
            position += _vertexPositionsNextLOD[20];
            position *= 0.5;
            _vertexPositionsNextLODVisible[19] = true;
            _vertexPositionsNextLOD[19] = position;
        }

        if (_vertexPositionsNextLODVisible[18] && _vertexPositionsNextLODVisible[24])
        {
            position = _vertexPositionsNextLOD[18];
            position += _vertexPositionsNextLOD[24];
            position *= 0.5;
            _vertexPositionsNextLODVisible[21] = true;
            _vertexPositionsNextLOD[21] = position;
        }

        if (_vertexPositionsNextLODVisible[20] && _vertexPositionsNextLODVisible[26])
        {
            position = _vertexPositionsNextLOD[20];
            position += _vertexPositionsNextLOD[26];
            position *= 0.5;
            _vertexPositionsNextLODVisible[23] = true;
            _vertexPositionsNextLOD[23] = position;
        }

        if (_vertexPositionsNextLODVisible[24] && _vertexPositionsNextLODVisible[26])
        {
            position = _vertexPositionsNextLOD[24];
            position += _vertexPositionsNextLOD[26];
            position *= 0.5;
            _vertexPositionsNextLODVisible[25] = true;
            _vertexPositionsNextLOD[25] = position;
        }

        if (_vertexPositionsNextLODVisible[11] && _vertexPositionsNextLODVisible[17])
        {
            position = _vertexPositionsNextLOD[11];
            position += _vertexPositionsNextLOD[17];
            position *= 0.5;
            _vertexPositionsNextLODVisible[14] = true;
            _vertexPositionsNextLOD[14] = position;
        }

        if (_vertexPositionsNextLODVisible[9] && _vertexPositionsNextLODVisible[15])
        {
            position = _vertexPositionsNextLOD[9];
            position += _vertexPositionsNextLOD[15];
            position *= 0.5;
            _vertexPositionsNextLODVisible[12] = true;
            _vertexPositionsNextLOD[12] = position;
        }

        if (_vertexPositionsNextLODVisible[19] && _vertexPositionsNextLODVisible[25])
        {
            position = _vertexPositionsNextLOD[19];
            position += _vertexPositionsNextLOD[25];
            position *= 0.5;
            _vertexPositionsNextLODVisible[22] = true;
            _vertexPositionsNextLOD[22] = position;
        }

        if (_vertexPositionsNextLODVisible[1] && _vertexPositionsNextLODVisible[7])
        {
            position = _vertexPositionsNextLOD[1];
            position += _vertexPositionsNextLOD[7];
            position *= 0.5;
            _vertexPositionsNextLODVisible[4] = true;
            _vertexPositionsNextLOD[4] = position;
        }

        if (_vertexPositionsNextLODVisible[12] && _vertexPositionsNextLODVisible[14])
        {
            position = _vertexPositionsNextLOD[12];
            position += _vertexPositionsNextLOD[14];
            position *= 0.5;
            _vertexPositionsNextLODVisible[13] = true;
            _vertexPositionsNextLOD[13] = position;
        }

        iaVector3f transformedCubePosition;
        transformedCubePosition._x = static_cast<float32>((_cubePosition._x - _cubeStartPosition._x) >> 1);
        transformedCubePosition._y = static_cast<float32>((_cubePosition._y - 2 - _cubeStartPosition._y) >> 1);
        transformedCubePosition._z = static_cast<float32>((_cubePosition._z - _cubeStartPosition._z) >> 1);

        for (int i = 0; i < 27; ++i)
        {
            if (_vertexPositionsNextLODVisible[i])
            {
                _vertexPositionsNextLOD[i] += transformedCubePosition;
                _vertexPositionsNextLOD[i] *= _scaleNextLOD;
                _vertexPositionsNextLOD[i] += _offsetNextLOD;
            }
        }
    }

    void iContouringCubes::generateGeometry(bool keepTriangles, uint32 neighborLODs)
    {
        iaVector3I geometryPosition(_cubePosition._x, _cubePosition._y - 2, _cubePosition._z);
        geometryPosition -= _cubeStartPosition;
        iaVector3f transformedCubePosition(geometryPosition._x, geometryPosition._y, geometryPosition._z);

        iaVector3f va;
        iaVector3f vb;
        iaVector3f vc;
        iaVector3f vd;

        iaVector3f v0;
        iaVector3f v1;
        iaVector3f v2;
        iaVector3f v3;

        iaVector3f na;
        iaVector3f nb;
        iaVector3f nc;
        iaVector3f nd;

        iaVector3f n0;
        iaVector3f n1;
        iaVector3f n2;
        iaVector3f n3;

        iaVector3f normal;
        iaVector3f normalA, normalB;
        iaVector3f ab;
        iaVector3f ac;
        iaVector3f ad;
        uint32 a, b, c, d;

        uint8 mata;
        uint8 matb;
        uint8 matc;
        uint8 matd;
        uint32 matKey = 0;

        memcpy(_mixedDensity, _density, 27);

#ifdef FILL_LOD_GAPS
        if (neighborLODs != 0 &&
            _voxelDataNextLOD != nullptr)
        {
            calculateNextLOD();

            if ((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0)
            {
                _mixedDensity[2] = _nextLODDensity[14];
                _mixedDensity[5] = _nextLODDensity[14];
                _mixedDensity[8] = _nextLODDensity[14];

                _mixedDensity[11] = _nextLODDensity[14];
                _mixedDensity[14] = _nextLODDensity[14];
                _mixedDensity[17] = _nextLODDensity[14];

                _mixedDensity[20] = _nextLODDensity[14];
                _mixedDensity[23] = _nextLODDensity[14];
                _mixedDensity[26] = _nextLODDensity[14];
            }

            if ((neighborLODs & HIGHER_NEIGHBOR_LOD_XNEGATIVE) != 0)
            {
                //       y
                //       |
                //       19
                //      /|
                //     22|
                //    /| |
                //   25| |
                //   | | 10
                //   | |/|
                //   | 13|
                //   |/| |
                //   16| |
                //   | | 1
                //   | |/
                //   | 4
                //   |/
                //   7
                //  /
                // z

                if ((geometryPosition._z % 2) == 0)
                {
                    if ((geometryPosition._y % 2) == 0)
                    {
                        _mixedDensity[13] = _nextLODDensity[13];
                        _mixedDensity[16] = _nextLODDensity[13];
                        _mixedDensity[22] = _nextLODDensity[13];
                        _mixedDensity[25] = _nextLODDensity[13];

                        _mixedDensity[4] = _nextLODDensity[4];
                        _mixedDensity[7] = _nextLODDensity[4];

                        _mixedDensity[1] = _nextLODDensity[1];

                        _mixedDensity[10] = _nextLODDensity[10];
                        _mixedDensity[19] = _nextLODDensity[10];
                    }
                    else
                    {
                        _mixedDensity[4] = _nextLODDensity[13];
                        _mixedDensity[7] = _nextLODDensity[13];
                        _mixedDensity[13] = _nextLODDensity[13];
                        _mixedDensity[16] = _nextLODDensity[13];

                        _mixedDensity[19] = _nextLODDensity[19];

                        _mixedDensity[10] = _nextLODDensity[10];
                        _mixedDensity[1] = _nextLODDensity[10];

                        _mixedDensity[22] = _nextLODDensity[22];
                        _mixedDensity[25] = _nextLODDensity[22];
                    }
                }
                else
                {
                    if ((geometryPosition._y % 2) == 0)
                    {
                        _mixedDensity[10] = _nextLODDensity[13];
                        _mixedDensity[13] = _nextLODDensity[13];
                        _mixedDensity[19] = _nextLODDensity[13];
                        _mixedDensity[22] = _nextLODDensity[13];

                        _mixedDensity[7] = _nextLODDensity[7];

                        _mixedDensity[1] = _nextLODDensity[4];
                        _mixedDensity[4] = _nextLODDensity[4];

                        _mixedDensity[16] = _nextLODDensity[16];
                        _mixedDensity[25] = _nextLODDensity[16];
                    }
                    else
                    {
                        _mixedDensity[1] = _nextLODDensity[13];
                        _mixedDensity[4] = _nextLODDensity[13];
                        _mixedDensity[10] = _nextLODDensity[13];
                        _mixedDensity[13] = _nextLODDensity[13];

                        _mixedDensity[25] = _nextLODDensity[25];

                        _mixedDensity[19] = _nextLODDensity[22];
                        _mixedDensity[22] = _nextLODDensity[22];

                        _mixedDensity[7] = _nextLODDensity[16];
                        _mixedDensity[16] = _nextLODDensity[16];
                    }
                }
            }

            if ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0)
            {
                _mixedDensity[18] = _nextLODDensity[22];
                _mixedDensity[19] = _nextLODDensity[22];
                _mixedDensity[20] = _nextLODDensity[22];

                _mixedDensity[21] = _nextLODDensity[22];
                _mixedDensity[22] = _nextLODDensity[22];
                _mixedDensity[23] = _nextLODDensity[22];

                _mixedDensity[24] = _nextLODDensity[22];
                _mixedDensity[25] = _nextLODDensity[22];
                _mixedDensity[16] = _nextLODDensity[22];
            }

            if ((neighborLODs & HIGHER_NEIGHBOR_LOD_YNEGATIVE) != 0)
            {
                //       9------10-----11 - x
                //      /      /      /
                //     12-----13-----14
                //    /      /      /
                //   15-----16-----17
                //  /
                // z

                if ((geometryPosition._x % 2) == 0)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        _mixedDensity[13] = _nextLODDensity[13];
                        _mixedDensity[14] = _nextLODDensity[13];
                        _mixedDensity[16] = _nextLODDensity[13];
                        _mixedDensity[17] = _nextLODDensity[13];

                        _mixedDensity[9] = _nextLODDensity[9];

                        _mixedDensity[10] = _nextLODDensity[10];
                        _mixedDensity[11] = _nextLODDensity[10];

                        _mixedDensity[12] = _nextLODDensity[12];
                        _mixedDensity[15] = _nextLODDensity[12];
                    }
                    else
                    {
                        _mixedDensity[10] = _nextLODDensity[13];
                        _mixedDensity[11] = _nextLODDensity[13];
                        _mixedDensity[13] = _nextLODDensity[13];
                        _mixedDensity[14] = _nextLODDensity[13];

                        _mixedDensity[15] = _nextLODDensity[15];

                        _mixedDensity[9] = _nextLODDensity[12];
                        _mixedDensity[12] = _nextLODDensity[12];

                        _mixedDensity[16] = _nextLODDensity[16];
                        _mixedDensity[17] = _nextLODDensity[16];
                    }
                }
                else
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        _mixedDensity[12] = _nextLODDensity[13];
                        _mixedDensity[13] = _nextLODDensity[13];
                        _mixedDensity[15] = _nextLODDensity[13];
                        _mixedDensity[16] = _nextLODDensity[13];

                        _mixedDensity[11] = _nextLODDensity[11];

                        _mixedDensity[9] = _nextLODDensity[10];
                        _mixedDensity[10] = _nextLODDensity[10];

                        _mixedDensity[14] = _nextLODDensity[14];
                        _mixedDensity[17] = _nextLODDensity[14];
                    }
                    else
                    {
                        _mixedDensity[9] = _nextLODDensity[13];
                        _mixedDensity[10] = _nextLODDensity[13];
                        _mixedDensity[12] = _nextLODDensity[13];
                        _mixedDensity[13] = _nextLODDensity[13];

                        _mixedDensity[17] = _nextLODDensity[17];

                        _mixedDensity[11] = _nextLODDensity[14];
                        _mixedDensity[14] = _nextLODDensity[14];

                        _mixedDensity[15] = _nextLODDensity[16];
                        _mixedDensity[16] = _nextLODDensity[16];
                    }
                }
            }

            if ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0)
            {
                _mixedDensity[6] = _nextLODDensity[16];
                _mixedDensity[7] = _nextLODDensity[16];
                _mixedDensity[8] = _nextLODDensity[16];

                _mixedDensity[15] = _nextLODDensity[16];
                _mixedDensity[16] = _nextLODDensity[16];
                _mixedDensity[17] = _nextLODDensity[16];

                _mixedDensity[24] = _nextLODDensity[16];
                _mixedDensity[25] = _nextLODDensity[16];
                _mixedDensity[26] = _nextLODDensity[16];
            }

            if ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZNEGATIVE) != 0)
            {
                //   y
                //   |
                //   21-----22-----23
                //   |      |      |
                //   |      |      |
                //   |      |      |
                //   |      |      |
                //   12-----13-----14
                //   |      |      |
                //   |      |      |
                //   |      |      |
                //   |      |      |
                //   3------4------5 - x

                if ((geometryPosition._x % 2) == 0)
                {
                    if ((geometryPosition._y % 2) == 0)
                    {
                        _mixedDensity[13] = _nextLODDensity[13];
                        _mixedDensity[14] = _nextLODDensity[13];
                        _mixedDensity[22] = _nextLODDensity[13];
                        _mixedDensity[23] = _nextLODDensity[13];

                        _mixedDensity[4] = _nextLODDensity[4];
                        _mixedDensity[5] = _nextLODDensity[4];

                        _mixedDensity[3] = _nextLODDensity[3];

                        _mixedDensity[12] = _nextLODDensity[12];
                        _mixedDensity[21] = _nextLODDensity[12];
                    }
                    else
                    {
                        _mixedDensity[4] = _nextLODDensity[13];
                        _mixedDensity[5] = _nextLODDensity[13];
                        _mixedDensity[13] = _nextLODDensity[13];
                        _mixedDensity[14] = _nextLODDensity[13];

                        _mixedDensity[21] = _nextLODDensity[21];

                        _mixedDensity[12] = _nextLODDensity[12];
                        _mixedDensity[3] = _nextLODDensity[12];

                        _mixedDensity[22] = _nextLODDensity[22];
                        _mixedDensity[23] = _nextLODDensity[22];
                    }
                }
                else
                {
                    if ((geometryPosition._y % 2) == 0)
                    {
                        _mixedDensity[21] = _nextLODDensity[13];
                        _mixedDensity[22] = _nextLODDensity[13];
                        _mixedDensity[12] = _nextLODDensity[13];
                        _mixedDensity[13] = _nextLODDensity[13];

                        _mixedDensity[23] = _nextLODDensity[14];
                        _mixedDensity[14] = _nextLODDensity[14];

                        _mixedDensity[5] = _nextLODDensity[5];

                        _mixedDensity[3] = _nextLODDensity[4];
                        _mixedDensity[4] = _nextLODDensity[4];
                    }
                    else
                    {
                        _mixedDensity[12] = _nextLODDensity[13];
                        _mixedDensity[13] = _nextLODDensity[13];
                        _mixedDensity[3] = _nextLODDensity[13];
                        _mixedDensity[4] = _nextLODDensity[13];

                        _mixedDensity[5] = _nextLODDensity[14];
                        _mixedDensity[14] = _nextLODDensity[14];

                        _mixedDensity[23] = _nextLODDensity[23];

                        _mixedDensity[21] = _nextLODDensity[22];
                        _mixedDensity[22] = _nextLODDensity[22];
                    }
                }
            }

            /*for (int i = 0; i < 27; ++i)
            {
                if (_vertexPositionsNextLODVisible[i])
                {
                    iaVector3f pos = _vertexPositionsNextLOD[i];
                    uint32 a = _meshBuilder.addVertex(pos + iaVector3f(0.1, 0, 0));
                    uint32 b = _meshBuilder.addVertex(pos + iaVector3f(0, 0.1, 0));
                    uint32 c = _meshBuilder.addVertex(pos + iaVector3f(-0.1, 0, 0));
                    uint32 d = _meshBuilder.addVertex(pos + iaVector3f(0, -0.1, 0));

                    _meshBuilder.accumulateNormal(a, iaVector3f(0, 1, 0));
                    _meshBuilder.accumulateNormal(b, iaVector3f(0, 1, 0));
                    _meshBuilder.accumulateNormal(c, iaVector3f(0, 1, 0));
                    _meshBuilder.accumulateNormal(d, iaVector3f(0, 1, 0));

                    addCheckedTriangle(a, b, c, true);
                    addCheckedTriangle(c, d, a, true);
                }
            }*/
        }
#endif

        bool lookupResult = false;

        if (_mixedDensity[13] > 0.0f)
        {
            if (_mixedDensity[14] <= 0.0f)
            {
                lookupResult = calculateVertex(_density[1], _density[2], _density[4], _density[5], _density[10], _density[11], _density[13], _density[14], va);
                va += transformedCubePosition;
                va += dirs[1];
                va *= _scale;
                va += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZNEGATIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[1])
                                {
                                    va = _vertexPositionsNextLOD[1];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[10])
                                {
                                    va = _vertexPositionsNextLOD[10];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[2])
                                {
                                    va = _vertexPositionsNextLOD[2];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[11])
                                {
                                    va = _vertexPositionsNextLOD[11];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[4])
                                {
                                    va = _vertexPositionsNextLOD[4];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    va = _vertexPositionsNextLOD[13];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[5])
                                {
                                    va = _vertexPositionsNextLOD[5];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[14])
                                {
                                    va = _vertexPositionsNextLOD[14];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[4], _density[5], _density[7], _density[8], _density[13], _density[14], _density[16], _density[17], vb);
                vb += transformedCubePosition;
                vb += dirs[1];
                vb += dirs[0];
                vb *= _scale;
                vb += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[4])
                                {
                                    vb = _vertexPositionsNextLOD[4];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vb = _vertexPositionsNextLOD[13];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[5])
                                {
                                    vb = _vertexPositionsNextLOD[5];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[14])
                                {
                                    vb = _vertexPositionsNextLOD[14];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[7])
                                {
                                    vb = _vertexPositionsNextLOD[7];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[16])
                                {
                                    vb = _vertexPositionsNextLOD[16];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[8])
                                {
                                    vb = _vertexPositionsNextLOD[8];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[17])
                                {
                                    vb = _vertexPositionsNextLOD[17];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26], vc);
                vc += transformedCubePosition;
                vc += dirs[1];
                vc += dirs[0];
                vc += dirs[4];
                vc *= _scale;
                vc += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vc = _vertexPositionsNextLOD[13];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[22])
                                {
                                    vc = _vertexPositionsNextLOD[22];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[14])
                                {
                                    vc = _vertexPositionsNextLOD[14];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[23])
                                {
                                    vc = _vertexPositionsNextLOD[23];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[16])
                                {
                                    vc = _vertexPositionsNextLOD[16];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[25])
                                {
                                    vc = _vertexPositionsNextLOD[25];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[17])
                                {
                                    vc = _vertexPositionsNextLOD[17];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[26])
                                {
                                    vc = _vertexPositionsNextLOD[26];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[10], _density[11], _density[13], _density[14], _density[19], _density[20], _density[22], _density[23], vd);
                vd += transformedCubePosition;
                vd += dirs[1];
                vd += dirs[4];
                vd *= _scale;
                vd += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[10])
                                {
                                    vd = _vertexPositionsNextLOD[10];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[19])
                                {
                                    vd = _vertexPositionsNextLOD[19];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[11])
                                {
                                    vd = _vertexPositionsNextLOD[11];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[20])
                                {
                                    vd = _vertexPositionsNextLOD[20];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vd = _vertexPositionsNextLOD[13];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[22])
                                {
                                    vd = _vertexPositionsNextLOD[22];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[14])
                                {
                                    vd = _vertexPositionsNextLOD[14];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[23])
                                {
                                    vd = _vertexPositionsNextLOD[23];
                                }
                            }
                        }
                    }
                }
#endif

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                d = _meshBuilder.addVertex(vd);

                ab = vb;
                ab -= va;
                ac = vc;
                ac -= va;
                ad = vd;
                ad -= va;
                normalA = ac % ab;
                normalB = ad % ab;

                _meshBuilder.accumulateNormal(a, normalA);
                _meshBuilder.accumulateNormal(b, normalA);
                _meshBuilder.accumulateNormal(c, normalA);

                _meshBuilder.accumulateNormal(a, normalB);
                _meshBuilder.accumulateNormal(c, normalB);
                _meshBuilder.accumulateNormal(d, normalB);

                addCheckedTriangle(c, b, a, keepTriangles);
                addCheckedTriangle(d, c, a, keepTriangles);
            }

            if (_mixedDensity[16] <= 0.0f)
            {
                lookupResult = calculateVertex(_density[4], _density[5], _density[7], _density[8], _density[13], _density[14], _density[16], _density[17], va);
                va += transformedCubePosition;
                va += dirs[1];
                va += dirs[0];
                va *= _scale;
                va += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[4])
                                {
                                    va = _vertexPositionsNextLOD[4];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    va = _vertexPositionsNextLOD[13];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[5])
                                {
                                    va = _vertexPositionsNextLOD[5];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[14])
                                {
                                    va = _vertexPositionsNextLOD[14];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[7])
                                {
                                    va = _vertexPositionsNextLOD[7];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[16])
                                {
                                    va = _vertexPositionsNextLOD[16];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[8])
                                {
                                    va = _vertexPositionsNextLOD[8];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[17])
                                {
                                    va = _vertexPositionsNextLOD[17];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[3], _density[4], _density[6], _density[7], _density[12], _density[13], _density[15], _density[16], vb);
                vb += transformedCubePosition;
                vb += dirs[0];
                vb *= _scale;
                vb += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[3])
                                {
                                    vb = _vertexPositionsNextLOD[3];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[12])
                                {
                                    vb = _vertexPositionsNextLOD[12];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[4])
                                {
                                    vb = _vertexPositionsNextLOD[4];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vb = _vertexPositionsNextLOD[13];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[6])
                                {
                                    vb = _vertexPositionsNextLOD[6];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[15])
                                {
                                    vb = _vertexPositionsNextLOD[15];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[7])
                                {
                                    vb = _vertexPositionsNextLOD[7];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[16])
                                {
                                    vb = _vertexPositionsNextLOD[16];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[12], _density[13], _density[15], _density[16], _density[21], _density[22], _density[24], _density[25], vc);
                vc += transformedCubePosition;
                vc += dirs[4];
                vc += dirs[0];
                vc *= _scale;
                vc += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[12])
                                {
                                    vc = _vertexPositionsNextLOD[12];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[21])
                                {
                                    vc = _vertexPositionsNextLOD[21];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vc = _vertexPositionsNextLOD[13];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[22])
                                {
                                    vc = _vertexPositionsNextLOD[22];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[15])
                                {
                                    vc = _vertexPositionsNextLOD[15];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[24])
                                {
                                    vc = _vertexPositionsNextLOD[24];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[16])
                                {
                                    vc = _vertexPositionsNextLOD[16];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[25])
                                {
                                    vc = _vertexPositionsNextLOD[25];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26], vd);
                vd += transformedCubePosition;
                vd += dirs[0];
                vd += dirs[1];
                vd += dirs[4];
                vd *= _scale;
                vd += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vd = _vertexPositionsNextLOD[13];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[22])
                                {
                                    vd = _vertexPositionsNextLOD[22];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[14])
                                {
                                    vd = _vertexPositionsNextLOD[14];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[23])
                                {
                                    vd = _vertexPositionsNextLOD[23];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[16])
                                {
                                    vd = _vertexPositionsNextLOD[16];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[25])
                                {
                                    vd = _vertexPositionsNextLOD[25];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[17])
                                {
                                    vd = _vertexPositionsNextLOD[17];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[26])
                                {
                                    vd = _vertexPositionsNextLOD[26];
                                }
                            }
                        }
                    }
                }
#endif

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                d = _meshBuilder.addVertex(vd);

                ab = vb;
                ab -= va;
                ac = vc;
                ac -= va;
                ad = vd;
                ad -= va;
                normalA = ac % ab;
                normalB = ad % ab;

                _meshBuilder.accumulateNormal(a, normalA);
                _meshBuilder.accumulateNormal(b, normalA);
                _meshBuilder.accumulateNormal(c, normalA);

                _meshBuilder.accumulateNormal(d, normalB);
                _meshBuilder.accumulateNormal(c, normalB);
                _meshBuilder.accumulateNormal(a, normalB);

                addCheckedTriangle(c, b, a, keepTriangles);
                addCheckedTriangle(d, c, a, keepTriangles);
            }

            if (_mixedDensity[22] <= 0.0f)
            {
                lookupResult = calculateVertex(_density[10], _density[11], _density[13], _density[14], _density[19], _density[20], _density[22], _density[23], va);
                va += transformedCubePosition;
                va += dirs[1];
                va += dirs[4];
                va *= _scale;
                va += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[10])
                                {
                                    va = _vertexPositionsNextLOD[10];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[19])
                                {
                                    va = _vertexPositionsNextLOD[19];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[11])
                                {
                                    va = _vertexPositionsNextLOD[11];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[20])
                                {
                                    va = _vertexPositionsNextLOD[20];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    va = _vertexPositionsNextLOD[13];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[22])
                                {
                                    va = _vertexPositionsNextLOD[22];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[14])
                                {
                                    va = _vertexPositionsNextLOD[14];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[23])
                                {
                                    va = _vertexPositionsNextLOD[23];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26], vb);
                vb += dirs[0];
                vb += dirs[1];
                vb += dirs[4];
                vb += transformedCubePosition;
                vb *= _scale;
                vb += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vb = _vertexPositionsNextLOD[13];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[22])
                                {
                                    vb = _vertexPositionsNextLOD[22];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[14])
                                {
                                    vb = _vertexPositionsNextLOD[14];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[23])
                                {
                                    vb = _vertexPositionsNextLOD[23];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[16])
                                {
                                    vb = _vertexPositionsNextLOD[16];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[25])
                                {
                                    vb = _vertexPositionsNextLOD[25];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[17])
                                {
                                    vb = _vertexPositionsNextLOD[17];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[26])
                                {
                                    vb = _vertexPositionsNextLOD[26];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[12], _density[13], _density[15], _density[16], _density[21], _density[22], _density[24], _density[25], vc);
                vc += dirs[4];
                vc += dirs[0];
                vc += transformedCubePosition;
                vc *= _scale;
                vc += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[12])
                                {
                                    vc = _vertexPositionsNextLOD[12];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[21])
                                {
                                    vc = _vertexPositionsNextLOD[21];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vc = _vertexPositionsNextLOD[13];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[22])
                                {
                                    vc = _vertexPositionsNextLOD[22];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[15])
                                {
                                    vc = _vertexPositionsNextLOD[15];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[24])
                                {
                                    vc = _vertexPositionsNextLOD[24];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[16])
                                {
                                    vc = _vertexPositionsNextLOD[16];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[25])
                                {
                                    vc = _vertexPositionsNextLOD[25];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[9], _density[10], _density[12], _density[13], _density[18], _density[19], _density[21], _density[22], vd);
                vd += dirs[4];
                vd += transformedCubePosition;
                vd *= _scale;
                vd += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[9])
                                {
                                    vd = _vertexPositionsNextLOD[9];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[18])
                                {
                                    vd = _vertexPositionsNextLOD[18];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[10])
                                {
                                    vd = _vertexPositionsNextLOD[10];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[19])
                                {
                                    vd = _vertexPositionsNextLOD[19];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[12])
                                {
                                    vd = _vertexPositionsNextLOD[12];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[21])
                                {
                                    vd = _vertexPositionsNextLOD[21];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vd = _vertexPositionsNextLOD[13];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[22])
                                {
                                    vd = _vertexPositionsNextLOD[22];
                                }
                            }
                        }
                    }
                }
#endif

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                d = _meshBuilder.addVertex(vd);

                ab = vb;
                ab -= va;
                ac = vc;
                ac -= va;
                ad = vd;
                ad -= va;
                normalA = ac % ab;
                normalB = ad % ab;

                _meshBuilder.accumulateNormal(a, normalA);
                _meshBuilder.accumulateNormal(b, normalA);
                _meshBuilder.accumulateNormal(c, normalA);

                _meshBuilder.accumulateNormal(d, normalB);
                _meshBuilder.accumulateNormal(c, normalB);
                _meshBuilder.accumulateNormal(a, normalB);

                addCheckedTriangle(c, b, a, keepTriangles);
                addCheckedTriangle(d, c, a, keepTriangles);
            }
        }
        else
        {
            if (_mixedDensity[14] > 0.0f)
            {
                lookupResult = calculateVertex(_density[1], _density[2], _density[4], _density[5], _density[10], _density[11], _density[13], _density[14], va);
                va += dirs[1];
                va += transformedCubePosition;
                va *= _scale;
                va += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZNEGATIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[1])
                                {
                                    va = _vertexPositionsNextLOD[1];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[10])
                                {
                                    va = _vertexPositionsNextLOD[10];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[2])
                                {
                                    va = _vertexPositionsNextLOD[2];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[11])
                                {
                                    va = _vertexPositionsNextLOD[11];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[4])
                                {
                                    va = _vertexPositionsNextLOD[4];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    va = _vertexPositionsNextLOD[13];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[5])
                                {
                                    va = _vertexPositionsNextLOD[5];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[14])
                                {
                                    va = _vertexPositionsNextLOD[14];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[4], _density[5], _density[7], _density[8], _density[13], _density[14], _density[16], _density[17], vb);
                vb += transformedCubePosition;
                vb += dirs[0];
                vb += dirs[1];
                vb *= _scale;
                vb += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[4])
                                {
                                    vb = _vertexPositionsNextLOD[4];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vb = _vertexPositionsNextLOD[13];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[5])
                                {
                                    vb = _vertexPositionsNextLOD[5];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[14])
                                {
                                    vb = _vertexPositionsNextLOD[14];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[7])
                                {
                                    vb = _vertexPositionsNextLOD[7];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[16])
                                {
                                    va = _vertexPositionsNextLOD[16];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[8])
                                {
                                    vb = _vertexPositionsNextLOD[8];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[17])
                                {
                                    vb = _vertexPositionsNextLOD[17];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26], vc);
                vc += transformedCubePosition;
                vc += dirs[0];
                vc += dirs[1];
                vc += dirs[4];
                vc *= _scale;
                vc += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vc = _vertexPositionsNextLOD[13];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[22])
                                {
                                    vc = _vertexPositionsNextLOD[22];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[14])
                                {
                                    vc = _vertexPositionsNextLOD[14];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[23])
                                {
                                    vc = _vertexPositionsNextLOD[23];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[16])
                                {
                                    vc = _vertexPositionsNextLOD[16];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[25])
                                {
                                    vc = _vertexPositionsNextLOD[25];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[17])
                                {
                                    vc = _vertexPositionsNextLOD[17];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[26])
                                {
                                    vc = _vertexPositionsNextLOD[26];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[10], _density[11], _density[13], _density[14], _density[19], _density[20], _density[22], _density[23], vd);
                vd += transformedCubePosition;
                vd += dirs[1];
                vd += dirs[4];
                vd *= _scale;
                vd += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[10])
                                {
                                    vd = _vertexPositionsNextLOD[10];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[19])
                                {
                                    vd = _vertexPositionsNextLOD[19];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[11])
                                {
                                    vd = _vertexPositionsNextLOD[11];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[20])
                                {
                                    vd = _vertexPositionsNextLOD[20];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vd = _vertexPositionsNextLOD[13];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[22])
                                {
                                    vd = _vertexPositionsNextLOD[22];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[14])
                                {
                                    vd = _vertexPositionsNextLOD[14];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[23])
                                {
                                    vd = _vertexPositionsNextLOD[23];
                                }
                            }
                        }
                    }
                }
#endif

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                d = _meshBuilder.addVertex(vd);

                ab = vb;
                ab -= va;
                ac = vc;
                ac -= va;
                ad = vd;
                ad -= va;
                normalA = ab % ac;
                normalB = ab % ad;

                _meshBuilder.accumulateNormal(a, normalA);
                _meshBuilder.accumulateNormal(b, normalA);
                _meshBuilder.accumulateNormal(c, normalA);

                _meshBuilder.accumulateNormal(d, normalB);
                _meshBuilder.accumulateNormal(c, normalB);
                _meshBuilder.accumulateNormal(a, normalB);

                addCheckedTriangle(a, b, c, keepTriangles);
                addCheckedTriangle(a, c, d, keepTriangles);
            }

            if (_mixedDensity[16] > 0.0f)
            {
                lookupResult = calculateVertex(_density[4], _density[5], _density[7], _density[8], _density[13], _density[14], _density[16], _density[17], va);
                va += transformedCubePosition;
                va += dirs[1];
                va += dirs[0];
                va *= _scale;
                va += _offset;

                //       y
                //       |
                //       18-----19-----20
                //      /|     /|     /|
                //     21-----22-----23|
                //    /| |   /| |   /| |
                //   24-----25-----26| |
                //   | | 9--|-|-10-|-|-11
                //   | |/|  | |/|  | |/|
                //   | 12---|-13---|-14|
                //   |/| |  |/| |  |/| |
                //   15-----16-----17| |
                //   | | 0--|-|-1--|-|-2--x
                //   | |/   | |/   | |/
                //   | 3----|-4----|-5
                //   |/     |/     |/
                //   6------7------8
                //  /
                // z

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[4])
                                {
                                    va = _vertexPositionsNextLOD[4];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    va = _vertexPositionsNextLOD[13];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[5])
                                {
                                    va = _vertexPositionsNextLOD[5];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[14])
                                {
                                    va = _vertexPositionsNextLOD[14];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[7])
                                {
                                    va = _vertexPositionsNextLOD[7];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[16])
                                {
                                    va = _vertexPositionsNextLOD[16];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[8])
                                {
                                    va = _vertexPositionsNextLOD[8];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[17])
                                {
                                    va = _vertexPositionsNextLOD[17];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[3], _density[4], _density[6], _density[7], _density[12], _density[13], _density[15], _density[16], vb);
                vb += transformedCubePosition;
                vb += dirs[0];
                vb *= _scale;
                vb += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[3])
                                {
                                    vb = _vertexPositionsNextLOD[3];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[12])
                                {
                                    vb = _vertexPositionsNextLOD[12];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[4])
                                {
                                    vb = _vertexPositionsNextLOD[4];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vb = _vertexPositionsNextLOD[13];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[6])
                                {
                                    vb = _vertexPositionsNextLOD[6];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[15])
                                {
                                    vb = _vertexPositionsNextLOD[15];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[7])
                                {
                                    vb = _vertexPositionsNextLOD[7];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[16])
                                {
                                    vb = _vertexPositionsNextLOD[16];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[12], _density[13], _density[15], _density[16], _density[21], _density[22], _density[24], _density[25], vc);
                vc += transformedCubePosition;
                vc += dirs[4];
                vc += dirs[0];
                vc *= _scale;
                vc += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[12])
                                {
                                    vc = _vertexPositionsNextLOD[12];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[21])
                                {
                                    vc = _vertexPositionsNextLOD[21];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vc = _vertexPositionsNextLOD[13];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[22])
                                {
                                    vc = _vertexPositionsNextLOD[22];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[15])
                                {
                                    vc = _vertexPositionsNextLOD[15];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[24])
                                {
                                    vc = _vertexPositionsNextLOD[24];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[16])
                                {
                                    vc = _vertexPositionsNextLOD[16];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[25])
                                {
                                    vc = _vertexPositionsNextLOD[25];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26], vd);
                vd += transformedCubePosition;
                vd += dirs[0];
                vd += dirs[1];
                vd += dirs[4];
                vd *= _scale;
                vd += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vd = _vertexPositionsNextLOD[13];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[22])
                                {
                                    vd = _vertexPositionsNextLOD[22];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[14])
                                {
                                    vd = _vertexPositionsNextLOD[14];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[23])
                                {
                                    vd = _vertexPositionsNextLOD[23];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[16])
                                {
                                    vd = _vertexPositionsNextLOD[16];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[25])
                                {
                                    vd = _vertexPositionsNextLOD[25];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[17])
                                {
                                    vd = _vertexPositionsNextLOD[17];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[26])
                                {
                                    vd = _vertexPositionsNextLOD[26];
                                }
                            }
                        }
                    }
                }
#endif

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                d = _meshBuilder.addVertex(vd);

                ab = vb;
                ab -= va;
                ac = vc;
                ac -= va;
                ad = vd;
                ad -= va;
                normalA = ab % ac;
                normalB = ab % ad;

                _meshBuilder.accumulateNormal(a, normalA);
                _meshBuilder.accumulateNormal(b, normalA);
                _meshBuilder.accumulateNormal(c, normalA);

                _meshBuilder.accumulateNormal(d, normalB);
                _meshBuilder.accumulateNormal(c, normalB);
                _meshBuilder.accumulateNormal(a, normalB);

                addCheckedTriangle(a, b, c, keepTriangles);
                addCheckedTriangle(a, c, d, keepTriangles);
            }

            if (_mixedDensity[22] > 0.0f)
            {
                lookupResult = calculateVertex(_density[10], _density[11], _density[13], _density[14], _density[19], _density[20], _density[22], _density[23], va);
                va += transformedCubePosition;
                va += dirs[1];
                va += dirs[4];
                va *= _scale;
                va += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[10])
                                {
                                    va = _vertexPositionsNextLOD[10];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[19])
                                {
                                    va = _vertexPositionsNextLOD[19];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[11])
                                {
                                    va = _vertexPositionsNextLOD[11];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[20])
                                {
                                    va = _vertexPositionsNextLOD[20];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    va = _vertexPositionsNextLOD[13];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[22])
                                {
                                    va = _vertexPositionsNextLOD[22];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[14])
                                {
                                    va = _vertexPositionsNextLOD[14];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[23])
                                {
                                    va = _vertexPositionsNextLOD[23];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26], vb);
                vb += transformedCubePosition;
                vb += dirs[0];
                vb += dirs[1];
                vb += dirs[4];
                vb *= _scale;
                vb += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vb = _vertexPositionsNextLOD[13];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[22])
                                {
                                    vb = _vertexPositionsNextLOD[22];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[14])
                                {
                                    vb = _vertexPositionsNextLOD[14];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[23])
                                {
                                    vb = _vertexPositionsNextLOD[23];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[16])
                                {
                                    vb = _vertexPositionsNextLOD[16];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[25])
                                {
                                    vb = _vertexPositionsNextLOD[25];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[17])
                                {
                                    vb = _vertexPositionsNextLOD[17];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[26])
                                {
                                    vb = _vertexPositionsNextLOD[26];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[12], _density[13], _density[15], _density[16], _density[21], _density[22], _density[24], _density[25], vc);
                vc += transformedCubePosition;
                vc += dirs[4];
                vc += dirs[0];
                vc *= _scale;
                vc += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[12])
                                {
                                    vc = _vertexPositionsNextLOD[12];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[21])
                                {
                                    vc = _vertexPositionsNextLOD[21];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vc = _vertexPositionsNextLOD[13];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[22])
                                {
                                    vc = _vertexPositionsNextLOD[22];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[15])
                                {
                                    vc = _vertexPositionsNextLOD[15];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[24])
                                {
                                    vc = _vertexPositionsNextLOD[24];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[16])
                                {
                                    vc = _vertexPositionsNextLOD[16];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[25])
                                {
                                    vc = _vertexPositionsNextLOD[25];
                                }
                            }
                        }
                    }
                }
#endif

                lookupResult = calculateVertex(_density[9], _density[10], _density[12], _density[13], _density[18], _density[19], _density[21], _density[22], vd);
                vd += transformedCubePosition;
                vd += dirs[4];
                vd *= _scale;
                vd += _offset;

#ifdef FILL_LOD_GAPS
                if (((neighborLODs & HIGHER_NEIGHBOR_LOD_XNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_ZNEGATIVE) != 0) ||
                    ((neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE) != 0) ||
                    !lookupResult)
                {
                    if ((geometryPosition._z % 2) == 0)
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[9])
                                {
                                    vd = _vertexPositionsNextLOD[9];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[18])
                                {
                                    vd = _vertexPositionsNextLOD[18];
                                }
                            }
                        }
                        else
                        {
                            if (geometryPosition._y % 2 == 0)
                            {
                                if (_vertexPositionsNextLODVisible[10])
                                {
                                    vd = _vertexPositionsNextLOD[10];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[19])
                                {
                                    vd = _vertexPositionsNextLOD[19];
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((geometryPosition._x % 2) == 0)
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[12])
                                {
                                    vd = _vertexPositionsNextLOD[12];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[21])
                                {
                                    vd = _vertexPositionsNextLOD[21];
                                }
                            }
                        }
                        else
                        {
                            if ((geometryPosition._y % 2) == 0)
                            {
                                if (_vertexPositionsNextLODVisible[13])
                                {
                                    vd = _vertexPositionsNextLOD[13];
                                }
                            }
                            else
                            {
                                if (_vertexPositionsNextLODVisible[22])
                                {
                                    vd = _vertexPositionsNextLOD[22];
                                }
                            }
                        }
                    }
                }
#endif

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                d = _meshBuilder.addVertex(vd);

                ab = vb;
                ab -= va;
                ac = vc;
                ac -= va;
                ad = vd;
                ad -= va;
                normalA = ab % ac;
                normalB = ab % ad;

                _meshBuilder.accumulateNormal(a, normalA);
                _meshBuilder.accumulateNormal(b, normalA);
                _meshBuilder.accumulateNormal(c, normalA);

                _meshBuilder.accumulateNormal(d, normalB);
                _meshBuilder.accumulateNormal(c, normalB);
                _meshBuilder.accumulateNormal(a, normalB);

                addCheckedTriangle(a, b, c, keepTriangles);
                addCheckedTriangle(a, c, d, keepTriangles);
            }
        }
    }

    void iContouringCubes::climb()
    {
        _cubePosition._y++;

        for (int i = 0; i < 9; ++i)
        {
            _density[i + 0] = _density[i + 9];
            _density[i + 9] = _density[i + 18];
            _density[i + 18] = _currentPoles[i]._currentDensityPole->getValue(_cubePosition._y);
        }
    }

    void iContouringCubes::startClimb(iaVector3I &startPosition)
    {
        _cubePosition = startPosition;

        _currentPoles[0]._currentDensityPole = &_voxelData->getDensityPole(_cubePosition._x, _cubePosition._z);
        _currentPoles[1]._currentDensityPole = &_voxelData->getDensityPole(_cubePosition._x + 1, _cubePosition._z);
        _currentPoles[2]._currentDensityPole = &_voxelData->getDensityPole(_cubePosition._x + 2, _cubePosition._z);

        _currentPoles[3]._currentDensityPole = &_voxelData->getDensityPole(_cubePosition._x, _cubePosition._z + 1);
        _currentPoles[4]._currentDensityPole = &_voxelData->getDensityPole(_cubePosition._x + 1, _cubePosition._z + 1);
        _currentPoles[5]._currentDensityPole = &_voxelData->getDensityPole(_cubePosition._x + 2, _cubePosition._z + 1);

        _currentPoles[6]._currentDensityPole = &_voxelData->getDensityPole(_cubePosition._x, _cubePosition._z + 2);
        _currentPoles[7]._currentDensityPole = &_voxelData->getDensityPole(_cubePosition._x + 1, _cubePosition._z + 2);
        _currentPoles[8]._currentDensityPole = &_voxelData->getDensityPole(_cubePosition._x + 2, _cubePosition._z + 2);

        for (int i = 0; i < 27; ++i)
        {
            _density[i] = 0;
        }

        for (int i = 0; i < 9; ++i)
        {
            _density[i + 18] = _currentPoles[i]._currentDensityPole->getValue(_cubePosition._y);
        }
    }

    void iContouringCubes::setVoxelData(iVoxelData *voxelData)
    {
        _voxelData = voxelData;
    }

    void iContouringCubes::setVoxelDataNextLOD(iVoxelData *voxelData)
    {
        _voxelDataNextLOD = voxelData;
    }

    float64 iContouringCubes::calcLODScale(uint32 lod)
    {
        return pow(2, lod);
    }

    iaVector3f iContouringCubes::calcLODOffset(uint32 lod)
    {
        iaVector3f result(0, 0, 0);

        if (lod > 0)
        {
            float32 value = pow(2, lod - 1) - 0.5;
            result.set(-value, -value, -value);
        }

        return result;
    }

    iMeshPtr iContouringCubes::compile(iaVector3I pos, iaVector3I volume, uint32 lod, uint32 neighborLODs)
    {
        iMeshPtr result;

        _lod = lod;
        _scale = calcLODScale(_lod);
        _scaleNextLOD = calcLODScale(_lod + 1);

        _offset = calcLODOffset(_lod);
        _offsetNextLOD = calcLODOffset(_lod + 1);

        if (neighborLODs != 0)
        {
            con_assert(_voxelDataNextLOD != nullptr, "no data available");
        }

        con_assert(_voxelData != nullptr, "no voxel data defined");
        if (_voxelData == nullptr)
        {
            return result;
        }

        if (pos._x < 0 ||
            pos._y < 0 ||
            pos._z < 0)
        {
            con_err("out of range");
            return result;
        }

        iaVector3I max = pos;
        max += volume;
        if (max._x > _voxelData->getWidth() ||
            max._y > _voxelData->getHeight() ||
            max._z > _voxelData->getDepth())
        {
            con_err("out of range");
            return result;
        }

        _meshBuilder.clear();

        iaVector3I marchingVolume;

        marchingVolume._x = volume._x - 2;
        marchingVolume._y = volume._y - 2;
        marchingVolume._z = volume._z - 2;

        _cubeStartPosition.set(pos._x, pos._y, pos._z);

        iaVector3I currentPosition;
        currentPosition._y = _cubeStartPosition._y;

        bool keepTriangles = false;
        uint32 nextTriangleIndex = 0;

        _trianglesToKeep.clear();

        uint32 LODs = 0;

        // run through columns
        for (int x = 0; x < marchingVolume._x; ++x)
        {
            if (x == 0)
            {
                LODs |= (neighborLODs & HIGHER_NEIGHBOR_LOD_XNEGATIVE);
            }
            else
            {
                LODs &= ~HIGHER_NEIGHBOR_LOD_XNEGATIVE;
            }

            if (x == marchingVolume._x - 1)
            {
                LODs |= (neighborLODs & HIGHER_NEIGHBOR_LOD_XPOSITIVE);
            }
            else
            {
                LODs &= ~HIGHER_NEIGHBOR_LOD_XPOSITIVE;
            }

            currentPosition._x = _cubeStartPosition._x + x;

            // run through rows
            for (int z = 0; z < marchingVolume._z; ++z)
            {
                if (z == 0)
                {
                    LODs |= (neighborLODs & HIGHER_NEIGHBOR_LOD_ZNEGATIVE);
                }
                else
                {
                    LODs &= ~HIGHER_NEIGHBOR_LOD_ZNEGATIVE;
                }

                if (z == marchingVolume._z - 1)
                {
                    LODs |= (neighborLODs & HIGHER_NEIGHBOR_LOD_ZPOSITIVE);
                }
                else
                {
                    LODs &= ~HIGHER_NEIGHBOR_LOD_ZPOSITIVE;
                }

                currentPosition._z = _cubeStartPosition._z + z;

                int y = 0;
                // process pole
                startClimb(currentPosition);
                climb();

                do
                {
                    climb();

                    if (y == 0)
                    {
                        LODs |= (neighborLODs & HIGHER_NEIGHBOR_LOD_YNEGATIVE);
                    }
                    else
                    {
                        LODs &= ~HIGHER_NEIGHBOR_LOD_YNEGATIVE;
                    }

                    if (y == marchingVolume._y - 1)
                    {
                        LODs |= (neighborLODs & HIGHER_NEIGHBOR_LOD_YPOSITIVE);
                    }
                    else
                    {
                        LODs &= ~HIGHER_NEIGHBOR_LOD_YPOSITIVE;
                    }

                    generateGeometry(true, LODs);

                    y++;
                } while (y < marchingVolume._y);
            }
        }

        if (_meshBuilder.getTrianglesCount() != 0)
        {
            _meshBuilder.normalizeNormals();

            for (auto iter : _trianglesToKeep)
            {
                result = _meshBuilder.createMesh(iter.second);
                break;
            }
        }

        return result;
    }
} // namespace igor

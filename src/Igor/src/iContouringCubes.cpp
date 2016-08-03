// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include "iContouringCubes.h"

#include <iTimer.h>

#include <iaMatrix3x3.h>
using namespace IgorAux;

#define NEIGHBOR_XPOSITIVE 0x20
#define NEIGHBOR_XNEGATIVE 0x10
#define NEIGHBOR_YPOSITIVE 0x08
#define NEIGHBOR_YNEGATIVE 0x04
#define NEIGHBOR_ZPOSITIVE 0x02
#define NEIGHBOR_ZNEGATIVE 0x01

namespace Igor
{

    /*
                 4
                 |   2
                 |  /
                 | /
                 |/
       3---------*---------1
                /|
               / |
              /  |
             0   |
                 5

        Y
        |
        |
        0---X
       /
      /
     Z

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

        _meshBuilder.setJoinVertexes(true);
    }

    /*! helper macro to map density values from 0&1-255 to lenght 0.0-1.0

    0 -> density is zero therefore it is outside the object
    1 -> is also integpreted als density zero (1-255 -> 0.0-1.0)
    */
#define rescale(value) ((value > 0 ? value - 1 : value) / 254.0)


    /*

    // orientation in this function
    // Y  z
    // | /
    // |/
    // 0---X

    // densities
    //     6------7
    //    /|     /|
    //   4------5 |
    //   | |    | |
    //   | |    | |
    //   | 2----|-3
    //   |/     |/
    //   0------1

    // deltas
    //     *---3--*
    //   10|    11|
    //   *--2---* |
    //   | 6    | 7
    //   4 |    5 |
    //   | *--1-|-*
    //   |8     |9
    //   *--0---*


    */
#define CALC_NORMALS

    void iContouringCubes::calculateVertex(uint8 density0, uint8 density1, uint8 density2, uint8 density3, uint8 density4, uint8 density5, uint8 density6, uint8 density7, iaVector3f& vertex)
    {
        int div = 0;
        iaVector3f calcPos;

        float32 d0 = rescale(static_cast<float32>(density0));
        float32 d1 = rescale(static_cast<float32>(density1));
        float32 d2 = rescale(static_cast<float32>(density2));
        float32 d3 = rescale(static_cast<float32>(density3));
        float32 d4 = rescale(static_cast<float32>(density4));
        float32 d5 = rescale(static_cast<float32>(density5));
        float32 d6 = rescale(static_cast<float32>(density6));
        float32 d7 = rescale(static_cast<float32>(density7));

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

        vertex = calcPos;

        //vertex.set(0.5,0.5,0.5);
    }

    void iContouringCubes::setNextLODVoxelOffset(const iaVector3I& offset)
    {
        _nextLODVoxelOffset = offset;
    }


    /*

    //
    //           ^           ^           ^
    //           |           |           |
    //           |           |           |
    //           g(6)--------h(7)--------i(8)
    //       ^  /        ^  /        ^  /
    //       | /         | /         | /
    //       |/          |/          |/
    //       d(3)--------e(4)--------f(5)
    //   ^  /        ^  /        ^  /
    //   | /     0   | /     1   | /
    //   |/          |/          |/
    //   a(0)--------b(1)--------c(2)
    //

    d --- c
    |    /|
    |   / |
    |  /  |
    | /   |
    |/    |
    a-----b

    d --- c
    |\   /|
    | \ / |
    |  e  |
    | / \ |
    |/   \|
    a-----b

    //       24-----25-----26
    //      /|     /|     /|
    //     21-----22-----23|
    //    /| |   /| |   /| |
    //   18-----19-----20| |
    //   | | 15-|-|-16-|-|-17
    //   | |/|  | |/|  | |/|
    //   | 12---|-13---|-14|      y
    //   |/| |  |/| |  |/| |      |
    //   9------10-----11| |      |
    //   | | 6--|-|-7--|-|-8      |
    //   | |/   | |/   | |/       0---X
    //   | 3----|-4----|-5       /
    //   |/     |/     |/       /
    //   0------1------2       z

    */
    void iContouringCubes::calculateNextLOD()
    {
        uint8 nextLODDensity[3 * 3 * 3];

        iaVector3I blockPos = _cubePosition;
        blockPos._x = blockPos._x >> 1;
        blockPos._y = blockPos._y >> 1;
        blockPos._z = blockPos._z >> 1;
        // blockPos += _nextLODVoxelOffset;

        iaVector3I pos;
        int i = 0;

        for (int x = 0; x < 3; ++x)
        {
            for (int z = 0; z < 3; ++z)
            {
                for (int y = 0; y < 3; ++y)
                {
                    nextLODDensity[i++] = _voxelDataNextLOD->getVoxelDensity(iaVector3I(blockPos._x + x, blockPos._y + y, blockPos._z + z));
                }
            }
        }

        calculateVertex(nextLODDensity[0], nextLODDensity[1], nextLODDensity[3], nextLODDensity[4], nextLODDensity[9], nextLODDensity[10], nextLODDensity[12], nextLODDensity[13], _vertexPositionsNextLOD[0]);

        calculateVertex(nextLODDensity[1], nextLODDensity[2], nextLODDensity[4], nextLODDensity[5], nextLODDensity[10], nextLODDensity[11], nextLODDensity[13], nextLODDensity[14], _vertexPositionsNextLOD[2]);
        _vertexPositionsNextLOD[2] += dirs[5];

        calculateVertex(nextLODDensity[3], nextLODDensity[4], nextLODDensity[6], nextLODDensity[7], nextLODDensity[12], nextLODDensity[13], nextLODDensity[15], nextLODDensity[16], _vertexPositionsNextLOD[6]);
        _vertexPositionsNextLOD[6] += dirs[5];
        _vertexPositionsNextLOD[6] += dirs[3];
        _vertexPositionsNextLOD[6] += dirs[0];

        calculateVertex(nextLODDensity[4], nextLODDensity[5], nextLODDensity[7], nextLODDensity[8], nextLODDensity[13], nextLODDensity[14], nextLODDensity[16], nextLODDensity[17], _vertexPositionsNextLOD[8]);
        _vertexPositionsNextLOD[8] += dirs[5];
        _vertexPositionsNextLOD[8] += dirs[0];

        calculateVertex(nextLODDensity[9], nextLODDensity[10], nextLODDensity[12], nextLODDensity[13], nextLODDensity[18], nextLODDensity[19], nextLODDensity[21], nextLODDensity[22], _vertexPositionsNextLOD[18]);
        _vertexPositionsNextLOD[18] += dirs[3];

        calculateVertex(nextLODDensity[10], nextLODDensity[11], nextLODDensity[13], nextLODDensity[14], nextLODDensity[19], nextLODDensity[20], nextLODDensity[22], nextLODDensity[23], _vertexPositionsNextLOD[20]);

        calculateVertex(nextLODDensity[12], nextLODDensity[13], nextLODDensity[15], nextLODDensity[16], nextLODDensity[21], nextLODDensity[22], nextLODDensity[24], nextLODDensity[25], _vertexPositionsNextLOD[24]);
        _vertexPositionsNextLOD[24] += dirs[3];
        _vertexPositionsNextLOD[24] += dirs[0];

        calculateVertex(nextLODDensity[13], nextLODDensity[14], nextLODDensity[16], nextLODDensity[17], nextLODDensity[22], nextLODDensity[23], nextLODDensity[25], nextLODDensity[26], _vertexPositionsNextLOD[26]);
        _vertexPositionsNextLOD[26] += dirs[0];

        _vertexPositionsNextLOD[1] = _vertexPositionsNextLOD[0];
        _vertexPositionsNextLOD[1] += _vertexPositionsNextLOD[2];
        _vertexPositionsNextLOD[1] *= 0.5;

        _vertexPositionsNextLOD[5] = _vertexPositionsNextLOD[2];
        _vertexPositionsNextLOD[5] += _vertexPositionsNextLOD[8];
        _vertexPositionsNextLOD[5] *= 0.5;

        _vertexPositionsNextLOD[7] = _vertexPositionsNextLOD[6];
        _vertexPositionsNextLOD[7] += _vertexPositionsNextLOD[8];
        _vertexPositionsNextLOD[7] *= 0.5;

        _vertexPositionsNextLOD[3] = _vertexPositionsNextLOD[0];
        _vertexPositionsNextLOD[3] += _vertexPositionsNextLOD[6];
        _vertexPositionsNextLOD[3] *= 0.5;

        _vertexPositionsNextLOD[4] = _vertexPositionsNextLOD[3];
        _vertexPositionsNextLOD[4] += _vertexPositionsNextLOD[5];
        _vertexPositionsNextLOD[4] *= 0.5;

        _vertexPositionsNextLOD[9] = _vertexPositionsNextLOD[0];
        _vertexPositionsNextLOD[9] += _vertexPositionsNextLOD[18];
        _vertexPositionsNextLOD[9] *= 0.5;

        _vertexPositionsNextLOD[11] = _vertexPositionsNextLOD[2];
        _vertexPositionsNextLOD[11] += _vertexPositionsNextLOD[20];
        _vertexPositionsNextLOD[11] *= 0.5;

        _vertexPositionsNextLOD[17] = _vertexPositionsNextLOD[8];
        _vertexPositionsNextLOD[17] += _vertexPositionsNextLOD[26];
        _vertexPositionsNextLOD[17] *= 0.5;

        _vertexPositionsNextLOD[15] = _vertexPositionsNextLOD[6];
        _vertexPositionsNextLOD[15] += _vertexPositionsNextLOD[24];
        _vertexPositionsNextLOD[15] *= 0.5;

        _vertexPositionsNextLOD[10] = _vertexPositionsNextLOD[9];
        _vertexPositionsNextLOD[10] += _vertexPositionsNextLOD[11];
        _vertexPositionsNextLOD[10] *= 0.5;

        _vertexPositionsNextLOD[14] = _vertexPositionsNextLOD[11];
        _vertexPositionsNextLOD[14] += _vertexPositionsNextLOD[17];
        _vertexPositionsNextLOD[14] *= 0.5;

        _vertexPositionsNextLOD[16] = _vertexPositionsNextLOD[15];
        _vertexPositionsNextLOD[16] += _vertexPositionsNextLOD[17];
        _vertexPositionsNextLOD[16] *= 0.5;

        _vertexPositionsNextLOD[12] = _vertexPositionsNextLOD[9];
        _vertexPositionsNextLOD[12] += _vertexPositionsNextLOD[15];
        _vertexPositionsNextLOD[12] *= 0.5;

        _vertexPositionsNextLOD[13] = _vertexPositionsNextLOD[12];
        _vertexPositionsNextLOD[13] += _vertexPositionsNextLOD[14];
        _vertexPositionsNextLOD[13] *= 0.5;

        _vertexPositionsNextLOD[19] = _vertexPositionsNextLOD[18];
        _vertexPositionsNextLOD[19] += _vertexPositionsNextLOD[20];
        _vertexPositionsNextLOD[19] *= 0.5;

        _vertexPositionsNextLOD[23] = _vertexPositionsNextLOD[20];
        _vertexPositionsNextLOD[23] += _vertexPositionsNextLOD[26];
        _vertexPositionsNextLOD[23] *= 0.5;

        _vertexPositionsNextLOD[25] = _vertexPositionsNextLOD[24];
        _vertexPositionsNextLOD[25] += _vertexPositionsNextLOD[26];
        _vertexPositionsNextLOD[25] *= 0.5;

        _vertexPositionsNextLOD[21] = _vertexPositionsNextLOD[24];
        _vertexPositionsNextLOD[21] += _vertexPositionsNextLOD[18];
        _vertexPositionsNextLOD[21] *= 0.5;

        _vertexPositionsNextLOD[22] = _vertexPositionsNextLOD[21];
        _vertexPositionsNextLOD[22] += _vertexPositionsNextLOD[23];
        _vertexPositionsNextLOD[22] *= 0.5;

        for (int i = 0; i < (3 * 3 * 3); ++i)
        {
            _vertexPositionsNextLOD[i] *= _scaleNextLOD;
            _vertexPositionsNextLOD[i] += _offset;
            _vertexPositionsNextLOD[i] -= _offsetNextLOD;
        }
    }

    void iContouringCubes::generateGeometry(const uint8* density, bool keepTriangles, uint32 neighborLODs)
    {
        iaVector3f transformedCubePosition(_cubePosition._x, _cubePosition._y, _cubePosition._z);
        transformedCubePosition._x -= _cubeStartPosition._x - 1; // TODO workaround I don't understand
        transformedCubePosition._y -= _cubeStartPosition._y + 2; // TODO workaround I don't understand
        transformedCubePosition._z -= _cubeStartPosition._z;

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

        if (neighborLODs != 0)
        {
            calculateNextLOD();
        }

        if (_density[13] > 0.0f)
        {
            if (_density[14] <= 0.0f)
            {
                calculateVertex(density[1], density[2], density[4], density[5], density[10], density[11], density[13], density[14], va);
                va += transformedCubePosition;
                va += dirs[5];

                calculateVertex(density[4], density[5], density[7], density[8], density[13], density[14], density[16], density[17], vb);
                vb += transformedCubePosition;
                vb += dirs[5];
                vb += dirs[0];

                calculateVertex(density[13], density[14], density[16], density[17], density[22], density[23], density[25], density[26], vc);
                vc += transformedCubePosition;
                vc += dirs[0];

                calculateVertex(density[10], density[11], density[13], density[14], density[19], density[20], density[22], density[23], vd);
                vd += transformedCubePosition;

                va *= _scale;
                vb *= _scale;
                vc *= _scale;
                vd *= _scale;

                va += _offset;
                vb += _offset;
                vc += _offset;
                vd += _offset;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                d = _meshBuilder.addVertex(vd);

#ifdef CALC_NORMALS
                ab = vb; ab -= va;
                ac = vc; ac -= va;
                ad = vd; ad -= va;
                normalA = ac % ab;
                normalB = ad % ab;

                _meshBuilder.accumulateNormal(a, normalA);
                _meshBuilder.accumulateNormal(b, normalA);
                _meshBuilder.accumulateNormal(c, normalA);

                _meshBuilder.accumulateNormal(a, normalB);
                _meshBuilder.accumulateNormal(c, normalB);
                _meshBuilder.accumulateNormal(d, normalB);
#else
                _meshBuilder.setNormal(a);
                _meshBuilder.setNormal(b);
                _meshBuilder.setNormal(c);
                _meshBuilder.setNormal(d);
#endif

                _meshBuilder.addTriangle(c, b, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                _meshBuilder.addTriangle(d, c, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
                }
            }

            if (density[16] <= 0.0f)
            {
                calculateVertex(density[4], density[5], density[7], density[8], density[13], density[14], density[16], density[17], va);
                va += transformedCubePosition;
                va += dirs[5];
                va += dirs[0];

                calculateVertex(density[3], density[4], density[6], density[7], density[12], density[13], density[15], density[16], vb);
                vb += transformedCubePosition;
                vb += dirs[5];
                vb += dirs[3];
                vb += dirs[0];

                calculateVertex(density[12], density[13], density[15], density[16], density[21], density[22], density[24], density[25], vc);
                vc += transformedCubePosition;
                vc += dirs[3];
                vc += dirs[0];

                calculateVertex(density[13], density[14], density[16], density[17], density[22], density[23], density[25], density[26], vd);
                vd += transformedCubePosition;
                vd += dirs[0];

                va *= _scale;
                vb *= _scale;
                vc *= _scale;
                vd *= _scale;

                va += _offset;
                vb += _offset;
                vc += _offset;
                vd += _offset;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                d = _meshBuilder.addVertex(vd);

#ifdef CALC_NORMALS
                ab = vb; ab -= va;
                ac = vc; ac -= va;
                ad = vd; ad -= va;
                normalA = ac % ab;
                normalB = ad % ab;

                _meshBuilder.accumulateNormal(a, normalA);
                _meshBuilder.accumulateNormal(b, normalA);
                _meshBuilder.accumulateNormal(c, normalA);

                _meshBuilder.accumulateNormal(d, normalB);
                _meshBuilder.accumulateNormal(c, normalB);
                _meshBuilder.accumulateNormal(a, normalB);
#else
                _meshBuilder.setNormal(a);
                _meshBuilder.setNormal(b);
                _meshBuilder.setNormal(c);
                _meshBuilder.setNormal(d);
#endif

                _meshBuilder.addTriangle(c, b, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                _meshBuilder.addTriangle(d, c, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
                }
            }

            if (density[22] <= 0.0f)
            {
                /*   if ((neighborLODs & NEIGHBOR_XPOSITIVE) != 0)
                   {
                       va = _vertexPositionsNextLOD[5];
                       va += _vertexPositionsNextLOD[7];
                       va *= 0.5;
                   }
                   else*/
                {
                    calculateVertex(density[10], density[11], density[13], density[14], density[19], density[20], density[22], density[23], va);
                }

                calculateVertex(density[13], density[14], density[16], density[17], density[22], density[23], density[25], density[26], vb);
                vb += dirs[0];

                calculateVertex(density[12], density[13], density[15], density[16], density[21], density[22], density[24], density[25], vc);
                vc += dirs[3];
                vc += dirs[0];

                if ((neighborLODs & NEIGHBOR_XNEGATIVE) != 0)
                {
                /*    if ((_cubePosition._z % 2) == 0)
                    {
                        if ((_cubePosition._x % 2) == 0)
                        {
                            if ((_cubePosition._y % 2) == 0)
                            {
                                vc = _vertexPositionsNextLOD[18];
                            }
                            else
                            {
                                vc = _vertexPositionsNextLOD[9];
                                vc._y -= 1;
                            }
                        }
                        else
                        {
                            if (_cubePosition._y % 2 == 0)
                            {
                                vc = _vertexPositionsNextLOD[10];
                            }
                            else
                            {
                                vc = _vertexPositionsNextLOD[19];
                                vc._y -= 1;
                            }

                            vc._x -= 1;
                        }
                    }
                    else
                    {
                        if ((_cubePosition._x % 2) == 0)
                        {
                            if ((_cubePosition._y % 2) == 0)
                            {
                                vc = _vertexPositionsNextLOD[12];
                            }
                            else
                            {
                                vc = _vertexPositionsNextLOD[21];
                                vc._y -= 1;
                            }
                        }
                        else
                        {
                            if ((_cubePosition._y % 2) == 0)
                            {
                                vc = _vertexPositionsNextLOD[13];
                            }
                            else
                            {
                                vc = _vertexPositionsNextLOD[22];
                                vc._y -= 1;
                            }

                            vc._x -= 1;
                        }

                        vc._z -= 1;
                    }*/

                    if ((_cubePosition._z % 2) == 0)
                    {
                        if ((_cubePosition._x % 2) == 0)
                        {
                            if ((_cubePosition._y % 2) == 0)
                            {
                                vc = _vertexPositionsNextLOD[9];
                            }
                        }
                    }
                }
                else
                {
      
                }

                /* if ((neighborLODs & NEIGHBOR_XNEGATIVE) != 0)
                 {
                     if (_cubePosition._z % 2 == 0)
                     {
                         vd = _vertexPositionsNextLOD[4];
                         vd += _vertexPositionsNextLOD[6];
                         vd *= 0.5;
                     }
                     else
                     {
                         vd = _vertexPositionsNextLOD[6];
                     }
                 }
                 else*/
                {
                    calculateVertex(density[9], density[10], density[12], density[13], density[18], density[19], density[21], density[22], vd);
                    vd += dirs[3];
                }

                va += transformedCubePosition;
                va *= _scale;

                vb += transformedCubePosition;
                vb *= _scale;

                vc += transformedCubePosition;
                vc *= _scale;

                vd += transformedCubePosition;
                vd *= _scale;

                va += _offset;
                vb += _offset;
                vc += _offset;
                vd += _offset;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                d = _meshBuilder.addVertex(vd);

#ifdef CALC_NORMALS
                ab = vb; ab -= va;
                ac = vc; ac -= va;
                ad = vd; ad -= va;
                normalA = ac % ab;
                normalB = ad % ab;

                _meshBuilder.accumulateNormal(a, normalA);
                _meshBuilder.accumulateNormal(b, normalA);
                _meshBuilder.accumulateNormal(c, normalA);

                _meshBuilder.accumulateNormal(d, normalB);
                _meshBuilder.accumulateNormal(c, normalB);
                _meshBuilder.accumulateNormal(a, normalB);
#else
                _meshBuilder.setNormal(a);
                _meshBuilder.setNormal(b);
                _meshBuilder.setNormal(c);
                _meshBuilder.setNormal(d);
#endif

                _meshBuilder.addTriangle(c, b, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                _meshBuilder.addTriangle(d, c, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
                }
            }
        }
        else
        {

            if (density[14] > 0.0f)
            {
                calculateVertex(density[1], density[2], density[4], density[5], density[10], density[11], density[13], density[14], va);
                va += transformedCubePosition;
                va += dirs[5];

                calculateVertex(density[4], density[5], density[7], density[8], density[13], density[14], density[16], density[17], vb);
                vb += transformedCubePosition;
                vb += dirs[0];
                vb += dirs[5];

                calculateVertex(density[13], density[14], density[16], density[17], density[22], density[23], density[25], density[26], vc);
                vc += transformedCubePosition;
                vc += dirs[0];

                calculateVertex(density[10], density[11], density[13], density[14], density[19], density[20], density[22], density[23], vd);
                vd += transformedCubePosition;

                va *= _scale;
                vb *= _scale;
                vc *= _scale;
                vd *= _scale;

                va += _offset;
                vb += _offset;
                vc += _offset;
                vd += _offset;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                d = _meshBuilder.addVertex(vd);

#ifdef CALC_NORMALS
                ab = vb; ab -= va;
                ac = vc; ac -= va;
                ad = vd; ad -= va;
                normalA = ab % ac;
                normalB = ab % ad;

                _meshBuilder.accumulateNormal(a, normalA);
                _meshBuilder.accumulateNormal(b, normalA);
                _meshBuilder.accumulateNormal(c, normalA);

                _meshBuilder.accumulateNormal(d, normalB);
                _meshBuilder.accumulateNormal(c, normalB);
                _meshBuilder.accumulateNormal(a, normalB);
#else
                _meshBuilder.setNormal(a);
                _meshBuilder.setNormal(b);
                _meshBuilder.setNormal(c);
                _meshBuilder.setNormal(d);
#endif

                _meshBuilder.addTriangle(a, b, c);
                if (keepTriangles)
                {
                    _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                _meshBuilder.addTriangle(a, c, d);
                if (keepTriangles)
                {
                    _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
                }
            }

            if (density[16] > 0.0f)
            {
                calculateVertex(density[4], density[5], density[7], density[8], density[13], density[14], density[16], density[17], va);
                va += transformedCubePosition;
                va += dirs[5];
                va += dirs[0];

                calculateVertex(density[3], density[4], density[6], density[7], density[12], density[13], density[15], density[16], vb);
                vb += transformedCubePosition;
                vb += dirs[5];
                vb += dirs[3];
                vb += dirs[0];

                calculateVertex(density[12], density[13], density[15], density[16], density[21], density[22], density[24], density[25], vc);
                vc += transformedCubePosition;
                vc += dirs[3];
                vc += dirs[0];

                calculateVertex(density[13], density[14], density[16], density[17], density[22], density[23], density[25], density[26], vd);
                vd += transformedCubePosition;
                vd += dirs[0];

                va *= _scale;
                vb *= _scale;
                vc *= _scale;
                vd *= _scale;

                va += _offset;
                vb += _offset;
                vc += _offset;
                vd += _offset;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                d = _meshBuilder.addVertex(vd);

#ifdef CALC_NORMALS
                ab = vb; ab -= va;
                ac = vc; ac -= va;
                ad = vd; ad -= va;
                normalA = ab % ac;
                normalB = ab % ad;

                _meshBuilder.accumulateNormal(a, normalA);
                _meshBuilder.accumulateNormal(b, normalA);
                _meshBuilder.accumulateNormal(c, normalA);

                _meshBuilder.accumulateNormal(d, normalB);
                _meshBuilder.accumulateNormal(c, normalB);
                _meshBuilder.accumulateNormal(a, normalB);
#else
                _meshBuilder.setNormal(a);
                _meshBuilder.setNormal(b);
                _meshBuilder.setNormal(c);
                _meshBuilder.setNormal(d);
#endif

                _meshBuilder.addTriangle(a, b, c);
                if (keepTriangles)
                {
                    _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                _meshBuilder.addTriangle(a, c, d);
                if (keepTriangles)
                {
                    _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
                }
            }

            if (density[22] > 0.0f)
            {
                calculateVertex(density[10], density[11], density[13], density[14], density[19], density[20], density[22], density[23], va);
                va += transformedCubePosition;

                calculateVertex(density[13], density[14], density[16], density[17], density[22], density[23], density[25], density[26], vb);
                vb += transformedCubePosition;
                vb += dirs[0];

                calculateVertex(density[12], density[13], density[15], density[16], density[21], density[22], density[24], density[25], vc);
                vc += transformedCubePosition;
                vc += dirs[3];
                vc += dirs[0];

                calculateVertex(density[9], density[10], density[12], density[13], density[18], density[19], density[21], density[22], vd);
                vd += transformedCubePosition;
                vd += dirs[3];

                va *= _scale;
                vb *= _scale;
                vc *= _scale;
                vd *= _scale;

                va += _offset;
                vb += _offset;
                vc += _offset;
                vd += _offset;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                d = _meshBuilder.addVertex(vd);

#ifdef CALC_NORMALS
                ab = vb; ab -= va;
                ac = vc; ac -= va;
                ad = vd; ad -= va;
                normalA = ab % ac;
                normalB = ab % ad;

                _meshBuilder.accumulateNormal(a, normalA);
                _meshBuilder.accumulateNormal(b, normalA);
                _meshBuilder.accumulateNormal(c, normalA);

                _meshBuilder.accumulateNormal(d, normalB);
                _meshBuilder.accumulateNormal(c, normalB);
                _meshBuilder.accumulateNormal(a, normalB);
#else
                _meshBuilder.setNormal(a);
                _meshBuilder.setNormal(b);
                _meshBuilder.setNormal(c);
                _meshBuilder.setNormal(d);
#endif

                _meshBuilder.addTriangle(a, b, c);
                if (keepTriangles)
                {
                    _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                _meshBuilder.addTriangle(a, c, d);
                if (keepTriangles)
                {
                    _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
                }
            }
        }
    }



    /*!

    //           ^           ^           ^
    //           |           |           |
    //           |           |           |
    //           g(6)--------h(7)--------i(8)
    //       ^  /        ^  /        ^  /
    //       | /         | /         | /   Y  Z
    //       |/          |/          |/    | /
    //       d(3)--------e(4)--------f(5)  |/
    //   ^  /        ^  /        ^  /      0---X
    //   | /         | /         | /
    //   |/          |/          |/
    //   a(0)--------b(1)--------c(2)

    */

    void iContouringCubes::climb()
    {
        for (int i = 0; i < 9; ++i)
        {
            _density[i + 0] = _density[i + 9];
            _density[i + 9] = _density[i + 18];
            _density[i + 18] = static_cast<float32>(_currentPoles[i]._currentDensityPole->getValue(_cubePosition._y));
        }

        _cubePosition._y++;
    }

    /*
    //           ^           ^           ^
    //           |           |           |
    //           |           |           |
    //           g(6)--------h(7)--------i(8)
    //       ^  /        ^  /        ^  /
    //       | /         | /         | /   Y  Z
    //       |/          |/          |/    | /
    //       d(3)--------e(4)--------f(5)  |/
    //   ^  /        ^  /        ^  /      0---X
    //   | /         | /         | /
    //   |/          |/          |/
    //   a(0)--------b(1)--------c(2)

    */

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
            _density[i + 18] = static_cast<float32>(_currentPoles[i]._currentDensityPole->getValue(_cubePosition._y));
        }

        _cubePosition._y++;
    }

    void iContouringCubes::setVoxelData(iVoxelData* voxelData)
    {
        _voxelData = voxelData;
    }

    void iContouringCubes::setVoxelDataNextLOD(iVoxelData* voxelData)
    {
        _voxelDataNextLOD = voxelData;
    }

    float64 iContouringCubes::calcLODScale(uint32 lod) const
    {
        return pow(2, lod);
    }

    iaVector3f iContouringCubes::calcLODOffset(uint32 lod) const
    {
        iaVector3f result(0,0,0);
        if (lod > 0)
        {
            float32 value = pow(2, lod - 1) - 0.5;
            result.set(-value, value, -value);
        }

        return result;
    }

    shared_ptr<iMesh> iContouringCubes::compile(iaVector3I pos, iaVector3I volume, uint32 lod, uint32 neighborLODs)
    {
        shared_ptr<iMesh> result;

        _lod = lod;
        _scale = calcLODScale(_lod);
        _scaleNextLOD = calcLODScale(_lod + 1);

        _offset = calcLODOffset(_lod);
        _offsetNextLOD = calcLODOffset(_lod + 1);

        if (neighborLODs != 0)
        {
            con_assert_sticky(_voxelDataNextLOD != nullptr, "no data available");
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

        marchingVolume._x = volume._x - 1;
        marchingVolume._y = volume._y;
        marchingVolume._z = volume._z - 1;

        _cubeStartPosition.set(pos._x, pos._y, pos._z);

        iaVector3I currentPosition;
        currentPosition._y = _cubeStartPosition._y;

        bool keepTriangles = false;
        uint32 nextTriangleIndex = 0;

        _trianglesToKeep.clear();

        uint32 LODs = 0;

        // run through collumns
        for (int x = 0; x < marchingVolume._x; ++x)
        {
            if (x == 0)
            {
                LODs |= (neighborLODs & NEIGHBOR_XNEGATIVE);
            }
            else
            {
                LODs &= ~NEIGHBOR_XNEGATIVE;
            }

            if (x == marchingVolume._x - 1)
            {
                LODs |= (neighborLODs & NEIGHBOR_XPOSITIVE);
            }
            else
            {
                LODs &= ~NEIGHBOR_XPOSITIVE;
            }

            currentPosition._x = _cubeStartPosition._x + x;

            // run through rows
            for (int z = 0; z < marchingVolume._z; ++z)
            {
                if (z == 0)
                {
                    LODs |= (neighborLODs & NEIGHBOR_ZNEGATIVE);
                }
                else
                {
                    LODs &= ~NEIGHBOR_ZNEGATIVE;
                }

                if (z == marchingVolume._z - 1)
                {
                    LODs |= (neighborLODs & NEIGHBOR_ZPOSITIVE);
                }
                else
                {
                    LODs &= ~NEIGHBOR_ZPOSITIVE;
                }

                currentPosition._z = _cubeStartPosition._z + z;

                int y = 0;
                // process pole
                startClimb(currentPosition);
                climb();

                do
                {
                    climb();

                    /*  if (x > 0 &&
                          x < marchingVolume._x - 1 &&
                          y > 0 &&
                          y < marchingVolume._y - 1 &&
                          z > 0 &&
                          z < marchingVolume._z - 1)
                      {
                          keepTriangles = true;
                      }
                      else
                      {
                          keepTriangles = false;
                      }*/



                    if (y == 0)
                    {
                        LODs |= (neighborLODs & NEIGHBOR_YNEGATIVE);
                    }
                    else
                    {
                        LODs &= ~NEIGHBOR_YNEGATIVE;
                    }

                    if (y == marchingVolume._y - 1)
                    {
                        LODs |= (neighborLODs & NEIGHBOR_YPOSITIVE);
                    }
                    else
                    {
                        LODs &= ~NEIGHBOR_YPOSITIVE;
                    }

                    generateGeometry(_density, true, LODs);

                    y++;
                } while (!(_cubePosition._y > marchingVolume._y + _cubeStartPosition._y));
            }
        }

        /*if ((neighborLODs & NEIGHBOR_XPOSITIVE) != 0)
        {
            uint32 a = _meshBuilder.addVertex(iaVector3f(32 * scale - 2, 0, 0));
            uint32 b = _meshBuilder.addVertex(iaVector3f(32 * scale - 2, 18 * scale, 0));
            uint32 c = _meshBuilder.addVertex(iaVector3f(32 * scale - 2, 18 * scale, 18 * scale));
            uint32 d = _meshBuilder.addVertex(iaVector3f(32 * scale - 2, 0, 18 * scale));

            iaVector3f normal(1,0,0);

            _meshBuilder.accumulateNormal(a, normal);
            _meshBuilder.accumulateNormal(b, normal);
            _meshBuilder.accumulateNormal(c, normal);
            _meshBuilder.accumulateNormal(d, normal);
            _meshBuilder.setTexCoord(a, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(b, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(c, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(d, iaVector2f(0.0, 0.0f), 0);

            _meshBuilder.addTriangle(a, b, c);
            _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
            _meshBuilder.addTriangle(a, c, d);
            _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
        }

        if ((neighborLODs & NEIGHBOR_XNEGATIVE) != 0)
        {
            uint32 a = _meshBuilder.addVertex(iaVector3f(2, 0, 0));
            uint32 b = _meshBuilder.addVertex(iaVector3f(2, 16 * scale, 0));
            uint32 c = _meshBuilder.addVertex(iaVector3f(2, 16 * scale, 16 * scale));
            uint32 d = _meshBuilder.addVertex(iaVector3f(2, 0, 16 * scale));

            iaVector3f normal(-1, 0, 0);

            _meshBuilder.accumulateNormal(a, normal);
            _meshBuilder.accumulateNormal(b, normal);
            _meshBuilder.accumulateNormal(c, normal);
            _meshBuilder.accumulateNormal(d, normal);
            _meshBuilder.setTexCoord(a, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(b, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(c, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(d, iaVector2f(0.0, 0.0f), 0);

            _meshBuilder.addTriangle(a, b, c);
            _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
            _meshBuilder.addTriangle(a, c, d);
            _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
        }

        if ((neighborLODs & NEIGHBOR_YPOSITIVE) != 0)
        {
            uint32 a = _meshBuilder.addVertex(iaVector3f(0, 32 * scale - 2, 0));
            uint32 b = _meshBuilder.addVertex(iaVector3f(18 * scale, 32 * scale - 2, 0));
            uint32 c = _meshBuilder.addVertex(iaVector3f(18 * scale, 32 * scale - 2, 18 * scale));
            uint32 d = _meshBuilder.addVertex(iaVector3f(0, 32 * scale - 2, 18 * scale));

            iaVector3f normal(0, 1, 0);

            _meshBuilder.accumulateNormal(a, normal);
            _meshBuilder.accumulateNormal(b, normal);
            _meshBuilder.accumulateNormal(c, normal);
            _meshBuilder.accumulateNormal(d, normal);
            _meshBuilder.setTexCoord(a, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(b, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(c, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(d, iaVector2f(0.0, 0.0f), 0);

            _meshBuilder.addTriangle(a, b, c);
            _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
            _meshBuilder.addTriangle(a, c, d);
            _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
        }

        if ((neighborLODs & NEIGHBOR_YNEGATIVE) != 0)
        {
            uint32 a = _meshBuilder.addVertex(iaVector3f(0, 2, 0));
            uint32 b = _meshBuilder.addVertex(iaVector3f(16 * scale, 2, 0));
            uint32 c = _meshBuilder.addVertex(iaVector3f(16 * scale, 2, 16 * scale));
            uint32 d = _meshBuilder.addVertex(iaVector3f(0, 2, 16 * scale));

            iaVector3f normal(0, -1, 0);

            _meshBuilder.accumulateNormal(a, normal);
            _meshBuilder.accumulateNormal(b, normal);
            _meshBuilder.accumulateNormal(c, normal);
            _meshBuilder.accumulateNormal(d, normal);
            _meshBuilder.setTexCoord(a, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(b, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(c, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(d, iaVector2f(0.0, 0.0f), 0);

            _meshBuilder.addTriangle(a, b, c);
            _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
            _meshBuilder.addTriangle(a, c, d);
            _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
        }

        if ((neighborLODs & NEIGHBOR_ZPOSITIVE) != 0)
        {
            uint32 a = _meshBuilder.addVertex(iaVector3f(0, 0, 32 * scale - 2));
            uint32 b = _meshBuilder.addVertex(iaVector3f(18 * scale, 0, 32 * scale - 2));
            uint32 c = _meshBuilder.addVertex(iaVector3f(18 * scale, 18 * scale, 32 * scale - 2));
            uint32 d = _meshBuilder.addVertex(iaVector3f(0, 18 * scale, 32 * scale - 2));

            iaVector3f normal(0, 0, 1);

            _meshBuilder.accumulateNormal(a, normal);
            _meshBuilder.accumulateNormal(b, normal);
            _meshBuilder.accumulateNormal(c, normal);
            _meshBuilder.accumulateNormal(d, normal);
            _meshBuilder.setTexCoord(a, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(b, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(c, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(d, iaVector2f(0.0, 0.0f), 0);

            _meshBuilder.addTriangle(a, b, c);
            _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
            _meshBuilder.addTriangle(a, c, d);
            _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
        }

        if ((neighborLODs & NEIGHBOR_ZNEGATIVE) != 0)
        {
            uint32 a = _meshBuilder.addVertex(iaVector3f(0, 0, 2));
            uint32 b = _meshBuilder.addVertex(iaVector3f(16 * scale, 0, 2));
            uint32 c = _meshBuilder.addVertex(iaVector3f(16 * scale, 16 * scale, 2));
            uint32 d = _meshBuilder.addVertex(iaVector3f(0, 16 * scale, 2));

            iaVector3f normal(0, 0, -1);

            _meshBuilder.accumulateNormal(a, normal);
            _meshBuilder.accumulateNormal(b, normal);
            _meshBuilder.accumulateNormal(c, normal);
            _meshBuilder.accumulateNormal(d, normal);
            _meshBuilder.setTexCoord(a, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(b, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(c, iaVector2f(0.0, 0.0f), 0);
            _meshBuilder.setTexCoord(d, iaVector2f(0.0, 0.0f), 0);

            _meshBuilder.addTriangle(a, b, c);
            _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
            _meshBuilder.addTriangle(a, c, d);
            _trianglesToKeep[0].push_back(_meshBuilder.getTrianglesCount() - 1);
        }/**/

        if (_meshBuilder.getTrianglesCount() != 0)
        {
#ifdef CALC_NORMALS
            _meshBuilder.normalizeNormals();
#endif

            for (auto iter : _trianglesToKeep)
            {
                result = _meshBuilder.createMesh(iter.second);
                break;
            }
        }

        return result;
    }
}

// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include "iContouringCubes.h"
#include "iContouringCubesLookup.h"

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

        _meshBuilder.setJoinVertexes(true);
    }

    /*! helper macro to map density values from 0&1-255 to lenght 0.0-1.0

    0 -> density is zero therefore it is outside the object
    1 -> is also integpreted als density zero (1-255 -> 0.0-1.0)
    */
#define decode(value) ((value > 0 ? value - 1 : value) / 254.0)

#define CALC_NORMALS
    bool iContouringCubes::calculateVertex(uint8 di0, uint8 di1, uint8 di2, uint8 di3, uint8 di4, uint8 di5, uint8 di6, uint8 di7, iaVector3f& vertex)
    {
        int div = 0;
        iaVector3f calcPos;

        uint8 density0 = _densityEncodedWork[di0];
        uint8 density1 = _densityEncodedWork[di1];
        uint8 density2 = _densityEncodedWork[di2];
        uint8 density3 = _densityEncodedWork[di3];
        uint8 density4 = _densityEncodedWork[di4];
        uint8 density5 = _densityEncodedWork[di5];
        uint8 density6 = _densityEncodedWork[di6];
        uint8 density7 = _densityEncodedWork[di7];

        float32 d0 = _densityWork[di0];
        float32 d1 = _densityWork[di1];
        float32 d2 = _densityWork[di2];
        float32 d3 = _densityWork[di3];
        float32 d4 = _densityWork[di4];
        float32 d5 = _densityWork[di5];
        float32 d6 = _densityWork[di6];
        float32 d7 = _densityWork[di7];

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

        //calcPos.set(0.5, 0.5, 0.5);

        vertex = calcPos;

        return div != 0 ? true : false;
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

    void iContouringCubes::interpolateEndcodedDensity(uint8 dst, uint8 srcA, uint8 srcB)
    {
        // just for aproximation 
        if (_densityNextLOD[dst] > 0)
        {
            _densityEncodedNextLOD[dst] = _densityNextLOD[dst] * 254 + 1;
        }
        else
        {
            _densityEncodedNextLOD[dst] = _densityNextLOD[dst] * 254;
        }
    }

    void iContouringCubes::calculateNextLOD()
    {
#define interpolateDensity(dst, srcA, srcB) _densityNextLOD[dst] = (_densityNextLOD[srcA] + _densityNextLOD[srcB]) * 0.5

        iaVector3I blockPos = _cubePosition;
        blockPos._y -= 2;
        blockPos._x = blockPos._x >> 1;
        blockPos._y = blockPos._y >> 1;
        blockPos._z = blockPos._z >> 1;
        blockPos += _nextLODVoxelOffset;

        _densityEncodedNextLOD[0] = _voxelDataNextLOD->getVoxelDensity(iaVector3I(blockPos._x + 0, blockPos._y + 0, blockPos._z + 0));
        _densityEncodedNextLOD[2] = _voxelDataNextLOD->getVoxelDensity(iaVector3I(blockPos._x + 1, blockPos._y + 0, blockPos._z + 0));
        _densityEncodedNextLOD[6] = _voxelDataNextLOD->getVoxelDensity(iaVector3I(blockPos._x + 0, blockPos._y + 0, blockPos._z + 1));
        _densityEncodedNextLOD[8] = _voxelDataNextLOD->getVoxelDensity(iaVector3I(blockPos._x + 1, blockPos._y + 0, blockPos._z + 1));
        _densityEncodedNextLOD[18] = _voxelDataNextLOD->getVoxelDensity(iaVector3I(blockPos._x + 0, blockPos._y + 1, blockPos._z + 0));
        _densityEncodedNextLOD[20] = _voxelDataNextLOD->getVoxelDensity(iaVector3I(blockPos._x + 1, blockPos._y + 1, blockPos._z + 0));
        _densityEncodedNextLOD[24] = _voxelDataNextLOD->getVoxelDensity(iaVector3I(blockPos._x + 0, blockPos._y + 1, blockPos._z + 1));
        _densityEncodedNextLOD[26] = _voxelDataNextLOD->getVoxelDensity(iaVector3I(blockPos._x + 1, blockPos._y + 1, blockPos._z + 1));

        _densityNextLOD[0] = decode(_densityEncodedNextLOD[0]);
        _densityNextLOD[2] = decode(_densityEncodedNextLOD[2]);
        _densityNextLOD[6] = decode(_densityEncodedNextLOD[6]);
        _densityNextLOD[8] = decode(_densityEncodedNextLOD[8]);
        _densityNextLOD[18] = decode(_densityEncodedNextLOD[18]);
        _densityNextLOD[20] = decode(_densityEncodedNextLOD[20]);
        _densityNextLOD[24] = decode(_densityEncodedNextLOD[24]);
        _densityNextLOD[26] = decode(_densityEncodedNextLOD[26]);

        interpolateDensity(1, 0, 2);
        interpolateDensity(3, 0, 6);
        interpolateDensity(5, 2, 8);
        interpolateDensity(7, 6, 8);
        interpolateDensity(4, 1, 7);

        interpolateDensity(19, 18, 20);
        interpolateDensity(21, 18, 24);
        interpolateDensity(23, 20, 26);
        interpolateDensity(25, 24, 26);
        interpolateDensity(22, 19, 25);

        interpolateDensity(9, 0, 18);
        interpolateDensity(10, 1, 19);
        interpolateDensity(11, 2, 20);
        interpolateDensity(12, 3, 21);
        interpolateDensity(13, 4, 22);
        interpolateDensity(14, 5, 23);
        interpolateDensity(15, 6, 24);
        interpolateDensity(16, 7, 25);
        interpolateDensity(17, 8, 26);
        
        interpolateEndcodedDensity(1, 0, 2);
        interpolateEndcodedDensity(3, 0, 6);
        interpolateEndcodedDensity(5, 2, 8);
        interpolateEndcodedDensity(7, 6, 8);
        interpolateEndcodedDensity(4, 1, 7);

        interpolateEndcodedDensity(19, 18, 20);
        interpolateEndcodedDensity(21, 18, 24);
        interpolateEndcodedDensity(23, 20, 26);
        interpolateEndcodedDensity(25, 24, 26);
        interpolateEndcodedDensity(22, 19, 25);

        interpolateEndcodedDensity(9, 0, 18);
        interpolateEndcodedDensity(10, 1, 19);
        interpolateEndcodedDensity(11, 2, 20);
        interpolateEndcodedDensity(12, 3, 21);
        interpolateEndcodedDensity(13, 4, 22);
        interpolateEndcodedDensity(14, 5, 23);
        interpolateEndcodedDensity(15, 6, 24);
        interpolateEndcodedDensity(16, 7, 25);
        interpolateEndcodedDensity(17, 8, 26);
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

        for (int i = 0; i < 27; ++i)
        {
            _densityWork[i] = _density[i];
            _densityEncodedWork[i] = _densityEncoded[i];
        }

        if (neighborLODs != 0 &&
            _voxelDataNextLOD != nullptr)
        {
            calculateNextLOD();

            if (((neighborLODs & NEIGHBOR_XNEGATIVE) != 0))
                /*||
                ((neighborLODs & NEIGHBOR_ZPOSITIVE) != 0) ||
                ((neighborLODs & NEIGHBOR_YPOSITIVE) != 0))*/
            {
                _densityEncodedWork[0] = _densityEncodedNextLOD[0];
                _densityEncodedWork[3] = _densityEncodedNextLOD[3];
                _densityEncodedWork[6] = _densityEncodedNextLOD[6];
                _densityEncodedWork[9] = _densityEncodedNextLOD[9];
                _densityEncodedWork[12] = _densityEncodedNextLOD[12];
                _densityEncodedWork[15] = _densityEncodedNextLOD[15];
                _densityEncodedWork[18] = _densityEncodedNextLOD[18];
                _densityEncodedWork[21] = _densityEncodedNextLOD[21];
                _densityEncodedWork[24] = _densityEncodedNextLOD[24];

                _densityWork[0] = _densityNextLOD[0];
                _densityWork[3] = _densityNextLOD[3];
                _densityWork[6] = _densityNextLOD[6];
                _densityWork[9] = _densityNextLOD[9];
                _densityWork[12] = _densityNextLOD[12];
                _densityWork[15] = _densityNextLOD[15];
                _densityWork[18] = _densityNextLOD[18];
                _densityWork[21] = _densityNextLOD[21];
                _densityWork[24] = _densityNextLOD[24];
            }
        }

        if (_densityEncodedWork[13] > 0.0f)
        {
            if (_densityEncodedWork[14] <= 0.0f)
            {

                calculateVertex(1, 2, 4, 5, 10, 11, 13, 14, va);
                va += transformedCubePosition;
                va += dirs[1];
                va *= _scale;
                va += _offset;

                calculateVertex(4, 5, 7, 8, 13, 14, 16, 17, vb);
                vb += transformedCubePosition;
                vb += dirs[1];
                vb += dirs[0];
                vb *= _scale;
                vb += _offset;

                calculateVertex(13, 14, 16, 17, 22, 23, 25, 26, vc);
                vc += transformedCubePosition;
                vc += dirs[1];
                vc += dirs[0];
                vc += dirs[4];
                vc *= _scale;
                vc += _offset;

                calculateVertex(10, 11, 13, 14, 19, 20, 22, 23, vd);
                vd += transformedCubePosition;
                vd += dirs[1];
                vd += dirs[4];
                vd *= _scale;
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

            if (_densityEncodedWork[16] <= 0.0f)
            {
                calculateVertex(4, 5, 7, 8, 13, 14, 16, 17, va);
                va += transformedCubePosition;
                va += dirs[1];
                va += dirs[0];
                va *= _scale;
                va += _offset;

                calculateVertex(3, 4, 6, 7, 12, 13, 15, 16, vb);
                vb += transformedCubePosition;
                vb += dirs[0];
                vb *= _scale;
                vb += _offset;

                calculateVertex(12, 13, 15, 16, 21, 22, 24, 25, vc);
                vc += transformedCubePosition;
                vc += dirs[4];
                vc += dirs[0];
                vc *= _scale;
                vc += _offset;

                calculateVertex(13, 14, 16, 17, 22, 23, 25, 26, vd);
                vd += transformedCubePosition;
                vd += dirs[0];
                vd += dirs[1];
                vd += dirs[4];
                vd *= _scale;
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

            if (_densityEncodedWork[22] <= 0.0f)
            {
                calculateVertex(10, 11, 13, 14, 19, 20, 22, 23, va);
                va += transformedCubePosition;
                va += dirs[1];
                va += dirs[4];
                va *= _scale;
                va += _offset;

                calculateVertex(13, 14, 16, 17, 22, 23, 25, 26, vb);
                vb += dirs[0];
                vb += dirs[1];
                vb += dirs[4];
                vb += transformedCubePosition;
                vb *= _scale;
                vb += _offset;

                calculateVertex(12, 13, 15, 16, 21, 22, 24, 25, vc);
                vc += dirs[4];
                vc += dirs[0];
                vc += transformedCubePosition;
                vc *= _scale;
                vc += _offset;

                calculateVertex(9, 10, 12, 13, 18, 19, 21, 22, vd);
                vd += dirs[4];
                vd += transformedCubePosition;
                vd *= _scale;
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
            if (_densityEncodedWork[14] > 0.0f)
            {
                calculateVertex(1, 2, 4, 5, 10, 11, 13, 14, va);
                va += dirs[1];
                va += transformedCubePosition;
                va *= _scale;
                va += _offset;

                calculateVertex(4, 5, 7, 8, 13, 14, 16, 17, vb);
                vb += transformedCubePosition;
                vb += dirs[0];
                vb += dirs[1];
                vb *= _scale;
                vb += _offset;

                calculateVertex(13, 14, 16, 17, 22, 23, 25, 26, vc);
                vc += transformedCubePosition;
                vc += dirs[0];
                vc += dirs[1];
                vc += dirs[4];
                vc *= _scale;
                vc += _offset;

                calculateVertex(10, 11, 13, 14, 19, 20, 22, 23, vd);
                vd += transformedCubePosition;
                vd += dirs[1];
                vd += dirs[4];
                vd *= _scale;
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

            if (_densityEncodedWork[16] > 0.0f)
            {
                calculateVertex(4, 5, 7, 8, 13, 14, 16, 17, va);
                va += transformedCubePosition;
                va += dirs[1];
                va += dirs[0];
                va *= _scale;
                va += _offset;

                calculateVertex(3, 4, 6, 7, 12, 13, 15, 16, vb);
                vb += transformedCubePosition;
                vb += dirs[0];
                vb *= _scale;
                vb += _offset;

                calculateVertex(12, 13, 15, 16, 21, 22, 24, 25, vc);
                vc += transformedCubePosition;
                vc += dirs[4];
                vc += dirs[0];
                vc *= _scale;
                vc += _offset;

                calculateVertex(13, 14, 16, 17, 22, 23, 25, 26, vd);
                vd += transformedCubePosition;
                vd += dirs[0];
                vd += dirs[1];
                vd += dirs[4];
                vd *= _scale;
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

            if (_densityEncodedWork[22] > 0.0f)
            {
                calculateVertex(10, 11, 13, 14, 19, 20, 22, 23, va);
                va += transformedCubePosition;
                va += dirs[1];
                va += dirs[4];
                va *= _scale;
                va += _offset;

                calculateVertex(13, 14, 16, 17, 22, 23, 25, 26, vb);
                vb += transformedCubePosition;
                vb += dirs[0];
                vb += dirs[1];
                vb += dirs[4];
                vb *= _scale;
                vb += _offset;

                calculateVertex(12, 13, 15, 16, 21, 22, 24, 25, vc);
                vc += transformedCubePosition;
                vc += dirs[4];
                vc += dirs[0];
                vc *= _scale;
                vc += _offset;

                calculateVertex(9, 10, 12, 13, 18, 19, 21, 22, vd);
                vd += transformedCubePosition;
                vd += dirs[4];
                vd *= _scale;
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

    void iContouringCubes::climb()
    {
        _cubePosition._y++;

        for (int i = 0; i < 9; ++i)
        {
            _densityEncoded[i + 0] = _densityEncoded[i + 9];
            _densityEncoded[i + 9] = _densityEncoded[i + 18];
            _densityEncoded[i + 18] = static_cast<float32>(_currentPoles[i]._currentDensityPole->getValue(_cubePosition._y));

            _density[i + 0] = _density[i + 9];
            _density[i + 9] = _density[i + 18];
            _density[i + 18] = decode(_densityEncoded[i + 18]);
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
            _densityEncoded[i] = 0;
            _density[i] = 0;
        }

        for (int i = 0; i < 9; ++i)
        {
            _densityEncoded[i + 18] = static_cast<float32>(_currentPoles[i]._currentDensityPole->getValue(_cubePosition._y));
            _density[i + 18] = decode(_densityEncoded[i + 18]);
        }
    }

    void iContouringCubes::setVoxelData(iVoxelData* voxelData)
    {
        _voxelData = voxelData;
    }

    void iContouringCubes::setVoxelDataNextLOD(iVoxelData* voxelData)
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
            result.set(-value, value * 2, -value);
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

                    generateGeometry(true, LODs);

                    y++;
                } while (y < marchingVolume._y);
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

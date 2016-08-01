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
//#define CALC_NORMALS

    void iContouringCubes::calculateVertex(uint8 density0, uint8 density1, uint8 density2, uint8 density3, uint8 density4, uint8 density5, uint8 density6, uint8 density7, iaVector3f& vertex, iaVector3f& normal)
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

		calcPos.set(0.5, 0.5, 0.5);

//#define lerp(v0, v1, t) v0 + t*(v1-v0)
#ifndef CALC_NORMALS
#define lerp(v0, v1, t) (1.0f-t)*v0 + t*v1

        float32 delta0 = d0 - d1;
        float32 delta1 = d2 - d3;
        float32 delta2 = d4 - d5;
        float32 delta3 = d6 - d7;

        float32 temp1 = lerp(delta0, delta1, calcPos._z);
        float32 temp2 = lerp(delta2, delta3, calcPos._z);
        normal._x = lerp(temp1, temp2, calcPos._y);

        float32 delta4 = d0 - d4;
        float32 delta5 = d1 - d5;
        float32 delta6 = d2 - d6;
        float32 delta7 = d3 - d7;

        temp1 = lerp(delta4, delta5, calcPos._x);
        temp2 = lerp(delta6, delta7, calcPos._x);
        normal._y = lerp(temp1, temp2, calcPos._z);

        float32 delta8 = d0 - d2;
        float32 delta9 = d4 - d6;
        float32 delta10 = d1 - d3;
        float32 delta11 = d5 - d7;

        temp1 = lerp(delta8, delta9, calcPos._y);
        temp2 = lerp(delta10, delta11, calcPos._y);
        normal._z = lerp(temp1, temp2, calcPos._x);

        normal.normalize();
#endif
    }

    uint32 calcMaterialKey(uint8 mat0, uint8 mat1, uint8 mat2)
    {
        return 0;
        /*if (mat0 > mat1)
        {
            std::swap(mat0, mat1);
        }
        if (mat0 > mat2)
        {
            std::swap(mat0, mat2);
        }
        if (mat1 > mat2)
        {
            std::swap(mat1, mat2);
        }

        return ((static_cast<uint32>(mat0) << 16) + (static_cast<uint32>(mat1) << 8) + static_cast<uint32>(mat2));*/
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


    void iContouringCubes::generateGeometry(const iaVector3f& transformedCubePosition, const uint8* density, bool keepTriangles, uint32 neighborLODs)
    {
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
        
        if (_density[13] > 0.0f)
        {
            if (_density[14] <= 0.0f)
            {
                a = _meshBuilder.addVertex(_vertexPositions[1]);
                b = _meshBuilder.addVertex(_vertexPositions[3]);
                c = _meshBuilder.addVertex(_vertexPositions[7]);
                d = _meshBuilder.addVertex(_vertexPositions[5]);

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
                _meshBuilder.setNormal(a, _vertexNormals[1]);
				_meshBuilder.setNormal(b, _vertexNormals[3]);
				_meshBuilder.setNormal(c, _vertexNormals[7]);
				_meshBuilder.setNormal(d, _vertexNormals[5]);
#endif

                _meshBuilder.addTriangle(c, b, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matb, matc)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                _meshBuilder.addTriangle(d, c, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matc, matd)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }
            }

            if (density[16] <= 0.0f)
            {
				a = _meshBuilder.addVertex(_vertexPositions[3]);
				b = _meshBuilder.addVertex(_vertexPositions[2]);
				c = _meshBuilder.addVertex(_vertexPositions[6]);
				d = _meshBuilder.addVertex(_vertexPositions[7]);

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
				_meshBuilder.setNormal(a, _vertexNormals[3]);
				_meshBuilder.setNormal(b, _vertexNormals[2]);
				_meshBuilder.setNormal(c, _vertexNormals[6]);
				_meshBuilder.setNormal(d, _vertexNormals[7]);
#endif

                _meshBuilder.addTriangle(c, b, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matb, matc)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                _meshBuilder.addTriangle(d, c, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matc, matd)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }
            }

            if (density[22] <= 0.0f)
            {
				a = _meshBuilder.addVertex(_vertexPositions[5]);
				b = _meshBuilder.addVertex(_vertexPositions[7]);
				c = _meshBuilder.addVertex(_vertexPositions[6]);
				d = _meshBuilder.addVertex(_vertexPositions[4]);

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
				_meshBuilder.setNormal(a, _vertexNormals[5]);
				_meshBuilder.setNormal(b, _vertexNormals[7]);
				_meshBuilder.setNormal(c, _vertexNormals[6]);
				_meshBuilder.setNormal(d, _vertexNormals[4]);
#endif

                _meshBuilder.addTriangle(c, b, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matb, matc)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                _meshBuilder.addTriangle(d, c, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matc, matd)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }
            }
        }
        else
        {

            if (density[14] > 0.0f)
            {
				a = _meshBuilder.addVertex(_vertexPositions[1]);
				b = _meshBuilder.addVertex(_vertexPositions[3]);
				c = _meshBuilder.addVertex(_vertexPositions[7]);
				d = _meshBuilder.addVertex(_vertexPositions[5]);

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
				_meshBuilder.setNormal(a, _vertexNormals[1]);
				_meshBuilder.setNormal(b, _vertexNormals[3]);
				_meshBuilder.setNormal(c, _vertexNormals[7]);
				_meshBuilder.setNormal(d, _vertexNormals[5]);
#endif

                _meshBuilder.addTriangle(a, b, c);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matb, matc)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                _meshBuilder.addTriangle(a, c, d);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matc, matd)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }
            }

            if (density[16] > 0.0f)
            {
				a = _meshBuilder.addVertex(_vertexPositions[3]);
				b = _meshBuilder.addVertex(_vertexPositions[2]);
				c = _meshBuilder.addVertex(_vertexPositions[6]);
				d = _meshBuilder.addVertex(_vertexPositions[7]);

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
				_meshBuilder.setNormal(a, _vertexNormals[3]);
				_meshBuilder.setNormal(b, _vertexNormals[2]);
				_meshBuilder.setNormal(c, _vertexNormals[6]);
				_meshBuilder.setNormal(d, _vertexNormals[7]);
#endif

                _meshBuilder.addTriangle(a, b, c);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matb, matc)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                _meshBuilder.addTriangle(a, c, d);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matc, matd)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }
            }

            if (density[22] > 0.0f)
            {
				a = _meshBuilder.addVertex(_vertexPositions[5]);
				b = _meshBuilder.addVertex(_vertexPositions[7]);
				c = _meshBuilder.addVertex(_vertexPositions[6]);
				d = _meshBuilder.addVertex(_vertexPositions[4]);

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
				_meshBuilder.setNormal(a, _vertexNormals[5]);
				_meshBuilder.setNormal(b, _vertexNormals[7]);
				_meshBuilder.setNormal(c, _vertexNormals[6]);
				_meshBuilder.setNormal(d, _vertexNormals[4]);
#endif

                _meshBuilder.addTriangle(a, b, c);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matb, matc)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                _meshBuilder.addTriangle(a, c, d);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matc, matd)].push_back(_meshBuilder.getTrianglesCount() - 1);
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
		climbVoxel();
		climbPositionAndNormals();
    }

	void iContouringCubes::climbVoxel()
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

	//
	//     6------7
	//    /|     /|
	//   4------5 |
	//   | |    | |
	//   | |    | |
	//   | 2----|-3
	//   |/     |/
	//   0------1
	*/

	void iContouringCubes::climbPositionAndNormals()
	{
		// TODO
		iaVector3f transformedCubePosition(_cubePosition._x, _cubePosition._y, _cubePosition._z);
		transformedCubePosition._x -= _cubeStartPosition._x - 1; // TODO workaround I don't understand
		transformedCubePosition._y -= _cubeStartPosition._y + 2; // TODO workaround I don't understand
		transformedCubePosition._z -= _cubeStartPosition._z;

		// TODO maybe this one is never used?!
		calculateVertex(_density[0], _density[1], _density[3], _density[4], _density[9], _density[10], _density[12], _density[13], _vertexPositions[0], _vertexNormals[0]);
		_vertexPositions[0] += transformedCubePosition;
		_vertexPositions[0] += dirs[5];
		_vertexPositions[0] += dirs[3];

		calculateVertex(_density[1], _density[2], _density[4], _density[5], _density[10], _density[11], _density[13], _density[14], _vertexPositions[1], _vertexNormals[1]);
		_vertexPositions[1] += transformedCubePosition;
		_vertexPositions[1] += dirs[5];

		calculateVertex(_density[3], _density[4], _density[6], _density[7], _density[12], _density[13], _density[15], _density[16], _vertexPositions[2], _vertexNormals[2]);
		_vertexPositions[2] += transformedCubePosition;
		_vertexPositions[2] += dirs[5];
		_vertexPositions[2] += dirs[3];
		_vertexPositions[2] += dirs[0];

		calculateVertex(_density[4], _density[5], _density[7], _density[8], _density[13], _density[14], _density[16], _density[17], _vertexPositions[3], _vertexNormals[3]);
		_vertexPositions[3] += transformedCubePosition;
		_vertexPositions[3] += dirs[5];
		_vertexPositions[3] += dirs[0];

		calculateVertex(_density[9], _density[10], _density[12], _density[13], _density[18], _density[19], _density[21], _density[22], _vertexPositions[4], _vertexNormals[4]);
		_vertexPositions[4] += transformedCubePosition;
		_vertexPositions[4] += dirs[3];

		calculateVertex(_density[10], _density[11], _density[13], _density[14], _density[19], _density[20], _density[22], _density[23], _vertexPositions[5], _vertexNormals[5]);
		_vertexPositions[5] += transformedCubePosition;

		calculateVertex(_density[12], _density[13], _density[15], _density[16], _density[21], _density[22], _density[24], _density[25], _vertexPositions[6], _vertexNormals[6]);
		_vertexPositions[6] += transformedCubePosition;
		_vertexPositions[6] += dirs[3];
		_vertexPositions[6] += dirs[0];

		calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26], _vertexPositions[7], _vertexNormals[7]);
		_vertexPositions[7] += transformedCubePosition;
		_vertexPositions[7] += dirs[0];

		for (int i = 0; i < 8; ++i)
		{
			_vertexPositions[i] *= _scale;
		}
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

        _currentPoles[0]._currentMaterialPole = &_voxelData->getMaterialPole(_cubePosition._x, _cubePosition._z);
        _currentPoles[1]._currentMaterialPole = &_voxelData->getMaterialPole(_cubePosition._x + 1, _cubePosition._z);
        _currentPoles[2]._currentMaterialPole = &_voxelData->getMaterialPole(_cubePosition._x + 2, _cubePosition._z);

        _currentPoles[3]._currentMaterialPole = &_voxelData->getMaterialPole(_cubePosition._x, _cubePosition._z + 1);
        _currentPoles[4]._currentMaterialPole = &_voxelData->getMaterialPole(_cubePosition._x + 1, _cubePosition._z + 1);
        _currentPoles[5]._currentMaterialPole = &_voxelData->getMaterialPole(_cubePosition._x + 2, _cubePosition._z + 1);

        _currentPoles[6]._currentMaterialPole = &_voxelData->getMaterialPole(_cubePosition._x, _cubePosition._z + 2);
        _currentPoles[7]._currentMaterialPole = &_voxelData->getMaterialPole(_cubePosition._x + 1, _cubePosition._z + 2);
        _currentPoles[8]._currentMaterialPole = &_voxelData->getMaterialPole(_cubePosition._x + 2, _cubePosition._z + 2);

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

    shared_ptr<iMesh> iContouringCubes::compile(iaVector3I pos, iaVector3I volume, uint32 lod, uint32 neighborLODs)
    {
        shared_ptr<iMesh> result;

        _lod = lod;
        _scale = pow(2, _lod);

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

					iaVector3f transformedCubePosition(_cubePosition._x, _cubePosition._y, _cubePosition._z);
					transformedCubePosition._x -= _cubeStartPosition._x - 1; // TODO workaround I don't understand
					transformedCubePosition._y -= _cubeStartPosition._y + 2; // TODO workaround I don't understand
					transformedCubePosition._z -= _cubeStartPosition._z;

                    generateGeometry(transformedCubePosition, _density, true, LODs);

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

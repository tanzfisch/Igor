// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include "iContouringCubes.h"

#include <iTimer.h>

#include <iaMatrix3x3.h>
using namespace IgorAux;

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
#define rescale(value) ((value - 1.0) / 254.0)


    /*

       Y
       |
       |
       0---X
      /
     /
    Z

    */
    iaVector3f iContouringCubes::calculateVertex(uint8 density0, uint8 density1, uint8 density2, uint8 density3, uint8 density4, uint8 density5, uint8 density6, uint8 density7)
    {
        int div = 0;
        iaVector3f calcPos;

        float32 d0 = rescale(static_cast<float32>(density1));
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

        return calcPos;
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
    void iContouringCubes::generateGeometry(bool keepTriangles)
    {
#define chooseMaterial(mat0, mat1, mat2, mat3) max(mat0, max(mat1, max(mat2, mat3)))

        iaVector3f va;
        iaVector3f vb;
        iaVector3f vc;
        iaVector3f vd;

        iaVector3f normal;
        iaVector3f ab;
        iaVector3f ac;
        iaVector3f ad;
        uint32 a, b, c;

        uint8 mata;
        uint8 matb;
        uint8 matc;
        uint8 matd;
        uint32 matKey = 0;

        iaVector3f transformed_cube_position(_cubePosition._x, _cubePosition._y, _cubePosition._z);
        transformed_cube_position._x -= _cubeStartPosition._x - 1; // TODO workaround
        transformed_cube_position._y -= _cubeStartPosition._y + 2; // TODO workaround
        transformed_cube_position._z -= _cubeStartPosition._z;

        if (_density[13] > 0.0f)
        {
            if (_density[14] <= 0.0f)
            {
                mata = chooseMaterial(_material[1], _material[4], _material[10], _material[13]);
                matb = chooseMaterial(_material[4], _material[7], _material[13], _material[16]);
                matc = chooseMaterial(_material[13], _material[16], _material[22], _material[25]);
                matd = chooseMaterial(_material[13], _material[16], _material[22], _material[25]);

                va = transformed_cube_position;
                va += dirs[5];
                va += calculateVertex(_density[1], _density[2], _density[4], _density[5], _density[10], _density[11], _density[13], _density[14]);

                vb = transformed_cube_position;
                vb += dirs[5];
                vb += dirs[0];
                vb += calculateVertex(_density[4], _density[5], _density[7], _density[8], _density[13], _density[14], _density[16], _density[17]);

                vc = transformed_cube_position;
                vc += dirs[0];
                vc += calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26]);

                vd = transformed_cube_position;
                vd += calculateVertex(_density[10], _density[11], _density[13], _density[14], _density[19], _density[20], _density[22], _density[23]);

				va *= _scale;
				vb *= _scale;
				vc *= _scale;
				vd *= _scale;

                ab = vb - va;
                ac = vc - va;
                normal = ac % ab;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                _meshBuilder.accumulateNormal(a, normal);
                _meshBuilder.accumulateNormal(b, normal);
                _meshBuilder.accumulateNormal(c, normal);
                _meshBuilder.setTexCoord(a, iaVector2f(static_cast<float32>(mata), 0.0f), 0);
                _meshBuilder.setTexCoord(b, iaVector2f(static_cast<float32>(matb), 0.0f), 0);
                _meshBuilder.setTexCoord(c, iaVector2f(static_cast<float32>(matc), 0.0f), 0);
                _meshBuilder.addTriangle(c, b, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matb, matc)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                ab = vb - va;
                ad = vd - va;
                normal = ad % ab;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vc);
                c = _meshBuilder.addVertex(vd);
                _meshBuilder.accumulateNormal(a, normal);
                _meshBuilder.accumulateNormal(b, normal);
                _meshBuilder.accumulateNormal(c, normal);
                _meshBuilder.setTexCoord(a, iaVector2f(static_cast<float32>(mata), 0.0f), 0);
                _meshBuilder.setTexCoord(b, iaVector2f(static_cast<float32>(matc), 0.0f), 0);
                _meshBuilder.setTexCoord(c, iaVector2f(static_cast<float32>(matd), 0.0f), 0);
                _meshBuilder.addTriangle(c, b, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matc, matd)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }
            }

            if (_density[16] <= 0.0f)
            {
                mata = chooseMaterial(_material[4], _material[5], _material[13], _material[14]);
                matb = chooseMaterial(_material[3], _material[4], _material[12], _material[13]);
                matc = chooseMaterial(_material[12], _material[13], _material[21], _material[22]);
                matd = chooseMaterial(_material[13], _material[14], _material[22], _material[23]);

                va = transformed_cube_position;
                va += dirs[5];
                va += dirs[0];
                va += calculateVertex(_density[4], _density[5], _density[7], _density[8], _density[13], _density[14], _density[16], _density[17]);

                vb = transformed_cube_position;
                vb += dirs[5];
                vb += dirs[3];
                vb += dirs[0];
                vb += calculateVertex(_density[3], _density[4], _density[6], _density[7], _density[12], _density[13], _density[15], _density[16]);

                vc = transformed_cube_position;
                vc += dirs[3];
                vc += dirs[0];
                vc += calculateVertex(_density[12], _density[13], _density[15], _density[16], _density[21], _density[22], _density[24], _density[25]);

                vd = transformed_cube_position;
                vd += dirs[0];
                vd += calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26]);
				
				va *= _scale;
				vb *= _scale;
				vc *= _scale;
				vd *= _scale;

                ab = vb - va;
                ac = vc - va;
                normal = ac % ab;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                _meshBuilder.accumulateNormal(a, normal);
                _meshBuilder.accumulateNormal(b, normal);
                _meshBuilder.accumulateNormal(c, normal);
                _meshBuilder.setTexCoord(a, iaVector2f(static_cast<float32>(mata), 0.0f), 0);
                _meshBuilder.setTexCoord(b, iaVector2f(static_cast<float32>(matb), 0.0f), 0);
                _meshBuilder.setTexCoord(c, iaVector2f(static_cast<float32>(matc), 0.0f), 0);
                _meshBuilder.addTriangle(c, b, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matb, matc)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                ab = vb - va;
                ad = vd - va;
                normal = ad % ab;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vc);
                c = _meshBuilder.addVertex(vd);
                _meshBuilder.accumulateNormal(a, normal);
                _meshBuilder.accumulateNormal(b, normal);
                _meshBuilder.accumulateNormal(c, normal);
                _meshBuilder.setTexCoord(a, iaVector2f(static_cast<float32>(mata), 0.0f), 0);
                _meshBuilder.setTexCoord(b, iaVector2f(static_cast<float32>(matc), 0.0f), 0);
                _meshBuilder.setTexCoord(c, iaVector2f(static_cast<float32>(matd), 0.0f), 0);
                _meshBuilder.addTriangle(c, b, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matc, matd)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }
            }

            if (_density[22] <= 0.0f)
            {
                mata = chooseMaterial(_material[19], _material[11], _material[13], _material[14]);
                matb = chooseMaterial(_material[13], _material[14], _material[16], _material[17]);
                matc = chooseMaterial(_material[12], _material[13], _material[15], _material[16]);
                matd = chooseMaterial(_material[9], _material[10], _material[12], _material[13]);

                va = transformed_cube_position;
                va += calculateVertex(_density[10], _density[11], _density[13], _density[14], _density[19], _density[20], _density[22], _density[23]);

                vb = transformed_cube_position;
                vb += dirs[0];
                vb += calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26]);

                vc = transformed_cube_position;
                vc += dirs[3];
                vc += dirs[0];
                vc += calculateVertex(_density[12], _density[13], _density[15], _density[16], _density[21], _density[22], _density[24], _density[25]);

                vd = transformed_cube_position;
                vd += dirs[3];
                vd += calculateVertex(_density[9], _density[10], _density[12], _density[13], _density[18], _density[19], _density[21], _density[22]);

				va *= _scale;
				vb *= _scale;
				vc *= _scale;
				vd *= _scale;

                ab = vb - va;
                ac = vc - va;
                normal = ac % ab;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                _meshBuilder.accumulateNormal(a, normal);
                _meshBuilder.accumulateNormal(b, normal);
                _meshBuilder.accumulateNormal(c, normal);
                _meshBuilder.setTexCoord(a, iaVector2f(static_cast<float32>(mata), 0.0f), 0);
                _meshBuilder.setTexCoord(b, iaVector2f(static_cast<float32>(matb), 0.0f), 0);
                _meshBuilder.setTexCoord(c, iaVector2f(static_cast<float32>(matc), 0.0f), 0);
                _meshBuilder.addTriangle(c, b, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matb, matc)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                ab = vb - va;
                ad = vd - va;
                normal = ad % ab;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vc);
                c = _meshBuilder.addVertex(vd);
                _meshBuilder.accumulateNormal(a, normal);
                _meshBuilder.accumulateNormal(b, normal);
                _meshBuilder.accumulateNormal(c, normal);
                _meshBuilder.setTexCoord(a, iaVector2f(static_cast<float32>(mata), 0.0f), 0);
                _meshBuilder.setTexCoord(b, iaVector2f(static_cast<float32>(matc), 0.0f), 0);
                _meshBuilder.setTexCoord(c, iaVector2f(static_cast<float32>(matd), 0.0f), 0);
                _meshBuilder.addTriangle(c, b, a);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matc, matd)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }
            }
        }
        else
        { // TODO optimize copy paste code here

            if (_density[14] > 0.0f)
            {
                mata = chooseMaterial(_material[1], _material[4], _material[10], _material[13]);
                matb = chooseMaterial(_material[4], _material[7], _material[13], _material[16]);
                matc = chooseMaterial(_material[13], _material[16], _material[22], _material[25]);
                matd = chooseMaterial(_material[13], _material[16], _material[22], _material[25]);

                va = transformed_cube_position;
                va += dirs[5];
                va += calculateVertex(_density[1], _density[2], _density[4], _density[5], _density[10], _density[11], _density[13], _density[14]);

                vb = transformed_cube_position;
                vb += dirs[0];
                vb += dirs[5];
                vb += calculateVertex(_density[4], _density[5], _density[7], _density[8], _density[13], _density[14], _density[16], _density[17]);

                vc = transformed_cube_position;
                vc += dirs[0];
                vc += calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26]);

                vd = transformed_cube_position;
                vd += calculateVertex(_density[10], _density[11], _density[13], _density[14], _density[19], _density[20], _density[22], _density[23]);

				va *= _scale;
				vb *= _scale;
				vc *= _scale;
				vd *= _scale;

                ab = vb - va;
                ac = vc - va;
                normal = ab % ac;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                _meshBuilder.accumulateNormal(a, normal);
                _meshBuilder.accumulateNormal(b, normal);
                _meshBuilder.accumulateNormal(c, normal);
                _meshBuilder.setTexCoord(a, iaVector2f(static_cast<float32>(mata), 0.0f), 0);
                _meshBuilder.setTexCoord(b, iaVector2f(static_cast<float32>(matb), 0.0f), 0);
                _meshBuilder.setTexCoord(c, iaVector2f(static_cast<float32>(matc), 0.0f), 0);
                _meshBuilder.addTriangle(a, b, c);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matb, matc)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                ab = vb - va;
                ad = vd - va;
                normal = ab % ad;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vc);
                c = _meshBuilder.addVertex(vd);
                _meshBuilder.accumulateNormal(a, normal);
                _meshBuilder.accumulateNormal(b, normal);
                _meshBuilder.accumulateNormal(c, normal);
                _meshBuilder.setTexCoord(a, iaVector2f(static_cast<float32>(mata), 0.0f), 0);
                _meshBuilder.setTexCoord(b, iaVector2f(static_cast<float32>(matc), 0.0f), 0);
                _meshBuilder.setTexCoord(c, iaVector2f(static_cast<float32>(matd), 0.0f), 0);
                _meshBuilder.addTriangle(a, b, c);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matc, matd)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }
            }

            if (_density[16] > 0.0f)
            {
                mata = chooseMaterial(_material[4], _material[5], _material[13], _material[14]);
                matb = chooseMaterial(_material[3], _material[4], _material[12], _material[13]);
                matc = chooseMaterial(_material[12], _material[13], _material[21], _material[22]);
                matd = chooseMaterial(_material[13], _material[14], _material[22], _material[23]);

                va = transformed_cube_position;
                va += dirs[5];
                va += dirs[0];
                va += calculateVertex(_density[4], _density[5], _density[7], _density[8], _density[13], _density[14], _density[16], _density[17]);

                vb = transformed_cube_position;
                vb += dirs[5];
                vb += dirs[3];
                vb += dirs[0];
                vb += calculateVertex(_density[3], _density[4], _density[6], _density[7], _density[12], _density[13], _density[15], _density[16]);

                vc = transformed_cube_position;
                vc += dirs[3];
                vc += dirs[0];
                vc += calculateVertex(_density[12], _density[13], _density[15], _density[16], _density[21], _density[22], _density[24], _density[25]);

                vd = transformed_cube_position;
                vd += dirs[0];
                vd += calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26]);

				va *= _scale;
				vb *= _scale;
				vc *= _scale;
				vd *= _scale;

                ab = vb - va;
                ac = vc - va;
                normal = ab % ac;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                _meshBuilder.accumulateNormal(a, normal);
                _meshBuilder.accumulateNormal(b, normal);
                _meshBuilder.accumulateNormal(c, normal);
                _meshBuilder.setTexCoord(a, iaVector2f(static_cast<float32>(mata), 0.0f), 0);
                _meshBuilder.setTexCoord(b, iaVector2f(static_cast<float32>(matb), 0.0f), 0);
                _meshBuilder.setTexCoord(c, iaVector2f(static_cast<float32>(matc), 0.0f), 0);
                _meshBuilder.addTriangle(a, b, c);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matb, matc)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                ab = vb - va;
                ad = vd - va;
                normal = ab % ad;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vc);
                c = _meshBuilder.addVertex(vd);
                _meshBuilder.accumulateNormal(a, normal);
                _meshBuilder.accumulateNormal(b, normal);
                _meshBuilder.accumulateNormal(c, normal);
                _meshBuilder.setTexCoord(a, iaVector2f(static_cast<float32>(mata), 0.0f), 0);
                _meshBuilder.setTexCoord(b, iaVector2f(static_cast<float32>(matc), 0.0f), 0);
                _meshBuilder.setTexCoord(c, iaVector2f(static_cast<float32>(matd), 0.0f), 0);
                _meshBuilder.addTriangle(a, b, c);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matc, matd)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }
            }

            if (_density[22] > 0.0f)
            {
                mata = chooseMaterial(_material[19], _material[11], _material[13], _material[14]);
                matb = chooseMaterial(_material[13], _material[14], _material[16], _material[17]);
                matc = chooseMaterial(_material[12], _material[13], _material[15], _material[16]);
                matd = chooseMaterial(_material[9], _material[10], _material[12], _material[13]);

                va = transformed_cube_position;
                va += calculateVertex(_density[10], _density[11], _density[13], _density[14], _density[19], _density[20], _density[22], _density[23]);

                vb = transformed_cube_position;
                vb += dirs[0];
                vb += calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26]);

                vc = transformed_cube_position;
                vc += dirs[3];
                vc += dirs[0];
                vc += calculateVertex(_density[12], _density[13], _density[15], _density[16], _density[21], _density[22], _density[24], _density[25]);

                vd = transformed_cube_position;
                vd += dirs[3];
                vd += calculateVertex(_density[9], _density[10], _density[12], _density[13], _density[18], _density[19], _density[21], _density[22]);

				va *= _scale;
				vb *= _scale;
				vc *= _scale;
				vd *= _scale;

                ab = vb - va;
                ac = vc - va;
                normal = ab % ac;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vb);
                c = _meshBuilder.addVertex(vc);
                _meshBuilder.accumulateNormal(a, normal);
                _meshBuilder.accumulateNormal(b, normal);
                _meshBuilder.accumulateNormal(c, normal);
                _meshBuilder.setTexCoord(a, iaVector2f(static_cast<float32>(mata), 0.0f), 0);
                _meshBuilder.setTexCoord(b, iaVector2f(static_cast<float32>(matb), 0.0f), 0);
                _meshBuilder.setTexCoord(c, iaVector2f(static_cast<float32>(matc), 0.0f), 0);
                _meshBuilder.addTriangle(a, b, c);
                if (keepTriangles)
                {
                    _trianglesToKeep[calcMaterialKey(mata, matb, matc)].push_back(_meshBuilder.getTrianglesCount() - 1);
                }

                ab = vb - va;
                ad = vd - va;
                normal = ab % ad;

                a = _meshBuilder.addVertex(va);
                b = _meshBuilder.addVertex(vc);
                c = _meshBuilder.addVertex(vd);
                _meshBuilder.accumulateNormal(a, normal);
                _meshBuilder.accumulateNormal(b, normal);
                _meshBuilder.accumulateNormal(c, normal);
                _meshBuilder.setTexCoord(a, iaVector2f(static_cast<float32>(mata), 0.0f), 0);
                _meshBuilder.setTexCoord(b, iaVector2f(static_cast<float32>(matc), 0.0f), 0);
                _meshBuilder.setTexCoord(c, iaVector2f(static_cast<float32>(matd), 0.0f), 0);
                _meshBuilder.addTriangle(a, b, c);
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
        for (int i = 0; i < 9; ++i)
        {
            _density[i + 0] = _density[i + 9];
            _density[i + 9] = _density[i + 18];
            _density[i + 18] = static_cast<float32>(_currentPoles[i]._currentDensityPole->getValue(_cubePosition._y));

            _material[i + 0] = _material[i + 9];
            _material[i + 9] = _material[i + 18];
            _material[i + 18] = static_cast<float32>(_currentPoles[i]._currentMaterialPole->getValue(_cubePosition._y));
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
            _material[i] = 0;
        }

        for (int i = 0; i < 9; ++i)
        {
            _density[i + 18] = static_cast<float32>(_currentPoles[i]._currentDensityPole->getValue(_cubePosition._y));
            _material[i + 18] = static_cast<float32>(_currentPoles[i]._currentMaterialPole->getValue(_cubePosition._y));
        }

        _cubePosition._y++;
    }

    void iContouringCubes::setVoxelData(iVoxelData* voxelData)
    {
        _voxelData = voxelData;
    }

    shared_ptr<iMesh> iContouringCubes::compile(iaVector3I pos, iaVector3I volume, float64 scale)
    {
        shared_ptr<iMesh> result;

		con_assert(scale > 0, "scale out of range");
		_scale = scale;
        if (scale <= 0)
        {
            return result;
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

        // run through collumns
        for (int x = 0; x < marchingVolume._x; ++x)
        {
            currentPosition._x = _cubeStartPosition._x + x;

            // run through rows
            for (int z = 0; z < marchingVolume._z; ++z)
            {
                currentPosition._z = _cubeStartPosition._z + z;

                // process pole
                startClimb(currentPosition);
                climb();

                do
                {
                    climb();
                    generateGeometry(true);

                } while (!(_cubePosition._y > marchingVolume._y + _cubeStartPosition._y));
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
}

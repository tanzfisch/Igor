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
            DensityPoleIterator pole;
            _currentPoles.push_back(pole);
        }

        _meshBuilder.setJoinVertexes(true);
    }

    /*! helper macro to map density values from 0&1-255 to lenght 0.0-1.0

    0 -> density is zero therefore it is outside the object
    1 -> is also integpreted als density zero (1-255 -> 0.0-1.0)

    \todo hmm maybe this is wrong. because 0 density in one block falls on the same position as full density on the next.
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
    void iContouringCubes::calculateVertex(float32 density0, float32 density1, float32 density2, float32 density3, float32 density4, float32 density5, float32 density6, float32 density7, iaVector3f& pos, iaVector3f& normal)
    {
        int div = 0;
        iaVector3f calcPos;

        float32 d0 = rescale(density1);
        float32 d1 = rescale(density1);
        float32 d2 = rescale(density2);
        float32 d3 = rescale(density3);
        float32 d4 = rescale(density4);
        float32 d5 = rescale(density5);
        float32 d6 = rescale(density6);
        float32 d7 = rescale(density7);

        if (density0 != 0 && density1 == 0)
        {
            calcPos += iaVector3f(d0, 0, 0);
            div++;
        }

        if (density1 != 0 && density0 == 0)
        {
            calcPos += iaVector3f(1 - d1, 0, 0);
            div++;
        }

        if (density0 != 0 && density2 == 0)
        {
            calcPos += iaVector3f(0, 0, d0);
            div++;
        }

        if (density2 != 0 && density0 == 0)
        {
            calcPos += iaVector3f(0, 0, 1 - d2);
            div++;
        }

        if (density0 != 0 && density4 == 0)
        {
            calcPos += iaVector3f(0, d0, 0);
            div++;
        }

        if (density4 != 0 && density0 == 0)
        {
            calcPos += iaVector3f(0, 1 - d4, 0);
            div++;
        }

        if (density7 != 0 && density6 == 0)
        {
            calcPos += iaVector3f(1 - d7, 1, 1);
            div++;
        }

        if (density6 != 0 && density7 == 0)
        {
            calcPos += iaVector3f(d6, 1, 1);
            div++;
        }

        if (density7 != 0 && density5 == 0)
        {
            calcPos += iaVector3f(1, 1, 1 - d7);
            div++;
        }

        if (density5 != 0 && density7 == 0)
        {
            calcPos += iaVector3f(1, 1, d5);
            div++;
        }

        if (density7 != 0 && density3 == 0)
        {
            calcPos += iaVector3f(1, 1 - d7, 1);
            div++;
        }

        if (density3 != 0 && density7 == 0)
        {
            calcPos += iaVector3f(1, d3, 1);
            div++;
        }

        if (density4 != 0 && density5 == 0)
        {
            calcPos += iaVector3f(d4, 1, 0);
            div++;
        }

        if (density5 != 0 && density4 == 0)
        {
            calcPos += iaVector3f(1 - d5, 1, 0);
            div++;
        }

        if (density5 != 0 && density1 == 0)
        {
            calcPos += iaVector3f(1, 1 - d5, 0);
            div++;
        }

        if (density1 != 0 && density5 == 0)
        {
            calcPos += iaVector3f(1, d1, 0);
            div++;
        }

        if (density1 != 0 && density3 == 0)
        {
            calcPos += iaVector3f(1, 0, d1);
            div++;
        }

        if (density3 != 0 && density1 == 0)
        {
            calcPos += iaVector3f(1, 0, 1 - d3);
            div++;
        }

        if (density3 != 0 && density2 == 0)
        {
            calcPos += iaVector3f(1 - d3, 0, 1);
            div++;
        }

        if (density2 != 0 && density3 == 0)
        {
            calcPos += iaVector3f(d2, 0, 1);
            div++;
        }

        if (density2 != 0 && density6 == 0)
        {
            calcPos += iaVector3f(0, d2, 1);
            div++;
        }

        if (density6 != 0 && density2 == 0)
        {
            calcPos += iaVector3f(0, 1 - d6, 1);
            div++;
        }

        if (density6 != 0 && density4 == 0)
        {
            calcPos += iaVector3f(0, 1, 1 - d6);
            div++;
        }

        if (density4 != 0 && density6 == 0)
        {
            calcPos += iaVector3f(0, 1, d4);
            div++;
        }

        if (div != 0)
        {
            calcPos /= static_cast<float32>(div);
        }

        normal = calcPos;
        normal.normalize();
        pos = calcPos;
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
        iaVector3f na;
        iaVector3f nb;
        iaVector3f nc;
        iaVector3f nd;
        
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

                calculateVertex(_density[1], _density[2], _density[4], _density[5], _density[10], _density[11], _density[13], _density[14], va, na);
                va += transformed_cube_position;
                va += dirs[5];

                calculateVertex(_density[4], _density[5], _density[7], _density[8], _density[13], _density[14], _density[16], _density[17], vb, nb);
                vb += transformed_cube_position;
                vb += dirs[5];
                vb += dirs[0];

                calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26], vc, nc);
                vc += transformed_cube_position;
                vc += dirs[0];

                calculateVertex(_density[10], _density[11], _density[13], _density[14], _density[19], _density[20], _density[22], _density[23], vd, nd);
                vd += transformed_cube_position;

                ab = vb - va;
                ac = vc - va;
                na = ac % ab;
                na.normalize();

                a = _meshBuilder.addVertex(va);
                _meshBuilder.setNormal(a, na);
                b = _meshBuilder.addVertex(vb);
                _meshBuilder.setNormal(b, na);
                c = _meshBuilder.addVertex(vc);
                _meshBuilder.setNormal(c, na);
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
                na = ad % ab;
                na.normalize();

                a = _meshBuilder.addVertex(va);
                _meshBuilder.setNormal(a, na);
                b = _meshBuilder.addVertex(vc);
                _meshBuilder.setNormal(b, na);
                c = _meshBuilder.addVertex(vd);
                _meshBuilder.setNormal(c, na);
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

                calculateVertex(_density[4], _density[5], _density[7], _density[8], _density[13], _density[14], _density[16], _density[17], va, na);
                va += transformed_cube_position;
                va += dirs[5];
                va += dirs[0];

                calculateVertex(_density[3], _density[4], _density[6], _density[7], _density[12], _density[13], _density[15], _density[16], vb, nb);
                vb += transformed_cube_position;
                vb += dirs[5];
                vb += dirs[3];
                vb += dirs[0];

                calculateVertex(_density[12], _density[13], _density[15], _density[16], _density[21], _density[22], _density[24], _density[25], vc, nc);
                vc += transformed_cube_position;
                vc += dirs[3];
                vc += dirs[0];

                calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26], vd, nd);
                vd += transformed_cube_position;
                vd += dirs[0];

                ab = vb - va;
                ac = vc - va;
                na = ac % ab;
                na.normalize();

                a = _meshBuilder.addVertex(va);
                _meshBuilder.setNormal(a, na);
                b = _meshBuilder.addVertex(vb);
                _meshBuilder.setNormal(b, na);
                c = _meshBuilder.addVertex(vc);
                _meshBuilder.setNormal(c, na);
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
                na = ad % ab;
                na.normalize();

                a = _meshBuilder.addVertex(va);
                _meshBuilder.setNormal(a, na);
                b = _meshBuilder.addVertex(vc);
                _meshBuilder.setNormal(b, na);
                c = _meshBuilder.addVertex(vd);
                _meshBuilder.setNormal(c, na);
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

                
                calculateVertex(_density[10], _density[11], _density[13], _density[14], _density[19], _density[20], _density[22], _density[23], va, na);
                va += transformed_cube_position;

                calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26], vb, nb);
                vb += transformed_cube_position;
                vb += dirs[0];

                calculateVertex(_density[12], _density[13], _density[15], _density[16], _density[21], _density[22], _density[24], _density[25], vc, nc);
                vc += transformed_cube_position;
                vc += dirs[3];
                vc += dirs[0];

                calculateVertex(_density[9], _density[10], _density[12], _density[13], _density[18], _density[19], _density[21], _density[22], vd, nd);
                vd += transformed_cube_position;
                vd += dirs[3];

                ab = vb - va;
                ac = vc - va;
                na = ac % ab;
                na.normalize();

                a = _meshBuilder.addVertex(va);
                _meshBuilder.setNormal(a, na);
                b = _meshBuilder.addVertex(vb);
                _meshBuilder.setNormal(b, na);
                c = _meshBuilder.addVertex(vc);
                _meshBuilder.setNormal(c, na);
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
                na = ad % ab;
                na.normalize();

                a = _meshBuilder.addVertex(va);
                _meshBuilder.setNormal(a, na);
                b = _meshBuilder.addVertex(vc);
                _meshBuilder.setNormal(b, na);
                c = _meshBuilder.addVertex(vd);
                _meshBuilder.setNormal(c, na);
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

                calculateVertex(_density[1], _density[2], _density[4], _density[5], _density[10], _density[11], _density[13], _density[14], va, na);
                va += transformed_cube_position;
                va += dirs[5];

                calculateVertex(_density[4], _density[5], _density[7], _density[8], _density[13], _density[14], _density[16], _density[17], vb, nb);
                vb += transformed_cube_position;
                vb += dirs[0];
                vb += dirs[5];

                calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26], vc, nc);
                vc += transformed_cube_position;
                vc += dirs[0];

                
                calculateVertex(_density[10], _density[11], _density[13], _density[14], _density[19], _density[20], _density[22], _density[23], vd, nd);
                vd += transformed_cube_position;

                ab = vb - va;
                ac = vc - va;
                na = ab % ac;
                na.normalize();

                a = _meshBuilder.addVertex(va);
                _meshBuilder.setNormal(a, na);
                b = _meshBuilder.addVertex(vb);
                _meshBuilder.setNormal(b, na);
                c = _meshBuilder.addVertex(vc);
                _meshBuilder.setNormal(c, na);
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
                na = ab % ad;
                na.normalize();

                a = _meshBuilder.addVertex(va);
                _meshBuilder.setNormal(a, na);
                b = _meshBuilder.addVertex(vc);
                _meshBuilder.setNormal(b, na);
                c = _meshBuilder.addVertex(vd);
                _meshBuilder.setNormal(c, na);
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

                calculateVertex(_density[4], _density[5], _density[7], _density[8], _density[13], _density[14], _density[16], _density[17], va, na);
                va += transformed_cube_position;
                va += dirs[5];
                va += dirs[0];

                calculateVertex(_density[3], _density[4], _density[6], _density[7], _density[12], _density[13], _density[15], _density[16], vb, nb);
                vb += transformed_cube_position;
                vb += dirs[5];
                vb += dirs[3];
                vb += dirs[0];

                calculateVertex(_density[12], _density[13], _density[15], _density[16], _density[21], _density[22], _density[24], _density[25], vc, nc);
                vc += transformed_cube_position;
                vc += dirs[3];
                vc += dirs[0];

                calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26], vd, nd);
                vd += transformed_cube_position;
                vd += dirs[0];

                ab = vb - va;
                ac = vc - va;
                na = ab % ac;
                na.normalize();

                a = _meshBuilder.addVertex(va);
                _meshBuilder.setNormal(a, na);
                b = _meshBuilder.addVertex(vb);
                _meshBuilder.setNormal(b, na);
                c = _meshBuilder.addVertex(vc);
                _meshBuilder.setNormal(c, na);
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
                na = ab % ad;
                na.normalize();

                a = _meshBuilder.addVertex(va);
                _meshBuilder.setNormal(a, na);
                b = _meshBuilder.addVertex(vc);
                _meshBuilder.setNormal(b, na);
                c = _meshBuilder.addVertex(vd);
                _meshBuilder.setNormal(c, na);
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

                
                calculateVertex(_density[10], _density[11], _density[13], _density[14], _density[19], _density[20], _density[22], _density[23], va, na);
                va += transformed_cube_position;

                
                calculateVertex(_density[13], _density[14], _density[16], _density[17], _density[22], _density[23], _density[25], _density[26], vb, nb);
                vb += transformed_cube_position;
                vb += dirs[0];

                calculateVertex(_density[12], _density[13], _density[15], _density[16], _density[21], _density[22], _density[24], _density[25], vc, nc);
                vc += transformed_cube_position;
                vc += dirs[3];
                vc += dirs[0];

                calculateVertex(_density[9], _density[10], _density[12], _density[13], _density[18], _density[19], _density[21], _density[22], vd, nd);
                vd += transformed_cube_position;
                vd += dirs[3];

                ab = vb - va;
                ac = vc - va;
                na = ab % ac;
                na.normalize();

                a = _meshBuilder.addVertex(va);
                _meshBuilder.setNormal(a, na);
                b = _meshBuilder.addVertex(vb);
                _meshBuilder.setNormal(b, na);
                c = _meshBuilder.addVertex(vc);
                _meshBuilder.setNormal(c, na);
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
                na = ab % ad;
                na.normalize();

                a = _meshBuilder.addVertex(va);
                _meshBuilder.setNormal(a, na);
                b = _meshBuilder.addVertex(vc);
                _meshBuilder.setNormal(b, na);
                c = _meshBuilder.addVertex(vd);
                _meshBuilder.setNormal(c, na);
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
            _density[i + 18] = static_cast<float32>(_currentPoles[i]._currentDensityBlock->_value);

            if (_currentPoles[i]._currentDensityBlock->_length + _currentPoles[i]._currentDensityBlockPosition - 1 <= _cubePosition._y)
            {
                _currentPoles[i]._currentDensityBlockPosition += _currentPoles[i]._currentDensityBlock->_length;
                _currentPoles[i]._currentDensityBlock++;
            }

            _material[i + 0] = _material[i + 9];
            _material[i + 9] = _material[i + 18];
            _material[i + 18] = static_cast<float32>(_currentPoles[i]._currentMaterialBlock->_value);

            if (_currentPoles[i]._currentMaterialBlock->_length + _currentPoles[i]._currentMaterialBlockPosition - 1 <= _cubePosition._y)
            {
                _currentPoles[i]._currentMaterialBlockPosition += _currentPoles[i]._currentMaterialBlock->_length;
                _currentPoles[i]._currentMaterialBlock++;
            }
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

    void iContouringCubes::getPoles(iaVector3I &startPosition)
    {
        _cubePosition = startPosition;

        _currentPoles[0]._currentDensityBlock = _voxelData->getDensityBlocks(_cubePosition._x, _cubePosition._z).getBlocks().begin();
        _currentPoles[1]._currentDensityBlock = _voxelData->getDensityBlocks(_cubePosition._x + 1, _cubePosition._z).getBlocks().begin();
        _currentPoles[2]._currentDensityBlock = _voxelData->getDensityBlocks(_cubePosition._x + 2, _cubePosition._z).getBlocks().begin();

        _currentPoles[3]._currentDensityBlock = _voxelData->getDensityBlocks(_cubePosition._x, _cubePosition._z + 1).getBlocks().begin();
        _currentPoles[4]._currentDensityBlock = _voxelData->getDensityBlocks(_cubePosition._x + 1, _cubePosition._z + 1).getBlocks().begin();
        _currentPoles[5]._currentDensityBlock = _voxelData->getDensityBlocks(_cubePosition._x + 2, _cubePosition._z + 1).getBlocks().begin();

        _currentPoles[6]._currentDensityBlock = _voxelData->getDensityBlocks(_cubePosition._x, _cubePosition._z + 2).getBlocks().begin();
        _currentPoles[7]._currentDensityBlock = _voxelData->getDensityBlocks(_cubePosition._x + 1, _cubePosition._z + 2).getBlocks().begin();
        _currentPoles[8]._currentDensityBlock = _voxelData->getDensityBlocks(_cubePosition._x + 2, _cubePosition._z + 2).getBlocks().begin();

        _currentPoles[0]._currentMaterialBlock = _voxelData->getMaterialBlocks(_cubePosition._x, _cubePosition._z).getBlocks().begin();
        _currentPoles[1]._currentMaterialBlock = _voxelData->getMaterialBlocks(_cubePosition._x + 1, _cubePosition._z).getBlocks().begin();
        _currentPoles[2]._currentMaterialBlock = _voxelData->getMaterialBlocks(_cubePosition._x + 2, _cubePosition._z).getBlocks().begin();

        _currentPoles[3]._currentMaterialBlock = _voxelData->getMaterialBlocks(_cubePosition._x, _cubePosition._z + 1).getBlocks().begin();
        _currentPoles[4]._currentMaterialBlock = _voxelData->getMaterialBlocks(_cubePosition._x + 1, _cubePosition._z + 1).getBlocks().begin();
        _currentPoles[5]._currentMaterialBlock = _voxelData->getMaterialBlocks(_cubePosition._x + 2, _cubePosition._z + 1).getBlocks().begin();

        _currentPoles[6]._currentMaterialBlock = _voxelData->getMaterialBlocks(_cubePosition._x, _cubePosition._z + 2).getBlocks().begin();
        _currentPoles[7]._currentMaterialBlock = _voxelData->getMaterialBlocks(_cubePosition._x + 1, _cubePosition._z + 2).getBlocks().begin();
        _currentPoles[8]._currentMaterialBlock = _voxelData->getMaterialBlocks(_cubePosition._x + 2, _cubePosition._z + 2).getBlocks().begin();

        auto poleIter = _currentPoles.begin();
        while (poleIter != _currentPoles.end())
        {
            uint64 nextBlockPosition = 0;

            do
            {
                (*poleIter)._currentDensityBlockPosition = nextBlockPosition;
                nextBlockPosition += (*(*poleIter)._currentDensityBlock)._length;
                (*poleIter)._currentDensityBlock++;
            } while (nextBlockPosition <= _cubePosition._y);
            (*poleIter)._currentDensityBlock--;

            nextBlockPosition = 0;

            do
            {
                (*poleIter)._currentMaterialBlockPosition = nextBlockPosition;
                nextBlockPosition += (*(*poleIter)._currentMaterialBlock)._length;
                (*poleIter)._currentMaterialBlock++;
            } while (nextBlockPosition <= _cubePosition._y);
            (*poleIter)._currentMaterialBlock--;

            poleIter++;
        }

        for (int i = 0; i < 27; ++i)
        {
            _density[i] = 0;
            _material[i] = 0;
        }
    }

    void iContouringCubes::setVoxelData(iVoxelData* voxelData)
    {
        _voxelData = voxelData;
    }

    shared_ptr<iMesh> iContouringCubes::compile(iaVector3I pos, iaVector3I volume)
    {
        shared_ptr<iMesh> result;
        con_assert(_voxelData != nullptr, "no voxel data defined");

        if (pos._x < 0 ||
            pos._y < 0 ||
            pos._z < 0)
        {
            con_err("out of range");
            return result;
        }

        iaVector3I max = pos;
        max += volume;
        if (max._x >= _voxelData->getWidth() ||
            max._y >= _voxelData->getHeight() ||
            max._z >= _voxelData->getDepth())
        {
            con_err("out of range");
            return result;
        }

        _meshBuilder.clear();

        iaVector3I marchingVolume;

        marchingVolume._x = volume._x - 2;
        marchingVolume._y = volume._y;
        marchingVolume._z = volume._z - 2;

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
                getPoles(currentPosition);
                climb();
                climb();

                int y = 0;

                do
                {
                    climb();

                    if (x > 0 &&
                        x < marchingVolume._x - 1 &&
                        y > 0 &&
                        y < marchingVolume._y - 3 &&
                        z > 0 &&
                        z < marchingVolume._z - 1)
                    {
                        keepTriangles = true;
                    }
                    else
                    {
                        keepTriangles = false;
                    }

                    generateGeometry(keepTriangles);

                    y++;
                } while (!(_cubePosition._y >= marchingVolume._y + _cubeStartPosition._y));
            }
        }

        if (_meshBuilder.getTrianglesCount() != 0)
        {
            //_meshBuilder.calcNormals(false);

            for (auto iter : _trianglesToKeep)
            {
                result = _meshBuilder.createMesh(iter.second);
                break;
            }
        }

        return result;
    }
}
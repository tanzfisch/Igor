// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "iSimpleCubes.h"

namespace Igor
{

    iaVector3f corners[] = {
        iaVector3f(0.0, 0.0, 0.0),   // 0
        iaVector3f(0.5, 0.0, 0.0),   // 1
        iaVector3f(1.0, 0.0, 0.0),   // 2
        iaVector3f(0.0, 0.5, 0.0),   // 3
        iaVector3f(0.5, 0.5, 0.0),   // 4
        iaVector3f(1.0, 0.5, 0.0),   // 5
        iaVector3f(0.0, 1.0, 0.0),   // 6
        iaVector3f(0.5, 1.0, 0.0),   // 7
        iaVector3f(1.0, 1.0, 0.0),   // 8
        iaVector3f(0.0, 0.0, -0.5),  // 9
        iaVector3f(0.5, 0.0, -0.5),  // 10
        iaVector3f(1.0, 0.0, -0.5),  // 11
        iaVector3f(0.0, 0.5, -0.5),  // 12
        iaVector3f(0.5, 0.5, -0.5),  // 13
        iaVector3f(1.0, 0.5, -0.5),  // 14
        iaVector3f(0.0, 1.0, -0.5),  // 15
        iaVector3f(0.5, 1.0, -0.5),  // 16
        iaVector3f(1.0, 1.0, -0.5),  // 17
        iaVector3f(0.0, 0.0, -1.0),  // 18
        iaVector3f(0.5, 0.0, -1.0),  // 19
        iaVector3f(1.0, 0.0, -1.0),  // 20
        iaVector3f(0.0, 0.5, -1.0),  // 21
        iaVector3f(0.5, 0.5, -1.0),  // 22
        iaVector3f(1.0, 0.5, -1.0),  // 23
        iaVector3f(0.0, 1.0, -1.0),  // 24
        iaVector3f(0.5, 1.0, -1.0),  // 25
        iaVector3f(1.0, 1.0, -1.0)}; // 26

    iSimpleCubes::iSimpleCubes()
        : _voxelData(0)
    {
    }

    /*

    //       24-----25-----26
    //      /|     /|     /|
    //     15-----16-----17|
    //    /| |   /| |   /| |
    //   6------7------8 | |
    //   | | 21-|-|-22-|-|-23
    //   | |/|  | |/|  | |/|
    //   | 12---|-13---|-14|      Z   Y
    //   |/| |  |/| |  |/| |      |  /
    //   3------4------5 | |      | /
    //   | | 18-|-|-19-|-|-20     |/
    //   | |/   | |/   | |/       0---X
    //   | 9----|-10---|-11
    //   |/     |/     |/
    //   0------1------2

    //       6-----------7
    //      /|          /|
    //     / |         / |     Z   Y
    //    /  |        /  |     |  /
    //   4-----------5   |     | /
    //   |   2-------|---3     |/
    //   |  /        |  /      0---X
    //   | /         | /
    //   |/          |/
    //   0-----------1
    //
    //       64---------128
    //      /|          /|
    //     / |         / |     Z   Y
    //    /  |        /  |     |  /
    //   16----------32  |     | /
    //   |   4-------|---8     |/
    //   |  /        |  /      0---X
    //   | /         | /
    //   |/          |/
    //   1-----------2

    */

    void iSimpleCubes::generateGeometry(iMeshBuilder &meshBuilder, uint8 edges)
    {
        // filter if completely inside or outside of volume
        if (edges == 0 || edges == 255)
        {
            return;
        }

        static iaVector3f va;
        static iaVector3f vb;
        static iaVector3f vc;
        static iaVector3f vd;
        static uint32 a, b, c;

        iaVector3f transformed_cube_position(_cubePosition._x, _cubePosition._z, -_cubePosition._y);

        if (edges & 1)
        {
            if (!(edges & 2))
            {
                va = transformed_cube_position;
                vb = transformed_cube_position;
                vc = transformed_cube_position;
                vd = transformed_cube_position;

                va += corners[2];
                vb += corners[20];
                vc += corners[26];
                vd += corners[8];

                a = meshBuilder.addVertex(va);
                b = meshBuilder.addVertex(vb);
                c = meshBuilder.addVertex(vc);
                meshBuilder.addTriangle(a, b, c);

                a = meshBuilder.addVertex(va);
                b = meshBuilder.addVertex(vc);
                c = meshBuilder.addVertex(vd);
                meshBuilder.addTriangle(a, b, c);
            }

            if (!(edges & 4))
            {
                va = transformed_cube_position;
                vb = transformed_cube_position;
                vc = transformed_cube_position;
                vd = transformed_cube_position;

                va += corners[20];
                vb += corners[18];
                vc += corners[24];
                vd += corners[26];

                a = meshBuilder.addVertex(va);
                b = meshBuilder.addVertex(vb);
                c = meshBuilder.addVertex(vc);
                meshBuilder.addTriangle(a, b, c);

                a = meshBuilder.addVertex(va);
                b = meshBuilder.addVertex(vc);
                c = meshBuilder.addVertex(vd);
                meshBuilder.addTriangle(a, b, c);
            }

            if (!(edges & 16))
            {
                va = transformed_cube_position;
                vb = transformed_cube_position;
                vc = transformed_cube_position;
                vd = transformed_cube_position;

                va += corners[6];
                vb += corners[8];
                vc += corners[26];
                vd += corners[24];

                a = meshBuilder.addVertex(va);
                b = meshBuilder.addVertex(vb);
                c = meshBuilder.addVertex(vc);
                meshBuilder.addTriangle(a, b, c);

                a = meshBuilder.addVertex(va);
                b = meshBuilder.addVertex(vc);
                c = meshBuilder.addVertex(vd);
                meshBuilder.addTriangle(a, b, c);
            }
        }
        else
        {
            if (edges & 2)
            {
                va = transformed_cube_position;
                vb = transformed_cube_position;
                vc = transformed_cube_position;
                vd = transformed_cube_position;

                va += corners[2];
                vb += corners[20];
                vc += corners[26];
                vd += corners[8];

                a = meshBuilder.addVertex(va);
                b = meshBuilder.addVertex(vb);
                c = meshBuilder.addVertex(vc);
                meshBuilder.addTriangle(a, c, b);

                a = meshBuilder.addVertex(va);
                b = meshBuilder.addVertex(vc);
                c = meshBuilder.addVertex(vd);
                meshBuilder.addTriangle(a, c, b);
            }

            if (edges & 4)
            {
                va = transformed_cube_position;
                vb = transformed_cube_position;
                vc = transformed_cube_position;
                vd = transformed_cube_position;

                va += corners[24];
                vb += corners[18];
                vc += corners[20];
                vd += corners[26];

                a = meshBuilder.addVertex(va);
                b = meshBuilder.addVertex(vb);
                c = meshBuilder.addVertex(vc);
                meshBuilder.addTriangle(a, b, c);

                a = meshBuilder.addVertex(va);
                b = meshBuilder.addVertex(vc);
                c = meshBuilder.addVertex(vd);
                meshBuilder.addTriangle(a, b, c);
            }

            if (edges & 16)
            {
                va = transformed_cube_position;
                vb = transformed_cube_position;
                vc = transformed_cube_position;
                vd = transformed_cube_position;

                va += corners[24];
                vb += corners[26];
                vc += corners[8];
                vd += corners[6];

                a = meshBuilder.addVertex(va);
                b = meshBuilder.addVertex(vb);
                c = meshBuilder.addVertex(vc);
                meshBuilder.addTriangle(a, b, c);

                a = meshBuilder.addVertex(va);
                b = meshBuilder.addVertex(vc);
                c = meshBuilder.addVertex(vd);
                meshBuilder.addTriangle(a, b, c);
            }
        }
    }

    /*!
    //       64---------128
    //      /|          /|
    //     / |         / |     Z   Y
    //    /  |        /  |     |  /
    //   16----------32  |     | /
    //   |   4-------|---8     |/
    //   |  /        |  /      0---X
    //   | /         | /
    //   |/          |/
    //   1-----------2
        
    //       ^           ^
    //       |           |
    //       |           |     Z   Y
    //       |           |     |  /
    //   ^   |       ^   |     | /
    //   |   d-------|---c     |/
    //   |  /        |  /      0---X
    //   | /         | /
    //   |/          |/
    //   a-----------b

    */

    uint8 iSimpleCubes::getNextCube()
    {
        uint8 result = 0;

        _a1Density = (*_ait)._density;
        _b1Density = (*_bit)._density;
        _c1Density = (*_cit)._density;
        _d1Density = (*_dit)._density;

        if (0 != _a1Density)
        {
            result |= 1;
        }

        if (0 != _b1Density)
        {
            result |= 2;
        }

        if (0 != _c1Density)
        {
            result |= 8;
        }

        if (0 != _d1Density)
        {
            result |= 4;
        }

        // climb one up
        if ((*_ait)._length + _aitPos - 1 <= _cubePosition._z)
        {
            _aitPos += (*_ait)._length;
            _ait++;
        }

        if ((*_bit)._length + _bitPos - 1 <= _cubePosition._z)
        {
            _bitPos += (*_bit)._length;
            _bit++;
        }

        if ((*_cit)._length + _citPos - 1 <= _cubePosition._z)
        {
            _citPos += (*_cit)._length;
            _cit++;
        }

        if ((*_dit)._length + _ditPos - 1 <= _cubePosition._z)
        {
            _ditPos += (*_dit)._length;
            _dit++;
        }

        _a2Density = (*_ait)._density;
        _b2Density = (*_bit)._density;
        _c2Density = (*_cit)._density;
        _d2Density = (*_dit)._density;

        if (0 != _a2Density)
        {
            result |= 16;
        }

        if (0 != _b2Density)
        {
            result |= 32;
        }

        if (0 != _c2Density)
        {
            result |= 128;
        }

        if (0 != _d2Density)
        {
            result |= 64;
        }

        _cubePosition._z++;

        return result;
    }

    /*
    //       ^           ^
    //       |           |
    //       |           |     Z   Y
    //       |           |     |  /
    //   ^   |       ^   |     | /
    //   |   d-------|---c     |/
    //   |  /        |  /      0---X
    //   | /         | /
    //   |/          |/
    //   a-----------b

    */

    void iSimpleCubes::startGettingCubes(iaVector3I &start_position)
    {
        _cubePosition = start_position;

        std::vector<iVoxelBlock> *a = _voxelData->getBlocks(_cubePosition._x, _cubePosition._y);
        std::vector<iVoxelBlock> *b = _voxelData->getBlocks(_cubePosition._x + 1, _cubePosition._y);
        std::vector<iVoxelBlock> *c = _voxelData->getBlocks(_cubePosition._x + 1, _cubePosition._y + 1);
        std::vector<iVoxelBlock> *d = _voxelData->getBlocks(_cubePosition._x, _cubePosition._y + 1);

        _aitPos = 0;
        _ait = a->begin();
        while (_aitPos <= _cubePosition._z)
        {
            _aitPos += (*_ait)._length;
            _ait++;
        }
        _ait--;
        _aitPos -= (*_ait)._length;

        _bitPos = 0;
        _bit = b->begin();
        while (_bitPos <= _cubePosition._z)
        {
            _bitPos += (*_bit)._length;
            _bit++;
        }
        _bit--;
        _bitPos -= (*_bit)._length;

        _citPos = 0;
        _cit = c->begin();
        while (_citPos <= _cubePosition._z)
        {
            _citPos += (*_cit)._length;
            _cit++;
        }
        _cit--;
        _citPos -= (*_cit)._length;

        _ditPos = 0;
        _dit = d->begin();
        while (_ditPos <= _cubePosition._z)
        {
            _ditPos += (*_dit)._length;
            _dit++;
        }
        _dit--;
        _ditPos -= (*_dit)._length;
    }

    void iSimpleCubes::setVoxelData(iVoxelData *voxelData)
    {
        _voxelData = voxelData;
    }

    void iSimpleCubes::compile(iMeshBuilder &meshBuilder, iaVector3I pos, iaVector3I volume)
    {
        if (pos._x < 0 ||
            pos._y < 0 ||
            pos._z < 0)
        {
            con_err("out of range");
            return;
        }

        iaVector3I max = pos;
        max += volume;
        if (max._x >= _voxelData->getWidth() ||
            max._y >= _voxelData->getDepth() ||
            max._z >= _voxelData->getHeight())
        {
            con_err("out of range");
            return;
        }

        _marchingVolume = volume;

        _cubeStartPosition.set(pos._x, pos._y, pos._z);

        iaVector3I currentPosition;
        currentPosition._z = _cubeStartPosition._z;

        // run through columns
        for (int x = 0; x < _marchingVolume._x - 1; ++x)
        {
            currentPosition._x = _cubeStartPosition._x + x;

            // run through rows
            for (int y = 0; y < _marchingVolume._y - 1; ++y)
            {
                currentPosition._y = _cubeStartPosition._y + y;

                // process pole
                startGettingCubes(currentPosition);
                do
                {
                    generateGeometry(meshBuilder, getNextCube());
                } while (!(_cubePosition._z >= _marchingVolume._z + _cubeStartPosition._z - 1));
            }
        }

        if (meshBuilder.getTrianglesCount() != 0)
        {
            meshBuilder.calcNormals(false);
        }
    }
} // namespace Igor
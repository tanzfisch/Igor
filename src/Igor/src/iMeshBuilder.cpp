// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iMeshBuilder.h>

#include <iMesh.h>
#include <iRenderer.h>
#include <iaString.h>
using namespace IgorAux;

#include <list>
#include <sstream>
using namespace std;

namespace Igor
{
    iMeshBuilder::iMeshBuilder()
    {
        clear();
    }

    iMeshBuilder::~iMeshBuilder()
    {
    }

    void iMeshBuilder::clear()
    {
        _vertexes.clear();
        _normals.clear();
        _colors.clear();
        _texCoords.clear();
        _triangles.clear();
        _indexMap.clear();
    }

    const vector<iaVector3f>& iMeshBuilder::getVertexes() const
    {
        return _vertexes;
    }

    const vector<iaVector3f>& iMeshBuilder::getNormals() const
    {
        return _normals;
    }

    const vector<iIndexedTriangle>& iMeshBuilder::getTriangles() const
    {
        return _triangles;
    }

    void iMeshBuilder::setGridSize(float32 gridSize)
    {
        _gridSize = gridSize;
    }

    float32 iMeshBuilder::getGridSize()
    {
        return _gridSize;
    }

    void iMeshBuilder::setJoinVertexes(bool joinVertexes)
    {
        con_assert(_vertexes.empty(), "can't change this setting unless no vertexes present");

        if (_vertexes.empty())
        {
            _joinVertexes = joinVertexes;
        }
    }

    bool iMeshBuilder::getJoinVertexes()
    {
        return _joinVertexes;
    }

    uint32 iMeshBuilder::addVertexSnapped(const iaVector3f& vertex)
    {
        iaVector3f snappedVertex(floor((vertex._x / _gridSize) + 0.5f) * _gridSize, 
            floor((vertex._y / _gridSize) + 0.5f) * _gridSize, 
            floor((vertex._z / _gridSize) + 0.5f) * _gridSize);
        return addVertex(vertex);
    }

    uint32 iMeshBuilder::addVertex(const iaVector3f& vertex)
    {
        uint32 result = 0;

        if (_joinVertexes)
        {
            auto iter = _indexMap.find(vertex);
            if (iter != _indexMap.end())
            {
                result = (*iter).second;
            }
            else
            {
                _vertexes.push_back(vertex);
                result = _vertexes.size() - 1;
                _indexMap[vertex] = result;
            }
        }
        else
        {
            _vertexes.push_back(vertex);
            result = _vertexes.size() - 1;
        }

        return result;
    }

    void iMeshBuilder::setNormal(uint32 index, const iaVector3f& normal)
    {
        while(_vertexes.size() > _normals.size())
        {
            _normals.push_back(normal);
        }

        _normals[index]._x = normal._x;
        _normals[index]._y = normal._y;
        _normals[index]._z = normal._z;
    }

    void iMeshBuilder::setColor(uint32 index, const iaColor4f& color)
    {
        while (_vertexes.size() > _colors.size())
        {
            _colors.push_back(color);
        }

        _colors[index]._r = color._r;
        _colors[index]._g = color._g;
        _colors[index]._b = color._b;
        _colors[index]._a = color._a;
    }

    void iMeshBuilder::setTexCoord(uint32 index, const iaVector2f& texCoord, uint32 unit)
    {
        while (_vertexes.size() > _texCoords[unit].size())
        {
            _texCoords[unit].push_back(texCoord);
        }

        _texCoords[unit][index]._x = texCoord._x;
        _texCoords[unit][index]._y = texCoord._y;
    }

    uint32 iMeshBuilder::addTriangle(const uint32 indexA, const uint32 indexB, const uint32 indexC)
    {
        iIndexedTriangle triangle;
        triangle._a = indexA;
        triangle._b = indexB;
        triangle._c = indexC;
        _triangles.push_back(triangle);

        return _triangles.size() - 1;
    }

    bool iMeshBuilder::checkConsistency()
    {
        if (_triangles.size() == 0)
        {
            return false;
        }

        if (_vertexes.size() == 0)
        {
            return false;
        }

        if (_normals.size() != 0)
        {
            if (_vertexes.size() != _normals.size())
            {
                con_err("vertexes " << _vertexes.size() << " != normals " << _normals.size());
                return false;
            }
        }

        if (_colors.size() != 0)
        {
            if (_vertexes.size() != _colors.size())
            {
                con_err("vertexes " << _vertexes.size() << " != colors " << _colors.size());
                return false;
            }
        }

        
        for (int texunit = 0; texunit < _texCoords.size();++texunit)
        {
            if (_vertexes.size() != _texCoords[texunit].size())
            {
                con_err("vertexes " << _vertexes.size() << " != texcoord " << _texCoords[texunit].size() << " texunit " << texunit);
                return false;
            }
        }

        auto iterIndex = _triangles.begin();
        while (iterIndex != _triangles.end())
        {
            if ((*iterIndex)._a > _vertexes.size() ||
                (*iterIndex)._b > _vertexes.size() ||
                (*iterIndex)._c > _vertexes.size())
            {
                con_err("vertexes out of range");
                return false;
            }

            iterIndex++;
        }

        return true;
    }

    uint32 iMeshBuilder::getTextureUnitCount() const
    {
        return static_cast<uint32>(_texCoords.size());
    }

    bool iMeshBuilder::hasNormals() const
    {
        return _normals.size() > 0 ? true : false;
    }

    bool iMeshBuilder::hasColors() const
    {
        return _colors.size() > 0 ? true : false;
    }

    uint32 iMeshBuilder::getVertexCount() const
    {
        return static_cast<unsigned int>(_vertexes.size());
    }

    uint32 iMeshBuilder::getTrianglesCount() const
    {
        return static_cast<unsigned int>(_triangles.size());
    }

    void iMeshBuilder::copyTriangles(vector<uint32> srcTriangleIndexes, iMeshBuilder& dst)
    {
        uint32 indexA = 0;
        uint32 indexB = 0;
        uint32 indexC = 0;

        uint32 a = 0;
        uint32 b = 0;
        uint32 c = 0;

        for (uint32 triangleIndex : srcTriangleIndexes)
        {
            indexA = _triangles[triangleIndex]._a;
            indexB = _triangles[triangleIndex]._b;
            indexC = _triangles[triangleIndex]._c;

            a = dst.addVertex(_vertexes[indexA]);
            b = dst.addVertex(_vertexes[indexB]);
            c = dst.addVertex(_vertexes[indexC]);

            if (_normals.size() > 0)
            {
                dst.setNormal(a, _normals[indexA]);
                dst.setNormal(b, _normals[indexB]);
                dst.setNormal(c, _normals[indexC]);
            }

            if (_colors.size() > 0)
            {
                dst.setColor(a, _colors[indexA]);
                dst.setColor(b, _colors[indexB]);
                dst.setColor(c, _colors[indexC]);
            }

            if (_texCoords.size() > 0)
            {
                for (auto iter : _texCoords)
                {
                    dst.setTexCoord(a, iter.second[indexA], iter.first);
                    dst.setTexCoord(b, iter.second[indexB], iter.first);
                    dst.setTexCoord(c, iter.second[indexC], iter.first);
                }
            }

            dst.addTriangle(a, b, c);
        }
    }

    void iMeshBuilder::calcBoundingSphere(iSpheref& sphere) const
    {
        sphere._radius = 0;

        sphere._center.set(0, 0, 0);

        for (uint32 i = 0; i < _vertexes.size(); i++) //! \todo check for bouncing bubble algorithm
        {
            sphere._center += _vertexes[i];
        }

        sphere._center /= static_cast<float32>(_vertexes.size());

        for (uint32 i = 0; i<_vertexes.size(); i++)
        {
            if ((sphere._center - _vertexes[i]).length() > sphere._radius)
            {
                sphere._radius = (sphere._center - _vertexes[i]).length();
            }
        }
    }

    shared_ptr<iMesh> iMeshBuilder::createMesh(vector<uint32> triangles)
    {
        iMesh* mesh = new iMesh();

        compile(mesh, triangles);

        iSpheref boundingSphere;
        calcBoundingSphere(boundingSphere);
        mesh->setBoundingSphere(boundingSphere);

        return shared_ptr<iMesh>(mesh);
    }

    shared_ptr<iMesh> iMeshBuilder::createMesh()
    {
        iMesh* mesh = new iMesh();

        compile(mesh);

        iSpheref boundingSphere;
        calcBoundingSphere(boundingSphere);
        mesh->setBoundingSphere(boundingSphere);

        return shared_ptr<iMesh>(mesh);
    }

    void iMeshBuilder::compile(iMesh* mesh, vector<uint32> triangles)
    {
        if (!checkConsistency())
        {
            return;
        }

        mesh->setTrianglesCount(static_cast<uint32>(triangles.size()));
        mesh->setIndexesCount(static_cast<uint32>(triangles.size() * 3));
        mesh->setIndexData(reinterpret_cast<uint32*>(new uint8[mesh->getIndexesCount() * sizeof(int32)]), mesh->getIndexesCount() * sizeof(int32));
        mesh->setVertexSize((3 + (hasNormals() ? 3 : 0) + (hasColors() ? 4 : 0) + (getTextureUnitCount() * 2)) * sizeof(float32));
        mesh->setHasNormals(hasNormals());
        mesh->setHasColors(hasColors());
        for (int i = 0; i < 4; ++i)
        {
            if (_texCoords.find(i) != _texCoords.end())
            {
                mesh->setTexture(i, true);
            }
        }

        uint32 tempBufferSize = (3 + (hasNormals() ? 3 : 0) + (hasColors() ? 4 : 0) + (getTextureUnitCount() * 2)) * _vertexes.size();
        float32* tempBuffer = new float32[tempBufferSize];

        map<uint32, uint32> indexMap;
        map<uint32, uint32>::iterator foundIndex;

        uint32 indexDataIndex = 0;
        uint32 vertexDataIndex = 0;

        uint32 oldIndex[3];
        uint32 oldVertexIndex = 0;
        uint32 newVertexIndex = 0;
        uint32 nextNewVertexIndex = 0;

        for (uint32 triangleID : triangles)
        {
            oldIndex[0] = _triangles[triangleID]._a;
            oldIndex[1] = _triangles[triangleID]._b;
            oldIndex[2] = _triangles[triangleID]._c;

            for (int i = 0; i < 3; ++i)
            {
                oldVertexIndex = oldIndex[i];

                foundIndex = indexMap.find(oldVertexIndex);
                if (foundIndex != indexMap.end())
                {
                    newVertexIndex = (*foundIndex).second;
                    mesh->_indexData[indexDataIndex++] = newVertexIndex;
                }
                else
                {
                    newVertexIndex = nextNewVertexIndex++;
                    indexMap[oldVertexIndex] = newVertexIndex;
                    mesh->_indexData[indexDataIndex++] = newVertexIndex;

                    tempBuffer[vertexDataIndex++] = _vertexes[oldVertexIndex]._x;
                    tempBuffer[vertexDataIndex++] = _vertexes[oldVertexIndex]._y;
                    tempBuffer[vertexDataIndex++] = _vertexes[oldVertexIndex]._z;

                    if (hasNormals())
                    {
                        tempBuffer[vertexDataIndex++] = _normals[oldVertexIndex]._x;
                        tempBuffer[vertexDataIndex++] = _normals[oldVertexIndex]._y;
                        tempBuffer[vertexDataIndex++] = _normals[oldVertexIndex]._z;
                    }

                    if (hasColors())
                    {
                        tempBuffer[vertexDataIndex++] = _colors[oldVertexIndex]._r;
                        tempBuffer[vertexDataIndex++] = _colors[oldVertexIndex]._g;
                        tempBuffer[vertexDataIndex++] = _colors[oldVertexIndex]._b;
                        tempBuffer[vertexDataIndex++] = _colors[oldVertexIndex]._a;
                    }

                    for (uint32 texIndex = 0; texIndex < getTextureUnitCount(); ++texIndex)
                    {
                        tempBuffer[vertexDataIndex++] = _texCoords[texIndex][oldVertexIndex]._x;
                        tempBuffer[vertexDataIndex++] = _texCoords[texIndex][oldVertexIndex]._y;
                    }
                }
            }
        }

        mesh->setVertexCount(static_cast<uint32>(indexMap.size()));
        mesh->setVertexData(reinterpret_cast<float32*>(new uint8[mesh->getVertexSize() * mesh->getVertexCount()]), mesh->getVertexSize() * mesh->getVertexCount());

        memcpy(mesh->_vertexData, tempBuffer, mesh->getVertexSize() * mesh->getVertexCount());

        delete[] tempBuffer;
    }

    void iMeshBuilder::compile(iMesh* mesh)
    {
        if (!checkConsistency())
        {
            return;
        }

        mesh->setTrianglesCount(static_cast<uint32>(_triangles.size()));
        mesh->setIndexesCount(static_cast<uint32>(_triangles.size() * 3));
        mesh->setIndexData(reinterpret_cast<uint32*>(new uint8[mesh->getIndexesCount() * sizeof(int32)]), mesh->getIndexesCount() * sizeof(int32));

        uint32 index = 0;

        auto interTriangle = _triangles.begin();
        while (_triangles.end() != interTriangle)
        {
            mesh->_indexData[index++] = (*interTriangle)._a;
            mesh->_indexData[index++] = (*interTriangle)._b;
            mesh->_indexData[index++] = (*interTriangle)._c;
            ++interTriangle;
        }

        mesh->setVertexSize((3 + (hasNormals() ? 3 : 0) + (hasColors() ? 4 : 0) + (getTextureUnitCount() * 2)) * sizeof(float32));
        mesh->setVertexCount(static_cast<uint32>(_vertexes.size()));
        mesh->setVertexData(reinterpret_cast<float32*>(new uint8[mesh->getVertexSize() * mesh->getVertexCount()]), mesh->getVertexSize() * mesh->getVertexCount());

        for (int i = 0; i < 4; ++i)
        {
            if (_texCoords.find(i) != _texCoords.end())
            {
                mesh->setTexture(i, true);
            }
        }

        mesh->setHasNormals(hasNormals());
        mesh->setHasColors(hasColors());

        index = 0;
        uint32 vertex = 0;

        while (_vertexes.size() > vertex)
        {
            mesh->_vertexData[index++] = _vertexes[vertex]._x;
            mesh->_vertexData[index++] = _vertexes[vertex]._y;
            mesh->_vertexData[index++] = _vertexes[vertex]._z;

            if (hasNormals())
            {
                mesh->_vertexData[index++] = _normals[vertex]._x;
                mesh->_vertexData[index++] = _normals[vertex]._y;
                mesh->_vertexData[index++] = _normals[vertex]._z;
            }

            if (hasColors())
            {
                mesh->_vertexData[index++] = _colors[vertex]._r;
                mesh->_vertexData[index++] = _colors[vertex]._g;
                mesh->_vertexData[index++] = _colors[vertex]._b;
                mesh->_vertexData[index++] = _colors[vertex]._a;
            }

            for (uint32 texIndex = 0; texIndex < getTextureUnitCount(); ++texIndex)
            {
                mesh->_vertexData[index++] = _texCoords[texIndex][vertex]._x;
                mesh->_vertexData[index++] = _texCoords[texIndex][vertex]._y;
            }

            ++vertex;
        }
    }

    void iMeshBuilder::calcNormals(bool sharpEdges)
    {
        if (!checkConsistency())
        {
            return;
        }

        uint32 a, b, c;
        iaVector3f ab;
        iaVector3f ac;
        iaVector3f n;
        iaVector3f *trianglenormals = new iaVector3f[_triangles.size()];

        for (uint32 triangleIndex = 0; triangleIndex < _triangles.size(); ++triangleIndex)
        {
            a = _triangles[triangleIndex]._a;
            b = _triangles[triangleIndex]._b;
            c = _triangles[triangleIndex]._c;

            ac = _vertexes[c]; ac -= _vertexes[a];
            ab = _vertexes[b]; ab -= _vertexes[a];

            n = ab % ac;
            n.normalize();
            trianglenormals[triangleIndex] = n;
        }

        if (sharpEdges)
        {
            //        separateTriangles(trianglenormals);
        }

        if (_normals.size() != 0)
        {
            _normals.clear();
        }

        iaVector3f fill;
        for (uint32 i = 0; i < _vertexes.size(); i++)
        {
            _normals.push_back(fill);
        }

        for (uint32 triangleIndex = 0; triangleIndex < _triangles.size(); ++triangleIndex)
        {
            a = _triangles[triangleIndex]._a;
            b = _triangles[triangleIndex]._b;
            c = _triangles[triangleIndex]._c;

            _normals[a] += trianglenormals[triangleIndex];
            _normals[b] += trianglenormals[triangleIndex];
            _normals[c] += trianglenormals[triangleIndex];
        }

        for (uint32 i = 0; i < _normals.size(); i++)
        {
            _normals[i].normalize();
        }

        delete[] trianglenormals;
    }

    void iMeshBuilder::getIndexOfVertexes(const iaVector3f& vertexA, int64& indexA, const iaVector3f& vertexB, int64& indexB, const iaVector3f& vertexC, int64& indexC)
    {
        indexA = -1;
        indexB = -1;
        indexC = -1;

        for (int64 i = 0; i < static_cast<int64>(_vertexes.size()); ++i)
        {
            if (fabs(_vertexes[i]._x - vertexA._x) < 0.00001f &&
                fabs(_vertexes[i]._y - vertexA._y) < 0.00001f &&
                fabs(_vertexes[i]._z - vertexA._z) < 0.00001f)
            {
                indexA = i;
            }

            if (fabs(_vertexes[i]._x - vertexB._x) < 0.00001f &&
                fabs(_vertexes[i]._y - vertexB._y) < 0.00001f &&
                fabs(_vertexes[i]._z - vertexB._z) < 0.00001f)
            {
                indexB = i;
            }

            if (fabs(_vertexes[i]._x - vertexC._x) < 0.00001f &&
                fabs(_vertexes[i]._y - vertexC._y) < 0.00001f &&
                fabs(_vertexes[i]._z - vertexC._z) < 0.00001f)
            {
                indexC = i;
            }

            if (indexA != -1 &&
                indexB != -1 &&
                indexC != -1)
            {
                break;
            }
        }
    }

/*    void iMeshBuilder::joinVertexes()
    {
        vector<iaVector3f> vertices = std::move(_vertexes);
        _normals.clear();
        _colors.clear();

        auto iterTex = _texCoords.begin();
        while (iterTex != _texCoords.end())
        {
            (*iterTex).second.clear();
            iterTex++;
        }
        _texCoords.clear();

        vector<iIndexedTriangle> triangleIndexes = std::move(_triangles);

        uint32 indexA, indexB, indexC;
        int64 destinationIndexA, destinationIndexB, destinationIndexC;
        iaVector3f va, vb, vc;

        for (int srcindex = 0; srcindex < triangleIndexes.size(); ++srcindex)
        {
            indexA = triangleIndexes[srcindex]._a;
            indexB = triangleIndexes[srcindex]._b;
            indexC = triangleIndexes[srcindex]._c;

            va = vertices[indexA];
            vb = vertices[indexB];
            vc = vertices[indexC];

            getIndexOfVertexes(va, destinationIndexA, vb, destinationIndexB, vc, destinationIndexC);

            if (destinationIndexA == -1)
            {
                addVertex(va);
                destinationIndexA = _vertexes.size() - 1;
            }

            if (destinationIndexB == -1)
            {
                addVertex(vb);
                destinationIndexB = _vertexes.size() - 1;
            }

            if (destinationIndexC == -1)
            {
                addVertex(vc);
                destinationIndexC = _vertexes.size() - 1;
            }

            addTriangle(static_cast<uint32>(destinationIndexA), static_cast<uint32>(destinationIndexB), static_cast<uint32>(destinationIndexC));
        }
    }*/

    void iMeshBuilder::calcPlanarTextureCoordinates(const iaVector3f& center, iaAxis direction, int texunit, float32 scale)
    {
        con_assert(!_vertexes.empty(), "no vertices");

        if (checkConsistency())
        {
            _texCoords[texunit].clear();

            float32 u;
            float32 v;
            iaVector3f temp;

            switch (direction)
            {
            case iaAxis::X:
                for (int i = 0; i < _vertexes.size(); ++i)
                {
                    temp = _vertexes[i];
                    temp -= center;
                    setTexCoord(i, iaVector2f(-temp._z * scale, temp._y * scale), texunit);
                }
                break;

            case iaAxis::Y:
                for (int i = 0; i < _vertexes.size(); ++i)
                {
                    temp = _vertexes[i];
                    temp -= center;
                    setTexCoord(i, iaVector2f(temp._x * scale, -temp._z * scale), texunit);
                }
                break;

            case iaAxis::Z:
                for (int i = 0; i < _vertexes.size(); ++i)
                {
                    temp = _vertexes[i];
                    temp -= center;
                    setTexCoord(i, iaVector2f(temp._x * scale, temp._y * scale), texunit);
                }
                break;
            }
        }
    }

    __IGOR_INLINE__ void SphereMap(const iaVector3f& normal, float32 &u, float32 &v)
    {
        u = asin(normal._x) / M_PI + 0.5f;
        v = asin(normal._y) / M_PI + 0.5f;
    }

    void iMeshBuilder::calcSphericalTexcoordsByPosition(const iaVector3f& center, int texunit, float32 scale)
    {
        con_assert(!_vertexes.empty(), "no vertices");

        if (checkConsistency())
        {
            _texCoords[texunit].clear();

            float32 u;
            float32 v;
            iaVector3f temp;

            for (uint32 i = 0; i < _vertexes.size(); ++i)
            {
                temp = _vertexes[i];
                temp -= center;
                temp.normalize();
                SphereMap(temp, u, v);
                setTexCoord(i, iaVector2f(v * scale, v * scale), texunit);
            }
        }
    }

    void iMeshBuilder::calcSphericalTexcoordsByNormals(int texunit, float32 scale)
    {
        con_assert(hasNormals(), "has no normals");

        if (checkConsistency() &&
            hasNormals())
        {
            _texCoords[texunit].clear();

            float32 u;
            float32 v;

            for (uint32 i = 0; i < _normals.size(); ++i)
            {
                SphereMap(_normals[i], u, v);
                setTexCoord(i, iaVector2f(u * scale, v * scale), texunit);
            }
        }
    }

    /*!

    void iMeshBuilder::calcNeighbors()
    {
        if (!_dirtyNeighbors)
        {
            return;
        }

        _neighbors.clear();
        _edges.clear();

        int v1a, v2a, v1b, v2b;
        Edge temp_edge;
        Neighborhood temp;
        temp._neighbors[0] = -1;
        temp._neighbors[1] = -1;
        temp._neighbors[2] = -1;
        temp._edges[0] = -1;
        temp._edges[1] = -1;
        temp._edges[2] = -1;

        for (uint32 i = 0; i < _triangles.size(); ++i)
        {
            _neighbors.push_back(temp);
        }

        for (uint32 t = 0; t < _triangles.size(); ++t)
        {
            for (uint32 b = 0; b < 3; ++b)
            {
                bool borderDone = false;

                v1a = _triangles[t].index[b];
                v2a = _triangles[t].index[(b + 1) % 3];

                if (_neighbors[t]._neighbors[b] == -1)
                {
                    for (uint32 it = 0; it < _triangles.size(); ++it)
                    {
                        if (t == it)
                        {
                            continue;
                        }

                        for (uint32 ib = 0; ib < 3; ++ib)
                        {
                            v1b = _triangles[it].index[ib];
                            v2b = _triangles[it].index[(ib + 1) % 3];

                            if (((v1a == v1b) && (v2a == v2b)) || ((v1a == v2b) && (v2a == v1b)))
                            {
                                _neighbors[t]._neighbors[b] = it;
                                _neighbors[it]._neighbors[ib] = t;
                                _neighbors[t]._edges[b] = _edges.size();
                                _neighbors[it]._edges[ib] = _edges.size();
                                borderDone = true;

                                temp_edge._vertexes[0] = v1a;
                                temp_edge._vertexes[1] = v2a;
                                temp_edge._triangles[0] = t;
                                temp_edge._triangles[1] = it;
                                _edges.push_back(temp_edge);

                                break;
                            }
                        }

                        if (borderDone)
                        {
                            break;
                        }
                    }
                }
            }
        }

        // alle _edges mit nur einem angrenzenden dreieck separat berarbeiten
        for (uint32 t = 0; t < _neighbors.size(); ++t)
        {
            for (uint32 b = 0; b < 3; ++b)
            {
                if (_neighbors[t]._neighbors[b] == -1)
                {
                    temp_edge._vertexes[0] = _triangles[t].index[b];
                    temp_edge._vertexes[1] = _triangles[t].index[(b + 1) % 3];
                    temp_edge._triangles[0] = t;
                    temp_edge._triangles[1] = -1;

                    _neighbors[t]._edges[b] = _edges.size();

                    _edges.push_back(temp_edge);
                }
            }
        }

        _dirtyNeighbors = false;
    }
    */

    /*void iMeshBuilder::smooth()
    {
        calcNeighbors();

        int32 *vertex_neighbors_count = new int32[_vertexes.size()];
        memset(vertex_neighbors_count,0,_vertexes.size() * sizeof(int32));

        vector<iaVector3f> new_vertexes;
        for(uint32 i=0;i<_vertexes.size();++i)
        {
            new_vertexes.push_back(iaVector3f());
        }

        for(uint32 i=0;i<_triangles.size();++i)
        {
            //! \todo doppelte gewichte werden jetzt einfach mal akzeptiert ;-)
            new_vertexes[_triangles[i].index[0]] += _vertexes[_triangles[i].index[1]];
            new_vertexes[_triangles[i].index[0]] += _vertexes[_triangles[i].index[2]];
            new_vertexes[_triangles[i].index[1]] += _vertexes[_triangles[i].index[0]];
            new_vertexes[_triangles[i].index[1]] += _vertexes[_triangles[i].index[2]];
            new_vertexes[_triangles[i].index[2]] += _vertexes[_triangles[i].index[0]];
            new_vertexes[_triangles[i].index[2]] += _vertexes[_triangles[i].index[1]];

            vertex_neighbors_count[_triangles[i].index[0]]+=2;
            vertex_neighbors_count[_triangles[i].index[1]]+=2;
            vertex_neighbors_count[_triangles[i].index[2]]+=2;
        }

        for(uint32 i=0;i<_vertexes.size();++i)
        {
            if(vertex_neighbors_count[i] != 0)
            {
                new_vertexes[i] /= static_cast<float32>(vertex_neighbors_count[i]);
            }
        }

        delete[] vertex_neighbors_count;
        _vertexes.clear();
        _vertexes = new_vertexes;
    }*/

    /*	void iMeshBuilder::subdivide()
        {
            calcNeighbors();

            int32 old_vertex_count = _vertexes.size();
            int32 old_triangle_count = _triangles.size();
            iaVector3f v3;
            iaVector2f v2;
            iaColor4f color;

            for(uint32 e=0;e<_edges.size();++e)
            {
                if(_normals.size())
                {
                    v3 = _normals[_edges[e]._vertexes[0]];
                    v3 += _normals[_edges[e]._vertexes[1]];
                    v3.normalize();

                    setNormal3f(v3);
                }

                if(_colors.size())
                {
                    color = _colors[_edges[e]._vertexes[0]];
                    color += _colors[_edges[e]._vertexes[1]];
                    color *= 0.5f;

                    setColor4f(color);
                }

                for(uint32 tex=0;tex<_textureCount;++tex)
                {
                    if(_texCoords[tex].size())
                    {
                        v2 = _texCoords[tex][_edges[e]._vertexes[0]];
                        v2 += _texCoords[tex][_edges[e]._vertexes[1]];
                        v2 *= 0.5f;

                        setTexCoord2f(v2,tex);
                    }
                }

                v3 = _vertexes[_edges[e]._vertexes[0]];
                v3 += _vertexes[_edges[e]._vertexes[1]];
                v3 *= 0.5f;

                setVertex3f(v3);
            }

            for(int32 t=0;t<old_triangle_count;++t)
            {
                setTriangle(_triangles[t].index[0],old_vertex_count + _neighbors[t]._edges[0],old_vertex_count + _neighbors[t]._edges[2]);
                setTriangle(_triangles[t].index[1],old_vertex_count + _neighbors[t]._edges[1],old_vertex_count + _neighbors[t]._edges[0]);
                setTriangle(_triangles[t].index[2],old_vertex_count + _neighbors[t]._edges[2],old_vertex_count + _neighbors[t]._edges[1]);

                _triangles[t].index[0] = old_vertex_count + _neighbors[t]._edges[0];
                _triangles[t].index[1] = old_vertex_count + _neighbors[t]._edges[1];
                _triangles[t].index[2] = old_vertex_count + _neighbors[t]._edges[2];
            }

            _dirtyNeighbors = true;
        }*/

        /*	void iMeshBuilder::separateTriangles(iaVector3f *trianglenormals)
            {
                const float32 threshold = 0.8f;
                int32 currentt = 0;
                int32 nextt = 0;
                vector<int32> actualgroup;
                iaVector3f a,ab,ac,n1,n2;
                vector<vector<int32>> vertexneighbors;
                vector<int32> temp_vector_long;

                calcNeighbors();

                for(uint32 v=0;v<_vertexes.size();++v)
                {
                    vertexneighbors.push_back(temp_vector_long);
                }

                list<int32> currentmesh;
                vector<int32> tempvertexneighbors;

                bool goon;
                bool foundnext;
                bool nextmesh;
                bool gap;
                int32 state = 0;
                list<int32>::const_iterator it;

                // find neighbor _triangles of each vertex
                for(uint32 t=0; t<_triangles.size(); ++t)
                {
                    vertexneighbors[_triangles[t].index[0]].push_back(t);
                    vertexneighbors[_triangles[t].index[1]].push_back(t);
                    vertexneighbors[_triangles[t].index[2]].push_back(t);
                }

                // sort _neighbors of each vertex
                for(uint32 v=0;v<_vertexes.size();++v)
                {
                    nextmesh = true;
                    currentmesh.clear();
                    tempvertexneighbors.clear();

                    while(tempvertexneighbors.size() < vertexneighbors[v].size())
                    {
                        if(nextmesh)
                        {
                            for (it = currentmesh.begin(); it != currentmesh.end(); ++it)
                            {
                                tempvertexneighbors.push_back((*it));
                            }

                            currentmesh.clear();

                            if (tempvertexneighbors.size() == vertexneighbors[v].size())
                            {
                                break;
                            }

                            currentt = -1;

                            for(int32 t=0;t<(int32)vertexneighbors[v].size();++t)
                            {
                                foundnext = true;
                                for(uint32 i=0;i<tempvertexneighbors.size();++i)
                                {
                                    if (vertexneighbors[v][t] == tempvertexneighbors[i])
                                    {
                                        foundnext = false;
                                    }
                                }

                                if(foundnext)
                                {
                                    currentt = vertexneighbors[v][t];
                                    break;
                                }
                            }

                            if (currentt == -1)
                            {
                                break;
                            }

                            currentmesh.push_back(currentt);
                            nextmesh = false;
                            state = 0;
                        }
                        else
                        {
                            gap = true;
                            for(int32 t=0;t<(int32)vertexneighbors[v].size();++t)
                            {
                                nextt = vertexneighbors[v][t];

                                goon = false;
                                for(it=currentmesh.begin();it!=currentmesh.end();++it)
                                {
                                    if((*it) == nextt)
                                    {
                                        goon = true;
                                        break;
                                    }
                                }

                                if (goon)
                                {
                                    continue;
                                }

                                for(int32 i=0;i<3;++i)
                                {
                                    if (_neighbors[currentt]._neighbors[i] == nextt)
                                    {
                                        if (state == 1)
                                        {
                                            currentmesh.push_front(nextt);
                                        }
                                        else
                                        {
                                            currentmesh.push_back(nextt);
                                        }

                                        currentt = nextt;
                                        gap=false;
                                        break;
                                    }
                                }
                            }

                            if(gap) state ++;

                            if (state == 1)
                            {
                                currentt = (*currentmesh.begin());
                            }
                            else if (state == 2)
                            {
                                nextmesh = true;
                            }
                        }
                    }

                    if(vertexneighbors[v].size() != tempvertexneighbors.size())
                    {
                        con_err("inconsistent data");
                    }

                    vertexneighbors[v] = tempvertexneighbors;
                }

                // find goups
                vector< vector<int32> > groups;
                int32 firstofgroup;
                int32 nsize;

                int32 original_vertex_count = _vertexes.size();
                for(int v=0;v<original_vertex_count;++v)
                {
                    // find first triangle of first group
                    firstofgroup = -1;

                    n1 = trianglenormals[vertexneighbors[v][0]];

                    nsize = vertexneighbors[v].size();

                    for(int32 t=1;t<nsize+1;++t)
                    {
                        n2 = trianglenormals[vertexneighbors[v][t%nsize]];

                        if(n1 * n2 < threshold)
                        {
                            firstofgroup = t%nsize;
                            break;
                        }

                        n1 = n2;
                    }

                    if (firstofgroup == -1)
                    {
                        continue; // no groups found
                    }

                    // build groups
                    vector<int32> group;

                    n1 = trianglenormals[vertexneighbors[v][firstofgroup]];
                    groups.clear();
                    group.push_back(vertexneighbors[v][firstofgroup]);

                    for(int32 t=firstofgroup+1;t<nsize+1+firstofgroup;++t)
                    {
                        n2 = trianglenormals[vertexneighbors[v][t%nsize]];

                        if(n1 * n2 < threshold) // next group
                        {
                            groups.push_back(group);
                            group.clear();
                        }

                        group.push_back(vertexneighbors[v][t%nsize]);

                        n1 = n2;
                    }

                    groups.push_back(group);

                    // separate groups
                    int32 newv;
                    for(uint32 g=0;g<groups.size()-1;++g) // last group holds its vertex
                    {
                        // allocate copy of vertex
                        if (_hasColors)
                        {
                            setColor4f(_colors[v]);
                        }

                        if (_hasNormals)
                        {
                            setNormal3f(_normals[v]);
                        }

                        for (uint32 tex = 0; tex < _texCoords.size(); ++tex)
                        {
                            setTexCoord2f(_texCoords[tex][v], tex);
                        }

                        newv = setVertex3f(_vertexes[v]); // get id of new vertex

                        for(uint32 t=0;t<groups[g].size();++t)
                        {
                            for(uint32 i=0;i<3;++i)
                            {
                                if(_triangles[groups[g][t]].index[i] == v)
                                {
                                    _triangles[groups[g][t]].index[i] = newv;
                                    break;
                                }
                            }
                        }
                    }
                }

                vertexneighbors.clear();
            }*/

};
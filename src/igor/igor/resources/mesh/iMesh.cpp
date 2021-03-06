// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/mesh/iMesh.h>

#include <igor/graphics/iRenderer.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iMesh::iMesh()
    {
    }

    iMesh::~iMesh()
    {
        resetIndexData();
        resetVertexData();
    }

    void iMesh::resetIndexData()
    {
        if (_indexData != nullptr)
        {
            delete _indexData;
            _indexData = nullptr;
            _indexDataSize = 0;
        }
    }

    void iMesh::resetVertexData()
    {
        if (_vertexData != nullptr)
        {
            delete _vertexData;
            _vertexData = nullptr;
            _vertexDataSize = 0;
        }
    }

    void iMesh::setHasNormals(bool hasNormals)
    {
        _hasNormals = hasNormals;
    }

    void iMesh::setHasColors(bool hasColors)
    {
        _hasColors = hasColors;
    }

    void iMesh::setVertexCount(uint32 count)
    {
        _vertexCount = count;
    }

    void iMesh::setIndexesCount(uint32 count)
    {
        _indexesCount = count;
    }

    void iMesh::setTrianglesCount(uint32 count)
    {
        _trianglesCount = count;
    }

    void iMesh::setIndexData(uint32 *data, uint32 size)
    {
        _indexData = data;
        _indexDataSize = size;
    }

    void iMesh::setVertexData(float32 *data, uint32 size)
    {
        _vertexData = data;
        _vertexDataSize = size;
    }

    void iMesh::setVertexSize(uint32 size)
    {
        _vertexSize = size;
    }

    void iMesh::setTexture(int texunit, bool active)
    {
        if (active)
        {
            _textures[texunit] = active;
        }
        else
        {
            auto iter = _textures.find(texunit);
            if (iter != _textures.end())
            {
                _textures.erase(iter);
            }
        }
    }

    void iMesh::setTextureCoordinatesCount(uint32 texCoord)
    {
        _textureCoordinatesCount = texCoord;
    }

    void iMesh::setBoundingBox(const iAABoxd &bbox)
    {
        _bbox = bbox;
    }

} // namespace igor
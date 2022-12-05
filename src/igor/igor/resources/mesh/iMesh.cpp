// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/mesh/iMesh.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <cstring>

namespace igor
{
    class iMeshDeleter
    {
    public:
        void operator()(iMesh *p) { delete p; }
    };

    iMeshPtr iMesh::create()
    {
        return std::shared_ptr<iMesh>(new iMesh(), iMeshDeleter());
    }

    iMesh::~iMesh()
    {
        if (_indexData != nullptr)
        {
            delete[] _indexData;
        }
        if (_vertexData != nullptr)
        {
            delete[] _vertexData;
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

    void iMesh::setIndexCount(uint32 count)
    {
        _indexCount = count;
    }

    void iMesh::setTrianglesCount(uint32 count)
    {
        _trianglesCount = count;
    }

    uint32 iMesh::getTrianglesCount() const
    {
        
        return _trianglesCount;
    }

    uint32 iMesh::getVertexCount() const
    {
        return _vertexCount;
    }

    uint32 iMesh::getIndexCount() const
    {
        return _indexCount;
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

    bool iMesh::hasColors() const
    {
        return _hasColors;
    }

    bool iMesh::hasNormals() const
    {
        return _hasNormals;
    }

    uint32 iMesh::getTextureCoordinatesCount() const
    {
        return _textureCoordinatesCount;
    }

    bool iMesh::hasTextures() const
    {
        return _textures.size() > 0 ? true : false;
    }

    bool iMesh::hasTextureUnit(uint32 unit) const
    {
        return _textures.find(unit) != _textures.end() ? true : false;
    }

    uint32 iMesh::getTextureUnitCount() const
    {
        return static_cast<uint32>(_textures.size());
    }

    const iAABoxd &iMesh::getBoundingBox() const
    {
        return _bbox;
    }

    void iMesh::bind()
    {
        if (_vertexArray == nullptr)
        {
            con_assert(_vertexData != nullptr, "no data");
            con_assert(_indexData != nullptr, "no data");
            
            _vertexArray = iVertexArray::create();

            iVertexBufferPtr vertexBuffer = iVertexBuffer::create(_vertexDataSize, _vertexData);
            vertexBuffer->setLayout(_layout);
            _vertexArray->addVertexBuffer(vertexBuffer);

            iIndexBufferPtr indexBuffer = iIndexBuffer::create(getIndexCount(), reinterpret_cast<const uint32 *>(_indexData));
            _vertexArray->setIndexBuffer(indexBuffer);

            if (!_keepData)
            {
                delete[] _indexData;
                _indexData = nullptr;
                delete[] _vertexData;
                _vertexData = nullptr;
            }
        }

        _vertexArray->bind();
    }

    void iMesh::setData(const void *indexData, uint32 indexDataSize, const void *vertexData, uint32 vertexDataSize, const iBufferLayout &layout, bool keepData)
    {
        _indexDataSize = indexDataSize;
        _indexData = new uint8[_indexDataSize];
        memcpy(_indexData, indexData, _indexDataSize);

        _vertexDataSize = vertexDataSize;
        _vertexData = new uint8[_vertexDataSize];
        memcpy(_vertexData, vertexData, _vertexDataSize);

        _keepData = keepData;
        _layout = layout;
    }

    const iVertexArrayPtr &iMesh::getVertexArray() const
    {
        return _vertexArray;
    }

    void iMesh::setVertexArray(const iVertexArrayPtr &vertexArray)
    {
        _vertexArray = vertexArray;
    }    

} // namespace igor
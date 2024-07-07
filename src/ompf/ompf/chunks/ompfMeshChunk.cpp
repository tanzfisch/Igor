// OMPF 3d model file format
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <ompf/chunks/ompfMeshChunk.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <iomanip>
#include <cstring>

namespace OMPF
{
    ompfMeshChunk::ompfMeshChunk()
        : ompfBaseChunk(OMPFChunkType::Mesh)
    {
    }

    ompfMeshChunk::~ompfMeshChunk()
    {
        if (_vertexData != nullptr)
        {
            delete[](char *) _vertexData;
        }

        if (_indexData != nullptr)
        {
            delete[](char *) _indexData;
        }
    }

    uint32 ompfMeshChunk::getVertexSize() const
    {
        return 12 + _normalsPerVertex * 12 + _colorsPerVertex * 12 + _texCoordPerVertex * 8;
    }

    void ompfMeshChunk::setVertexCount(uint32 count)
    {
        _vertexCount = count;
    }

    uint32 ompfMeshChunk::getVertexCount() const
    {
        return _vertexCount;
    }

    void ompfMeshChunk::setNormalsPerVertex(uint8 count)
    {
        _normalsPerVertex = count;
    }

    uint8 ompfMeshChunk::getNormalsPerVertex() const
    {
        return _normalsPerVertex;
    }

    void ompfMeshChunk::setColorsPerVertex(uint8 count)
    {
        _colorsPerVertex = count;
    }

    uint8 ompfMeshChunk::getColorsPerVertex() const
    {
        return _colorsPerVertex;
    }

    void ompfMeshChunk::setTexCoordPerVertex(uint8 count)
    {
        _texCoordPerVertex = count;
    }

    uint8 ompfMeshChunk::getTexCoordPerVertex() const
    {
        return _texCoordPerVertex;
    }

    void ompfMeshChunk::setMeshType(OMPFMeshType type)
    {
        _meshType = type;
    }

    OMPFMeshType ompfMeshChunk::getMeshType() const
    {
        return _meshType;
    }

    void ompfMeshChunk::setIndexCount(uint32 count)
    {
        _indexCount = count;
    }

    uint32 ompfMeshChunk::getIndexCount() const
    {
        return _indexCount;
    }

    IGOR_DISABLE_WARNING(4100)
    uint32 ompfMeshChunk::getSize(const ompfSettings &settings)
    {
        uint32 attributes = 1; // normals per vertex
        attributes += 1;       // colors per vertex
        attributes += 1;       // texcoord per vertex
        attributes += 1;       // mesh type
        attributes += 4;       // vertex count
        attributes += 4;       // index count
        attributes += 4;       // material id

        return attributes + _vertexDataSize + _indexDataSize;
    }
    IGOR_ENABLE_WARNING(4100)

    void ompfMeshChunk::setIndexData(const char *data, uint32 size)
    {
        if (_indexData != nullptr)
        {
            delete[](char *) _indexData;
        }

        _indexDataSize = size;

        _indexData = new char[_indexDataSize];
        memcpy(_indexData, data, _indexDataSize);
    }

    const void *ompfMeshChunk::getIndexData() const
    {
        return _indexData;
    }

    void ompfMeshChunk::setVertexData(const char *data, uint32 size)
    {
        if (_vertexData != nullptr)
        {
            delete[](char *) _vertexData;
        }

        _vertexDataSize = size;

        _vertexData = new char[_vertexDataSize];
        memcpy(_vertexData, data, _vertexDataSize);
    }

    uint32 ompfMeshChunk::getVertexDataSize() const
    {
        return _vertexDataSize;
    }

    uint32 ompfMeshChunk::getIndexDataSize() const
    {
        return _indexDataSize;
    }

    const void *ompfMeshChunk::getVertexData() const
    {
        return _vertexData;
    }

    void ompfMeshChunk::setMaterialChunkID(uint32 id)
    {
        _materialChunkID = id;
    }

    uint32 ompfMeshChunk::getMaterialChunkID() const
    {
        return _materialChunkID;
    }

    bool ompfMeshChunk::write(std::ofstream &file, const ompfSettings &settings)
    {
        if (!ompfBaseChunk::write(file, settings))
        {
            return false;
        }

        con_assert((4 * _indexCount) == _indexDataSize, "inconsistent index data");
        con_assert((getVertexSize() * _vertexCount) == _vertexDataSize, "inconsistent vertex data");

        con_trace("---------------------------------------------------");
        con_trace("write ompfMeshChunk " << this->getName());

        if (!iaSerializable::writeUInt32(file, _materialChunkID))
        {
            return false;
        }
        con_trace("material chunk ID " << _materialChunkID);

        if (!iaSerializable::writeUInt8(file, _normalsPerVertex))
        {
            return false;
        }
        con_trace("normals per vertex " << _normalsPerVertex);

        if (!iaSerializable::writeUInt8(file, _colorsPerVertex))
        {
            return false;
        }
        con_trace("colors per vertex " << _colorsPerVertex);

        if (!iaSerializable::writeUInt8(file, _texCoordPerVertex))
        {
            return false;
        }
        con_trace("tex coord per vertex " << _texCoordPerVertex);

        uint8 meshType = static_cast<uint8>(_meshType);
        iaSerializable::writeUInt8(file, meshType);
        con_trace("mesh type " << static_cast<uint8>(_meshType));

        iaSerializable::writeUInt32(file, _vertexCount);
        iaSerializable::write(file, static_cast<char *>(_vertexData), _vertexDataSize);
        con_trace("vertex count " << _vertexCount << " vertex data size " << _vertexDataSize);

        iaSerializable::writeUInt32(file, _indexCount);
        iaSerializable::write(file, static_cast<char *>(_indexData), _indexDataSize);
        con_trace("index count " << _indexCount << " index data size " << _indexDataSize);

        return true;
    }

    bool ompfMeshChunk::read(std::ifstream &file, ompfSettings &settings)
    {
        con_trace("---------------------------------------------------");
        con_trace("read ompfMeshChunk " << this->getName());

        if (!ompfBaseChunk::read(file, settings))
        {
            return false;
        }

        if (!iaSerializable::readUInt32(file, _materialChunkID))
        {
            return false;
        }
        con_trace("material chunk ID " << _materialChunkID);

        if (!iaSerializable::readUInt8(file, _normalsPerVertex))
        {
            return false;
        }
        con_trace("normals per vertex " << _normalsPerVertex);

        if (!iaSerializable::readUInt8(file, _colorsPerVertex))
        {
            return false;
        }
        con_trace("colors per vertex " << _colorsPerVertex);

        if (!iaSerializable::readUInt8(file, _texCoordPerVertex))
        {
            return false;
        }
        con_trace("tex coord per vertex " << _texCoordPerVertex);

        uint8 meshType = 0;
        iaSerializable::readUInt8(file, meshType);
        _meshType = static_cast<OMPFMeshType>(meshType);
        con_assert(_meshType == OMPFMeshType::Triangles, "only triangles supported");
        con_trace("mesh type " << static_cast<int>(_meshType));

        iaSerializable::readUInt32(file, _vertexCount);
        _vertexDataSize = (getVertexSize() * _vertexCount);
        if (_vertexData != nullptr)
        {
            delete[](char *) _vertexData;
        }
        _vertexData = new char[_vertexDataSize];
        iaSerializable::read(file, static_cast<char *>(_vertexData), _vertexDataSize);
        con_trace("vertex count " << _vertexCount << " vertex data size " << _vertexDataSize);

        iaSerializable::readUInt32(file, _indexCount);
        _indexDataSize = 4 * _indexCount;
        if (_indexData != nullptr)
        {
            delete[](char *) _indexData;
        }
        _indexData = new char[_indexDataSize];
        iaSerializable::read(file, static_cast<char *>(_indexData), _indexDataSize);
        con_trace("index count " << _indexCount << " index data size " << _indexDataSize);

        return true;
    }
} // namespace OMPF

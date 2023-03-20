// OMPF 3d model file format
// (c) Copyright 2012-2023 by Martin Loga
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

    void ompfMeshChunk::setAmbient(const iaColor3c &ambient)
    {
        _ambient = ambient;
    }

    void ompfMeshChunk::setTexture(const iaString &texture, uint32 texunit)
    {
        _textures[texunit] = texture;
    }

    iaString ompfMeshChunk::getTexture(uint32 texunit) const
    {
        auto tex = _textures.find(texunit);
        if (tex != _textures.end())
        {
            return tex->second;
        }
        else
        {
            con_err("texture unit " << texunit << " not available");
            return "";
        }
    }

    uint32 ompfMeshChunk::getVertexSize() const
    {
        return 12 + _normalsPerVertex * 12 + _colorsPerVertex * 12 + _texCoordPerVertex * 8;
    }

    const iaColor3c &ompfMeshChunk::getAmbient() const
    {
        return _ambient;
    }

    void ompfMeshChunk::setDiffuse(const iaColor3c &diffuse)
    {
        _diffuse = diffuse;
    }

    const iaColor3c &ompfMeshChunk::getDiffuse() const
    {
        return _diffuse;
    }

    void ompfMeshChunk::setSpecular(const iaColor3c &specular)
    {
        _specular = specular;
    }

    const iaColor3c &ompfMeshChunk::getSpecular() const
    {
        return _specular;
    }

    void ompfMeshChunk::setVertexCount(uint32 count)
    {
        _vertexCount = count;
    }

    uint32 ompfMeshChunk::getVertexCount() const
    {
        return _vertexCount;
    }

    void ompfMeshChunk::setEmissive(const iaColor3c &emissive)
    {
        _emissive = emissive;
    }

    const iaColor3c &ompfMeshChunk::getEmissive() const
    {
        return _emissive;
    }

    void ompfMeshChunk::setShininess(const float32 shininess)
    {
        _shininess = shininess;
    }

    float32 ompfMeshChunk::getShininess() const
    {
        return _shininess;
    }

    void ompfMeshChunk::setAlpha(const float32 alpha)
    {
        _alpha = alpha;
    }

    float32 ompfMeshChunk::getAlpha() const
    {
        return _alpha;
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

    __IGOR_DISABLE_WARNING__(4100)
    uint32 ompfMeshChunk::getSize(const ompfSettings &settings)
    {
        uint32 material = 3;   // ambient
        material += 3;         // diffuse
        material += 3;         // specular
        material += 3;         // emissive
        material += 4;         // shininess
        material += 4;         // alpha
        uint32 attributes = 1; // normals per vertex
        attributes += 1;       // colors per vertex
        attributes += 1;       // texcoord per vertex
        attributes += 1;       // mesh type
        attributes += 4;       // vertex count
        attributes += 4;       // index count
        attributes += 4;       // material chunk id
        uint32 textures = 0;

        auto iterTex = _textures.begin();
        while (iterTex != _textures.end())
        {
            textures += static_cast<uint32>((*iterTex).second.getLength()) + 2;
            iterTex++;
        }

        return material + attributes + _vertexDataSize + _indexDataSize + textures;
    }
    __IGOR_ENABLE_WARNING__(4100)

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

    uint32 ompfMeshChunk::getTextureCount() const
    {
        return static_cast<uint32>(_textures.size());
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

        con_assert((4 * _indexCount) == _indexDataSize, "inconsistend index data");
        con_assert((getVertexSize() * _vertexCount) == _vertexDataSize, "inconsistend vertex data");

        con_trace("---------------------------------------------------");
        con_trace("write ompfMeshChunk " << this->getName());

        if (!iaSerializable::writeUInt32(file, _materialChunkID))
        {
            return false;
        }
        con_trace("materialChunkID " << _materialChunkID);

        if (!iaSerializable::write(file, _ambient))
        {
            return false;
        }
        con_trace("ambient " << _ambient);

        if (!iaSerializable::write(file, _diffuse))
        {
            return false;
        }
        con_trace("diffuse " << _diffuse);

        if (!iaSerializable::write(file, _specular))
        {
            return false;
        }
        con_trace("specular " << _specular);

        if (!iaSerializable::write(file, _emissive))
        {
            return false;
        }
        con_trace("emissive " << _emissive);

        if (!iaSerializable::writeFloat32(file, _shininess))
        {
            return false;
        }
        con_trace("shininess " << _shininess);

        if (!iaSerializable::writeFloat32(file, _alpha))
        {
            return false;
        }
        con_trace("shininess " << _alpha);

        if (!iaSerializable::writeUInt8(file, _normalsPerVertex))
        {
            return false;
        }
        con_trace("normalsPerVertex " << _normalsPerVertex);

        if (!iaSerializable::writeUInt8(file, _colorsPerVertex))
        {
            return false;
        }
        con_trace("colorsPerVertex " << _colorsPerVertex);

        if (!iaSerializable::writeUInt8(file, _texCoordPerVertex))
        {
            return false;
        }
        con_trace("texCoordPerVertex " << _texCoordPerVertex);

        uint8 meshType = static_cast<uint8>(_meshType);
        iaSerializable::writeUInt8(file, meshType);
        con_trace("meshType " << static_cast<uint8>(_meshType));

        iaSerializable::writeUInt8(file, static_cast<uint8>(getTextureCount()));
        con_trace("textureCount " << getTextureCount());

        uint32 i = 0;
        for(const auto &pair : _textures)
        {
            con_assert(pair.first == i, "inconsistent texture units");

            iaSerializable::writeUTF8(file, pair.second);
            con_trace("texture " << i << " " << pair.second);
            i++;
        }

        iaSerializable::writeUInt32(file, _vertexCount);
        iaSerializable::write(file, static_cast<char *>(_vertexData), _vertexDataSize);
        con_trace("vertexCount " << _vertexCount << " vertexDataSize " << _vertexDataSize);

        iaSerializable::writeUInt32(file, _indexCount);
        iaSerializable::write(file, static_cast<char *>(_indexData), _indexDataSize);
        con_trace("indexCount " << _indexCount << " indexDataSize " << _indexDataSize);

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
        con_trace("materialChunkID " << _materialChunkID);

        if (!iaSerializable::read(file, _ambient))
        {
            return false;
        }
        con_trace("ambient " << _ambient);

        if (!iaSerializable::read(file, _diffuse))
        {
            return false;
        }
        con_trace("diffuse " << _diffuse);

        if (!iaSerializable::read(file, _specular))
        {
            return false;
        }
        con_trace("specular " << _specular);

        if (!iaSerializable::read(file, _emissive))
        {
            return false;
        }
        con_trace("emissive " << _emissive);

        if (!iaSerializable::readFloat32(file, _shininess))
        {
            return false;
        }
        con_trace("shininess " << _shininess);

        if (!iaSerializable::readFloat32(file, _alpha))
        {
            return false;
        }
        con_trace("shininess " << _alpha);

        if (!iaSerializable::readUInt8(file, _normalsPerVertex))
        {
            return false;
        }
        con_trace("normalsPerVertex " << _normalsPerVertex);

        if (!iaSerializable::readUInt8(file, _colorsPerVertex))
        {
            return false;
        }
        con_trace("colorsPerVertex " << _colorsPerVertex);

        if (!iaSerializable::readUInt8(file, _texCoordPerVertex))
        {
            return false;
        }
        con_trace("texCoordPerVertex " << _texCoordPerVertex);

        uint8 meshType = 0;
        iaSerializable::readUInt8(file, meshType);
        _meshType = static_cast<OMPFMeshType>(meshType);
        con_assert(_meshType == OMPFMeshType::Triangles, "only triangles supported");
        con_trace("meshType " << static_cast<int>(_meshType));

        uint8 textureCount = 0;
        iaSerializable::readUInt8(file, textureCount);
        con_trace("textureCount " << textureCount);

        for (int i = 0; i < textureCount; ++i)
        {
            iaString temp;
            iaSerializable::readUTF8(file, temp);
            setTexture(temp, i);

            con_trace("texture " << i << " " << temp);
        }

        iaSerializable::readUInt32(file, _vertexCount);
        _vertexDataSize = (getVertexSize() * _vertexCount);
        if (_vertexData != nullptr)
        {
            delete[](char *) _vertexData;
        }
        _vertexData = new char[_vertexDataSize];
        iaSerializable::read(file, static_cast<char *>(_vertexData), _vertexDataSize);
        con_trace("vertexCount " << _vertexCount << " vertexDataSize " << _vertexDataSize);

        iaSerializable::readUInt32(file, _indexCount);
        _indexDataSize = 4 * _indexCount;
        if (_indexData != nullptr)
        {
            delete[](char *) _indexData;
        }
        _indexData = new char[_indexDataSize];
        iaSerializable::read(file, static_cast<char *>(_indexData), _indexDataSize);
        con_trace("indexCount " << _indexCount << " indexDataSize " << _indexDataSize);

        return true;
    }
} // namespace OMPF

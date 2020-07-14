// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

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

bool iMesh::hasIndexData()
{
    return _indexData != nullptr ? true : false;
}

bool iMesh::hasVertexData()
{
    return _vertexData != nullptr ? true : false;
}

uint32 iMesh::getVertexCount() const
{
    return _vertexCount;
}

uint32 iMesh::getIndexesCount() const
{
    return _indexesCount;
}

uint32 iMesh::getTrianglesCount() const
{
    return _trianglesCount;
}

const iAABoxd &iMesh::getBoundingBox() const
{
    return _bbox;
}

uint32 *iMesh::getIndexData() const
{
    return _indexData;
}

uint32 iMesh::getIndexDataSize() const
{
    return _indexDataSize;
}

float32 *iMesh::getVertexData() const
{
    return _vertexData;
}

uint32 iMesh::getVertexDataSize() const
{
    return _vertexDataSize;
}

uint32 iMesh::getVertexSize() const
{
    return _vertexSize;
}
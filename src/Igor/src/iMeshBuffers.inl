// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

uint32 iMeshBuffers::getIndexBufferObject() const
{
    return _indexBufferObject;
}

uint32 iMeshBuffers::getVertexBufferObject() const
{
    return _vertexBufferObject;
}

uint32 iMeshBuffers::getVertexArrayObject() const
{
    return _vertexArrayObject;
}

uint32 iMeshBuffers::getVertexCount() const
{
    return _vertexCount;
}

uint32 iMeshBuffers::getIndexesCount() const
{
    return _indexesCount;
}

uint32 iMeshBuffers::getTrianglesCount() const
{
    return _trianglesCount;
}
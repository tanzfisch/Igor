// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/mesh/iMeshBuffers.h>

namespace igor
{

    iMeshBuffers::~iMeshBuffers()
    {
        /*if (_vertexBufferObject != INVALID_BUFFER_ID)
        {
            iRenderer::getInstance().destroyBuffer(_vertexBufferObject);
        }

        if (_indexBufferObject != INVALID_BUFFER_ID)
        {
            iRenderer::getInstance().destroyBuffer(_indexBufferObject);
        }

        if (_vertexArrayObject != INVALID_BUFFER_ID)
        {
            iRenderer::getInstance().destroyVertexArray(_vertexArrayObject);
        }*/
    }

    void iMeshBuffers::setVertexArrayObject(uint32 vao)
    {
        _vertexArrayObject = vao;
    }

    void iMeshBuffers::setVertexBufferObject(uint32 vbo)
    {
        _vertexBufferObject = vbo;
    }

    void iMeshBuffers::setIndexBufferObject(uint32 ibo)
    {
        _indexBufferObject = ibo;
    }

    void iMeshBuffers::setVertexCount(uint32 count)
    {
        _vertexCount = count;
    }

    void iMeshBuffers::setIndexesCount(uint32 count)
    {
        _indexesCount = count;
    }

    void iMeshBuffers::setTrianglesCount(uint32 count)
    {
        _trianglesCount = count;
    }

    bool iMeshBuffers::isReady()
    {
        return _ready;
    }

    void iMeshBuffers::setReady(bool ready)
    {
        _ready = ready;
    }

}
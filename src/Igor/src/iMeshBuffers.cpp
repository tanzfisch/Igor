// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iMeshBuffers.h>
#include <iRenderer.h>

namespace Igor
{

    iMeshBuffers::~iMeshBuffers()
    {
        if (_vertexBufferObject != 0)
        {
            iRenderer::getInstance().destroyBuffer(_vertexBufferObject);
        }

        if (_indexBufferObject != 0)
        {
            iRenderer::getInstance().destroyBuffer(_indexBufferObject);
        }

        if (_vertexArrayObject != 0)
        {
            iRenderer::getInstance().destroyVertexArray(_vertexArrayObject);
        }
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

}
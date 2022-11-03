// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/buffers/iVertexBuffer.h>

#include <igor/renderer/utils/iRendererUtils.h>

namespace igor
{
    iVertexBufferPtr iVertexBuffer::create(uint32 size, const void *vertexData)
    {
        return std::make_shared<iVertexBuffer>(iVertexBuffer(size, vertexData));
    }

    iVertexBuffer::iVertexBuffer(uint32 size, const void *vertexData)
    {
        glCreateBuffers(1, &_vertexBufferObject);
        GL_CHECK_ERROR();
        bind();

        if (vertexData != nullptr)
        {
            glBufferData(GL_ARRAY_BUFFER, size, vertexData, GL_STATIC_DRAW);
            GL_CHECK_ERROR();
        }
        else
        {
            glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
            GL_CHECK_ERROR();
        }
    }

    iVertexBuffer::~iVertexBuffer()
    {
        glDeleteBuffers(1, &_vertexBufferObject);
        GL_CHECK_ERROR();
    }

    void iVertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
        GL_CHECK_ERROR();
    }

    void iVertexBuffer::unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, INVALID_BUFFER_ID);
        GL_CHECK_ERROR();
    }

    void iVertexBuffer::setData(uint32 size, const void *vertexData)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
        GL_CHECK_ERROR();
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertexData);
        GL_CHECK_ERROR();
    }

    void iVertexBuffer::setLayout(const iBufferLayout &layout)
    {
        _layout = layout;
    }

    const iBufferLayout &iVertexBuffer::getLayout() const
    {
        return _layout;
    }

}
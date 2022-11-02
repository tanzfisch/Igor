// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/graphics/buffers/iVertexBuffer.h>

#define GL_GLEXT_PROTOTYPES
#include <GLee.h>

namespace igor
{

    /*! definition of invalid buffer id
     */
    static const uint32 INVALID_BUFFER_ID = 0;

    iVertexBuffer::iVertexBuffer(uint32 size, const void *vertexData)
    {
        glCreateBuffers(1, &_vertexBufferObject);
        bind();

        if (vertexData != nullptr)
        {
            glBufferData(GL_ARRAY_BUFFER, size, vertexData, GL_STATIC_DRAW);
        }
        else
        {
            glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        }
    }

    iVertexBuffer::~iVertexBuffer()
    {
        glDeleteBuffers(1, &_vertexBufferObject);
    }

    iVertexBufferPtr iVertexBuffer::create(uint32 size, const void *vertexData)
    {
        return std::make_shared<iVertexBuffer>(iVertexBuffer(size, vertexData));
    }

    void iVertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
    }

    void iVertexBuffer::unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, INVALID_BUFFER_ID);
    }

    void iVertexBuffer::setData(uint32 size, const void *vertexData)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertexData);
    }

    void iVertexBuffer::setInfo(const iComponentInfo &info)
    {
        _info = info;
    }

    const iComponentInfo &iVertexBuffer::getInfo() const
    {
        return _info;
    }

}
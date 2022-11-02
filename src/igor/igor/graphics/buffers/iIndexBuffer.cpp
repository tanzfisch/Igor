// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/graphics/buffers/iIndexBuffer.h>

#define GL_GLEXT_PROTOTYPES
#include <GLee.h>

namespace igor
{

    /*! definition of invalid buffer id
     */
    static const uint32 INVALID_BUFFER_ID = 0;

    iIndexBuffer::iIndexBuffer(uint32 count, const uint32 *indices)
        : _indexCount(count)
    {
        glCreateBuffers(1, &_indexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, _indexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32), indices, GL_STATIC_DRAW);
    }

    iIndexBuffer::~iIndexBuffer()
    {
        glDeleteBuffers(1, &_indexBufferObject);
    }

    void iIndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferObject);
    }

    void iIndexBuffer::unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, INVALID_BUFFER_ID);
    }

    uint32 iIndexBuffer::getIndexCount() const
    {
        return _indexCount;
    }

}

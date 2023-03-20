// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/buffers/iVertexBuffer.h>

#include <igor/renderer/utils/iRendererUtils.h>

namespace igor
{

    class iVertexBufferDeleter
    {
    public:
        void operator()(iVertexBuffer *p) { delete p; }
    };

    iVertexBufferPtr iVertexBuffer::create(uint32 size, const void *vertexData)
    {
        return std::shared_ptr<iVertexBuffer>(new iVertexBuffer(size, vertexData), iVertexBufferDeleter());
    }

    iVertexBuffer::iVertexBuffer(uint32 size, const void *vertexData)
    {
        _dynamic = vertexData == nullptr;

        glCreateBuffers(1, &_vertexBufferObject);
        GL_CHECK_ERROR();
        
        glNamedBufferData(_vertexBufferObject, size, vertexData, _dynamic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
        GL_CHECK_ERROR();
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

    void iVertexBuffer::setData(uint32 size, const void *vertexData)
    {
        _bufferSize = size;

        glNamedBufferSubData(_vertexBufferObject, 0, size, vertexData);
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

    bool iVertexBuffer::isDynamic() const
    {
        return _dynamic;
    }

    uint32 iVertexBuffer::getVertexCount() const
    {
        if(_layout.getStride() == 0)
        {
            con_err("layout not set");
            return 0;
        }
        return _bufferSize / _layout.getStride();
    }
    uint32 iVertexBuffer::getVertexSize() const
    {
        return _layout.getStride();
    }
    uint32 iVertexBuffer::getBufferSize() const
    {
        return _bufferSize;
    }

}
// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/renderer/buffers/iIndexBuffer.h>

#include <igor/renderer/utils/iRendererUtils.h>

namespace igor
{
    class iIndexBufferDeleter
    {
    public:
        void operator()(iIndexBuffer * p) { delete p; }
    };

    iIndexBufferPtr iIndexBuffer::create(uint32 count, const uint32 *indices)
    {
        return std::shared_ptr<iIndexBuffer>(new iIndexBuffer(count, indices), iIndexBufferDeleter());
    }

    iIndexBuffer::iIndexBuffer(uint32 count, const uint32 *indices)
        : _indexCount(count)
    {
        _dynamic = indices == nullptr;

        glCreateBuffers(1, &_indexBufferObject);
        GL_CHECK_ERROR();

        glNamedBufferData(_indexBufferObject, count * sizeof(uint32), indices, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        GL_CHECK_ERROR();
    }

    iIndexBuffer::~iIndexBuffer()
    {
        glDeleteBuffers(1, &_indexBufferObject);
        GL_CHECK_ERROR();
    }

    void iIndexBuffer::bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferObject);
        GL_CHECK_ERROR();
    }

    void iIndexBuffer::setData(uint32 size, const void *indexData)
    {
        glNamedBufferSubData(_indexBufferObject, 0, size, indexData);
        GL_CHECK_ERROR();
    }

    uint32 iIndexBuffer::getIndexCount() const
    {
        return _indexCount;
    }

    bool iIndexBuffer::isDynamic() const
    {
        return _dynamic;
    }
}

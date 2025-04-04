// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/buffers/iInstancingBuffer.h>

#include <cstring>

namespace igor
{

    class iInstancingBufferDeleter
    {
    public:
        void operator()(iInstancingBuffer *p) { delete p; }
    };

    iInstancingBufferPtr iInstancingBuffer::create(const iBufferLayout &layout, uint32 maxInstanceSizeHint)
    {
        return std::shared_ptr<iInstancingBuffer>(new iInstancingBuffer(layout, maxInstanceSizeHint), iInstancingBufferDeleter());
    }

    iInstancingBuffer::iInstancingBuffer(const iBufferLayout &layout, uint32 maxInstanceSizeHint)
        : _layout(layout)
    {
        _instanceSize = layout.getStride();
        _instanceDataSize = _instanceSize * maxInstanceSizeHint;

        _instancingData = new uint8[_instanceDataSize];
        _instancingDataPtr = _instancingData;

        _vertexBuffer = iVertexBuffer::create(_instanceDataSize);
        _vertexBuffer->setLayout(_layout);

        _instanceCount = 0;
    }

    iInstancingBuffer::~iInstancingBuffer()
    {
        if (_instancingData != nullptr)
        {
            delete[] _instancingData;
            _instancingData = nullptr;
        }
    }

    iVertexBufferPtr iInstancingBuffer::getVertexBuffer() const
    {
        return _vertexBuffer;
    }

    void iInstancingBuffer::finalizeData()
    {
        uint32 dataSize = (uint32)((uint8 *)_instancingDataPtr - (uint8 *)_instancingData);
        if (dataSize == 0)
        {
            return;
        }
        _vertexBuffer->setData(dataSize, _instancingData);
    }

    void iInstancingBuffer::bind()
    {
        _vertexBuffer->bind();
    }

    uint32 iInstancingBuffer::getInstanceCount() const
    {
        return _instanceCount;
    }

    void iInstancingBuffer::resizeBuffer(uint32 newSize)
    {
        uint8 *newBuffer = new uint8[newSize];
        memcpy(newBuffer, _instancingData, _instanceDataSize);

        delete[] _instancingData;
        _instancingData = newBuffer;
        _instanceDataSize = newSize;

        _instancingDataPtr = _instancingData + _instanceCount * _instanceSize;

        _vertexBuffer = iVertexBuffer::create(_instanceDataSize);
        _vertexBuffer->setLayout(_layout);
    }

    void iInstancingBuffer::setSizeHint(uint32 maxInstanceSizeHint)
    {
        if (maxInstanceSizeHint < _instanceDataSize / _instanceSize)
        {
            return;
        }

        resizeBuffer(maxInstanceSizeHint * _instanceSize);
    }

    void iInstancingBuffer::addInstance(uint32 size, const void *data)
    {
        // buffer too small. double it
        if (_instancingDataPtr + size > _instancingData + _instanceDataSize)
        {
            resizeBuffer(_instanceDataSize * 2);
        }

        memcpy(_instancingDataPtr, data, size);
        _instancingDataPtr += size;

        _instanceCount += size / _instanceSize;
    }

    void iInstancingBuffer::clear()
    {
        _instancingDataPtr = _instancingData;
        _instanceCount = 0;
    }

}
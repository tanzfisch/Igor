// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
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

    iInstancingBufferPtr iInstancingBuffer::create(uint32 maxInstances, const iBufferLayout &layout)
    {
        return std::shared_ptr<iInstancingBuffer>(new iInstancingBuffer(maxInstances, layout), iInstancingBufferDeleter());
    }

    iInstancingBuffer::iInstancingBuffer(uint32 maxInstances, const iBufferLayout &layout)
    {
        _instanceSize = layout.getStride();
        _instanceDataSize = _instanceSize * maxInstances;

        _instancingData = new uint8[_instanceDataSize];
        _instancingDataPtr = _instancingData;        

        _vertexBuffer = iVertexBuffer::create(_instanceDataSize);
        _vertexBuffer->setLayout(layout);

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

    void iInstancingBuffer::pushData()
    {
        uint32 dataSize = (uint32)((uint8 *)_instancingDataPtr - (uint8 *)_instancingData);
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

    void iInstancingBuffer::addInstance(uint32 size, const void *data)
    {
        con_assert(_instancingDataPtr + size <= _instancingData + _instanceDataSize, "buffer overflow");
        memcpy(_instancingDataPtr, data, size);
        _instancingDataPtr += size;

        _instanceCount += size / _instanceSize;
    }

    void iInstancingBuffer::clear()
    {
        _instancingDataPtr = _instancingData;
    }

}
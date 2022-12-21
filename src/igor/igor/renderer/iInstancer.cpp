// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/iInstancer.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <cstring>

namespace igor
{

    iInstancer::iInstancer(uint32 instanceSize, uint32 maxInstanceCount)
        : _maxInstanceCount(maxInstanceCount), _instanceSize(instanceSize)
    {
        _bufferSize = _maxInstanceCount * _instanceSize;
        _instanceDataBuffer = new char[_bufferSize];
        _currentBufferPosition = _instanceDataBuffer;
    }

    iInstancer::~iInstancer()
    {
        delete[] _instanceDataBuffer;

        if (_instanceArrayObject != 0)
        {
            // TODO this needs to use iVertexArray iRenderer::getInstance().destroyBuffer(_instanceArrayObject);
        }
    }

    void iInstancer::clearInstances()
    {
        _currentBufferPosition = _instanceDataBuffer;
        _instanceCount = 0;
    }

    void iInstancer::addInstance(void *buffer)
    {
        con_assert(_instanceCount < _maxInstanceCount, "instancer ran out of memory. max count is " << _maxInstanceCount);

        memcpy(_currentBufferPosition, buffer, _instanceSize);
        _currentBufferPosition += _instanceSize;
        _instanceCount++;
    }

    void iInstancer::setInstanceArrayObject(uint32 instanceArrayObject)
    {
        _instanceArrayObject = instanceArrayObject;
    }

    uint32 iInstancer::getInstanceSize() const
    {
        return _instanceSize;
    }

    uint32 iInstancer::getInstanceCount() const
    {
        return _instanceCount;
    }

    const void *iInstancer::getInstanceDataBuffer() const
    {
        return _instanceDataBuffer;
    }

    uint32 iInstancer::getInstanceArrayObject() const
    {
        return _instanceArrayObject;
    }

    uint32 iInstancer::getInstanceDataBufferSize() const
    {
        return _bufferSize;
    }

    uint32 iInstancer::getInstanceMaxCount() const
    {
        return _maxInstanceCount;
    }

} // namespace igor
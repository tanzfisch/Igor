// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/graphics/iInstancer.h>

#include <igor/graphics/iRenderer.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iInstancer::iInstancer(uint32 instanceSize, uint32 maxInstanceCount)
        : _maxInstanceCount(maxInstanceCount), _instanceSize(instanceSize)
    {
        _bufferSize = _maxInstanceCount * _instanceSize;
        _instanceDataBuffer = new char[_bufferSize];
    }

    iInstancer::~iInstancer()
    {
        delete[] _instanceDataBuffer;

        if (_instanceArrayObject != 0)
        {
            iRenderer::getInstance().destroyBuffer(_instanceArrayObject);
        }
    }

    void iInstancer::clearInstances()
    {
        _currentBufferPosition = _instanceDataBuffer;
        _instanceCount = 0;
    }

    void iInstancer::addInstance(void *buffer)
    {
        memcpy(_currentBufferPosition, buffer, _instanceSize);
        _currentBufferPosition += _instanceSize;
        _instanceCount++;
    }

    void iInstancer::setInstanceArrayObject(uint32 instanceArrayObject)
    {
        _instanceArrayObject = instanceArrayObject;
    }

} // namespace igor
// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

uint32 iInstancer::getInstanceSize() const
{
    return _instanceSize;
}

uint32 iInstancer::getInstanceCount() const
{
    return _instanceCount;
}

char* iInstancer::getInstanceDataBuffer() const
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
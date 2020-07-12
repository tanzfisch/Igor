// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

__IGOR_INLINE__ uint32 iInstancer::getInstanceSize() const
{
    return _instanceSize;
}

__IGOR_INLINE__ uint32 iInstancer::getInstanceCount() const
{
    return _instanceCount;
}

__IGOR_INLINE__ char *iInstancer::getInstanceDataBuffer() const
{
    return _instanceDataBuffer;
}

__IGOR_INLINE__ uint32 iInstancer::getInstanceArrayObject() const
{
    return _instanceArrayObject;
}

__IGOR_INLINE__ uint32 iInstancer::getInstanceDataBufferSize() const
{
    return _bufferSize;
}

__IGOR_INLINE__ uint32 iInstancer::getInstanceMaxCount() const
{
    return _maxInstanceCount;
}
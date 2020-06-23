// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
iaBits<T>::iaBits()
{
    _bitField = 0;
}

template <class T>
iaBits<T>::iaBits(T values)
{
    _bitField = values;
}

template <class T>
__IGOR_INLINE__ void iaBits<T>::setData(T data)
{
    _bitField = data;
}

template <class T>
__IGOR_INLINE__ void iaBits<T>::set(uint64 bit, bool value)
{
    _bitField ^= (-(value ? 1 : 0) ^ _bitField) & (1 << bit);
}

template <class T>
__IGOR_INLINE__ void iaBits<T>::set(uint64 bit)
{
    _bitField |= 1 << bit;
}

template <class T>
__IGOR_INLINE__ void iaBits<T>::reset(uint64 bit)
{
    _bitField &= ~(1 << bit);
}

template <class T>
__IGOR_INLINE__ void iaBits<T>::toggle(uint64 bit)
{
    _bitField ^= 1 << bit;
}

template <class T>
__IGOR_INLINE__ bool iaBits<T>::get(uint64 bit) const
{
    return ((_bitField >> bit) & 1) ? true : false;
}

template <class T>
T iaBits<T>::getData() const
{
    return _bitField;
}

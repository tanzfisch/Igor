// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
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
void iaBits<T>::setValues(T values)
{
    _bitField = values;
}

template <class T>
void iaBits<T>::set(uint64 bit, bool value)
{
    _bitField ^= (-(value ? 1 : 0) ^ _bitField) & (1 << bit);
}

template <class T>
void iaBits<T>::set(uint64 bit)
{
    _bitField |= 1 << bit;
}

template <class T>
void iaBits<T>::reset(uint64 bit)
{
    _bitField &= ~(1 << bit);
}

template <class T>
void iaBits<T>::toggle(uint64 bit)
{
    _bitField ^= 1 << bit;
}

template <class T>
bool iaBits<T>::get(uint64 bit) const
{
    return ((_bitField >> bit) & 1) ? true : false;
}

template <class T>
T iaBits<T>::getValues() const
{
    return _bitField;
}

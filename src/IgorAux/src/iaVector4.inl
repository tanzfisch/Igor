// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
__IGOR_INLINE__ std::wostream& operator<<(std::wostream &ostr, const iaVector4<T> &v)
{
    ostr << "(" << v._vec._x << ", " << v._vec._y << ", " << v._vec._z << ", " << v._w << ")";
    return ostr;
}

template <class T>
T& iaVector4<T>::operator[] (int i)
{
    return (&_vec._x)[i];
}

template <class T>
const T& iaVector4<T>::operator[] (int i) const
{
    return (&_vec._x)[i];
}

template <class T>
bool iaVector4<T>::operator == (const iaVector4<T> &a) const
{
    if ((_vec._x == a._vec._x) &&
        (_vec._y == a._vec._y) &&
        (_vec._z == a._vec._z) &&
        (_w == a._w))
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <class T>
bool iaVector4<T>::operator != (const iaVector4<T> &a) const
{
    if ((_vec._x != a._vec._x) ||
        (_vec._y != a._vec._y) ||
        (_vec._z != a._vec._z) ||
        (_w != a._w))
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <class T>
iaVector4<T> iaVector4<T>::operator = (const iaVector4<T> &src)
{
    iaVector4<T> result;
    result._vec = _vec = src._vec;
    result._w = _w = src._w;
    return result;
}

template <class T>
iaVector4<T> iaVector4<T>::operator = (const iaVector3<T> &a)
{
    iaVector4<T> result;
    result._vec = _vec = a._vec;
    return result;
}

template <class T>
iaVector4<T>::iaVector4()
{
    _vec._x = 0;
    _vec._y = 0;
    _vec._z = 0;
    _w = 0;
}

template <class T>
iaVector4<T>::iaVector4(T x, T y, T z, T w)
{
    _vec._x = x;
    _vec._y = y;
    _vec._z = z;
    _w = w;
}

template <class T>
T* iaVector4<T>::getData()
{
    return &_vec._x;
}

template <class T>
const T* iaVector4<T>::getData() const
{
    return &_vec._x;
}

template <class T>
void iaVector4<T>::set(T x, T y, T z, T w)
{
    _vec._x = x;
    _vec._y = y;
    _vec._z = z;
    _w = w;
}

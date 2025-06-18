// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

template <class T>
iaVector4<T>::iaVector4(T x, T y, T z, T w)
{
    _x = x;
    _y = y;
    _z = z;
    _w = w;
}

template <class T>
iaVector4<T>::iaVector4(T x, T y, T z)
{
    _x = x;
    _y = y;
    _z = z;    
}

template <class T>
iaVector4<T>::iaVector4(const iaVector4<T> &vec)
{
    _x = vec._x;
    _y = vec._y;
    _z = vec._z;
    _w = vec._w;
}

template <class T>
iaVector4<T>::iaVector4(const iaVector3<T> &vec)
{
    _x = vec._x;
    _y = vec._y;
    _z = vec._z;
}

template <class T>
IGOR_INLINE std::wostream &operator<<(std::wostream &ostr, const iaVector4<T> &v)
{
    ostr << "(" << v._x << ", " << v._y << ", " << v._z << ", " << v._w << ")";
    return ostr;
}

template <class T>
T &iaVector4<T>::operator[](int i)
{
    return (&_x)[i];
}

template <class T>
const T &iaVector4<T>::operator[](int i) const
{
    return (&_x)[i];
}

template <class T>
bool iaVector4<T>::operator==(const iaVector4<T> &a) const
{
    if ((_x == a._x) &&
        (_y == a._y) &&
        (_z == a._z) &&
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
bool iaVector4<T>::operator!=(const iaVector4<T> &a) const
{
    if ((_x != a._x) ||
        (_y != a._y) ||
        (_z != a._z) ||
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
iaVector4<T> iaVector4<T>::operator=(const iaVector4<T> &src)
{
    iaVector4<T> result;
    result._x = _x = src._x;
    result._y = _y = src._y;
    result._z = _z = src._z;
    result._w = _w = src._w;
    return result;
}

template <class T>
iaVector4<T> iaVector4<T>::operator=(const iaVector3<T> &a)
{
    iaVector4<T> result;
    result._x = _x = a._x;
    result._y = _y = a._y;
    result._z = _z = a._z;
    result._w = static_cast<T>(1);
    return result;
}

template <class T>
T *iaVector4<T>::getData()
{
    return &_x;
}

template <class T>
const T *iaVector4<T>::getData() const
{
    return &_x;
}

template <class T>
void iaVector4<T>::set(T x, T y, T z, T w)
{
    _x = x;
    _y = y;
    _z = z;
    _w = w;
}

template <class T>
template <class T2>
iaVector4<T2> iaVector4<T>::convert() const
{
    iaVector4<T2> result(static_cast<T2>(_x), static_cast<T2>(_y), static_cast<T2>(_z), static_cast<T2>(_w));
    return result;
}

template <class T>
iaVector4<T> lerp(const iaVector4<T> &a, const iaVector4<T> &b, T t)
{
    iaVector4<T> result;
    // (1 - t) * v0 + t * v1;
    result._x = b._x * t + a._x * (1.0 - t);
    result._y = b._y * t + a._y * (1.0 - t);
    result._z = b._z * t + a._z * (1.0 - t);
    result._w = b._w * t + a._w * (1.0 - t);

    return result;
}

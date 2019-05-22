// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
__IGOR_INLINE__ std::wostream& operator<<(std::wostream &ostr, const iaVector4<T> &v)
{
    ostr << "(" << v._x << ", " << v._y << ", " << v._z << ", " << v._w << ")";
    return ostr;
}

template <class T>
T& iaVector4<T>::operator[] (int i)
{
    return (&_x)[i];
}

template <class T>
const T& iaVector4<T>::operator[] (int i) const
{
    return (&_x)[i];
}

template <class T>
bool iaVector4<T>::operator == (const iaVector4<T> &a) const
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
bool iaVector4<T>::operator != (const iaVector4<T> &a) const
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
iaVector4<T> iaVector4<T>::operator = (const iaVector4<T> &src)
{
    iaVector4<T> result;
    result._x = _x = src._x;
	result._y = _y = src._y;
	result._z = _z = src._z;
    result._w = _w = src._w;
    return result;
}

template <class T>
iaVector4<T> iaVector4<T>::operator = (const iaVector3<T> &a)
{
    iaVector4<T> result;
    result._x = _x = a._x;
	result._y = _y = a._y;
	result._z = _z = a._z;
	result._w = static_cast<T>(1);
    return result;
}

template <class T>
iaVector4<T>::iaVector4()
{
}

template <class T>
iaVector4<T>::iaVector4(T x, T y, T z, T w)
{
    _x = x;
    _y = y;
    _z = z;
    _w = w;
}

template <class T>
T* iaVector4<T>::getData()
{
    return &_x;
}

template <class T>
const T* iaVector4<T>::getData() const
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

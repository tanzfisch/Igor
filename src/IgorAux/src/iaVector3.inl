// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
iaVector3<T>::iaVector3(T x, T y, T z)
{
    _x = x;
    _y = y;
    _z = z;
}

template <class T>
iaVector3<T> iaVector3<T>::operator + (const iaVector3<T> &a) const
{
    iaVector3<T> c;

    c._x = _x + a._x;
    c._y = _y + a._y;
    c._z = _z + a._z;

    return c;
}

template <class T>
__IGOR_INLINE__ std::wostream& operator << (std::wostream &ostr, const iaVector3<T> &v)
{
    ostr << "(" << v._x << ", " << v._y << ", " << v._z << ")";
    return ostr;
}

template <class T>
T& iaVector3<T>::operator[] (int i)
{
    return (&_x)[i];
}

template <class T>
const T& iaVector3<T>::operator[] (int i) const
{
    return (&_x)[i];
}

template <class T>
iaVector3<T> iaVector3<T>::operator - (const iaVector3<T> &a) const
{
    iaVector3<T> c;
    c._x = _x - a._x;
    c._y = _y - a._y;
    c._z = _z - a._z;

    return c;
}

template <class T>
bool iaVector3<T>::operator == (const iaVector3<T> &a) const
{
    if (_x == a._x &&
        _y == a._y &&
        _z == a._z)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <class T>
bool iaVector3<T>::operator != (const iaVector3<T> &a) const
{
    if (_x != a._x ||
        _y != a._y ||
        _z != a._z)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <class T>
void iaVector3<T>::operator += (const iaVector3<T> &a)
{
    _x += a._x;
    _y += a._y;
    _z += a._z;
}

template <class T>
void iaVector3<T>::operator -= (const iaVector3<T> &a)
{
    _x -= a._x;
    _y -= a._y;
    _z -= a._z;
}

template <class T>
iaVector3<T> iaVector3<T>::operator = (const iaVector3<T> &a)
{
    iaVector3<T> result;
    result._x = _x = a._x;
    result._y = _y = a._y;
    result._z = _z = a._z;

    return result;
}

template <class T>
iaVector3<T> iaVector3<T>::operator % (const iaVector3<T> &a) const
{
    iaVector3<T> result;

    result._x = _y * a._z - _z * a._y;
    result._y = -_x * a._z + _z * a._x;
    result._z = _x * a._y - _y * a._x;

    return result;
}

template <class T>
iaVector3<T> iaVector3<T>::operator * (T a) const
{
    iaVector3<T> result;
    result._x = _x*a;
    result._y = _y*a;
    result._z = _z*a;

    return result;
}

template <class T>
iaVector3<T> iaVector3<T>::operator / (T a) const
{
    iaVector3<T> result;
    result._x = _x / a;
    result._y = _y / a;
    result._z = _z / a;

    return result;
}

template <class T>
void iaVector3<T>::operator *= (T a)
{
    _x *= a;
    _y *= a;
    _z *= a;
}

template <class T>
void iaVector3<T>::operator /= (T a)
{
    _x /= a;
    _y /= a;
    _z /= a;
}

template <class T>
T iaVector3<T>::operator * (const iaVector3<T> &a) const
{
    return _x * a._x + _y * a._y + _z * a._z;
}

template <class T>
const T* iaVector3<T>::getData() const
{
    return &_x;
}

template <class T>
T* iaVector3<T>::getData()
{
    return &_x;
}

template <class T>
void iaVector3<T>::set(T x, T y, T z)
{
    _x = x;
    _y = y;
    _z = z;
}

template <class T>
void iaVector3<T>::conjugate(void)
{
    _x *= -1;
    _y *= -1;
    _z *= -1;
}

template <class T>
T iaVector3<T>::length(void) const
{
    return (T)sqrt(_x*_x + _y*_y + _z*_z);
}

template <class T>
T iaVector3<T>::length2(void) const
{
    return (_x*_x + _y*_y + _z*_z);
}

template <class T>
T iaVector3<T>::distance(const iaVector3<T> &V) const
{
    return (T)sqrt((_x - V._x)*(_x - V._x) + (_y - V._y)*(_y - V._y) + (_z - V._z)*(_z - V._z));
}

template <class T>
T iaVector3<T>::distance2(const iaVector3<T> &V) const
{
    return ((_x - V._x)*(_x - V._x) + (_y - V._y)*(_y - V._y) + (_z - V._z)*(_z - V._z));
}

template <class T>
iaVector3<T> iaVector3<T>::negate(void)
{
    _x *= -1;
    _y *= -1;
    _z *= -1;

    return *this;
}

template <class T>
iaVector3<T> iaVector3<T>::normalize(void)
{
    T h = static_cast<T>(sqrt(static_cast<float64>(_x)*static_cast<float64>(_x) + static_cast<float64>(_y)*static_cast<float64>(_y) + static_cast<float64>(_z)*static_cast<float64>(_z)));
    _x /= h;
    _y /= h;
    _z /= h;

    return *this;
}

template <class T>
T iaVector3<T>::angle(iaVector3<T> &a) const
{
    return (T)(acos(((*this)*a) / (length()*a.length())));
}

template <class T>
T iaVector3<T>::angleX() const
{
    return (T)acos(_x / length());
}

template <class T>
T iaVector3<T>::angleY() const
{
    return (T)acos(_y / length());
}

template <class T>
T iaVector3<T>::angleZ() const
{
    return (T)(_z / length());
}
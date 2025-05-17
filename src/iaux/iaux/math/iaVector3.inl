// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

template <class T>
std::wostream &operator<<(std::wostream &ostr, const iaVector3<T> &v)
{
    ostr << "(" << v._x << ", " << v._y << ", " << v._z << ")";
    return ostr;
}

template <class T>
iaVector3<T>::iaVector3(T x, T y, T z)
{
    _x = x;
    _y = y;
    _z = z;
}

template <class T>
IGOR_INLINE iaVector3<T> iaVector3<T>::operator+(const iaVector3<T> &a) const
{
    iaVector3<T> c;

    c._x = _x + a._x;
    c._y = _y + a._y;
    c._z = _z + a._z;

    return c;
}

template <class T>
iaVector3<T> iaVector3<T>::project(const iaVector3<T> &v) const
{
    iaVector3<T> a(_x, _y, _z);
    iaVector3<T> b = v;
    T s = b.dot(a);
    s /= b.dot(b);
    return b * s;
}

template <class T>
iaVector3<T> iaVector3<T>::reject(const iaVector3<T> &v)
{
    return (*this) - project(v);
}

template <class T>
T &iaVector3<T>::operator[](int i)
{
    return (&_x)[i];
}

template <class T>
const T &iaVector3<T>::operator[](int i) const
{
    return (&_x)[i];
}

template <class T>
IGOR_INLINE iaVector3<T> iaVector3<T>::operator-(const iaVector3<T> &a) const
{
    iaVector3<T> c;
    c._x = _x - a._x;
    c._y = _y - a._y;
    c._z = _z - a._z;

    return c;
}

template <class T>
bool iaVector3<T>::operator==(const iaVector3<T> &a) const
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
bool iaVector3<T>::operator!=(const iaVector3<T> &a) const
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
IGOR_INLINE void iaVector3<T>::operator+=(const iaVector3<T> &a)
{
    _x += a._x;
    _y += a._y;
    _z += a._z;
}

template <class T>
IGOR_INLINE void iaVector3<T>::operator-=(const iaVector3<T> &a)
{
    _x -= a._x;
    _y -= a._y;
    _z -= a._z;
}

template <class T>
iaVector3<T> iaVector3<T>::operator=(const iaVector3<T> &a)
{
    iaVector3<T> result;
    result._x = _x = a._x;
    result._y = _y = a._y;
    result._z = _z = a._z;

    return result;
}

template <class T>
IGOR_INLINE iaVector3<T> iaVector3<T>::operator%(const iaVector3<T> &a) const
{
    iaVector3<T> result;

    result._x = _y * a._z - _z * a._y;
    result._y = -_x * a._z + _z * a._x;
    result._z = _x * a._y - _y * a._x;

    return result;
}

template <class T>
IGOR_INLINE iaVector3<T> iaVector3<T>::operator*(T a) const
{
    iaVector3<T> result;
    result._x = _x * a;
    result._y = _y * a;
    result._z = _z * a;

    return result;
}

template <class T>
IGOR_INLINE iaVector3<T> iaVector3<T>::operator/(T a) const
{
    iaVector3<T> result;
    result._x = _x / a;
    result._y = _y / a;
    result._z = _z / a;

    return result;
}

template <class T>
IGOR_INLINE void iaVector3<T>::operator*=(T a)
{
    _x *= a;
    _y *= a;
    _z *= a;
}

template <class T>
IGOR_INLINE void iaVector3<T>::operator/=(T a)
{
    _x /= a;
    _y /= a;
    _z /= a;
}

template <class T>
IGOR_INLINE T iaVector3<T>::dot(const iaVector3<T> &a) const
{
    return _x * a._x + _y * a._y + _z * a._z;
}

template <class T>
const T *iaVector3<T>::getData() const
{
    return &_x;
}

template <class T>
T *iaVector3<T>::getData()
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
IGOR_INLINE T iaVector3<T>::length() const
{
    return (T)sqrt(_x * _x + _y * _y + _z * _z);
}

template <class T>
IGOR_INLINE T iaVector3<T>::length2() const
{
    return (_x * _x + _y * _y + _z * _z);
}

template <class T>
IGOR_INLINE bool iaVector3<T>::zero() const
{
    return _x == 0.0 && _y == 0.0 && _z == 0.0;
}

template <class T>
IGOR_INLINE T iaVector3<T>::distance(const iaVector3<T> &V) const
{
    return static_cast<T>(sqrt((_x - V._x) * (_x - V._x) + (_y - V._y) * (_y - V._y) + (_z - V._z) * (_z - V._z)));
}

template <class T>
IGOR_INLINE T iaVector3<T>::distance2(const iaVector3<T> &V) const
{
    return ((_x - V._x) * (_x - V._x) + (_y - V._y) * (_y - V._y) + (_z - V._z) * (_z - V._z));
}

template <class T>
IGOR_INLINE void iaVector3<T>::negate()
{
    _x *= -1;
    _y *= -1;
    _z *= -1;
}

template <class T>
void iaVector3<T>::normalize()
{
    if(_x == 0 && _y == 0 && _z == 0)
    {
        return;
    }

    T h = static_cast<T>(sqrt(static_cast<float64>(_x) * static_cast<float64>(_x) + static_cast<float64>(_y) * static_cast<float64>(_y) + static_cast<float64>(_z) * static_cast<float64>(_z)));
    _x /= h;
    _y /= h;
    _z /= h;
}

template <class T>
T iaVector3<T>::angle(const iaVector3<T> &a) const
{
    return (T)(acos(((*this).dot(a)) / (length() * a.length())));
}

template <class T>
T iaVector3<T>::angleXZ() const
{
    iaVector3<T> temp(_x, 0, _z);
    temp.normalize();

    float64 gk = -temp._z;
    float64 heading = asin(gk);

    if (temp._x < 0)
    {
        heading = M_PI - heading;
    }

    return heading;
}

template <class T>
template <class T2>
iaVector3<T2> iaVector3<T>::convert() const
{
    iaVector3<T2> result(static_cast<T2>(_x), static_cast<T2>(_y), static_cast<T2>(_z));
    return result;
}

template <class T>
iaVector3<T> lerp(const iaVector3<T> &a, const iaVector3<T> &b, T t)
{
    iaVector3<T> result;
    // (1 - t) * v0 + t * v1;
    result._x = b._x * t + a._x * (1.0 - t);
    result._y = b._y * t + a._y * (1.0 - t);
    result._z = b._z * t + a._z * (1.0 - t);

    return result;
}

template <class T>
IGOR_INLINE iaVector3<T> iaVector3<T>::operator*(const iaVector3<T> &vec) const
{
    iaVector3<T> result;
    result._x = _x * vec._x;
    result._y = _y * vec._y;
    result._z = _z * vec._z;
    return result;
}

template <class T>
IGOR_INLINE void iaVector3<T>::operator*=(const iaVector3<T> &vec)
{
    _x *= vec._x;
    _y *= vec._y;
    _z *= vec._z;
}
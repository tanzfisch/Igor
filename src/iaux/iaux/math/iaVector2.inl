// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

template <class T>
IGOR_INLINE std::wostream &operator<<(std::wostream &ostr, const iaVector2<T> &vec)
{
    ostr << "(" << vec._x << ", " << vec._y << ")";
    return ostr;
}

template <class T>
IGOR_INLINE T &iaVector2<T>::operator[](int index)
{
    return (&_x)[index];
}

template <class T>
IGOR_INLINE const T &iaVector2<T>::operator[](int index) const
{
    return (&_x)[index];
}

template <class T>
IGOR_INLINE iaVector2<T> iaVector2<T>::operator+(const iaVector2<T> &vec) const
{
    iaVector2<T> result;
    result._x = _x + vec._x;
    result._y = _y + vec._y;

    return result;
}

template <class T>
IGOR_INLINE bool iaVector2<T>::operator==(const iaVector2<T> &vec) const
{
    return ((_x == vec._x) && (_y == vec._y)) ? true : false;
}

template <class T>
IGOR_INLINE bool iaVector2<T>::operator!=(const iaVector2<T> &vec) const
{
    return ((_x != vec._x) || (_y != vec._y)) ? true : false;
}

template <class T>
IGOR_INLINE void iaVector2<T>::operator+=(const iaVector2<T> &vec)
{
    _x += vec._x;
    _y += vec._y;
}

template <class T>
IGOR_INLINE iaVector2<T> iaVector2<T>::operator-(const iaVector2<T> &vec) const
{
    iaVector2<T> result;
    result._x = _x - vec._x;
    result._y = _y - vec._y;

    return result;
}

template <class T>
IGOR_INLINE void iaVector2<T>::operator-=(const iaVector2<T> &vec)
{
    _x -= vec._x;
    _y -= vec._y;
}

template <class T>
IGOR_INLINE iaVector2<T> iaVector2<T>::operator=(const iaVector2<T> &vec)
{
    iaVector2<T> result;
    result._x = _x = vec._x;
    result._y = _y = vec._y;

    return result;
}

template <class T>
IGOR_INLINE iaVector2<T> iaVector2<T>::operator*(T scalar) const
{
    iaVector2<T> result;
    result._x = _x * scalar;
    result._y = _y * scalar;

    return result;
}

template <class T>
IGOR_INLINE void iaVector2<T>::operator*=(T scalar)
{
    _x *= scalar;
    _y *= scalar;
}

template <class T>
IGOR_INLINE iaVector2<T> iaVector2<T>::operator/(T scalar) const
{
    iaVector2<T> result;
    result._x = _x / scalar;
    result._y = _y / scalar;

    return result;
}

template <class T>
IGOR_INLINE void iaVector2<T>::operator/=(T scalar)
{
    _x /= scalar;
    _y /= scalar;
}

template <class T>
IGOR_INLINE T iaVector2<T>::dot(const iaVector2<T> &vec) const
{
    return _x * vec._x + _y * vec._y;
}

template <class T>
IGOR_INLINE iaVector2<T> iaVector2<T>::operator*(const iaVector2<T> &vec) const
{
    iaVector2<T> result;
    result._x = _x * vec._x;
    result._y = _y * vec._y;
    return result;
}

template <class T>
IGOR_INLINE void iaVector2<T>::operator*=(const iaVector2<T> &vec)
{
    _x *= vec._x;
    _y *= vec._y;
}

template <class T>
IGOR_INLINE iaVector2<T>::iaVector2(T x, T y)
{
    _x = x;
    _y = y;
}

template <class T>
IGOR_INLINE T *iaVector2<T>::getData()
{
    return &_x;
}

template <class T>
IGOR_INLINE const T *iaVector2<T>::getData() const
{
    return &_x;
}

template <class T>
IGOR_INLINE void iaVector2<T>::set(T x, T y)
{
    _x = x;
    _y = y;
}

template <class T>
IGOR_INLINE T iaVector2<T>::length() const
{
    return (T)sqrt(_x * _x + _y * _y);
}

template <class T>
IGOR_INLINE T iaVector2<T>::length2() const
{
    return (_x * _x + _y * _y);
}

template <class T>
IGOR_INLINE bool iaVector2<T>::zero() const
{
    return _x == 0.0 && _y == 0.0;
}

template <class T>
IGOR_INLINE T iaVector2<T>::distance(const iaVector2<T> &vec) const
{
    T dx = _x - vec._x;
    T dy = _y - vec._y;
    return (T)sqrt(dx * dx + dy * dy);
}

template <class T>
IGOR_INLINE T iaVector2<T>::distance2(const iaVector2<T> &vec) const
{
    T dx = _x - vec._x;
    T dy = _y - vec._y;
    return dx * dx + dy * dy;
}

template <class T>
IGOR_INLINE T iaVector2<T>::angle(const iaVector2<T> &vec) const
{
    return (T)(acos(((*this).dot(vec)) / (length() * vec.length())));
}

template <class T>
IGOR_INLINE T iaVector2<T>::angle() const
{
    return (T)std::atan2(_y, _x);
}

template <class T>
IGOR_INLINE void iaVector2<T>::rotateXY(T angle)
{
    T c = (T)cos(angle);
    T s = (T)sin(angle);

    T t = c * _x - s * _y;
    _y = s * _x + c * _y;
    _x = t;
}

template <class T>
IGOR_INLINE void iaVector2<T>::normalize()
{
    if (_x == 0 && _y == 0)
    {
        return;
    }

    T h = (T)sqrt(_x * _x + _y * _y);
    _x /= h;
    _y /= h;
}

template <class T>
IGOR_INLINE void iaVector2<T>::negate()
{
    _x *= -1;
    _y *= -1;
}

template <class T>
template <class T2>
IGOR_INLINE iaVector2<T2> iaVector2<T>::convert() const
{
    iaVector2<T2> result(static_cast<T2>(_x), static_cast<T2>(_y));
    return result;
}

template <class T>
iaVector2<T> lerp(const iaVector2<T> &a, const iaVector2<T> &b, T t)
{
    iaVector2<T> result;
    // (1 - t) * v0 + t * v1;
    result._x = b._x * t + a._x * (1.0 - t);
    result._y = b._y * t + a._y * (1.0 - t);

    return result;
}

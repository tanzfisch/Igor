// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
__IGOR_INLINE__ std::wostream& operator<<(std::wostream &ostr, const iaVector2<T> &vec)
{
    ostr << "(" << vec._x << ", " << vec._y << ")";
    return ostr;
}

template <class T>
T& iaVector2<T>::operator[] (int index)
{
    return (&_x)[index];
}

template <class T>
const T& iaVector2<T>::operator[] (int index) const
{
    return (&_x)[index];
}

template <class T>
iaVector2<T> iaVector2<T>::operator+  (const iaVector2<T> &vec) const
{
    iaVector2<T> result;
    result._x = _x + vec._x;
    result._y = _y + vec._y;

    return result;
}

template <class T>
bool iaVector2<T>::operator == (const iaVector2<T> &vec) const
{
    return ((_x == vec._x) && (_y == vec._y)) ? true : false;
}

template <class T>
bool iaVector2<T>::operator != (const iaVector2<T> &vec) const
{
    return ((_x != vec._x) || (_y != vec._y)) ? true : false;
}

template <class T>
void iaVector2<T>::operator+= (const iaVector2<T> &vec)
{
    _x += vec._x;
    _y += vec._y;
}

template <class T>
iaVector2<T> iaVector2<T>::operator- (const iaVector2<T> &vec) const
{
    iaVector2<T> result;
    result._x = _x - vec._x;
    result._y = _y - vec._y;

    return result;
}

template <class T>
void iaVector2<T>::operator-= (const iaVector2<T> &vec)
{
    _x -= vec._x;
    _y -= vec._y;
}

template <class T>
iaVector2<T> iaVector2<T>::operator= (const iaVector2<T> &vec)
{
    iaVector2<T> result;
    result._x = _x = vec._x;
    result._y = _y = vec._y;

    return result;
}

template <class T>
iaVector2<T> iaVector2<T>::operator* (T scalar) const
{
    iaVector2<T> result;
    result._x = _x * scalar;
    result._y = _y * scalar;

    return result;
}

template <class T>
void iaVector2<T>::operator*= (T scalar)
{
    _x *= scalar;
    _y *= scalar;
}

template <class T>
iaVector2<T> iaVector2<T>::operator/ (T scalar) const
{
    iaVector2<T> result;
    result._x = _x / scalar;
    result._y = _y / scalar;

    return result;
}

template <class T>
void iaVector2<T>::operator/= (T scalar)
{
    _x /= scalar;
    _y /= scalar;
}

template <class T>
T iaVector2<T>::operator* (const iaVector2<T> &vec) const
{
    return _x*vec._x + _y*vec._y;
}

template <class T>
iaVector2<T>::iaVector2(T x, T y)
{
    _x = x;
    _y = y;
}

template <class T>
T* iaVector2<T>::getData()
{
    return &_x;
}

template <class T>
const T* iaVector2<T>::getData() const
{
    return &_x;
}

template <class T>
void iaVector2<T>::set(T x, T y)
{
    _x = x;
    _y = y;
}

template <class T>
T iaVector2<T>::length(void) const
{
    return (T)sqrt(_x*_x + _y*_y);
}

template <class T>
T iaVector2<T>::length2(void) const
{
    return (_x*_x + _y*_y);
}

template <class T>
T iaVector2<T>::distance(iaVector2<T> &vec) const
{
    T dx = _x - vec._x;
    T dy = _y - vec._y;
    return (T)sqrt(dx*dx + dy*dy);
}

template <class T>
T iaVector2<T>::distance2(iaVector2<T> &vec) const
{
    T dx = _x - vec._x;
    T dy = _y - vec._y;
    return dx*dx + dy*dy;
}

template <class T>
T iaVector2<T>::angle(iaVector2<T> &vec) const
{
    return (T)(acos(((*this)*vec) / (length()*vec.length())));
}

template <class T>
T iaVector2<T>::angleX() const
{
    return (T)acos(_x / length());
}

template <class T>
T iaVector2<T>::angleY() const
{
    return (T)acos(_y / length());
}

template <class T>
void  iaVector2<T>::rotateXY(T angle)
{
    T c = (T)cos(angle);
    T s = (T)sin(angle);

    T t = c*_x + s*_y;
    _y = -s*_x + c*_y;
    _x = t;
}

template <class T>
void iaVector2<T>::normalize(void)
{
    T h = (T)sqrt(_x*_x + _y*_y);
    _x /= h;
    _y /= h;
}

template <class T>
void iaVector2<T>::conjugate(void)
{
    _x *= -1;
    _y *= -1;
}
// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
std::wostream &operator<<(std::wostream &ostr, const iaCircle<T> &circle)
{
    ostr << "(" << circle._center._x << ", " << circle._center._y << ", r:" << circle._radius << ")";
    return ostr;
}

template <class T>
const T iaCircle<T>::getX() const
{
    return _center._x;
}

template <class T>
const T iaCircle<T>::getY() const
{
    return _center._y;
}

template <class T>
const T iaCircle<T>::getRadius() const
{
    return _radius;
}

template <class T>
const iaVector2<T> &iaCircle<T>::getCenter() const
{
    return _center;
}

template <class T>
void iaCircle<T>::setX(T x)
{
    _center._x = x;
}

template <class T>
void iaCircle<T>::setY(T y)
{
    _center._y = y;
}

template <class T>
void iaCircle<T>::setRadius(T radius)
{
    _radius = radius;
}

template <class T>
void iaCircle<T>::setCenter(const iaVector2<T> &center)
{
    _center = center;
}

template <class T>
void iaCircle<T>::set(const iaVector2<T> &center, T radius)
{
    _center = center;
    _radius = radius;
}
template <class T>
void iaCircle<T>::set(T x, T y, T radius)
{
    _center.set(x, y);
    _radius = radius;
}

template <class T>
iaCircle<T> iaCircle<T>::operator=(const iaCircle<T> &circle)
{
    iaCircle<T> result;
    result._center = _center = circle._center;
    result._radius = _radius = circle._radius;

    return result;
}

template <class T>
bool iaCircle<T>::operator==(const iaCircle<T> &circle) const
{
    return ((_center == circle._center) && (_radius == circle._radius)) ? true : false;
}

template <class T>
bool iaCircle<T>::operator!=(const iaCircle<T> &circle) const
{
    return ((_center != circle._center) || (_radius != circle._radius)) ? true : false;
}
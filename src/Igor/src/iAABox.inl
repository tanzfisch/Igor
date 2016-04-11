// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
iAABox<T>::iAABox(const iaVector3<T>& center, const iaVector3<T>& halfWidths)
    : _center(center)
    , _halfWidths(halfWidths)
{

}

template <class T>
void iAABox<T>::set(const iaVector3<T>& min, const iaVector3<T>& max)
{
    _halfWidths = max;
    _halfWidths -= min;

    _center = _halfWidths;
    _center *= 0.5;
    _center += min;
}

template <class T>
__IGOR_INLINE__ bool iAABox<T>::intersects(const iAABox<T> &box)
{
    iaVector3<T> sum = _halfWidths;
    sum += box._halfWidths;

    if (abs(_center._x - box._center._x) < sum._x)
    {
        if (abs(_center._y - box._center._y) < sum._y)
        {
            if (abs(_center._z - box._center._z) < sum._z)
            {
                return true;
            }
        }
    }
    return false;
}

template <class T>
__IGOR_INLINE__ bool iAABox<T>::intersects(const iaVector3<T> &vec)
{
    if (abs(_center._x - vec._x) < _halfWidths._x)
    {
        if (abs(_center._y - vec._y) < _halfWidths._y)
        {
            if (abs(_center._z - vec._z) < _halfWidths._z)
            {
                return true;
            }
        }
    }
    return false;
}

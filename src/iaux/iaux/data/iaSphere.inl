// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
iaSphere<T>::iaSphere(iaVector3<T> pos, T radius)
{
    _center = pos;
    _radius = radius;
}

template <class T>
bool iaSphere<T>::contains(const iaSphere<T> &sphere)
{
    iaVector3<T> diff = sphere._center - _center;
    T distance = diff.length();

    if (distance + sphere._radius <= _radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <class T>
bool iaSphere<T>::operator==(const iaSphere<T> &sphere) const
{
    return ((_center == sphere._center) && (_radius == sphere._radius)) ? true : false;
}

template <class T>
bool iaSphere<T>::operator!=(const iaSphere<T> &sphere) const
{
    return ((_center != sphere._center) || (_radius != sphere._radius)) ? true : false;
}

template <class T>
void iaSphere<T>::merge(iaSphere<T> &sphere)
{
    if (contains(sphere))
    {
        return;
    }

    if (sphere.contains(*this))
    {
        _center = sphere._center;
        _radius = sphere._radius;
        return;
    }

    iaVector3<T> newCenterOffset = sphere._center - _center;
    T newRadius = _radius + sphere._radius + newCenterOffset.length();
    newRadius *= static_cast<T>(0.5);
    newCenterOffset.normalize();
    newCenterOffset *= newRadius - _radius;
    _radius = newRadius;
    _center += newCenterOffset;
}

// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
iSphere<T>::iSphere(iaVector3<T> pos, T radius)
{
	_center = pos;
	_radius = radius;
}

template <class T>
bool iSphere<T>::contains(const iSphere<T>& sphere)
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
bool iSphere<T>::operator == (const iSphere<T> &sphere) const
{
    return ((_center == sphere._center) && (_radius == sphere._radius)) ? true : false;
}

template <class T>
bool iSphere<T>::operator != (const iSphere<T> &sphere) const
{
    return ((_center != sphere._center) || (_radius != sphere._radius)) ? true : false;
}

template <class T>
void iSphere<T>::merge(iSphere<T> &sphere)
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

    iaVector3<T> newCenter = sphere._center - _center;
    T newRadius = _radius + sphere._radius + newCenter.length();
    newRadius *= static_cast<T>(0.5);
    newCenter.normalize();
    newCenter *= newRadius - _radius;
    _radius = newRadius;
    _center += newCenter;
}



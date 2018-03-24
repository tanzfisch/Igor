// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
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
    _center = min;
    _center += max;
    _center *= 0.5;

    _halfWidths = max;
    _halfWidths -= min;
    _halfWidths *= 0.5;
}


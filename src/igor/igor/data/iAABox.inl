// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

template <class T>
iAABox<T>::iAABox(const iaVector3<T>& center, const iaVector3<T>& halfWidths)
    : _center(center)
    , _halfWidths(halfWidths)
{

}

template <class T>
iAABox<T> iAABox<T>::fromMinMax(const iaVector3<T> &min, const iaVector3<T> &max)
{
    iAABox result;
    result.setMinMax(min, max);

    return result;
}

template <class T>
void iAABox<T>::setMinMax(const iaVector3<T>& min, const iaVector3<T>& max)
{
    _center = min;
    _center += max;
    _center *= 0.5;

    _halfWidths = max;
    _halfWidths -= min;
    _halfWidths *= 0.5;
}

template <class T>
void iAABox<T>::getMinMax(iaVector3<T> &min, iaVector3<T> &max)
{
    min = _center - _halfWidths;
    max = _center + _halfWidths;
}

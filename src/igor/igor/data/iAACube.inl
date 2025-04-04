// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
iAACube<T>::iAACube(const iaVector3<T> &center, T halfEdgeLenght)
    : _center(center), _halfEdgeLength(halfEdgeLenght)
{
}

template <class T>
const iaVector3<T> &iAACube<T>::getCenter() const
{
    return _center;
}

template <class T>
const T &iAACube<T>::getHalfEdge() const
{
    return _halfEdgeLength;
}
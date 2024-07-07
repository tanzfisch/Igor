// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
iPlane<T>::iPlane(iaVector3<T> normal, T distance)
{
    _distance = distance;
    _normal = normal;
}

/*! source https://graphics.stanford.edu/~mdfisher/Code/Engine/Plane.cpp.html
*/
template <class T>
IGOR_INLINE void iPlane<T>::normalize()
{
    T h = (T)sqrt(_normal._x*_normal._x + _normal._y*_normal._y + _normal._z*_normal._z);
    _normal._x /= h;
    _normal._y /= h;
    _normal._z /= h;
    _distance /= h;
}

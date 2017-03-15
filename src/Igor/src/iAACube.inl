// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
iAACube<T>::iAACube(const iaVector3<T>& center, T halfEdgeLenght)
    : _center(center)
    , _halfEdgeLength(halfEdgeLenght)
{
}

template <class T>
__IGOR_INLINE__ bool iAACube<T>::inFrontOf(const iPlane<T> &plane)
{
    //! \todo calculation is not precise
#define SQRT2 3 // 1.414213562373095

    T distancePlanePoint = (plane._normal * _center) + plane._distance;
    if (distancePlanePoint <= -_halfEdgeLength * SQRT2)
    {
        return false;
    }
    else
    {
        return true;
    }
}

template <class T>
__IGOR_INLINE__ bool iAACube<T>::intersects(const iSphere<T> &sphere)
{
    T maxDistance = _halfEdgeLength + sphere._radius;

    if (abs(_center._x - sphere._center._x) < maxDistance)
    {
        if (abs(_center._y - sphere._center._y) < maxDistance)
        {
            if (abs(_center._z - sphere._center._z) < maxDistance)
            {
                return true;
            }
        }
    }
    return false;
}

template <class T>
__IGOR_INLINE__ bool iAACube<T>::intersects(const iFrustum<T> &frustum)
{
    if (!inFrontOf(frustum._nearPlane))
    {
        return false;
    }

    if (!inFrontOf(frustum._leftPlane))
    {
        return false;
    }

    if (!inFrontOf(frustum._rightPlane))
    {
        return false;
    }

    if (!inFrontOf(frustum._bottomPlane))
    {
        return false;
    }

    if (!inFrontOf(frustum._topPlane))
    {
        return false;
    }

    if (!inFrontOf(frustum._farPlane))
    {
        return false;
    }

    return true;
}

// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
iSphere<T>::iSphere(iaVector3<T> pos, T radius)
{
	_center = pos;
	_radius = radius;
}

template <class T>
__IGOR_INLINE__ bool iSphere<T>::intersects(iSphere<T> &sphere)
{
    iaVector3<T> diff = sphere._center - _center;
    T distance = diff.length();
    distance -= _radius;
    distance -= sphere._radius;
    if (distance <= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <class T>
__IGOR_INLINE__ bool iSphere<T>::contains(iSphere<T> &sphere)
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
__IGOR_INLINE__ void iSphere<T>::merge(iSphere<T> &sphere)
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

    iaVector3f newCenter = sphere._center - _center;
    T newRadius = _radius + sphere._radius + newCenter.length();
    newRadius *= static_cast<T>(0.5);
    newCenter.normalize();
    newCenter *= newRadius - _radius;
    _radius = newRadius;
    _center += newCenter;
}

template <class T>
__IGOR_INLINE__ bool iSphere<T>::inFrontOf(iPlane<T> &plane)
{
	T distancePlanePoint = (plane.normal * _center) + plane.distance;
	if(distancePlanePoint <= -_radius) 
	{
		return false;
	}
	else 
	{
		return true;
	}
}

template <class T>
__IGOR_INLINE__ bool iSphere<T>::intersects(iFrustum<T> &frustum)
{
	if(!inFrontOf(frustum.near_plane)) 
		return false;
	if(!inFrontOf(frustum.left_plane)) 
		return false;
	if(!inFrontOf(frustum.right_plane)) 
		return false;
	if(!inFrontOf(frustum.bottom_plane)) 
		return false;
	if(!inFrontOf(frustum.top_plane)) 
		return false;
	if(!inFrontOf(frustum.far_plane)) 
		return false;		
	return true;
}

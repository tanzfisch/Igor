// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
__IGOR_INLINE__ std::wostream& operator<<(std::wostream& ostr, const iaQuaternion<T>& q)
{
	ostr << "(" << q._x << ", " << q._y << ", " << q._z << ", " << q._w << ")";
	return ostr;
}

/*template <class T>
iaQuaternion<T> slerp(iaQuaternion<T> qa, iaQuaternion<T> qb, T t)
{
    iaQuaternion<T> qm;
    // Calculate angle between them.
    T cosHalfTheta = qa.w * qb.w + qa.x * qb.x + qa.y * qb.y + qa.z * qb.z;

    // if qa=qb or qa=-qb then theta = 0 and we can return qa
    if (abs(cosHalfTheta) >= 1.0)
    {
        qm.w = qa.w;qm.x = qa.x;qm.y = qa.y;qm.z = qa.z;
        return qm;
    }

    // Calculate temporary values.
    double halfTheta = acos(cosHalfTheta);
    double sinHalfTheta = sqrt(1.0 - cosHalfTheta*cosHalfTheta);
    // if theta = 180 degrees then result is not fully defined
    // we could rotate around any axis normal to qa or qb
    if (fabs(sinHalfTheta) < 0.001){ // fabs is float32ing point absolute
        qm.w = (qa.w * 0.5 + qb.w * 0.5);
        qm.x = (qa.x * 0.5 + qb.x * 0.5);
        qm.y = (qa.y * 0.5 + qb.y * 0.5);
        qm.z = (qa.z * 0.5 + qb.z * 0.5);
        return qm;
    }
    double ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
    double ratioB = sin(t * halfTheta) / sinHalfTheta;
    //calculate iaQuaternion.
    qm.w = (qa.w * ratioA + qb.w * ratioB);
    qm.x = (qa.x * ratioA + qb.x * ratioB);
    qm.y = (qa.y * ratioA + qb.y * ratioB);
    qm.z = (qa.z * ratioA + qb.z * ratioB);
    return qm;
}*/


/*template <class T>
void iaQuaternion<T>::slerp(iaQuaternion<T> q1, iaQuaternion<T> q2, T t)
{
	T angle = q1.dot(q2);

	if (angle < (T)0.0)
	{
		q1 *= (T)-1.0;
		angle *= (T)-1.0;
	}

	T scale;
	T invscale;

	if ((angle + (T)1.0) >(T)0.05)
	{
		if (((T)1.0 - angle) >= (T)0.05)  // spherical interpolation
		{
			T theta = (T)acos(angle);
			T invsintheta = (T)1.0 / (T)sin(theta);
			scale = (T)sin(theta * ((T)1.0 - t)) * invsintheta;
			invscale = (T)sin(theta * t) * invsintheta;
		}
		else // linear interploation
		{
			scale = (T)1.0 - t;
			invscale = t;
		}
	}
	else
	{
		q2.set(-q1._y, q1._x, -q1._w, q1._z);
		scale = (T)sin((T)M_PI * ((T)0.5 - t));
		invscale = (T)sin((T)M_PI * t);
	}

	*this = (q1*scale) + (q2*invscale);
}*/

template <class T>
T& iaQuaternion<T>::operator[] (int i)
{
	return (&_x)[i];
}

template <class T>
const T& iaQuaternion<T>::operator[] (int i) const
{
	return (&_x)[i];
}

template <class T>
void iaQuaternion<T>::set(T x, T y, T z, T w)
{
    _x = x;
    _y = y;
    _z = z;
    _w = w;
}

template <class T>
iaQuaternion<T> iaQuaternion<T>::operator +	(iaQuaternion<T> &a)
{
    iaQuaternion<T> c;

    c._x = _x + a._x;
    c._y = _y + a._y;
    c._z = _z + a._z;
    c._w = _w + a._w;

    return c;
}

template <class T>
void iaQuaternion<T>::operator += (iaQuaternion<T> &a)
{
    _x += a._x;
    _y += a._y;
    _z += a._z;
    _w += a._w;
}

template <class T>
iaQuaternion<T> iaQuaternion<T>::operator -	(iaQuaternion<T> &a)
{
    iaQuaternion<T> c;

    c._x = _x - a._x;
    c._y = _y - a._y;
    c._z = _z - a._z;
    c._w = _w - a._w;

    return c;
}

template <class T>
void iaQuaternion<T>::operator -= (iaQuaternion<T> &a)
{
    _x -= a._x;
    _y -= a._y;
    _z -= a._z;
    _w -= a._w;
}

template <class T>
iaQuaternion<T> iaQuaternion<T>::operator * (iaQuaternion<T> &a)
{
    iaQuaternion<T> c;

    c._x = _w * a._x + _x * a._w + _y * a._z - _z * a._y;
    c._y = _w * a._y - _x * a._z + _y * a._w + _z * a._x;
    c._z = _w * a._z + _x * a._y - _y * a._x + _z * a._w;
    c._w = _w * a._w - _x * a._x - _y * a._y - _z * a._z;

    return c;
}

template <class T>
iaQuaternion<T> iaQuaternion<T>::operator * (T a)
{
    iaQuaternion<T> c;
    c._x = _x*a;
    c._y = _y*a;
    c._z = _z*a;
    c._w = _w*a;

    return c;
}

template <class T>
void iaQuaternion<T>::operator *= (T a)
{
    _x *= a;
    _y *= a;
    _z *= a;
    _w *= a;
}

template <class T>
T iaQuaternion<T>::length(void)
{
    return (T)sqrt(_x*_x + _y*_y + _z*_z + _w*_w);
}

template <class T>
T iaQuaternion<T>::dot(iaQuaternion<T> &a)
{
    return a._x*_x + a._y*_y + a._z*_z + a._w*_w;
}

template <class T>
T iaQuaternion<T>::length2(void)
{
    return (_x*_x + _y*_y + _z*_z + _w*_w);
}

template <class T>
void iaQuaternion<T>::conjugate(void)
{
    _x *= -1;
    _y *= -1;
    _z *= -1;
    _w *= -1;
}

template <class T>
void iaQuaternion<T>::normalize(void)
{
    T h = _x*_x + _y*_y + _z*_z + _w*_w;
    _x /= h;
    _y /= h;
    _z /= h;
    _w /= h;
}

template <class T>
iaQuaternion<T>::iaQuaternion(void)
{
    _x = 0;
    _y = 0;
    _z = 0;
    _w = 0;
}

template <class T>
void iaQuaternion<T>::setEuler(T x, T y, T z)
{
	// Abbreviations for the various angular functions
	double cy = cos(z * 0.5);
	double sy = sin(z * 0.5);
	double cp = cos(y * 0.5);
	double sp = sin(y * 0.5);
	double cr = cos(x * 0.5);
	double sr = sin(x * 0.5);

	_w = cy * cp * cr + sy * sp * sr;
	_x = cy * cp * sr - sy * sp * cr;
	_y = sy * cp * sr + cy * sp * cr;
	_z = sy * cp * cr - cy * sp * sr;
}

template <class T>
void iaQuaternion<T>::getEuler(T& x, T& y, T& z) const
{
	// roll (x-axis rotation)
	double sinr_cosp = +2.0 * (_w * _x + _y * _z);
	double cosr_cosp = +1.0 - 2.0 * (_x * _x + _y * _y);
	x = atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = +2.0 * (_w * _y - _z * _x);
	if (fabs(sinp) >= 1)
		y = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
	else
		y = asin(sinp);

	// yaw (z-axis rotation)
	double siny_cosp = +2.0 * (_w * _z + _x * _y);
	double cosy_cosp = +1.0 - 2.0 * (_y * _y + _z * _z);
	z = atan2(siny_cosp, cosy_cosp);
}

template <class T>
void iaQuaternion<T>::setEuler(const iaVector3<T>& vec)
{
	setEuler(vec._x, vec._y, vec._z);
}

template <class T>
void iaQuaternion<T>::getEuler(iaVector3<T>& vec) const
{
	getEuler(vec._x, vec._y, vec._z);
}

template <class T>
iaQuaternion<T>::iaQuaternion(T x, T y, T z, T w)
{
    _x = x;
    _y = y;
    _z = z;
    _w = w;
}

template <class T>
iaQuaternion<T>::~iaQuaternion(void)
{
}

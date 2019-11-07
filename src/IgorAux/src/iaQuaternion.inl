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
iaQuaternion<T>::iaQuaternion()
{
    identity();
}

template <class T>
iaQuaternion<T>::iaQuaternion(T x, T y, T z, T w)
{
    set(x, y, z, w);
}

template <class T>
iaQuaternion<T>::iaQuaternion(const iaVector3<T>& vec)
{
    setEuler(vec);
}

template <class T>
iaQuaternion<T>::iaQuaternion(const iaVector3<T>& axis, T angle)
{
    setAxisAngle(axis, angle);
}

template <class T>
iaQuaternion<T>::iaQuaternion(T x, T y, T z)
{
    setEuler(x, y, z);
}

template <class T>
iaQuaternion<T> iaQuaternion<T>::operator*(T rhs)
{
    iaQuaternion<T> result;

    result._x = _x * rhs;
    result._y = _y * rhs;
    result._z = _z * rhs;
    result._w = _w * rhs;

    return result;
}

template <class T>
void iaQuaternion<T>::operator*=(T rhs)
{
    _x *= rhs;
    _y *= rhs;
    _z *= rhs;
    _w *= rhs;
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
void iaQuaternion<T>::setAxisAngle(const iaVector3<T>& axis, T angle)
{
    T sha = sin(angle * 0.5);

    _w = cos(angle * 0.5);
    _x = axis._x * sha;
    _y = axis._y * sha;
    _z = axis._z * sha;
}

template <class T>
void iaQuaternion<T>::getAxisAngle(iaVector3<T>& axis, T& angle) const
{
    T scale = sqrt(_x * _x + _y * _y + _z * _z);

    if (abs(scale) < 0.000001 ||
        _w > 1.0 ||
        _w < -1.0)
    {
        angle = 0.0;
        axis.set(0.0, 1.0, 0.0);
    }
    else
    {
        T inv = 1.0 / scale;
        angle = 2.0f * acosf(_w);
        axis.set(_x * inv, _y * inv, _z * inv);
    }
}

template <class T>
void iaQuaternion<T>::negate()
{
    _x *= -1;
    _y *= -1;
    _z *= -1;
}

template <class T>
void iaQuaternion<T>::identity()
{
    _x = 0;
    _y = 0;
    _z = 0;
    _w = 1;
}

template <class T>
bool iaQuaternion<T>::operator==(const iaQuaternion<T>& rhs) const
{
    return ((_x == rhs._x) &&
        (_y == rhs._y) &&
        (_z == rhs._z) &&
        (_w == rhs._w));
}

template <class T>
bool iaQuaternion<T>::operator!=(const iaQuaternion<T>& rhs) const
{
    return ((_x != rhs._x) ||
        (_y != rhs._y) ||
        (_z != rhs._z) ||
        (_w != rhs._w));
}

template <class T>
iaQuaternion<T> iaQuaternion<T>::operator+(const iaQuaternion<T>& rhs)
{
    iaQuaternion<T> c;

    c._x = _x + rhs._x;
    c._y = _y + rhs._y;
    c._z = _z + rhs._z;
    c._w = _w + rhs._w;

    return c;
}

template <class T>
void iaQuaternion<T>::operator+=(const iaQuaternion<T>& rhs)
{
    _x += rhs._x;
    _y += rhs._y;
    _z += rhs._z;
    _w += rhs._w;
}

template <class T>
iaQuaternion<T> iaQuaternion<T>::operator-(const iaQuaternion<T>& rhs)
{
    iaQuaternion<T> c;

    c._x = _x - rhs._x;
    c._y = _y - rhs._y;
    c._z = _z - rhs._z;
    c._w = _w - rhs._w;

    return c;
}

template <class T>
void iaQuaternion<T>::operator-=(const iaQuaternion<T>& rhs)
{
    _x -= rhs._x;
    _y -= rhs._y;
    _z -= rhs._z;
    _w -= rhs._w;
}

template <class T>
iaQuaternion<T> iaQuaternion<T>::operator * (const iaQuaternion<T>& rhs)
{
    iaQuaternion<T> c;

    c._x = _w * rhs._x + _x * rhs._w + _y * rhs._z - _z * rhs._y;
    c._y = _w * rhs._y - _x * rhs._z + _y * rhs._w + _z * rhs._x;
    c._z = _w * rhs._z + _x * rhs._y - _y * rhs._x + _z * rhs._w;
    c._w = _w * rhs._w - _x * rhs._x - _y * rhs._y - _z * rhs._z;

    return c;
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
__IGOR_INLINE__ const T* iaQuaternion<T>::getData() const
{
    return (T*)&_x;
}

template <class T>
__IGOR_INLINE__ T* iaQuaternion<T>::getData()
{
    return (T*)&_x;
}

template <class T>
__IGOR_INLINE__ void iaQuaternion<T>::normalize()
{
    float64 d = sqrt(_x * _x + _y + _y + _z * _z + _w * _w);
    _x /= d;
    _y /= d;
    _z /= d;
    _w /= d;
}

template <class T>
T dotProduct(iaQuaternion<T> a, iaQuaternion<T> b)
{
    return a._x * b._x + a._y * b._y + a._z * b._z + a._w * b._w;
}

template <class T>
iaQuaternion<T> slerp(iaQuaternion<T> a, iaQuaternion<T> b, T t) {

    // Only unit quaternions are valid rotations.
    // Normalize to avoid undefined behavior.
    a.normalize();
    b.normalize();

    // Compute the cosine of the angle between the two vectors.
    T dot = dotProduct(a, b);

    // If the dot product is negative, slerp won't take
    // the shorter path. Note that b and -b are equivalent when
    // the negation is applied to all four components. Fix by 
    // reversing one quaternion.
    if (dot < 0.0) {
        b *= (T)-1;
        dot = -dot;
    }

    T DOT_THRESHOLD = 0.9995;
    if (dot > DOT_THRESHOLD) {
        // If the inputs are too close for comfort, linearly interpolate
        // and normalize the result.

        iaQuaternion<T> result = a + (b - a) * t;
        result.normalize();
        return result;
    }

    // Since dot is in range [0, DOT_THRESHOLD], acos is safe
    T theta_0 = acos(dot);        // theta_0 = angle between input vectors
    T theta = theta_0 * t;          // theta = angle between a and result
    T sin_theta = sin(theta);     // compute this value only once
    T sin_theta_0 = sin(theta_0); // compute this value only once

    T s0 = cos(theta) - dot * sin_theta / sin_theta_0;  // == sin(theta_0 - theta) / sin(theta_0)
    T s1 = sin_theta / sin_theta_0;

    return (a * s0) + (b * s1);
}
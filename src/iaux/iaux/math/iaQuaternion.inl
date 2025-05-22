// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

template <class T>
IGOR_INLINE std::wostream &operator<<(std::wostream &ostr, const iaQuaternion<T> &q)
{
    ostr << "(" << q._w << ", " << q._x << ", " << q._y << ", " << q._z << ")";
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
iaQuaternion<T>::iaQuaternion(T w, T x, T y, T z)
{
    set(w, x, y, z);
}

template <class T>
iaQuaternion<T>::iaQuaternion(T x, T y, T z)
{
    set(0, x, y, z);
}

template <class T>
iaQuaternion<T>::iaQuaternion(const iaQuaternion<T> &other)
{
    _w = other._w;
    _x = other._x;
    _y = other._y;
    _z = other._z;
}

template <class T>
const iaQuaternion<T> iaQuaternion<T>::operator*(T scalar)
{
    return {
        _w * scalar,
        _x * scalar,
        _y * scalar,
        _z * scalar};
}

template <class T>
iaQuaternion<T> &iaQuaternion<T>::operator*=(T scalar)
{
    _w *= scalar;
    _x *= scalar;
    _y *= scalar;
    _z *= scalar;
    return *this;
}

template <class T>
void iaQuaternion<T>::set(T w, T x, T y, T z)
{
    _w = w;
    _x = x;
    _y = y;
    _z = z;
}

template <class T>
const iaQuaternion<T> iaQuaternion<T>::fromAxisAngle(const iaVector3<T> &axis, T angle)
{
    /*T sha = sin(angle * 0.5);

    _w = cos(angle * 0.5);
    _x = axis._x * sha;
    _y = axis._y * sha;
    _z = axis._z * sha;*/

    T halfAngle = angle * 0.5;

    T sin_2 = sin(halfAngle);
    T cos_2 = cos(halfAngle);

    T sin_norm = sin_2 / sqrt(axis._x * axis._x + axis._y * axis._y + axis._z * axis._z);

    return {cos_2, axis._x * sin_norm, axis._y * sin_norm, axis._z * sin_norm};
}

template <class T>
void iaQuaternion<T>::toAxisAngle(iaVector3<T> &axis, T &angle) const
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
    _w = 1;
    _x = 0;
    _y = 0;
    _z = 0;
}

template <class T>
iaQuaternion<T> &iaQuaternion<T>::operator=(const iaQuaternion<T> &other)
{
    _w = other._w;
    _x = other._x;
    _y = other._y;
    _z = other._z;
    return *this;
}

template <class T>
bool iaQuaternion<T>::operator==(const iaQuaternion<T> &other) const
{
    return ((_w == other._w) &&
            (_x == other._x) &&
            (_y == other._y) &&
            (_z == other._z));
}

template <class T>
bool iaQuaternion<T>::operator!=(const iaQuaternion<T> &other) const
{
    return ((_w != other._w) ||
            (_x != other._x) ||
            (_y != other._y) ||
            (_z != other._z));
}

template <class T>
const iaQuaternion<T> iaQuaternion<T>::operator+(const iaQuaternion<T> &other)
{
    return {_w + other._w,
            _x + other._x,
            _y + other._y,
            _z + other._z};
}

template <class T>
iaQuaternion<T> &iaQuaternion<T>::operator+=(const iaQuaternion<T> &other)
{
    _w += other._w;
    _x += other._x;
    _y += other._y;
    _z += other._z;
    return *this;
}

template <class T>
const iaQuaternion<T> iaQuaternion<T>::operator-(const iaQuaternion<T> &other)
{
    return {_w - other._w,
            _x - other._x,
            _y - other._y,
            _z - other._z};
}

template <class T>
iaQuaternion<T> &iaQuaternion<T>::operator-=(const iaQuaternion<T> &other)
{
    _w -= other._w;
    _x -= other._x;
    _y -= other._y;
    _z -= other._z;
    return *this;
}

template <class T>
const iaQuaternion<T> iaQuaternion<T>::operator*(const iaQuaternion<T> &other) const
{
    return iaQuaternion<T>(*this) *= other;
}

template <class T>
iaQuaternion<T> &iaQuaternion<T>::operator*=(const iaQuaternion<T> &other)
{
    const T w1 = _w;
    const T x1 = _x;
    const T y1 = _y;
    const T z1 = _z;

    const T &w2 = other._w;
    const T &x2 = other._x;
    const T &y2 = other._y;
    const T &z2 = other._z;

    _w = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
    _x = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
    _y = w1 * y2 + y1 * w2 + z1 * x2 - x1 * z2;
    _z = w1 * z2 + z1 * w2 + x1 * y2 - y1 * x2;
    return *this;
}

template <class T>
const iaQuaternion<T> iaQuaternion<T>::fromEuler(T pitch, T yaw, T roll)
{
    double cx = cos(pitch * 0.5);
    double sx = sin(pitch * 0.5);
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cz = cos(roll * 0.5);
    double sz = sin(roll * 0.5);

    return {cx * cy * cz - sx * sy * sz,
            sx * cy * cz + sy * sz * cx,
            sy * cx * cz - sx * sz * cy,
            sx * sy * cz + sz * cx * cy};
}

template <class T>
const iaVector3<T> iaQuaternion<T>::toEuler() const
{
    iaVector3<T> result;

    T unit = (_x * _x) + (_y * _y) + (_z * _z) + (_w * _w);
    T test = _x * _w - _y * _z;

    if (test > 0.4999995 * unit)
    {
        result._x = M_PI / 2.0;
        result._y = 2.0 * atan2(_y, _x);
        result._z = 0;
    }
    else if (test < -0.4999995 * unit)
    {
        result._x = -M_PI / 2.0;
        result._y = -2.0 * atan2(_y, _x);
        result._z = 0;
    }
    else
    {
        result._x = asin(2.0 * (_w * _x - _y * _z));
        result._y = atan2(2.0 * _w * _y + 2.0 * _z * _x, 1.0 - 2.0 * (_x * _x + _y * _y));
        result._z = atan2(2.0 * _w * _z + 2.0 * _x * _y, 1.0 - 2.0 * (_z * _z + _x * _x));
    }

    return result;
}

template <class T>
const iaQuaternion<T> iaQuaternion<T>::fromEuler(const iaVector3<T> &vec)
{
    return fromEuler(vec._x, vec._y, vec._z);
}

template <class T>
IGOR_INLINE const T *iaQuaternion<T>::getData() const
{
    return (T *)&_x;
}

template <class T>
IGOR_INLINE T *iaQuaternion<T>::getData()
{
    return (T *)&_x;
}

template <class T>
IGOR_INLINE iaQuaternion<T> &iaQuaternion<T>::normalize()
{
    const T d = 1.0 / norm();
    _x *= d;
    _y *= d;
    _z *= d;
    _w *= d;
    return *this;
}

template <class T>
T dotProduct(iaQuaternion<T> a, iaQuaternion<T> b)
{
    return a._x * b._x + a._y * b._y + a._z * b._z + a._w * b._w;
}

template <class T>
iaQuaternion<T> slerp(iaQuaternion<T> a, iaQuaternion<T> b, T t)
{
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
    if (dot < 0.0)
    {
        b *= (T)-1;
        dot = -dot;
    }

    T DOT_THRESHOLD = 0.9995;
    if (dot > DOT_THRESHOLD)
    {
        // If the inputs are too close for comfort, linearly interpolate
        // and normalize the result.

        iaQuaternion<T> result = a + (b - a) * t;
        result.normalize();
        return result;
    }

    // Since dot is in range [0, DOT_THRESHOLD], acos is safe
    T theta_0 = acos(dot);        // theta_0 = angle between input vectors
    T theta = theta_0 * t;        // theta = angle between a and result
    T sin_theta = sin(theta);     // compute this value only once
    T sin_theta_0 = sin(theta_0); // compute this value only once

    T s0 = cos(theta) - dot * sin_theta / sin_theta_0; // == sin(theta_0 - theta) / sin(theta_0)
    T s1 = sin_theta / sin_theta_0;

    return (a * s0) + (b * s1);
}

template <class T>
const iaQuaternion<T> iaQuaternion<T>::conjugate() const
{
    return {_w, -_x, -_y, -_z};
}

template <class T>
T iaQuaternion<T>::magnitude2() const
{
    return _w * _w + _x * _x + _y * _y + _z * _z;
}

template <class T>
const iaQuaternion<T> iaQuaternion<T>::inverse() const
{
    T mag = magnitude2();
    if (mag == 0.0)
    {
        con_err("Cannot invert a zero-magnitude quaternion.");
        return iaQuaternion<T>();
    }

    iaQuaternion<T> conj = conjugate();
    return {conj._w / mag, conj._x / mag, conj._y / mag, conj._z / mag};
}

template <class T>
const iaVector3<T> iaQuaternion<T>::rotate(const iaVector3<T> &vec) const
{
    /*iaQuaternion<T> p(0, vec._x, vec._y, vec._z);

    iaQuaternion<T> c = (*this).conjugate();

    iaQuaternion<T> result = (*this) * p * c;

    return {result._x, result._y, result._z};*/

    // t = 2q x v
    T tx = 2. * (_y * vec._z - _z * vec._y);
    T ty = 2. * (_z * vec._x - _x * vec._z);
    T tz = 2. * (_x * vec._y - _y * vec._x);

    // v + w t + q x t
    return {vec._x + _w * tx + _y * tz - _z * ty,
            vec._y + _w * ty + _z * tx - _x * tz,
            vec._z + _w * tz + _x * ty - _y * tx};
}

template <class T>
T iaQuaternion<T>::dot(const iaQuaternion<T> &other) const
{
    return _w * other._w + _x * other._x + _y * other._y + _z * other._z;
}

template <class T>
T iaQuaternion<T>::norm() const
{
    return sqrt(_w * _w + _x * _x + _y * _y + _z * _z);
}

template <class T>
T iaQuaternion<T>::normSquared() const
{
    return _w * _w + _x * _x + _y * _y + _z * _z;
}

template <class T>
const iaMatrix<T> iaQuaternion<T>::toMatrix() const
{

    T xx = _x * _x;
    T yy = _y * _y;
    T zz = _z * _z;
    T xy = _x * _y;
    T xz = _x * _z;
    T yz = _y * _z;
    T wx = _w * _x;
    T wy = _w * _y;
    T wz = _w * _z;

    iaMatrix<T> result;
    result[0] = 1.0 - 2.0 * (yy + zz);
    result[1] = 2.0 * (xy + wz);
    result[2] = 2.0 * (xz - wy);

    result[4] = 2.0 * (xy - wz);
    result[5] = 1.0 - 2.0 * (xx + zz);
    result[6] = 2.0 * (yz + wx);

    result[8] = 2.0 * (xz + wy);
    result[9] = 2.0 * (yz - wx);
    result[10] = 1.0 - 2.0 * (xx + yy);

    return result;
}
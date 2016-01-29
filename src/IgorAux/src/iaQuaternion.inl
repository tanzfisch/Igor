// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

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

template <class T>
void iaQuaternion<T>::set(T x, T y, T z, T w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

template <class T>
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
        q2.set(-q1.y, q1.x, -q1.w, q1.z);
        scale = (T)sin((T)M_PI * ((T)0.5 - t));
        invscale = (T)sin((T)M_PI * t);
    }

    *this = (q1*scale) + (q2*invscale);
}

template <class T>
iaQuaternion<T> iaQuaternion<T>::operator +	(iaQuaternion<T> &a)
{
    iaQuaternion<T> c;

    c.x = x + a.x;
    c.y = y + a.y;
    c.z = z + a.z;
    c.w = w + a.w;

    return c;
}

template <class T>
void iaQuaternion<T>::operator += (iaQuaternion<T> &a)
{
    x += ax;
    y += ay;
    z += az;
    w += aw;
}

template <class T>
iaQuaternion<T> iaQuaternion<T>::operator -	(iaQuaternion<T> &a)
{
    iaQuaternion<T> c;

    c.x = x - a.x;
    c.y = y - a.y;
    c.z = z - a.z;
    c.w = w - a.w;

    return c;
}

template <class T>
void iaQuaternion<T>::operator -= (iaQuaternion<T> &a)
{
    x -= ax;
    y -= ay;
    z -= az;
    w -= aw;
}

template <class T>
iaQuaternion<T> iaQuaternion<T>::operator * (iaQuaternion<T> &a)
{
    iaQuaternion<T> c;

    c.x = w * a.x + x * a.w + y * a.z - z * a.y;
    c.y = w * a.y - x * a.z + y * a.w + z * a.x;
    c.z = w * a.z + x * a.y - y * a.x + z * a.w;
    c.w = w * a.w - x * a.x - y * a.y - z * a.z;

    return c;
}

template <class T>
iaQuaternion<T> iaQuaternion<T>::operator * (T a)
{
    iaQuaternion<T> c;
    c.x = x*a;
    c.y = y*a;
    c.z = z*a;
    c.w = w*a;

    return c;
}

template <class T>
void iaQuaternion<T>::operator *= (T a)
{
    x *= a;
    y *= a;
    z *= a;
    w *= a;
}

template <class T>
T iaQuaternion<T>::length(void)
{
    return (T)sqrt(x*x + y*y + z*z + w*w);
}

template <class T>
T iaQuaternion<T>::dot(iaQuaternion<T> &a)
{
    return a.x*x + a.y*y + a.z*z + a.w*w;
}

template <class T>
T iaQuaternion<T>::length2(void)
{
    return (x*x + y*y + z*z + w*w);
}

template <class T>
void iaQuaternion<T>::conjugate(void)
{
    x *= -1;
    y *= -1;
    z *= -1;
    w *= -1;
}

template <class T>
void iaQuaternion<T>::normalize(void)
{
    T h = x*x + y*y + z*z + w*w;
    x /= h;
    y /= h;
    z /= h;
    w /= h;
}

template <class T>
iaQuaternion<T>::iaQuaternion(void)
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

template <class T>
iaQuaternion<T>::iaQuaternion(T x, T y, T z, T w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

template <class T>
iaQuaternion<T>::~iaQuaternion(void)
{
}

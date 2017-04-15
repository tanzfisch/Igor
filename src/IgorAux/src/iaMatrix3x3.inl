// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
__IGOR_INLINE__ std::wostream& operator<<(std::wostream &ostr, const iaMatrix3x3<T> &m)
{
    ostr << "|" << fixed << right << setprecision(2) << setw(8) << m._right._x << " " << right << setprecision(2) << setw(8) << m._top._x << " " << right << setprecision(2) << setw(8) << m._depth._x << "|" << endl;
    ostr << "|" << fixed << right << setprecision(2) << setw(8) << m._right._y << " " << right << setprecision(2) << setw(8) << m._top._y << " " << right << setprecision(2) << setw(8) << m._depth._y << "|" << endl;
    ostr << "|" << fixed << right << setprecision(2) << setw(8) << m._right._z << " " << right << setprecision(2) << setw(8) << m._top._z << " " << right << setprecision(2) << setw(8) << m._depth._z << "|" << endl;
    return ostr;
}

template <class T>
__IGOR_INLINE__ T& iaMatrix3x3<T>::operator[] (int i)
{
    return ((T*)&_right)[i];
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::operator += (iaMatrix3x3<T> &a)
{
    T *m = (T*)&_right;
    T *n = (T*)&a;

    for (long x = 0; x < 9; x++)
    {
        m[x] += n[x];
    }
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::operator -= (iaMatrix3x3<T> &a)
{
    T *m = (T*)&_right;
    T *n = (T*)&a;

    for (long x = 0; x < 9; x++)
    {
        m[x] -= n[x];
    }
}

template <class T>
__IGOR_INLINE__ bool iaMatrix3x3<T>::operator == (const iaMatrix3x3<T> &a) const
{
    T *m = (T*)&_right;
    T *n = (T*)&a;

    for (long x = 0; x < 9; x++)
    {
        if (m[x] != n[x])
        {
            return false;
        }
    }

    return true;
}

template <class T>
__IGOR_INLINE__ bool iaMatrix3x3<T>::operator != (const iaMatrix3x3<T> &a) const
{
    T *m = (T*)&_right;
    T *n = (T*)&a;

    for (long x = 0; x < 9; x++)
    {
        if (m[x] != n[x])
        {
            return true;
        }
    }

    return false;
}

template <class T>
__IGOR_INLINE__ iaVector3<T> iaMatrix3x3<T>::operator * (iaVector3<T> &a)
{
    iaVector3<T> result;
    result = _right * a._x + _top * a._y + _depth * a._z;

    return result;
}

template <class T>
__IGOR_INLINE__ iaVector4<T> iaMatrix3x3<T>::operator * (iaVector4<T> &a)
{
    iaVector4<T> result;

    result._vec._x = _right._x  *  a._vec._x + _top._x  *  a._vec._y + _depth._x  *  a._vec._z;
    result._vec._y = _right._y  *  a._vec._x + _top._y  *  a._vec._y + _depth._y  *  a._vec._z;
    result._vec._z = _right._z  *  a._vec._x + _top._z  *  a._vec._y + _depth._z  *  a._vec._z;
    result._w = a._w;
    return result;
}

template <class T>
__IGOR_INLINE__ iaMatrix3x3<T> iaMatrix3x3<T>::operator* (iaMatrix3x3<T> &m)
{
    iaMatrix3x3<T> matrix;

    matrix._right._x = _right._x * m._right._x + _top._x * m._right._y + _depth._x * m._right._z;
    matrix._right._y = _right._y * m._right._x + _top._y * m._right._y + _depth._y * m._right._z;
    matrix._right._z = _right._z * m._right._x + _top._z * m._right._y + _depth._z * m._right._z;

    matrix._top._x = _right._x * m._top._x + _top._x * m._top._y + _depth._x * m._top._z;
    matrix._top._y = _right._y * m._top._x + _top._y * m._top._y + _depth._y * m._top._z;
    matrix._top._z = _right._z * m._top._x + _top._z * m._top._y + _depth._z * m._top._z;

    matrix._depth._x = _right._x * m._depth._x + _top._x * m._depth._y + _depth._x * m._depth._z;
    matrix._depth._y = _right._y * m._depth._x + _top._y * m._depth._y + _depth._y * m._depth._z;
    matrix._depth._z = _right._z * m._depth._x + _top._z * m._depth._y + _depth._z * m._depth._z;

    return matrix;
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::operator*= (iaMatrix3x3<T> &m)
{
    iaMatrix3x3<T> matrix;

    matrix._right._x = _right._x * m._right._x + _top._x * m._right._y + _depth._x * m._right._z;
    matrix._right._y = _right._y * m._right._x + _top._y * m._right._y + _depth._y * m._right._z;
    matrix._right._z = _right._z * m._right._x + _top._z * m._right._y + _depth._z * m._right._z;

    matrix._top._x = _right._x * m._top._x + _top._x * m._top._y + _depth._x * m._top._z;
    matrix._top._y = _right._y * m._top._x + _top._y * m._top._y + _depth._y * m._top._z;
    matrix._top._z = _right._z * m._top._x + _top._z * m._top._y + _depth._z * m._top._z;

    matrix._depth._x = _right._x * m._depth._x + _top._x * m._depth._y + _depth._x * m._depth._z;
    matrix._depth._y = _right._y * m._depth._x + _top._y * m._depth._y + _depth._y * m._depth._z;
    matrix._depth._z = _right._z * m._depth._x + _top._z * m._depth._y + _depth._z * m._depth._z;

    (*this) = matrix;
}

template <class T>
__IGOR_INLINE__ iaMatrix3x3<T>::iaMatrix3x3()
{
    identity();
}

/*!
\todo replace with memcpy
*/
template <class T>
__IGOR_INLINE__ iaMatrix3x3<T>::iaMatrix3x3(const T data[16])
{
    T * localData = getData();

    for (int i = 0; i < 9; i++)
    {
        localData[i] = data[i];
    }
}

template <class T>
__IGOR_INLINE__ iaMatrix3x3<T>::~iaMatrix3x3()
{
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::transpose()
{
    T *m = (T*)&_right;
    T temp;

    temp = m[1]; m[1] = m[3]; m[3] = temp;
    temp = m[5]; m[5] = m[7]; m[7] = temp;
    temp = m[2]; m[2] = m[6]; m[6] = temp;
}

template <class T>
__IGOR_INLINE__ T iaMatrix3x3<T>::determinant()
{
    return   _right[0] * _top[1] * _depth[2] + _top[0] * _depth[1] * _right[2] + _right[1] * _top[2] * _depth[0]
        - _right[2] * _top[1] * _depth[0] - _right[1] * _top[0] * _depth[2] - _top[2] * _depth[1] * _right[0];
}

template <class T>
__IGOR_INLINE__ const T* iaMatrix3x3<T>::getData() const
{
    return (T*)&_right;
}

template <class T>
__IGOR_INLINE__ T* iaMatrix3x3<T>::getData()
{
    return (T*)&_right;
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::setData(T* data)
{
    for (int i = 0; i < 9; i++)
    {
        ((T*)&_right)[i] = data[i];
    }
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::identity(void)
{
    _right._x = 1;   _top._x = 0;   _depth._x = 0;
    _right._y = 0;   _top._y = 1;   _depth._y = 0;
    _right._z = 0;   _top._z = 0;   _depth._z = 1;
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::scale(iaVector3<T> &s)
{
    iaMatrix3x3<T> scaleMatrix;

    scaleMatrix._right._x = s._x;
    scaleMatrix._top._y = s._y;
    scaleMatrix._depth._z = s._z;

    (*this) *= scaleMatrix;
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::scale(T x, T y, T z)
{
    iaMatrix3x3<T> scaleMatrix;

    scaleMatrix._right._x = x;
    scaleMatrix._top._y = y;
    scaleMatrix._depth._z = z;

    (*this) *= scaleMatrix;
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::rotate(T angle, iaAxis axis)
{
    iaMatrix3x3<T> rotation;

    auto c = cos(angle);
    auto s = sin(angle);

    switch (axis)
    {
    case iaAxis::X:

        rotation._right._x = 1;
        rotation._right._y = 0;
        rotation._right._z = 0;

        rotation._top._x = 0;
        rotation._top._y = c;
        rotation._top._z = s;

        rotation._depth._x = 0;
        rotation._depth._y = -s;
        rotation._depth._z = c;
        break;

    case iaAxis::Y:
        rotation._right._x = c;
        rotation._right._y = 0;
        rotation._right._z = -s;

        rotation._top._x = 0;
        rotation._top._y = 1;
        rotation._top._z = 0;

        rotation._depth._x = s;
        rotation._depth._y = 0;
        rotation._depth._z = c;
        break;

    case iaAxis::Z:
        rotation._right._x = c;
        rotation._right._y = s;
        rotation._right._z = 0;

        rotation._top._x = -s;
        rotation._top._y = c;
        rotation._top._z = 0;

        rotation._depth._x = 0;
        rotation._depth._y = 0;
        rotation._depth._z = 1;
        break;

    default:;
    }

    (*this) *= rotation;
}
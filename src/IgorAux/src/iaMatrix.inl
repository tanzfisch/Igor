// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
__IGOR_INLINE__ std::wostream& operator<<(std::wostream &ostr, const iaMatrix<T> &m)
{
    ostr << "|" << fixed << right << setprecision(2) << setw(8) << m._right._x << " " << right << setprecision(2) << setw(8) << m._top._x << " " << right << setprecision(2) << setw(8) << m._depth._x << " " << right << setprecision(2) << setw(8) << m._pos._x << "|" << endl;
    ostr << "|" << fixed << right << setprecision(2) << setw(8) << m._right._y << " " << right << setprecision(2) << setw(8) << m._top._y << " " << right << setprecision(2) << setw(8) << m._depth._y << " " << right << setprecision(2) << setw(8) << m._pos._y << "|" << endl;
    ostr << "|" << fixed << right << setprecision(2) << setw(8) << m._right._z << " " << right << setprecision(2) << setw(8) << m._top._z << " " << right << setprecision(2) << setw(8) << m._depth._z << " " << right << setprecision(2) << setw(8) << m._pos._z << "|" << endl;
    ostr << "|" << fixed << right << setprecision(2) << setw(8) << m._w0 << " " << right << setprecision(2) << setw(8) << m._w1 << " " << right << setprecision(2) << setw(8) << m._w2 << " " << right << setprecision(2) << setw(8) << m._w3 << "|" << endl;
    return ostr;
}

template <class T>
__IGOR_INLINE__ T& iaMatrix<T>::operator[] (int i)
{
    return ((T*)&_right)[i];
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::operator += (iaMatrix<T> &a)
{
    T *m = (T*)&_right;
    T *n = (T*)&a;

    for (long x = 0; x < 16; x++)
    {
        m[x] += n[x];
    }
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::operator -= (iaMatrix<T> &a)
{
    T *m = (T*)&_right;
    T *n = (T*)&a;

    for (long x = 0; x < 16; x++)
    {
        m[x] -= n[x];
    }
}

template <class T>
__IGOR_INLINE__ bool iaMatrix<T>::operator == (const iaMatrix<T> &a) const
{
    T *m = (T*)&_right;
    T *n = (T*)&a;

    for (long x = 0; x < 16; x++)
    {
        if (m[x] != n[x])
        {
            return false;
        }
    }

    return true;
}

template <class T>
__IGOR_INLINE__ bool iaMatrix<T>::operator != (const iaMatrix<T> &a) const
{
    T *m = (T*)&_right;
    T *n = (T*)&a;

    for (long x = 0; x < 16; x++)
    {
        if (m[x] != n[x])
        {
            return true;
        }
    }

    return false;
}

template <class T>
__IGOR_INLINE__ iaVector3<T> iaMatrix<T>::operator * (iaVector3<T> &a)
{
    iaVector3<T> result;
    result = _right * a._x + _top * a._y + _depth * a._z + _pos;

    return result;
}

template <class T>
__IGOR_INLINE__ iaVector4<T> iaMatrix<T>::operator * (iaVector4<T> &a)
{
    iaVector4<T> result;

    result._vec._x = _right._x  *  a._vec._x + _top._x  *  a._vec._y + _depth._x  *  a._vec._z + _pos._x  *  a._w;
    result._vec._y = _right._y  *  a._vec._x + _top._y  *  a._vec._y + _depth._y  *  a._vec._z + _pos._y  *  a._w;
    result._vec._z = _right._z  *  a._vec._x + _top._z  *  a._vec._y + _depth._z  *  a._vec._z + _pos._z  *  a._w;
    result._w = _w0 * a._vec._x + _w1 * a._vec._y + _w2 * a._vec._z + _w3 * a._w;

    return result;
}

template <class T>
__IGOR_INLINE__ iaMatrix<T> iaMatrix<T>::operator* (iaMatrix<T> &m)
{
    iaMatrix<T> matrix;

    matrix._right._x = _right._x * m._right._x + _top._x * m._right._y + _depth._x * m._right._z + _pos._x * m._w0;
    matrix._right._y = _right._y * m._right._x + _top._y * m._right._y + _depth._y * m._right._z + _pos._y * m._w0;
    matrix._right._z = _right._z * m._right._x + _top._z * m._right._y + _depth._z * m._right._z + _pos._z * m._w0;
    matrix._w0 = _w0 * m._right._x + _w1 * m._right._y + _w2 * m._right._z + _w3 * m._w0;

    matrix._top._x = _right._x * m._top._x + _top._x * m._top._y + _depth._x * m._top._z + _pos._x * m._w1;
    matrix._top._y = _right._y * m._top._x + _top._y * m._top._y + _depth._y * m._top._z + _pos._y * m._w1;
    matrix._top._z = _right._z * m._top._x + _top._z * m._top._y + _depth._z * m._top._z + _pos._z * m._w1;
    matrix._w1 = _w0 * m._top._x + _w1 * m._top._y + _w2 * m._top._z + _w3 * m._w1;

    matrix._depth._x = _right._x * m._depth._x + _top._x * m._depth._y + _depth._x * m._depth._z + _pos._x * m._w2;
    matrix._depth._y = _right._y * m._depth._x + _top._y * m._depth._y + _depth._y * m._depth._z + _pos._y * m._w2;
    matrix._depth._z = _right._z * m._depth._x + _top._z * m._depth._y + _depth._z * m._depth._z + _pos._z * m._w2;
    matrix._w2 = _w0 * m._depth._x + _w1 * m._depth._y + _w2 * m._depth._z + _w3 * m._w2;

    matrix._pos._x = _right._x * m._pos._x + _top._x * m._pos._y + _depth._x * m._pos._z + _pos._x * m._w3;
    matrix._pos._y = _right._y * m._pos._x + _top._y * m._pos._y + _depth._y * m._pos._z + _pos._y * m._w3;
    matrix._pos._z = _right._z * m._pos._x + _top._z * m._pos._y + _depth._z * m._pos._z + _pos._z * m._w3;
    matrix._w3 = _w0 * m._pos._x + _w1 * m._pos._y + _w2 * m._pos._z + _w3 * m._w3;

    return matrix;
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::operator*= (iaMatrix<T> &m)
{
    iaMatrix<T> matrix;

    matrix[0] = _right._x * m._right._x + _top._x * m._right._y + _depth._x * m._right._z + _pos._x * m._w0;
    matrix._right._y = _right._y * m._right._x + _top._y * m._right._y + _depth._y * m._right._z + _pos._y * m._w0;
    matrix._right._z = _right._z * m._right._x + _top._z * m._right._y + _depth._z * m._right._z + _pos._z * m._w0;
    matrix._w0 = _w0 * m._right._x + _w1 * m._right._y + _w2 * m._right._z + _w3 * m._w0;

    matrix._top._x = _right._x * m._top._x + _top._x * m._top._y + _depth._x * m._top._z + _pos._x * m._w1;
    matrix._top._y = _right._y * m._top._x + _top._y * m._top._y + _depth._y * m._top._z + _pos._y * m._w1;
    matrix._top._z = _right._z * m._top._x + _top._z * m._top._y + _depth._z * m._top._z + _pos._z * m._w1;
    matrix._w1 = _w0 * m._top._x + _w1 * m._top._y + _w2 * m._top._z + _w3 * m._w1;

    matrix._depth._x = _right._x * m._depth._x + _top._x * m._depth._y + _depth._x * m._depth._z + _pos._x * m._w2;
    matrix._depth._y = _right._y * m._depth._x + _top._y * m._depth._y + _depth._y * m._depth._z + _pos._y * m._w2;
    matrix._depth._z = _right._z * m._depth._x + _top._z * m._depth._y + _depth._z * m._depth._z + _pos._z * m._w2;
    matrix._w2 = _w0 * m._depth._x + _w1 * m._depth._y + _w2 * m._depth._z + _w3 * m._w2;

    matrix._pos._x = _right._x * m._pos._x + _top._x * m._pos._y + _depth._x * m._pos._z + _pos._x * m._w3;
    matrix._pos._y = _right._y * m._pos._x + _top._y * m._pos._y + _depth._y * m._pos._z + _pos._y * m._w3;
    matrix._pos._z = _right._z * m._pos._x + _top._z * m._pos._y + _depth._z * m._pos._z + _pos._z * m._w3;
    matrix._w3 = _w0 * m._pos._x + _w1 * m._pos._y + _w2 * m._pos._z + _w3 * m._w3;

    (*this) = matrix;
}

template <class T>
__IGOR_INLINE__ iaMatrix<T>::iaMatrix()
{
    identity();
}

/*!
\todo replace with memcpy
*/
template <class T>
__IGOR_INLINE__ iaMatrix<T>::iaMatrix(const T data[16])
{
    T * localData = getData();

    for (int i = 0; i < 16; i++)
    {
        localData[i] = data[i];
    }
}

template <class T>
__IGOR_INLINE__ iaMatrix<T>::~iaMatrix()
{
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::frustum(T left, T _right, T bottom, T _top, T nearplain, T farplain)
{
    T *m = getData();

    m[0] = (static_cast<T>(2.0) * nearplain) / (_right - left);
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;
    m[4] = 0;
    m[5] = (static_cast<T>(2.0) * nearplain) / (_top - bottom);
    m[6] = 0;
    m[7] = 0;
    m[8] = (_right + left) / (_right - left);
    m[9] = (_top + bottom) / (_top - bottom);
    m[10] = -(farplain + nearplain) / (farplain - nearplain);
    m[11] = static_cast<T>(-1.0);
    m[12] = 0;
    m[13] = 0;
    m[14] = -(static_cast<T>(2.0) * farplain * nearplain) / (farplain - nearplain);
    m[15] = static_cast<T>(0.0);
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::ortho(T left, T _right, T bottom, T _top, T nearplain, T farplain)
{
    T *m = getData();

    m[0] = static_cast<T>(2.0) / (_right - left);
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;
    m[4] = 0;
    m[5] = static_cast<T>(2.0) / (_top - bottom);
    m[6] = 0;
    m[7] = 0;
    m[8] = 0;
    m[9] = 0;
    m[10] = static_cast<T>(-2.0) / (farplain - nearplain);
    m[11] = 0;
    m[12] = -((_right + left) / (_right - left));
    m[13] = -((_top + bottom) / (_top - bottom));
    m[14] = -((farplain + nearplain) / (farplain - nearplain));
    m[15] = static_cast<T>(1);
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::lookAt(iaVector3<T> &eye, iaVector3<T> &coi, iaVector3<T> &_top)
{
    iaMatrix<T> lookat;
    iaVector3<T> x, y, z;

    z = eye - coi;
    z.normalize();
    y = _top;
    x = y % z;
    y = z % x;
    x.normalize();
    y.normalize();

    // Build resulting view matrix. 
    lookat._right[0] = x[0];
    lookat._top[0] = x[1];
    lookat._depth[0] = x[2];
    lookat._pos[0] = -(x*eye);

    lookat._right[1] = y[0];
    lookat._top[1] = y[1];
    lookat._depth[1] = y[2];
    lookat._pos[1] = -(y*eye);

    lookat._right[2] = z[0];
    lookat._top[2] = z[1];
    lookat._depth[2] = z[2];
    lookat._pos[2] = -(z*eye);

    lookat._w0 = static_cast<T>(0);
    lookat._w1 = static_cast<T>(0);
    lookat._w2 = static_cast<T>(0);
    lookat._w3 = static_cast<T>(1);

    (*this) *= lookat;
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::transpose()
{
    T *m = (T*)&_right;
    T temp;

    temp = m[1]; m[1] = m[4]; m[4] = temp;
    temp = m[2]; m[2] = m[8]; m[8] = temp;
    temp = m[3]; m[3] = m[12]; m[12] = temp;
    temp = m[6]; m[6] = m[9]; m[9] = temp;
    temp = m[7]; m[7] = m[13]; m[13] = temp;
    temp = m[11]; m[11] = m[14]; m[14] = temp;
}

// return the inverse of this matrix, that is,
// the inverse of the rotation, the inverse of the scaling, and
// the opposite of the translation vector.
#define MATSWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}


template <class T>
__IGOR_INLINE__ bool iaMatrix<T>::invert()
{
    T *mat = (T*)&_right;
    T matr[4][4], ident[4][4];
    int i, j, k, l, ll;
    int icol = 0, irow = 0;
    int indxc[4], indxr[4], ipiv[4];
    T big, dum, pivinv, temp;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            matr[i][j] = mat[4 * i + j];
            ident[i][j] = 0.0f;
        }
        ident[i][i] = 1.0f;
    }

    // Gauss-jordan elimination with full pivoting.  Yes, folks, a
    // GL Matrix4 is inverted like any other, since the identity is
    // still the identity.

    // from numerical recipies in C second edition, pg 39

    for (j = 0; j <= 3; j++)
    {
        ipiv[j] = 0;
    }

    for (i = 0; i <= 3; i++)
    {
        big = 0.0;
        for (j = 0; j <= 3; j++)
        {
            if (ipiv[j] != 1)
            {
                for (k = 0; k <= 3; k++)
                {
                    if (ipiv[k] == 0)
                    {
                        if (fabs(matr[j][k]) >= big)
                        {
                            big = (T)fabs(matr[j][k]);
                            irow = j;
                            icol = k;
                        }
                    }
                    else if (ipiv[k] > 1)
                    {
                        return false;
                    }
                }
            }
        }

        ++(ipiv[icol]);
        if (irow != icol)
        {
            for (l = 0; l <= 3; l++)
            {
                MATSWAP(matr[irow][l], matr[icol][l]);
            }

            for (l = 0; l <= 3; l++)
            {
                MATSWAP(ident[irow][l], ident[icol][l]);
            }
        }

        indxr[i] = irow;
        indxc[i] = icol;
        if (matr[icol][icol] == 0.0f)
        {
            return false;
        }

        pivinv = 1.0f / matr[icol][icol];
        matr[icol][icol] = 1.0f;
        for (l = 0; l <= 3; l++)
        {
            matr[icol][l] *= pivinv;
        }

        for (l = 0; l <= 3; l++)
        {
            ident[icol][l] *= pivinv;
        }

        for (ll = 0; ll <= 3; ll++)
        {
            if (ll != icol)
            {
                dum = matr[ll][icol];
                matr[ll][icol] = 0.0f;
                for (l = 0; l <= 3; l++)
                {
                    matr[ll][l] -= matr[icol][l] * dum;
                }

                for (l = 0; l <= 3; l++)
                {
                    ident[ll][l] -= ident[icol][l] * dum;
                }
            }
        }
    }

    for (l = 3; l >= 0; l--)
    {
        if (indxr[l] != indxc[l])
        {
            for (k = 0; k <= 3; k++)
            {
                MATSWAP(matr[k][indxr[l]], matr[k][indxc[l]]);
            }
        }
    }

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            mat[4 * i + j] = matr[i][j];
        }
    }
    return true;
}

template <class T>
__IGOR_INLINE__ T iaMatrix<T>::determinant()
{
    return 	_right[0] * (_top[1] * _depth[2] * w3 +
        _top[2] * w2*_pos[1] +
        w1*_depth[1] * _pos[2] -
        w1*_depth[2] * _pos[1] -
        _top[2] * _depth[1] * w3 -
        w2*_pos[2] * _top[1]) -
        _right[1] * (_top[0] * _depth[2] * w3 +
        _top[2] * w2*_pos[0] +
        _depth[0] * _pos[2] * w1 -
        w1*_depth[2] * _pos[0] -
        _top[2] * _depth[0] * w3 -
        w2*_pos[2] * _top[0]) +
        _right[2] * (_top[0] * _depth[1] * w3 +
        _top[1] * w2*_pos[0] +
        _depth[0] * _pos[1] * w1 -
        w1*_depth[1] * _pos[0] -
        _top[1] * _depth[0] * w3 -
        w2*_pos[1] * _top[0]) -
        w0 * (_top[0] * _depth[1] * _pos[2] +
        _top[1] * _depth[2] * _pos[0] +
        _depth[0] * _pos[1] * _top[2] -
        _top[2] * _depth[1] * _pos[0] -
        _top[1] * _depth[0] * _pos[2] -
        _depth[2] * _pos[1] * _top[0]);
}

template <class T>
__IGOR_INLINE__ T iaMatrix<T>::determinant3x3()
{
    return   _right[0] * _top[1] * _depth[2] + _top[0] * _depth[1] * _right[2] + _right[1] * _top[2] * _depth[0]
        - _right[2] * _top[1] * _depth[0] - _right[1] * _top[0] * _depth[2] - _top[2] * _depth[1] * _right[0];
}

template <class T>
__IGOR_INLINE__ const T* iaMatrix<T>::getData() const
{
    return (T*)&_right;
}

template <class T>
__IGOR_INLINE__ T* iaMatrix<T>::getData()
{
    return (T*)&_right;
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::setData(T* data)
{
    for (int i = 0; i < 16; i++)
        ((T*)&_right)[i] = data[i];
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::identity(void)
{
    _right._x = 1;   _top._x = 0;   _depth._x = 0;    _pos._x = 0;
    _right._y = 0;   _top._y = 1;   _depth._y = 0;    _pos._y = 0;
    _right._z = 0;   _top._z = 0;   _depth._z = 1;    _pos._z = 0;
    _w0 = 0;         _w1 = 0;       _w2 = 0;          _w3 = 1;
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::scale(iaVector3<T> &s)
{
    iaMatrix<T> scaleMatrix;

    scaleMatrix._right._x = s._x;
    scaleMatrix._top._y = s._y;
    scaleMatrix._depth._z = s._z;

    (*this) *= scaleMatrix;
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::scale(T x, T y, T z)
{
    iaMatrix<T> scaleMatrix;

    scaleMatrix._right._x = x;
    scaleMatrix._top._y = y;
    scaleMatrix._depth._z = z;

    (*this) *= scaleMatrix;
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::translate(T x, T y, T z)
{
    iaMatrix<T> translation;
    translation._pos._x = x;
    translation._pos._y = y;
    translation._pos._z = z;

    (*this) *= translation;
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::translate(const iaVector3<T> &a)
{
    iaMatrix<T> translation;
    translation._pos = a;

    (*this) *= translation;
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::move(T distance, iaAxis axis)
{
    switch (axis)
    {
    case iaAxis::X:
        _pos += _right * distance;
        break;

    case iaAxis::Y:
        _pos += _top * distance;
        break;

    case iaAxis::Z:
        _pos += _depth * distance;
        break;
    }
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::rotate(T angle, iaAxis axis)
{
    iaMatrix<T> rotation;

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
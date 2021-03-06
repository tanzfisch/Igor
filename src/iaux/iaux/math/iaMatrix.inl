// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
__IGOR_INLINE__ iaConsole &operator<<(iaConsole &console, const iaMatrix<T> &m)
{
    console << "/" << std::setfill(L' ') << std::fixed << std::right << std::setprecision(2) << std::setw(10) << m._right._x << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._top._x << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._depth._x << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._pos._x << "\\" << iaux::endlTab;
    console << "|" << std::setfill(L' ') << std::fixed << std::right << std::setprecision(2) << std::setw(10) << m._right._y << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._top._y << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._depth._y << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._pos._y << "|" << iaux::endlTab;
    console << "|" << std::setfill(L' ') << std::fixed << std::right << std::setprecision(2) << std::setw(10) << m._right._z << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._top._z << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._depth._z << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._pos._z << "|" << iaux::endlTab;
    console << "\\" << std::setfill(L' ') << std::fixed << std::right << std::setprecision(2) << std::setw(10) << m._w0 << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._w1 << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._w2 << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._w3 << "/" << iaux::endl;
    return console;
}

template <class T>
__IGOR_INLINE__ T &iaMatrix<T>::operator[](int i)
{
    return ((T *)this)[i];
}

template <class T>
__IGOR_INLINE__ T iaMatrix<T>::operator[](int i) const
{
    return ((T *)this)[i];
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::operator+=(iaMatrix<T> &a)
{
    T *m = (T *)&_right;
    T *n = (T *)&a;

    for (long x = 0; x < 16; x++)
    {
        m[x] += n[x];
    }
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::operator-=(iaMatrix<T> &a)
{
    T *m = (T *)&_right;
    T *n = (T *)&a;

    for (long x = 0; x < 16; x++)
    {
        m[x] -= n[x];
    }
}

template <class T>
__IGOR_INLINE__ bool iaMatrix<T>::operator==(const iaMatrix<T> &a) const
{
    T *m = (T *)&_right;
    T *n = (T *)&a;

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
__IGOR_INLINE__ bool iaMatrix<T>::operator!=(const iaMatrix<T> &a) const
{
    T *m = (T *)&_right;
    T *n = (T *)&a;

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
__IGOR_INLINE__ iaVector3<T> iaMatrix<T>::operator*(iaVector3<T> &a) const
{
    iaVector3<T> result;
    result = _right * a._x + _top * a._y + _depth * a._z + _pos;

    return result;
}

template <class T>
__IGOR_INLINE__ iaVector3<T> iaMatrix<T>::operator*(const iaVector3<T> &a) const
{
    iaVector3<T> result;
    result = _right * a._x + _top * a._y + _depth * a._z + _pos;

    return result;
}

template <class T>
__IGOR_INLINE__ iaVector4<T> iaMatrix<T>::operator*(iaVector4<T> &a) const
{
    iaVector4<T> result;

    result._x = _right._x * a._x + _top._x * a._y + _depth._x * a._z + _pos._x * a._w;
    result._y = _right._y * a._x + _top._y * a._y + _depth._y * a._z + _pos._y * a._w;
    result._z = _right._z * a._x + _top._z * a._y + _depth._z * a._z + _pos._z * a._w;
    result._w = _w0 * a._x + _w1 * a._y + _w2 * a._z + _w3 * a._w;

    return result;
}

template <class T>
__IGOR_INLINE__ iaVector4<T> iaMatrix<T>::operator*(const iaVector4<T> &a) const
{
    iaVector4<T> result;

    result._x = _right._x * a._x + _top._x * a._y + _depth._x * a._z + _pos._x * a._w;
    result._y = _right._y * a._x + _top._y * a._y + _depth._y * a._z + _pos._y * a._w;
    result._z = _right._z * a._x + _top._z * a._y + _depth._z * a._z + _pos._z * a._w;
    result._w = _w0 * a._x + _w1 * a._y + _w2 * a._z + _w3 * a._w;

    return result;
}

template <class T>
__IGOR_INLINE__ iaMatrix<T> iaMatrix<T>::operator*(const iaMatrix<T> &m)
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
__IGOR_INLINE__ void iaMatrix<T>::operator*=(const iaMatrix<T> &m)
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

    (*this) = matrix;
}

template <class T>
iaMatrix<T>::iaMatrix()
{
    identity();
}

/*!
\todo replace with memcpy
*/
template <class T>
iaMatrix<T>::iaMatrix(const T data[16])
{
    T *localData = getData();

    for (int i = 0; i < 16; i++)
    {
        localData[i] = data[i];
    }
}

template <class T>
iaMatrix<T>::~iaMatrix()
{
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::grammSchmidt(iaVector3<T> &depth)
{
    _depth = depth;
    _depth.normalize();

    if (abs(_depth._z) > (T)0.57777)
    {
        _right = _depth % iaVector3<T>(-_depth._y, _depth._z, 0.0f);
    }
    else
    {
        _right = _depth % iaVector3<T>(-_depth._y, _depth._x, 0.0f);
    }

    _right.normalize();
    _top = _right % _depth;

    _w0 = 0.0f;
    _w1 = 0.0f;
    _w2 = 0.0f;
    _w3 = 1.0f;
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::grammSchmidt(iaVector3<T> &depth, iaVector3<T> &top)
{
    _top = top;

    _depth = depth;
    _depth.normalize();

    _right = _top % _depth;
    _right.normalize();

    _top = _depth % _right;
    _top.normalize();

    _w0 = 0.0f;
    _w1 = 0.0f;
    _w2 = 0.0f;
    _w3 = 1.0f;
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::perspective(T fov, T aspect, T nearplain, T farplain)
{
    T xmin, xmax, ymin, ymax;

    ymax = nearplain * static_cast<T>(tan(fov * static_cast<T>(M_PI) / static_cast<T>(360.0)));
    ymin = -ymax;

    xmin = ymin * aspect;
    xmax = ymax * aspect;

    frustum(xmin, xmax, ymin, ymax, nearplain, farplain);
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
__IGOR_INLINE__ void iaMatrix<T>::lookAt(const iaVector3<T> &eye, const iaVector3<T> &coi, const iaVector3<T> &_top)
{
    iaMatrix<T> lookat;
    iaVector3<T> x, y, z;

    z = eye - coi;
    z.normalize();

    y = _top;
    x = y % z;
    x.normalize();

    y = z % x;
    y.normalize();

    // Build resulting view matrix.
    lookat._right[0] = x[0];
    lookat._top[0] = x[1];
    lookat._depth[0] = x[2];
    lookat._pos[0] = -(x * eye);

    lookat._right[1] = y[0];
    lookat._top[1] = y[1];
    lookat._depth[1] = y[2];
    lookat._pos[1] = -(y * eye);

    lookat._right[2] = z[0];
    lookat._top[2] = z[1];
    lookat._depth[2] = z[2];
    lookat._pos[2] = -(z * eye);

    lookat._w0 = static_cast<T>(0);
    lookat._w1 = static_cast<T>(0);
    lookat._w2 = static_cast<T>(0);
    lookat._w3 = static_cast<T>(1);

    (*this) *= lookat;
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::transpose()
{
    T *m = (T *)&_right;
    T temp;

    temp = m[1];
    m[1] = m[4];
    m[4] = temp;
    temp = m[2];
    m[2] = m[8];
    m[8] = temp;
    temp = m[3];
    m[3] = m[12];
    m[12] = temp;
    temp = m[6];
    m[6] = m[9];
    m[9] = temp;
    temp = m[7];
    m[7] = m[13];
    m[13] = temp;
    temp = m[11];
    m[11] = m[14];
    m[14] = temp;
}

// return the inverse of this matrix, that is,
// the inverse of the rotation, the inverse of the scaling, and
// the opposite of the translation vector.
#define MATSWAP(a, b) \
    {                 \
        temp = (a);   \
        (a) = (b);    \
        (b) = temp;   \
    }

template <class T>
__IGOR_INLINE__ bool iaMatrix<T>::invert()
{
    T *mat = (T *)&_right;
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
    return _right[0] * (_top[1] * _depth[2] * _w3 +
                        _top[2] * _w2 * _pos[1] +
                        _w1 * _depth[1] * _pos[2] -
                        _w1 * _depth[2] * _pos[1] -
                        _top[2] * _depth[1] * _w3 -
                        _w2 * _pos[2] * _top[1]) -
           _right[1] * (_top[0] * _depth[2] * _w3 +
                        _top[2] * _w2 * _pos[0] +
                        _depth[0] * _pos[2] * _w1 -
                        _w1 * _depth[2] * _pos[0] -
                        _top[2] * _depth[0] * _w3 -
                        _w2 * _pos[2] * _top[0]) +
           _right[2] * (_top[0] * _depth[1] * _w3 +
                        _top[1] * _w2 * _pos[0] +
                        _depth[0] * _pos[1] * _w1 -
                        _w1 * _depth[1] * _pos[0] -
                        _top[1] * _depth[0] * _w3 -
                        _w2 * _pos[1] * _top[0]) -
           _w0 * (_top[0] * _depth[1] * _pos[2] +
                  _top[1] * _depth[2] * _pos[0] +
                  _depth[0] * _pos[1] * _top[2] -
                  _top[2] * _depth[1] * _pos[0] -
                  _top[1] * _depth[0] * _pos[2] -
                  _depth[2] * _pos[1] * _top[0]);
}

template <class T>
__IGOR_INLINE__ T iaMatrix<T>::determinant3x3()
{
    return _right[0] * _top[1] * _depth[2] + _top[0] * _depth[1] * _right[2] + _right[1] * _top[2] * _depth[0] - _right[2] * _top[1] * _depth[0] - _right[1] * _top[0] * _depth[2] - _top[2] * _depth[1] * _right[0];
}

template <class T>
__IGOR_INLINE__ const T *iaMatrix<T>::getData() const
{
    return (T *)&_right;
}

template <class T>
__IGOR_INLINE__ T *iaMatrix<T>::getData()
{
    return (T *)&_right;
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::setData(const T *data)
{
    for (int i = 0; i < 16; i++)
    {
        ((T *)&_right)[i] = data[i];
    }
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::identity(void)
{
    _right._x = 1;
    _top._x = 0;
    _depth._x = 0;
    _pos._x = 0;
    _right._y = 0;
    _top._y = 1;
    _depth._y = 0;
    _pos._y = 0;
    _right._z = 0;
    _top._z = 0;
    _depth._z = 1;
    _pos._z = 0;
    _w0 = 0;
    _w1 = 0;
    _w2 = 0;
    _w3 = 1;
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::scale(const iaVector3<T> &s)
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
__IGOR_INLINE__ void iaMatrix<T>::shear(const iaVector3<T> &vec)
{
    iaMatrix<T> matrix;

    matrix._right._y = vec._z;
    matrix._right._z = vec._y;
    matrix._top._z = vec._x;

    (*this) *= matrix;
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::shear(T xy, T xz, T yz)
{
    iaMatrix<T> matrix;

    matrix._right._y = yz;
    matrix._right._z = xz;
    matrix._top._z = xy;

    (*this) *= matrix;
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::rotate(const iaVector3<T> &vec)
{
    iaMatrix<T> rotation;

    rotation.rotate(vec._z, iaAxis::Z);
    rotation.rotate(vec._y, iaAxis::Y);
    rotation.rotate(vec._x, iaAxis::X);

    (*this) *= rotation;
}

template <class T>
__IGOR_INLINE__ void iaMatrix<T>::rotate(T x, T y, T z)
{
    iaMatrix<T> rotation;

    rotation.rotate(z, iaAxis::Z);
    rotation.rotate(y, iaAxis::Y);
    rotation.rotate(x, iaAxis::X);

    (*this) *= rotation;
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

template <class T>
void iaMatrix<T>::recompose(const iaVector3<T> &scale, const iaQuaternion<T> &orientation, const iaVector3<T> &translate, const iaVector3<T> &shear, const iaVector4<T> &perspective)
{
    identity();

    // apply perspective
    _w0 = perspective._x;
    _w1 = perspective._y;
    _w2 = perspective._z;
    _w3 = perspective._w;

    // translate
    this->translate(translate);

    // apply rotation
    T xx = orientation._x * orientation._x;
    T xy = orientation._x * orientation._y;
    T xz = orientation._x * orientation._z;
    T xw = orientation._x * orientation._w;
    T yy = orientation._y * orientation._y;
    T yz = orientation._y * orientation._z;
    T yw = orientation._y * orientation._w;
    T zz = orientation._z * orientation._z;
    T zw = orientation._z * orientation._w;

    // Construct a composite rotation matrix from the quaternion values
    const T values[16] = {1 - 2 * (yy + zz), 2 * (xy - zw), 2 * (xz + yw), 0,
                          2 * (xy + zw), 1 - 2 * (xx + zz), 2 * (yz - xw), 0,
                          2 * (xz - yw), 2 * (yz + xw), 1 - 2 * (xx + yy), 0,
                          0, 0, 0, 1};

    iaMatrix<T> rotationMatrix(values);
    (*this) *= rotationMatrix;

    if (shear._z) // YZ
    {
        iaMatrix<T> shearMatrix;
        shearMatrix._right._y = shear._z;
        (*this) *= shearMatrix;
    }

    if (shear._y) // XZ
    {
        iaMatrix<T> shearMatrix;
        shearMatrix._right._z = shear._y;
        (*this) *= shearMatrix;
    }

    if (shear._x) // XY
    {
        iaMatrix<T> shearMatrix;
        shearMatrix._top._z = shear._x;
        (*this) *= shearMatrix;
    }

    /*	iaMatrix<T> shearMatrix;
	shearMatrix.shear(shear);
	*this = shearMatrix * *this;*/

    iaMatrix<T> scaleMatrix;
    scaleMatrix.scale(scale);
    (*this) *= scaleMatrix;
}

template <class T>
bool iaMatrix<T>::decompose(iaVector3<T> &scale, iaQuaternion<T> &orientation, iaVector3<T> &translate, iaVector3<T> &shear, iaVector4<T> &perspective) const
{
    iaMatrix<T> localMatrix(*this);

    // Normalize the matrix.
    if (_w3 == 0)
    {
        return false;
    }

    for (int i = 0; i < 16; ++i)
    {
        localMatrix[i] /= localMatrix._w3;
    }

    // perspectiveMatrix is used to solve for perspective, but it also provides
    // an easy way to test for singularity of the upper 3x3 component.
    iaMatrix<T> perspectiveMatrix(localMatrix);
    perspectiveMatrix._w0 = 0;
    perspectiveMatrix._w1 = 0;
    perspectiveMatrix._w2 = 0;
    perspectiveMatrix._w3 = 1;

    if (perspectiveMatrix.determinant() == 0)
    {
        return false;
    }

    // First, isolate perspective.  This is the messiest.
    if (localMatrix._w0 != 0 || localMatrix._w1 != 0 || localMatrix._w2 != 0)
    {
        // rightHandSide is the right hand side of the equation.
        iaVector4<T> rightHandSide;
        rightHandSide[0] = localMatrix._w0;
        rightHandSide[1] = localMatrix._w1;
        rightHandSide[2] = localMatrix._w2;
        rightHandSide[3] = localMatrix._w3;

        // Solve the equation by inverting perspectiveMatrix and multiplying
        // rightHandSide by the inverse.  (This is the easiest way, not
        // necessarily the best.)
        iaMatrix<T> transposedInversePerspectiveMatrix(perspectiveMatrix);
        if (!transposedInversePerspectiveMatrix.invert())
        {
            return false;
        }
        transposedInversePerspectiveMatrix.transpose();

        perspective = transposedInversePerspectiveMatrix * rightHandSide;

        // Clear the perspective partition
        localMatrix._w0 = 0;
        localMatrix._w1 = 0;
        localMatrix._w2 = 0;
        localMatrix._w3 = 1;
    }
    else
    {
        // No perspective.
        perspective.set(0, 0, 0, 1);
    }

    // Next take care of translation (easy).
    translate = localMatrix._pos;
    localMatrix._pos.set(0, 0, 0);

    // Vector4 type and functions need to be added to the common set.
    iaVector3<T> row[3], pdum3;

    // Now get scale and shear.
    row[0].set(localMatrix[0], localMatrix[4], localMatrix[8]);
    row[1].set(localMatrix[1], localMatrix[5], localMatrix[9]);
    row[2].set(localMatrix[2], localMatrix[6], localMatrix[10]);

    // Compute X scale factor and normalize first row.
    scale._x = row[0].length();
    row[0].normalize();

    // Compute XY shear factor and make 2nd row orthogonal to 1st.
    shear._z = row[0] * row[1];
    row[1] = row[1] + (row[0] * -shear._z);

    // Now, compute Y scale and normalize 2nd row.
    scale._y = row[1].length();
    row[1].normalize();
    shear._z /= scale._y;

    // Compute XZ and YZ shears, orthogonalize 3rd row.
    shear._y = row[0] * row[2];
    row[2] = row[2] + (row[0] * -shear._y);
    shear._x = row[1] * row[2];
    row[2] = row[2] + (row[1] * -shear._x);

    // Next, get Z scale and normalize 3rd row.
    scale._z = row[2].length();
    row[2].normalize();
    shear._y /= scale._z;
    shear._x /= scale._z;

    // At this point, the matrix (in rows[]) is orthonormal.
    // Check for a coordinate system flip.  If the determinant
    // is -1, then negate the matrix and the scaling factors.
    pdum3 = row[1] % row[2];
    if ((row[0] * pdum3) < 0)
    {
        // scale.negate();
        scale._x *= -1;
        row[0].negate();
        row[1].negate();
        row[2].negate();
    }

    // Now, get the rotations out
    T s, t;

    t = row[0][0] + row[1][1] + row[2][2] + 1.0;

    if (t > 1e-4)
    {
        s = 0.5 / sqrt(t);
        orientation._w = 0.25 / s;
        orientation._x = (row[2][1] - row[1][2]) * s;
        orientation._y = (row[0][2] - row[2][0]) * s;
        orientation._z = (row[1][0] - row[0][1]) * s;
    }
    else if (row[0][0] > row[1][1] && row[0][0] > row[2][2])
    {
        s = sqrt(1.0 + row[0][0] - row[1][1] - row[2][2]) * 2.0; // S=4*qx
        orientation._x = 0.25 * s;
        orientation._y = (row[0][1] + row[1][0]) / s;
        orientation._z = (row[0][2] + row[2][0]) / s;
        orientation._w = (row[2][1] - row[1][2]) / s;
    }
    else if (row[1][1] > row[2][2])
    {
        s = sqrt(1.0 + row[1][1] - row[0][0] - row[2][2]) * 2.0; // S=4*qy
        orientation._x = (row[0][1] + row[1][0]) / s;
        orientation._y = 0.25 * s;
        orientation._z = (row[1][2] + row[2][1]) / s;
        orientation._w = (row[0][2] - row[2][0]) / s;
    }
    else
    {
        s = sqrt(1.0 + row[2][2] - row[0][0] - row[1][1]) * 2.0; // S=4*qz
        orientation._x = (row[0][2] + row[2][0]) / s;
        orientation._y = (row[1][2] + row[2][1]) / s;
        orientation._z = 0.25 * s;
        orientation._w = (row[1][0] - row[0][1]) / s;
    }

    return true;
}

template <class T>
template <class T2>
iaMatrix<T2> iaMatrix<T>::convert()
{
    iaMatrix<T2> result;

    for (int i = 0; i < 16; ++i)
    {
        result[i] = static_cast<T2>((*this)[i]);
    }

    return result;
}

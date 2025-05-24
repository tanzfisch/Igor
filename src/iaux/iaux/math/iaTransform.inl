// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

template <class T>
IGOR_INLINE std::wostream &operator<<(std::wostream &ostr, const iaTransform<T> &transform)
{
    ostr << "t" << transform._position << " r" << transform._orientation << " s" << transform._scale;
    return ostr;
}

template <class T>
iaTransform<T>::iaTransform(const iaVector3<T> &position, const iaQuaternion<T> orientation, const iaVector3<T> &scale)
{
    _position = position;
    _orientation = orientation;
    _scale = scale;
}

template <class T>
void iaTransform<T>::set(const iaVector3<T> &position, const iaQuaternion<T> orientation, const iaVector3<T> &scale)
{
    _position = position;
    _orientation = orientation;
    _scale = scale;
}

template <class T>
iaTransform<T>::iaTransform()
{
    identity();
}

template <class T>
iaTransform<T> lerp(const iaTransform<T> &a, const iaTransform<T> &b, T t)
{
    iaTransform<T> result;

    result._position = lerp(a._position, b._position, t);
    result._scale = lerp(a._scale, b._scale, t);
    result._orientation = lerp(a._orientation, b._orientation, t);

    return result;
}

template <class T>
IGOR_INLINE const iaMatrix<T> iaTransform<T>::getMatrix() const
{
    iaMatrix<T> matrix;
    matrix.translate(_position);
    matrix *= _orientation.toMatrix();
    matrix.scale(_scale);
    return matrix;
}

template <class T>
IGOR_INLINE bool iaTransform<T>::hasScale() const
{
    return _scale != iaVector3d(1, 1, 1);
}

template <class T>
IGOR_INLINE bool iaTransform<T>::hasTranslation() const
{
    return _position != iaVector3d();
}

template <class T>
IGOR_INLINE bool iaTransform<T>::hasRotation() const
{
    return _orientation != iaQuaterniond();
}

template <class T>
IGOR_INLINE bool iaTransform<T>::operator==(const iaTransform<T> &other) const
{
    if (_position != other._position)
    {
        return false;
    }
    if (_orientation != other._orientation)
    {
        return false;
    }
    if (_scale != other._scale)
    {
        return false;
    }

    return true;
}

template <class T>
IGOR_INLINE bool iaTransform<T>::operator!=(const iaTransform<T> &other) const
{
    if (_position == other._position)
    {
        return false;
    }
    if (_orientation == other._orientation)
    {
        return false;
    }
    if (_scale == other._scale)
    {
        return false;
    }

    return true;
}

template <class T>
IGOR_INLINE iaTransform<T> iaTransform<T>::operator*(const iaTransform<T> &other)
{
    iaTransform<T> transform;

    /*result.scale = B.scale * A.scale
    result.rotation = B.rotation * A.rotation
    result.position = B.rotation * (B.scale * A.position) + B.position*/

    transform._scale = _scale * other._scale;
    transform._orientation = _orientation * other._orientation;
    transform._position = _orientation.rotate(_scale * other._position) + _position;

    return transform;
}

template <class T>
IGOR_INLINE void iaTransform<T>::operator*=(const iaTransform<T> &other)
{
    iaTransform<T> transform;

    (*this) = transform;
}

template <class T>
void iaTransform<T>::identity()
{
    _position.set(0.0, 0.0, 0.0);
    _orientation.set(1.0, 0.0, 0.0, 0.0);
    _scale.set(1.0, 1.0, 1.0);
}
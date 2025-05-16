// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

/*template <class T>
IGOR_INLINE std::wostream& operator<<(std::wostream& ostr, const iaTransform<T>& t)
{
    iaVector3d rotate;
    t._orientation.getEuler(rotate);

    ostr << "t" << t._position << "\n";
    ostr << "r" << rotate << "\n";
    ostr << "s" << t._scale << "\n";
    return ostr;
}*/

template <class T>
iaTransform<T>::iaTransform(const iaVector3<T>& position, const iaVector3<T> orientation, const iaVector3<T>& scale)
{
    _position = position;
    _orientation = orientation;
    _scale = scale;
}

template <class T>
void iaTransform<T>::set(const iaVector3<T> &position, const iaVector3<T> orientation, const iaVector3<T> &scale)
{
    _position = position;
    _orientation = orientation;
    _scale = scale;
}

template <class T>
iaTransform<T>::iaTransform()
{
    _position.set(0.0, 0.0, 0.0);
    _orientation.set(0.0, 0.0, 0.0, 1.0);
    _scale.set(1.0, 1.0, 1.0);
}

template <class T>
iaTransform<T> lerp(const iaTransform<T>& a, const iaTransform<T>& b, T t)
{
    iaTransform<T> result;

    result._position = lerp(a._position, b._position, t);
    result._scale = lerp(a._scale, b._scale, t);
    result._orientation = lerp(a._orientation, b._orientation, t);

    return result;
}

template <class T>
IGOR_INLINE const iaMatrix<T> iaTransform<T>::getMatrix() const;
{
    iaMatrix<T> matrix;
    matrix.translate(_position);
    matrix.rotate(_orientation);
    matrix.scale(_scale);
    return matrix;
}

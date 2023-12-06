// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
IGOR_INLINE std::wostream& operator<<(std::wostream& ostr, const iaTransform<T>& t)
{
    iaVector3d rotate;
    t._orientation.getEuler(rotate);

    ostr << "t" << t._translate << "\n";
    ostr << "r" << rotate << "\n";
    ostr << "s" << t._scale << "\n";
    return ostr;
}

template <class T>
iaTransform<T> lerp(const iaTransform<T>& a, const iaTransform<T>& b, T t)
{
    iaTransform<T> result;

    result._translate = lerp(a._translate, b._translate, t);
    result._scale = lerp(a._scale, b._scale, t);
    result._shear = lerp(a._shear, b._shear, t);
    result._orientation = slerp(a._orientation, b._orientation, t);

    return result;
}

template <class T>
iaTransform<T>::iaTransform(const iaVector3<T>& translate, const iaQuaternion<T> orientation, const iaVector3<T>& scale)
{
    _translate = translate;
    _orientation = orientation;
    _scale = scale;
    _shear.set(0.0, 0.0, 0.0);
}

template <class T>
iaTransform<T>::iaTransform(const iaVector3<T>& translate, const iaQuaternion<T> orientation, const iaVector3<T>& scale, const iaVector3<T>& shear)
{
    _translate = translate;
    _orientation = orientation;
    _scale = scale;
    _shear = shear;
}

template <class T>
iaTransform<T>::iaTransform()
{
    _translate.set(0.0, 0.0, 0.0);
    _orientation.set(0.0, 0.0, 0.0, 1.0);
    _scale.set(1.0, 1.0, 1.0);
    _shear.set(0.0, 0.0, 0.0);
}

template <class T>
iaTransform<T>::iaTransform(const iaMatrix<T>& matrix)
{
    setMatrix(matrix);
}

template <class T>
IGOR_INLINE iaTransform<T>::~iaTransform()
{
}

template <class T>
IGOR_INLINE void iaTransform<T>::getMatrix(iaMatrix<T>& matrix) const
{
    matrix.recompose(_scale, _orientation, _translate, _shear, iaVector4<T>(0, 0, 0, 1));
}

template <class T>
IGOR_INLINE bool iaTransform<T>::hasShear() const
{
    return _shear != iaVector3d();
}

template <class T>
IGOR_INLINE bool iaTransform<T>::hasScale() const
{
    return _scale != iaVector3d(1, 1, 1);
}

template <class T>
IGOR_INLINE bool iaTransform<T>::hasTranslation() const
{
    return _translate != iaVector3d();
}

template <class T>
IGOR_INLINE bool iaTransform<T>::hasRotation() const
{
    return _orientation != iaQuaterniond();
}
// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
__IGOR_INLINE__ std::wostream& operator<<(std::wostream& ostr, const iaTransform<T>& t)
{
	iaVector3d rotate;
	_orientation.getEuler(rotate);

	ostr << "t" << m_translate << "\n";
	ostr << "r" << rotate << "\n";
	ostr << "s" << m_scale << "\n";
	return ostr;
}

template <class T>
iaTransform<T> lerp(const iaTransform<T>& a, const iaTransform<T>& b, float64 t)
{
    iaTransform<T> result;
    // (1 - t) * v0 + t * v1;
    result._translate = a._translate * (1.0 - t) + b._translate * t;

    return result;
}

template <class T>
iaTransform<T>::iaTransform(const iaVector3<T>& translate, const iaQuaternion<T> orientation, const iaVector3<T>& scale)
{
	_translate = translate;
	_orientation = orientation;
	_scale = scale;
	_shear.set(0.0, 0.0, 0.0);
	_perspective.set(0.0, 0.0, 0.0, 1.0);
}

template <class T>
iaTransform<T>::iaTransform(const iaVector3<T>& translate, const iaQuaternion<T> orientation, const iaVector3<T>& scale, const iaVector3<T>& shear)
{
	_translate = translate;
	_orientation = orientation;
	_scale = scale;
	_shear = shear;
	_perspective.set(0.0, 0.0, 0.0, 1.0);
}

template <class T>
iaTransform<T>::iaTransform(const iaVector3<T>& translate, const iaQuaternion<T> orientation, const iaVector3<T>& scale, const iaVector3<T>& shear, const iaVector4<T>& perspective)
{
	_translate = translate;
	_orientation = orientation;
	_scale = scale;
	_shear = shear;
	_perspective = perspective;
}

template <class T>
iaTransform<T>::iaTransform()
{
	_translate.set(0.0, 0.0, 0.0);
	_orientation.set(0.0, 0.0, 0.0, 1.0);
	_scale.set(1.0, 1.0, 1.0);
	_shear.set(0.0, 0.0, 0.0);
	_perspective.set(0.0, 0.0, 0.0, 1.0);
	
}

template <class T>
iaTransform<T>::iaTransform(const iaMatrix<T>& matrix)
{
	setMatrix(matrix);
}

template <class T>
__IGOR_INLINE__ iaTransform<T>::~iaTransform()
{
}

template <class T>
__IGOR_INLINE__ void iaTransform<T>::getMatrix(iaMatrix<T>& matrix) const
{
	matrix.recompose(_scale, _orientation, _translate, _shear, iaVector4<T>(0, 0, 0, 1));
}

template <class T>
__IGOR_INLINE__ void iaTransform<T>::setMatrix(const iaMatrix<T>& matrix)
{
	iaVector4d perspective;
	matrix.decompose(_scale, _orientation, _translate, _shear, perspective);
}

template <class T>
__IGOR_INLINE__ bool iaTransform<T>::hasShear() const
{
	return _shear != iaVector3d();
}

template <class T>
__IGOR_INLINE__ bool iaTransform<T>::hasScale() const
{
	return _scale != iaVector3d(1,1,1);
}

template <class T>
__IGOR_INLINE__ bool iaTransform<T>::hasTranslation() const
{
	return _translate != iaVector3d();
}

template <class T>
__IGOR_INLINE__ bool iaTransform<T>::hasRotation() const
{
	return _orientation != iaQuaterniond();
}
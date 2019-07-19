// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
__IGOR_INLINE__ std::wostream& operator<<(std::wostream& ostr, const iaTransform<T>& t)
{
	ostr << "translate" << m_translate << "\n";
	ostr << "rotate   " << m._rotate << "\n";
	ostr << "scale    " << m_scale << "\n";
	return ostr;
}

template <class T>
iaTransform<T>::iaTransform(const iaVector3<T>& translate, const iaVector3<T>& rotate, const iaVector3<T>& scale)
{
	_translate = translate;
	_rotate = rotate;
	_scale = scale;
}

template <class T>
iaTransform<T>::iaTransform()
{
	identity();
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
__IGOR_INLINE__ void iaTransform<T>::operator+=(iaTransform<T>& rsv)
{
	_translate += rsv._translate;
	_rotate += rsv._rotate;
	_scale._x *= rsv._scale._x;
	_scale._y *= rsv._scale._y;
	_scale._z *= rsv._scale._z;
}

template <class T>
__IGOR_INLINE__ void iaTransform<T>::getMatrix(iaMatrix<T>& matrix) const
{
	matrix.identity();
	matrix.translate(_translate);
	matrix.rotate(_rotate);
	matrix.scale(_scale);
}

template <class T>
__IGOR_INLINE__ void iaTransform<T>::setMatrix(const iaMatrix<T>& matrix)
{
	iaQuaterniond orientation;
	iaVector3d shear;
	iaVector4d perspective;
	matrix.decompose(_scale, orientation, _translate, shear, perspective);
	orientation.getEuler(_rotate);
}

template <class T>
__IGOR_INLINE__ bool iaTransform<T>::operator == (const iaTransform<T>& rsv) const
{
	if (_translate != rsv._translate)
	{
		return false;
	}

	if (_rotate != rsv._rotate)
	{
		return false;
	}

	if (_scale != rsv._scale)
	{
		return false;
	}

	return true;
}

template <class T>
__IGOR_INLINE__ void iaTransform<T>::identity()
{
	_rotate.set(0.0, 0.0, 0.0);
	_scale.set(1.0, 1.0, 1.0);
	_translate.set(0.0, 0.0, 0.0);
}

template <class T>
__IGOR_INLINE__ void iaTransform<T>::translate(T x, T y, T z)
{
	_translate._x += x;
	_translate._y += y;
	_translate._z += z;
}

template <class T>
__IGOR_INLINE__ void iaTransform<T>::translate(const iaVector3<T>& a)
{
	_translate += a;
}

template <class T>
__IGOR_INLINE__ void iaTransform<T>::scale(const iaVector3<T>& scale)
{
	_scale._x *= scale._x;
	_scale._y *= scale._y;
	_scale._z *= scale._z;
}

template <class T>
__IGOR_INLINE__ void iaTransform<T>::scale(T x, T y, T z)
{
	_scale._x *= x;
	_scale._y *= y;
	_scale._z *= z;
}

template <class T>
__IGOR_INLINE__ void iaTransform<T>::rotate(const iaVector3<T>& vec)
{
	_rotate += vec;
}

template <class T>
__IGOR_INLINE__ void iaTransform<T>::rotate(T x, T y, T z)
{
	_rotate._x += x;
	_rotate._y += y;
	_rotate._z += z;
}

template <class T>
__IGOR_INLINE__ void iaTransform<T>::rotate(T angle, iaAxis axis)
{
	_rotate[static_cast<int>(axis)] += angle;
}
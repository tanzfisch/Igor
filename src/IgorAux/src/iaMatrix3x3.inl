// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
__IGOR_INLINE__ std::wostream& operator<<(std::wostream& ostr, const iaMatrix3x3<T>& m)
{
	ostr << "|" << std::fixed << std::right << std::setprecision(2) << std::setw(8) << m._m[0] << " " << std::right << std::setprecision(2) << std::setw(8) << m._m[3] << " " << std::right << std::setprecision(2) << std::setw(8) << m._m[6] << "|" << std::endl;
	ostr << "|" << std::fixed << std::right << std::setprecision(2) << std::setw(8) << m._m[1] << " " << std::right << std::setprecision(2) << std::setw(8) << m._m[4] << " " << std::right << std::setprecision(2) << std::setw(8) << m._m[6] << "|" << std::endl;
	ostr << "|" << std::fixed << std::right << std::setprecision(2) << std::setw(8) << m._m[2] <<       " " << std::right << std::setprecision(2) << std::setw(8) << m._m[5]     << " " << std::right << std::setprecision(2) << std::setw(8) << m._m[8] <<     "|" << std::endl;
	return ostr;
}

template <class T>
__IGOR_INLINE__ T& iaMatrix3x3<T>::operator[] (const int i)
{
	return _m[i];
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::operator += (iaMatrix3x3<T>& m)
{
	for (long x = 0; x < 9; x++)
	{
		_m[x] += m[x];
	}
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::operator -= (iaMatrix3x3<T>& m)
{
	for (long x = 0; x < 9; x++)
	{
		_m[x] -= m[x];
	}
}

template <class T>
__IGOR_INLINE__ bool iaMatrix3x3<T>::operator == (const iaMatrix3x3<T>& m) const
{
	for (long x = 0; x < 9; x++)
	{
		if (_m[x] != m[x])
		{
			return false;
		}
	}

	return true;
}

template <class T>
__IGOR_INLINE__ bool iaMatrix3x3<T>::operator != (const iaMatrix3x3<T>& m) const
{
	for (long x = 0; x < 9; x++)
	{
		if (_m[x] != m[x])
		{
			return true;
		}
	}

	return false;
}

template <class T>
__IGOR_INLINE__ iaVector2<T> iaMatrix3x3<T>::operator*(iaVector2<T>& v)
{
	iaVector2<T> result;
	result._x = _m[0] * v._x + _m[3] * v._y + _m[6];
	result._y = _m[1] * v._x + _m[4] * v._y + _m[7];

	return result;
}

template <class T>
__IGOR_INLINE__ iaVector3<T> iaMatrix3x3<T>::operator*(iaVector3<T>& a)
{
	iaVector3<T> result;

	result._x = _m[0] * a._x + _m[3] * a._y + _m[6] * a._z;
	result._y = _m[1] * a._x + _m[4] * a._y + _m[7] * a._z;
	result._z = _m[2] * a._x + _m[5] * a._y + _m[8] * a._z;

	return result;
}

template <class T>
__IGOR_INLINE__ iaMatrix3x3<T> iaMatrix3x3<T>::operator*(iaMatrix3x3<T>& m)
{
	iaMatrix3x3<T> matrix;

	matrix._m[0] = _m[0] * m._m[0] + _m[3] * m._m[1] + _m[6] * m._m[2];
	matrix._m[1] = _m[1] * m._m[0] + _m[4] * m._m[1] + _m[7] * m._m[2];
	matrix._m[2] = _m[2] * m._m[0] + _m[5] * m._m[1] + _m[8] * m._m[2];

	matrix._m[3] = _m[0] * m._m[3] + _m[3] * m._m[4] + _m[6] * m._m[5];
	matrix._m[4] = _m[1] * m._m[3] + _m[4] * m._m[4] + _m[7] * m._m[5];
	matrix._m[5] = _m[2] * m._m[3] + _m[5] * m._m[4] + _m[8] * m._m[5];

	matrix._m[6] = _m[0] * m._m[6] + _m[3] * m._m[7] + _m[6] * m._m[8];
	matrix._m[7] = _m[1] * m._m[6] + _m[4] * m._m[7] + _m[7] * m._m[8];
	matrix._m[8] = _m[2] * m._m[6] + _m[5] * m._m[7] + _m[8] * m._m[8];

	return matrix;
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::operator*=(iaMatrix3x3<T>& m)
{
	iaMatrix3x3<T> matrix;

	matrix._m[0] = _m[0] * m._m[0] + _m[3] * m._m[1] + _m[6] * m._m[2];
	matrix._m[1] = _m[1] * m._m[0] + _m[4] * m._m[1] + _m[7] * m._m[2];
	matrix._m[2] = _m[2] * m._m[0] + _m[5] * m._m[1] + _m[8] * m._m[2];

	matrix._m[3] = _m[0] * m._m[3] + _m[3] * m._m[4] + _m[6] * m._m[5];
	matrix._m[4] = _m[1] * m._m[3] + _m[4] * m._m[4] + _m[7] * m._m[5];
	matrix._m[5] = _m[2] * m._m[3] + _m[5] * m._m[4] + _m[8] * m._m[5];

	matrix._m[6] = _m[0] * m._m[6] + _m[3] * m._m[7] + _m[6] * m._m[8];
	matrix._m[7] = _m[1] * m._m[6] + _m[4] * m._m[7] + _m[7] * m._m[8];
	matrix._m[8] = _m[2] * m._m[6] + _m[5] * m._m[7] + _m[8] * m._m[8];

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
__IGOR_INLINE__ iaMatrix3x3<T>::iaMatrix3x3(const T data[9])
{
	T* localData = getData();

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
	T temp;

	temp = _m[1]; _m[1] = _m[3]; _m[3] = temp;
	temp = _m[5]; _m[5] = _m[7]; _m[7] = temp;
	temp = _m[2]; _m[2] = _m[6]; _m[6] = temp;
}

template <class T>
__IGOR_INLINE__ T iaMatrix3x3<T>::determinant()
{
	return   _m[0] * _m[4] * _m[8] + _m[3] * _m[7] * _m[2] + _m[4] * _m[5] * _m[6]
		- _m[2] * _m[4] * _m[6] - _m[4] * _m[3] * _m[8] - _m[5] * _m[7] * _m[0];
}

template <class T>
__IGOR_INLINE__ const T* iaMatrix3x3<T>::getData() const
{
	return (T*)this;
}

template <class T>
__IGOR_INLINE__ T* iaMatrix3x3<T>::getData()
{
	return (T*)this;
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::setData(T* data)
{
	for (int i = 0; i < 9; i++)
	{
		_m[i] = data[i];
	}
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::identity(void)
{
	_m[0] = static_cast<T>(1); _m[3] = static_cast<T>(0); _m[6] = static_cast<T>(0);
	_m[1] = static_cast<T>(0); _m[4] = static_cast<T>(1); _m[7] = static_cast<T>(0);
	_m[2] = static_cast<T>(0); _m[5] = static_cast<T>(0); _m[8] = static_cast<T>(1);
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::scale(iaVector2<T>& s)
{
	iaMatrix3x3<T> scaleMatrix;

	scaleMatrix._m[0] = s._x;
	scaleMatrix._m[4] = s._y;

	(*this) *= scaleMatrix;
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::scale(T x, T y)
{
	iaMatrix3x3<T> scaleMatrix;

	scaleMatrix._m[0] = x;
	scaleMatrix._m[4] = y;

	(*this) *= scaleMatrix;
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::rotate(T angle)
{
	iaMatrix3x3<T> rotation;

	auto c = cos(angle);
	auto s = sin(angle);

	rotation._m[0] = c;
	rotation._m[1] = s;
	rotation._m[3] = -s;
	rotation._m[4] = c;

	(*this) *= rotation;
}

template <class T>
template <class T2>
iaMatrix3x3<T2> iaMatrix3x3<T>::convert()
{
	iaMatrix3x3<T2> result;

	for (int i = 0; i < 9; ++i)
	{
		T2 temp = static_cast<T2>((*this)[i]);
		result[i] = temp;
	}

	return result;
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::translate(T x, T y)
{
	iaMatrix3x3<T> translation;
	translation._m[6] = x;
	translation._m[7] = y;

	(*this) *= translation;
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::translate(const iaVector2<T>& v)
{
	iaMatrix3x3<T> translation;
	translation._m[6] = v._x;
	translation._m[7] = v._y;

	(*this) *= translation;
}

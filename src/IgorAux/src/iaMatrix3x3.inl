// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
__IGOR_INLINE__ std::wostream& operator<<(std::wostream& ostr, const iaMatrix3x3<T>& m)
{
	ostr << "|" << fixed << right << setprecision(2) << setw(8) << m._right._x << " " << right << setprecision(2) << setw(8) << m._top._x << " " << right << setprecision(2) << setw(8) << m._depth._x << "|" << endl;
	ostr << "|" << fixed << right << setprecision(2) << setw(8) << m._right._y << " " << right << setprecision(2) << setw(8) << m._top._y << " " << right << setprecision(2) << setw(8) << m._depth._y << "|" << endl;
	ostr << "|" << fixed << right << setprecision(2) << setw(8) << m._right._z << " " << right << setprecision(2) << setw(8) << m._top._z << " " << right << setprecision(2) << setw(8) << m._depth._z << "|" << endl;
	return ostr;
}

template <class T>
__IGOR_INLINE__ T& iaMatrix3x3<T>::operator[] (int i)
{
	return ((T*)& _right)[i];
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::operator += (iaMatrix3x3<T>& a)
{
	T* m = (T*)& _right;
	T* n = (T*)& a;

	for (long x = 0; x < 9; x++)
	{
		m[x] += n[x];
	}
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::operator -= (iaMatrix3x3<T>& a)
{
	T* m = (T*)& _right;
	T* n = (T*)& a;

	for (long x = 0; x < 9; x++)
	{
		m[x] -= n[x];
	}
}

template <class T>
__IGOR_INLINE__ bool iaMatrix3x3<T>::operator == (const iaMatrix3x3<T>& a) const
{
	T* m = (T*)& _right;
	T* n = (T*)& a;

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
__IGOR_INLINE__ bool iaMatrix3x3<T>::operator != (const iaMatrix3x3<T>& a) const
{
	T* m = (T*)& _right;
	T* n = (T*)& a;

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
__IGOR_INLINE__ iaVector2<T> iaMatrix3x3<T>::operator*(iaVector2<T>& a)
{
	iaVector2<T> result;
	result = _right * a._x + _top * a._y + _pos;

	return result;
}

template <class T>
__IGOR_INLINE__ iaVector3<T> iaMatrix3x3<T>::operator*(iaVector3<T>& a)
{
	iaVector3<T> result;

	result._x = _right._x * a._x + _top._x * a._y + _pos._x * a._z;
	result._y = _right._y * a._x + _top._y * a._y + _pos._y * a._z;
	result._z = _w0 * a._x + _w1 * a._y + _w2 * a._z;

	return result;
}

template <class T>
__IGOR_INLINE__ iaMatrix3x3<T> iaMatrix3x3<T>::operator*(iaMatrix3x3<T>& m)
{
	iaMatrix3x3<T> matrix;

	matrix._right._x = _right._x * m._right._x + _top._x * m._right._y + _pos._x * m._w0;
	matrix._right._y = _right._y * m._right._x + _top._y * m._right._y + _pos._y * m._w0;
	matrix._w0 = _w0 * m._right._x + _w1 * m._right._y + _w2 * m._w0;

	matrix._top._x = _right._x * m._top._x + _top._x * m._top._y + _pos._x * m._w1;
	matrix._top._y = _right._y * m._top._x + _top._y * m._top._y + _pos._y * m._w1;
	matrix._w1 = _w0 * m._top._x + _w1 * m._top._y + _w2 * m._w1;

	matrix._pos._x = _right._x * m._pos._x + _top._x * m._pos._y + _pos._x * m._w2;
	matrix._pos._y = _right._y * m._pos._x + _top._y * m._pos._y + _pos._y * m._w2;
	matrix._w2 = _w0 * m._pos._x + _w1 * m._pos._y + _w2 * m._w2;

	return matrix;
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::operator*=(iaMatrix3x3<T>& m)
{
	iaMatrix3x3<T> matrix;

	matrix._right._x = _right._x * m._right._x + _top._x * m._right._y + _pos._x * m._w0;
	matrix._right._y = _right._y * m._right._x + _top._y * m._right._y + _pos._y * m._w0;
	matrix._w0 = _w0 * m._right._x + _w1 * m._right._y + _w2 * m._w0;

	matrix._top._x = _right._x * m._top._x + _top._x * m._top._y + _pos._x * m._w1;
	matrix._top._y = _right._y * m._top._x + _top._y * m._top._y + _pos._y * m._w1;
	matrix._w1 = _w0 * m._top._x + _w1 * m._top._y + _w2 * m._w1;

	matrix._pos._x = _right._x * m._pos._x + _top._x * m._pos._y + _pos._x * m._w2;
	matrix._pos._y = _right._y * m._pos._x + _top._y * m._pos._y + _pos._y * m._w2;
	matrix._w2 = _w0 * m._pos._x + _w1 * m._pos._y + _w2 * m._w2;

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
	T* m = (T*)& _right;
	T temp;

	temp = m[1]; m[1] = m[3]; m[3] = temp;
	temp = m[5]; m[5] = m[7]; m[7] = temp;
	temp = m[2]; m[2] = m[6]; m[6] = temp;
}

template <class T>
__IGOR_INLINE__ T iaMatrix3x3<T>::determinant()
{
	return   _right[0] * _top[1] * _w2 + _top[0] * _pos[1] * _w0 + _right[1] * _w1 * _pos[0]
		- _w0 * _top[1] * _pos[0] - _right[1] * _top[0] * _w2 - _w1 * _pos[1] * _right[0];
}

template <class T>
__IGOR_INLINE__ const T* iaMatrix3x3<T>::getData() const
{
	return (T*)& _right;
}

template <class T>
__IGOR_INLINE__ T* iaMatrix3x3<T>::getData()
{
	return (T*)& _right;
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::setData(T* data)
{
	for (int i = 0; i < 9; i++)
	{
		((T*)& _right)[i] = data[i];
	}
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::identity(void)
{
	_right._x = 1;   _top._x = 0;   _pos._x = 0;
	_right._y = 0;   _top._y = 1;   _pos._y = 0;
	_w0 = 0; _w1 = 0; _w2 = 1;
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::scale(iaVector2<T>& s)
{
	iaMatrix3x3<T> scaleMatrix;

	scaleMatrix._right._x = s._x;
	scaleMatrix._top._y = s._y;

	(*this) *= scaleMatrix;
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::scale(T x, T y)
{
	iaMatrix3x3<T> scaleMatrix;

	scaleMatrix._right._x = x;
	scaleMatrix._top._y = y;

	(*this) *= scaleMatrix;
}

template <class T>
__IGOR_INLINE__ void iaMatrix3x3<T>::rotate(T angle)
{
	iaMatrix3x3<T> rotation;

	auto c = cos(angle);
	auto s = sin(angle);

	rotation._right._x = c;
	rotation._right._y = s;
	rotation._top._x = -s;
	rotation._top._y = c;

	(*this) *= rotation;
}
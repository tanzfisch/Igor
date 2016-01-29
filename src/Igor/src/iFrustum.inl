// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
void iFrustum<T>::set(iaMatrix<T>& viewProjection)
{
	_leftPlane._normal.set(viewProjection._w0 + viewProjection._right._x, viewProjection._w1 + viewProjection._top._x, viewProjection._w2 + viewProjection._depth._x);
	_leftPlane._distance = viewProjection._w3 + viewProjection._pos._x;
	_leftPlane.normalize();

	_rightPlane._normal.set(viewProjection._w0 - viewProjection._right._x, viewProjection._w1 - viewProjection._top._x, viewProjection._w2 - viewProjection._depth._x);
	_rightPlane._distance = viewProjection._w3 - viewProjection._pos._x;
	_rightPlane.normalize();

	_topPlane._normal.set(viewProjection._w0 - viewProjection._right._y, viewProjection._w1 - viewProjection._top._y, viewProjection._w2 - viewProjection._depth._y);
	_topPlane._distance = viewProjection._w3 - viewProjection._pos._y;
	_topPlane.normalize();

	_bottomPlane._normal.set(viewProjection._w0 + viewProjection._right._y, viewProjection._w1 + viewProjection._top._y, viewProjection._w2 + viewProjection._depth._y);
	_bottomPlane._distance = viewProjection._w3 + viewProjection._pos._y;
	_bottomPlane.normalize();

	_nearPlane._normal.set(viewProjection._w0 + viewProjection._right._z, viewProjection._w1 + viewProjection._top._z, viewProjection._w2 + viewProjection._depth._z);
	_nearPlane._distance = viewProjection._w3 + viewProjection._pos._z;
	_nearPlane.normalize();

	_farPlane._normal.set(viewProjection._w0 - viewProjection._right._z, viewProjection._w1 - viewProjection._top._z, viewProjection._w2 - viewProjection._depth._z);
	_farPlane._distance = viewProjection._w3 - viewProjection._pos._z;
	_farPlane.normalize();
}


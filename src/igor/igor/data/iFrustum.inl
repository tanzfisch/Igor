// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
IGOR_INLINE std::wostream &operator<<(std::wostream &ostr, const iFrustum<T> &frustum)
{
    ostr << "near  :" << frustum._nearPlane._normal << " - " << frustum._nearPlane._distance << "\n";
    ostr << "far   :" << frustum._farPlane._normal << " - " << frustum._farPlane._distance << "\n";
    ostr << "left  :" << frustum._leftPlane._normal << " - " << frustum._leftPlane._distance << "\n";
    ostr << "right :" << frustum._rightPlane._normal << " - " << frustum._rightPlane._distance << "\n";
    ostr << "top   :" << frustum._topPlane._normal << " - " << frustum._topPlane._distance << "\n";
    ostr << "bottom:" << frustum._bottomPlane._normal << " - " << frustum._bottomPlane._distance << "\n";
    return ostr;
}

template <class T>
iFrustum<T>::iFrustum(const iaMatrix<T> &matrix)
{
    set(matrix);
}

template <class T>
void iFrustum<T>::set(const iaMatrix<T> &matrix)
{
    _rightPlane._normal.set(matrix._w0 - matrix._right._x, matrix._w1 - matrix._top._x, matrix._w2 - matrix._depth._x);
    _rightPlane._distance = -(matrix._w3 - matrix._pos._x);
    _rightPlane.normalize();

    _leftPlane._normal.set(matrix._w0 + matrix._right._x, matrix._w1 + matrix._top._x, matrix._w2 + matrix._depth._x);
    _leftPlane._distance = -(matrix._w3 + matrix._pos._x);
    _leftPlane.normalize();

    _topPlane._normal.set(matrix._w0 - matrix._right._y, matrix._w1 - matrix._top._y, matrix._w2 - matrix._depth._y);
    _topPlane._distance = -(matrix._w3 - matrix._pos._y);
    _topPlane.normalize();

    _bottomPlane._normal.set(matrix._w0 + matrix._right._y, matrix._w1 + matrix._top._y, matrix._w2 + matrix._depth._y);
    _bottomPlane._distance = -(matrix._w3 + matrix._pos._y);
    _bottomPlane.normalize();

    _farPlane._normal.set(matrix._w0 - matrix._right._z, matrix._w1 - matrix._top._z, matrix._w2 - matrix._depth._z);
    _farPlane._distance = -(matrix._w3 - matrix._pos._z);
    _farPlane.normalize();

    _nearPlane._normal.set(matrix._w0 + matrix._right._z, matrix._w1 + matrix._top._z, matrix._w2 + matrix._depth._z);
    _nearPlane._distance = -(matrix._w3 + matrix._pos._z);
    _nearPlane.normalize();
}

template <class T>
IGOR_INLINE bool iFrustum<T>::operator==(const iFrustum<T> &frustum) const
{
    if (_rightPlane._normal != frustum._rightPlane._normal || _rightPlane._distance != frustum._rightPlane._distance ||
        _leftPlane._normal != frustum._leftPlane._normal || _leftPlane._distance != frustum._leftPlane._distance ||
        _nearPlane._normal != frustum._nearPlane._normal || _nearPlane._distance != frustum._nearPlane._distance ||
        _farPlane._normal != frustum._farPlane._normal || _farPlane._distance != frustum._farPlane._distance ||
        _topPlane._normal != frustum._topPlane._normal || _topPlane._distance != frustum._topPlane._distance ||
        _bottomPlane._normal != frustum._bottomPlane._normal || _bottomPlane._distance != frustum._bottomPlane._distance)
    {
        return false;
    }

    return true;
}

template <class T>
IGOR_INLINE bool iFrustum<T>::operator!=(const iFrustum<T> &frustum) const
{
    if (_rightPlane._normal == frustum._rightPlane._normal && _rightPlane._distance == frustum._rightPlane._distance &&
        _leftPlane._normal == frustum._leftPlane._normal && _leftPlane._distance == frustum._leftPlane._distance &&
        _nearPlane._normal == frustum._nearPlane._normal && _nearPlane._distance == frustum._nearPlane._distance &&
        _farPlane._normal == frustum._farPlane._normal && _farPlane._distance == frustum._farPlane._distance &&
        _topPlane._normal == frustum._topPlane._normal && _topPlane._distance == frustum._topPlane._distance &&
        _bottomPlane._normal == frustum._bottomPlane._normal && _bottomPlane._distance == frustum._bottomPlane._distance)
    {
        return false;
    }

    return true;
}
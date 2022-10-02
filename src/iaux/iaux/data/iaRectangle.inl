// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
__IGOR_INLINE__ const T iaRectangle<T>::getX() const
{
    return _x;
}

template <class T>
__IGOR_INLINE__ const T iaRectangle<T>::getY() const
{
    return _y;
}

template <class T>
__IGOR_INLINE__ const T iaRectangle<T>::getLeft() const
{
    return _x;
}

template <class T>
__IGOR_INLINE__ const T iaRectangle<T>::getRight() const
{
    return _x + _width;
}

template <class T>
__IGOR_INLINE__ const T iaRectangle<T>::getTop() const
{
    return _y;
}

template <class T>
__IGOR_INLINE__ const T iaRectangle<T>::getBottom() const
{
    return _y + _height;
}

template <class T>
__IGOR_INLINE__ const T iaRectangle<T>::getWidth() const
{
    return _width;
}

template <class T>
__IGOR_INLINE__ const T iaRectangle<T>::getHeight() const
{
    return _height;
}

template <class T>
const iaVector2<T> iaRectangle<T>::getCenter() const
{
    iaVector2<T> result;

    result._x = _x + _width * 0.5;
    result._y = _y + _height * 0.5;

    return result;
}

template <class T>
void iaRectangle<T>::setX(const T x)
{
    _x = x;
}

template <class T>
void iaRectangle<T>::setY(const T y)
{
    _y = y;
}

template <class T>
void iaRectangle<T>::setWidth(const T width)
{
    _width = width;
}

template <class T>
void iaRectangle<T>::setHeight(const T height)
{
    _height = height;
}

template <class T>
void iaRectangle<T>::setSize(T width, T height)
{
    _width = width;
    _height = height;
}

template <class T>
void iaRectangle<T>::adjust(T x, T y, T width, T height)
{
    _x += x;
    _y += y;
    _width += width;
    _height += height;
}

template <class T>
void iaRectangle<T>::set(T x, T y, T width, T height)
{
    _x = x;
    _y = y;
    _width = width;
    _height = height;
}

template <class T>
iaRectangle<T> iaRectangle<T>::operator=(const iaRectangle<T> &rectangle)
{
    iaRectangle<T> result;
    result._x = _x = rectangle._x;
    result._y = _y = rectangle._y;
    result._width = _width = rectangle._width;
    result._height = _height = rectangle._height;

    return result;
}

template <class T>
void iaRectangle<T>::setCenter(const iaVector2<T> &center)
{
    _x = center._x - _width * 0.5;
    _y = center._y - _height * 0.5;
}

template <class T>
void iaRectangle<T>::setCenter(T x, T y)
{
    _x = x - _width * 0.5;
    _y = y - _height * 0.5;
}
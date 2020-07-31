// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
__IGOR_INLINE__ const T iRectangle<T>::getX() const
{
    return _x;
}

template <class T>
__IGOR_INLINE__ const T iRectangle<T>::getY() const
{
    return _y;
}

template <class T>
__IGOR_INLINE__ const T iRectangle<T>::getLeft() const
{
    return _x;
}

template <class T>
__IGOR_INLINE__ const T iRectangle<T>::getRight() const
{
    return _x + _width;
}

template <class T>
__IGOR_INLINE__ const T iRectangle<T>::getTop() const
{
    return _y;
}

template <class T>
__IGOR_INLINE__ const T iRectangle<T>::getBottom() const
{
    return _y + _height;
}

template <class T>
__IGOR_INLINE__ const T iRectangle<T>::getWidth() const
{
    return _width;
}

template <class T>
__IGOR_INLINE__ const T iRectangle<T>::getHeight() const
{
    return _height;
}

template <class T>
void iRectangle<T>::setX(const T x)
{
    _x = x;
}

template <class T>
void iRectangle<T>::setY(const T y)
{
    _y = y;
}

template <class T>
void iRectangle<T>::setWidth(const T width)
{
    _width = width;
}

template <class T>
void iRectangle<T>::setHeight(const T height)
{
    _height = height;
}

template <class T>
void iRectangle<T>::setSize(T width, T height)
{
    _width = width;
    _height = height;
}

template <class T>
void iRectangle<T>::adjust(T x, T y, T width, T height)
{
    _x += x;
    _y += y;
    _width += width;
    _height += height;
}

template <class T>
void iRectangle<T>::set(T x, T y, T width, T height)
{
    _x = x;
    _y = y;
    _width = width;
    _height = height;
}

template <class T>
iRectangle<T> iRectangle<T>::operator=(const iRectangle<T> &rectangle)
{
    iRectangle<T> result;
    result._x = _x = rectangle._x;
    result._y = _y = rectangle._y;
    result._width = _width = rectangle._width;
    result._height = _height = rectangle._height;

    return result;
}
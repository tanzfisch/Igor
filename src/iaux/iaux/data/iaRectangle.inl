// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <typename T>
std::wostream &operator<<(std::wostream &ostr, const iaRectangle<T> &rectangle)
{
    ostr << "(" << rectangle._x << ", " << rectangle._y << ", " << rectangle._width << ", " << rectangle._height << ")";
    return ostr;
}

template <typename T>
iaRectangle<T>::iaRectangle(T x, T y, T width, T height)
{
    _x = x;
    _y = y;
    _width = width;
    _height = height;
}

template <typename T>
iaRectangle<T>::iaRectangle(const iaVector2<T> &pos, const iaVector2<T> &size)
{
    _x = pos._x;
    _y = pos._y;
    _width = size._x;
    _height = size._y;
}

template <typename T>
iaRectangle<T>::iaRectangle(const iaRectangle<T> &rect)
{
    _x = rect._x;
    _y = rect._y;
    _width = rect._width;
    _height = rect._height;
}

template <typename T>
bool iaRectangle<T>::operator==(const iaRectangle<T> &other) const
{
    if (_x == other._x &&
        _y == other._y &&
        _width == other._width &&
        _height == other._height)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T>
bool iaRectangle<T>::operator!=(const iaRectangle<T> &other) const
{
    if (_x != other._x ||
        _y != other._y ||
        _width != other._width ||
        _height != other._height)
    {
        return true;
    }
    else
    {
        return false;
    }
}


template <typename T>
const T iaRectangle<T>::getX() const
{
    return _x;
}

template <typename T>
const T iaRectangle<T>::getY() const
{
    return _y;
}

template <typename T>
const T iaRectangle<T>::getLeft() const
{
    return _x;
}

template <typename T>
const T iaRectangle<T>::getRight() const
{
    return _x + _width;
}

template <typename T>
const T iaRectangle<T>::getTop() const
{
    return _y;
}

template <typename T>
const T iaRectangle<T>::getBottom() const
{
    return _y + _height;
}

template <typename T>
const T iaRectangle<T>::getWidth() const
{
    return _width;
}

template <typename T>
const T iaRectangle<T>::getHeight() const
{
    return _height;
}

template <typename T>
const iaVector2<T> iaRectangle<T>::getCenter() const
{
    iaVector2<T> result;

    result._x = _x + _width * 0.5;
    result._y = _y + _height * 0.5;

    return result;
}

template <typename T>
const iaVector2<T> iaRectangle<T>::getTopLeft() const
{
    iaVector2<T> result;

    result._x = _x;
    result._y = _y;

    return result;
}

template <typename T>
const iaVector2<T> iaRectangle<T>::getTopRight() const
{
    iaVector2<T> result;

    result._x = _x + _width;
    result._y = _y;

    return result;
}

template <typename T>
const iaVector2<T> iaRectangle<T>::getBottomLeft() const
{
    iaVector2<T> result;

    result._x = _x;
    result._y = _y + _height;

    return result;
}

template <typename T>
const iaVector2<T> iaRectangle<T>::getBottomRight() const
{
    iaVector2<T> result;

    result._x = _x + _width;
    result._y = _y + _height;

    return result;
}

template <typename T>
void iaRectangle<T>::setX(const T x)
{
    _x = x;
}

template <typename T>
void iaRectangle<T>::setY(const T y)
{
    _y = y;
}

template <typename T>
void iaRectangle<T>::setWidth(const T width)
{
    _width = width;
}

template <typename T>
void iaRectangle<T>::setHeight(const T height)
{
    _height = height;
}

template <typename T>
void iaRectangle<T>::setSize(T width, T height)
{
    _width = width;
    _height = height;
}

template <typename T>
void iaRectangle<T>::setTopLeft(const iaVector2<T> &pos)
{
    _x = pos._x;
    _y = pos._y;
}

template <typename T>
void iaRectangle<T>::setTopLeft(T x, T y)
{
    _x = x;
    _y = y;
}

template <typename T>
void iaRectangle<T>::adjust(T x, T y, T width, T height)
{
    _x += x;
    _y += y;
    _width += width;
    _height += height;
}

template <typename T>
void iaRectangle<T>::set(T x, T y, T width, T height)
{
    _x = x;
    _y = y;
    _width = width;
    _height = height;
}

template <typename T>
iaRectangle<T> iaRectangle<T>::operator=(const iaRectangle<T> &rectangle)
{
    iaRectangle<T> result;
    result._x = _x = rectangle._x;
    result._y = _y = rectangle._y;
    result._width = _width = rectangle._width;
    result._height = _height = rectangle._height;

    return result;
}

template <typename T>
void iaRectangle<T>::setCenter(const iaVector2<T> &center)
{
    _x = center._x - _width * 0.5;
    _y = center._y - _height * 0.5;
}

template <typename T>
void iaRectangle<T>::setCenter(T x, T y)
{
    _x = x - _width * 0.5;
    _y = y - _height * 0.5;
}
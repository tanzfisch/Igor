// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

template <class T>
std::wostream &operator<<(std::wostream &ostr, const iaColor4<T> &color)
{
    ostr << "(" << color._r << ", " << color._g << ", " << color._b << ", " << color._a << ")";
    return ostr;
}

template <typename T>
iaColor4<T>::iaColor4()
{
    _r = static_cast<T>(0);
    _g = static_cast<T>(0);
    _b = static_cast<T>(0);
    _a = static_cast<T>(0);
}

template <>
iaColor4<float32>::iaColor4();

template <>
iaColor4<uint8>::iaColor4();

template <typename T>
iaColor4<T>::iaColor4(T r, T g, T b, T a)
{
    _r = r;
    _g = g;
    _b = b;
    _a = a;
}

template <typename T>
void iaColor4<T>::set(T r, T g, T b, T a)
{
    _r = r;
    _g = g;
    _b = b;
    _a = a;
}

template <typename T>
T *iaColor4<T>::getData()
{
    return &_r;
}

template <typename T>
iaColor4<T> iaColor4<T>::operator=(const iaColor4<T> &color)
{
    iaColor4<T> result;
    result._r = _r = color._r;
    result._g = _g = color._g;
    result._b = _b = color._b;
    result._a = _a = color._a;

    return result;
}

template <typename T>
void iaColor4<T>::operator+=(const iaColor4<T> &color)
{
    _r += color._r;
    _g += color._g;
    _b += color._b;
    _a += color._a;
}

template <typename T>
void iaColor4<T>::operator-=(const iaColor4<T> &color)
{
    _r -= color._r;
    _g -= color._g;
    _b -= color._b;
    _a -= color._a;
}

template <typename T>
void iaColor4<T>::operator*=(float32 factor)
{
    _r *= factor;
    _g *= factor;
    _b *= factor;
    _a *= factor;
}

template <typename T>
void iaColor4<T>::operator*(float32 factor)
{
    iaColor4<T> result;
    result._r = _r * factor;
    result._g = _g * factor;
    result._b = _b * factor;
    result._a = _a * factor;
}

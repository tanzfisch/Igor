// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
std::wostream &operator<<(std::wostream &ostr, const iaColor3<T> &color)
{
    ostr << "(" << color._r << ", " << color._g << ", " << color._b << ")";
    return ostr;
}

template <typename T>
iaColor3<T>::iaColor3()
{
    _r = (T)0;
    _g = (T)0;
    _b = (T)0;
}

template <typename T>
iaColor3<T>::iaColor3(T r, T g, T b)
{
    _r = r;
    _g = g;
    _b = b;
}

template <typename T>
void iaColor3<T>::set(T r, T g, T b)
{
    _r = r;
    _g = g;
    _b = b;
}

template <typename T>
T *iaColor3<T>::getData()
{
    return &_r;
}

template <typename T>
iaColor3<T> iaColor3<T>::operator=(const iaColor3<T> &color)
{
    iaColor3<T> result;
    result._r = _r = color._r;
    result._g = _g = color._g;
    result._b = _b = color._b;

    return result;
}

template <typename T>
void iaColor3<T>::operator+=(const iaColor3<T> &color)
{
    _r += color._r;
    _g += color._g;
    _b += color._b;
}

template <typename T>
void iaColor3<T>::operator-=(const iaColor3<T> &color)
{
    _r -= color._r;
    _g -= color._g;
    _b -= color._b;
}

template <typename T>
void iaColor3<T>::operator*=(float32 value)
{
    _r = static_cast<T>(static_cast<float32>(_r) * value);
    _g = static_cast<T>(static_cast<float32>(_g) * value);
    _b = static_cast<T>(static_cast<float32>(_b) * value);
}

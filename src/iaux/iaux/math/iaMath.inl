// Igor game engineiaRandom
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

template <> inline
const float32 iaMath::lerp(const float32 &value1, const float32 &value2, float64 w)
{
    return value2 * w + value1 * (1.0 - w);
}

template <> inline
const float64 iaMath::lerp(const float64 &value1, const float64 &value2, float64 w)
{
    return value2 * w + value1 * (1.0 - w);
}

template <> inline
const uint32 iaMath::lerp(const uint32 &value1, const uint32 &value2, float64 w)
{
    return (uint32)round((float64)value2 * w + (float64)value1 * (1.0 - w));
}

template <> inline
const int32 iaMath::lerp(const int32 &value1, const int32 &value2, float64 w)
{
    return (int32)round((float64)value2 * w + (float64)value1 * (1.0 - w));
}

template <> inline
const uint64 iaMath::lerp(const uint64 &value1, const uint64 &value2, float64 w)
{
    return (uint64)round((float64)value2 * w + (float64)value1 * (1.0 - w));
}

template <> inline
const int64 iaMath::lerp(const int64 &value1, const int64 &value2, float64 w)
{
    return (int64)round((float64)value2 * w + (float64)value1 * (1.0 - w));
}

template <> inline
const iaColor3<float32> iaMath::lerp(const iaColor3<float32> &value1, const iaColor3<float32> &value2, float64 w)
{
    iaColor3<float32> result;

    const float64 w1 = 1.0 - w;

    result._r = value2._r * w + value1._r * w1;
    result._g = value2._g * w + value1._g * w1;
    result._b = value2._b * w + value1._b * w1;

    return result;
}

template <> inline
const iaColor3<uint8> iaMath::lerp(const iaColor3<uint8> &value1, const iaColor3<uint8> &value2, float64 w)
{
    iaColor3<uint8> result;

    const float64 w1 = 1.0 - w;

    result._r = value2._r * w + value1._r * w1;
    result._g = value2._g * w + value1._g * w1;
    result._b = value2._b * w + value1._b * w1;

    return result;
}

template <> inline
const iaColor4<float32> iaMath::lerp(const iaColor4<float32> &value1, const iaColor4<float32> &value2, float64 w)
{
    iaColor4<float32> result;

    const float64 w1 = 1.0 - w;

    result._r = value2._r * w + value1._r * w1;
    result._g = value2._g * w + value1._g * w1;
    result._b = value2._b * w + value1._b * w1;
    result._a = value2._a * w + value1._a * w1;

    return result;
}

template <> inline
const iaColor4<uint8> iaMath::lerp(const iaColor4<uint8> &value1, const iaColor4<uint8> &value2, float64 w)
{
    iaColor4<uint8> result;

    const float64 w1 = 1.0 - w;

    result._r = value2._r * w + value1._r * w1;
    result._g = value2._g * w + value1._g * w1;
    result._b = value2._b * w + value1._b * w1;
    result._a = value2._a * w + value1._a * w1;

    return result;
}    

template <> inline
const iaVector2<float32> iaMath::lerp(const iaVector2<float32> &value1, const iaVector2<float32> &value2, float64 w)
{
    iaVector2<float32> result;

    const float64 w1 = 1.0 - w;

    result._x = value2._x * w + value1._x * w1;
    result._y = value2._y * w + value1._y * w1;

    return result;
}

template <> inline
const iaVector2<float64> iaMath::lerp(const iaVector2<float64> &value1, const iaVector2<float64> &value2, float64 w)
{
    iaVector2<float64> result;

    const float64 w1 = 1.0 - w;

    result._x = value2._x * w + value1._x * w1;
    result._y = value2._y * w + value1._y * w1;

    return result;
}

template <> inline
const iaVector3<float32> iaMath::lerp(const iaVector3<float32> &value1, const iaVector3<float32> &value2, float64 w)
{
    iaVector3<float32> result;

    const float64 w1 = 1.0 - w;

    result._x = value2._x * w + value1._x * w1;
    result._y = value2._y * w + value1._y * w1;
    result._z = value2._z * w + value1._z * w1;

    return result;
}

template <> inline
const iaVector3<float64> iaMath::lerp(const iaVector3<float64> &value1, const iaVector3<float64> &value2, float64 w)
{
    iaVector3<float64> result;

    const float64 w1 = 1.0 - w;

    result._x = value2._x * w + value1._x * w1;
    result._y = value2._y * w + value1._y * w1;
    result._z = value2._z * w + value1._z * w1;

    return result;
}

template <> inline
const iaVector4<float32> iaMath::lerp(const iaVector4<float32> &value1, const iaVector4<float32> &value2, float64 w)
{
    con_assert(value1._w == value2._w, "can't lerp this");

    iaVector4<float32> result;

    const float64 w1 = 1.0 - w;

    result._x = value2._x * w + value1._x * w1;
    result._y = value2._y * w + value1._y * w1;
    result._z = value2._z * w + value1._z * w1;
    result._w = value2._w;

    return result;
}

template <> inline
const iaVector4<float64> iaMath::lerp(const iaVector4<float64> &value1, const iaVector4<float64> &value2, float64 w)
{
    con_assert(value1._w == value2._w, "can't lerp this");
    
    iaVector4<float64> result;

    const float64 w1 = 1.0 - w;

    result._x = value2._x * w + value1._x * w1;
    result._y = value2._y * w + value1._y * w1;
    result._z = value2._z * w + value1._z * w1;
    result._w = value2._w;

    return result;
}
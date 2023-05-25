// Igor game engineiaRandom
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/math/iaMath.h>

#include <iaux/math/iaVector2.h>
#include <iaux/math/iaVector3.h>
#include <iaux/math/iaVector4.h>
#include <iaux/data/iaColor4.h>
#include <iaux/data/iaColor3.h>

namespace iaux
{

    template <>
    const float32 iaMath::lerp(const float32 &value1, const float32 &value2, float64 w)
    {
        return value1 * w + value2 * (1.0 - w);
    }

    template <>
    const float64 iaMath::lerp(const float64 &value1, const float64 &value2, float64 w)
    {
        return value1 * w + value2 * (1.0 - w);
    }

    template <>
    const uint32 iaMath::lerp(const uint32 &value1, const uint32 &value2, float64 w)
    {
        return (uint32)round((float64)value1 * w + (float64)value2 * (1.0 - w));
    }

    template <>
    const int32 iaMath::lerp(const int32 &value1, const int32 &value2, float64 w)
    {
        return (int32)round((float64)value1 * w + (float64)value2 * (1.0 - w));
    }

    template <>
    const iaColor4<float32> iaMath::lerp(const iaColor4<float32> &value1, const iaColor4<float32> &value2, float64 w)
    {
        iaColor4<float32> result;

        const float64 w1 = 1.0 - w;

        result._r = value1._r * w + value2._r * w1;
        result._g = value1._g * w + value2._g * w1;
        result._b = value1._b * w + value2._b * w1;
        result._a = value1._a * w + value2._a * w1;

        return result;
    }

    template <>
    const iaColor4<uint8> iaMath::lerp(const iaColor4<uint8> &value1, const iaColor4<uint8> &value2, float64 w)
    {
        iaColor4<uint8> result;

        const float64 w1 = 1.0 - w;

        result._r = value1._r * w + value2._r * w1;
        result._g = value1._g * w + value2._g * w1;
        result._b = value1._b * w + value2._b * w1;
        result._a = value1._a * w + value2._a * w1;

        return result;
    }

    template <>
    const iaVector2<float32> iaMath::lerp(const iaVector2<float32> &value1, const iaVector2<float32> &value2, float64 w)
    {
        iaVector2<float32> result;

        const float64 w1 = 1.0 - w;

        result._x = value1._x * w + value2._x * w1;
        result._y = value1._y * w + value2._y * w1;

        return result;
    }

    template <>
    const iaVector2<float64> iaMath::lerp(const iaVector2<float64> &value1, const iaVector2<float64> &value2, float64 w)
    {
        iaVector2<float64> result;

        const float64 w1 = 1.0 - w;

        result._x = value1._x * w + value2._x * w1;
        result._y = value1._y * w + value2._y * w1;

        return result;
    }

    template <>
    const iaVector3<float32> iaMath::lerp(const iaVector3<float32> &value1, const iaVector3<float32> &value2, float64 w)
    {
        iaVector3<float32> result;

        const float64 w1 = 1.0 - w;

        result._x = value1._x * w + value2._x * w1;
        result._y = value1._y * w + value2._y * w1;
        result._z = value1._z * w + value2._z * w1;

        return result;
    }

    template <>
    const iaVector3<float64> iaMath::lerp(const iaVector3<float64> &value1, const iaVector3<float64> &value2, float64 w)
    {
        iaVector3<float64> result;

        const float64 w1 = 1.0 - w;

        result._x = value1._x * w + value2._x * w1;
        result._y = value1._y * w + value2._y * w1;
        result._z = value1._z * w + value2._z * w1;

        return result;
    }

    template <>
    const iaVector4<float32> iaMath::lerp(const iaVector4<float32> &value1, const iaVector4<float32> &value2, float64 w)
    {
        iaVector4<float32> result;

        const float64 w1 = 1.0 - w;

        result._x = value1._x * w + value2._x * w1;
        result._y = value1._y * w + value2._y * w1;
        result._z = value1._z * w + value2._z * w1;
        result._w = value1._w;

        return result;
    }

    template <>
    const iaVector4<float64> iaMath::lerp(const iaVector4<float64> &value1, const iaVector4<float64> &value2, float64 w)
    {
        iaVector4<float64> result;

        const float64 w1 = 1.0 - w;

        result._x = value1._x * w + value2._x * w1;
        result._y = value1._y * w + value2._y * w1;
        result._z = value1._z * w + value2._z * w1;
        result._w = value1._w;

        return result;
    }

}
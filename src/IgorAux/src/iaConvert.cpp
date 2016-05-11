#include <iaConvert.h>

namespace IgorAux
{

    iaColor3c iaConvert::convert3c(const iaColor3f& color)
    {
        iaColor3c result;

        result._r = static_cast<uint8>(color._r * 255.0f + 0.5f);
        result._g = static_cast<uint8>(color._g * 255.0f + 0.5f);
        result._b = static_cast<uint8>(color._b * 255.0f + 0.5f);

        return result;
    }

    iaColor3f iaConvert::convert3f(const iaColor3c& color)
    {
        iaColor3f result;

        result._r = static_cast<float32>(color._r / 255.0f);
        result._g = static_cast<float32>(color._g / 255.0f);
        result._b = static_cast<float32>(color._b / 255.0f);

        return result;
    }

    iaVector3f iaConvert::convert3f(const iaVector3I& vector)
    {
        iaVector3f result;

        result._x = static_cast<float32>(vector._x);
        result._y = static_cast<float32>(vector._y);
        result._z = static_cast<float32>(vector._z);

        return result;
    }

    iaVector3I iaConvert::convert3I(const iaVector3f& vector)
    {
        iaVector3I result;

        result._x = static_cast<uint64>(vector._x);
        result._y = static_cast<uint64>(vector._y);
        result._z = static_cast<uint64>(vector._z);

        return result;
    }

}
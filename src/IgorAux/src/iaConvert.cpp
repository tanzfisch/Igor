#include <iaConvert.h>

namespace IgorAux
{

    iaColor3c iaConvert::convert(const iaColor3f& color)
    {
        iaColor3c result;

        result._r = static_cast<uint8>(color._r * 255.0f + 0.5f);
        result._g = static_cast<uint8>(color._g * 255.0f + 0.5f);
        result._b = static_cast<uint8>(color._b * 255.0f + 0.5f);

        return result;
    }

    iaColor3f iaConvert::convert(const iaColor3c& color)
    {
        iaColor3f result;

        result._r = static_cast<float32>(color._r / 255.0f);
        result._g = static_cast<float32>(color._g / 255.0f);
        result._b = static_cast<float32>(color._b / 255.0f);

        return result;
    }

}
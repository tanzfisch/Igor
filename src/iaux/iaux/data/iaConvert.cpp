// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/data/iaConvert.h>

namespace IgorAux
{

    void iaConvert::convert(const iaColor3f &src, iaColor3c &dst)
    {
        dst._r = static_cast<uint8>(src._r * 255.0f + 0.5f);
        dst._g = static_cast<uint8>(src._g * 255.0f + 0.5f);
        dst._b = static_cast<uint8>(src._b * 255.0f + 0.5f);
    }

    void iaConvert::convert(const iaColor3c &src, iaColor3f &dst)
    {
        dst._r = static_cast<float32>(src._r / 255.0f);
        dst._g = static_cast<float32>(src._g / 255.0f);
        dst._b = static_cast<float32>(src._b / 255.0f);
    }

    void iaConvert::convert(const iaColor4f &src, iaColor4c &dst)
    {
        dst._r = static_cast<uint8>(src._r * 255.0f + 0.5f);
        dst._g = static_cast<uint8>(src._g * 255.0f + 0.5f);
        dst._b = static_cast<uint8>(src._b * 255.0f + 0.5f);
        dst._a = static_cast<uint8>(src._a * 255.0f + 0.5f);
    }

    void iaConvert::convert(const iaColor4c &src, iaColor4f &dst)
    {
        dst._r = static_cast<float32>(src._r / 255.0f);
        dst._g = static_cast<float32>(src._g / 255.0f);
        dst._b = static_cast<float32>(src._b / 255.0f);
        dst._a = static_cast<float32>(src._a / 255.0f);
    }
} // namespace IgorAux
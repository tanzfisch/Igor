// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iaConvert.h>

namespace IgorAux
{

    void iaConvert::convert(const iaColor3f& src, iaColor3c& dst)
    {
        dst._r = static_cast<uint8>(src._r * 255.0f + 0.5f);
        dst._g = static_cast<uint8>(src._g * 255.0f + 0.5f);
        dst._b = static_cast<uint8>(src._b * 255.0f + 0.5f);
    }

    void iaConvert::convert(const iaColor3c& src, iaColor3f& dst)
    {
        dst._r = static_cast<float32>(src._r / 255.0f);
        dst._g = static_cast<float32>(src._g / 255.0f);
        dst._b = static_cast<float32>(src._b / 255.0f);
    }

    void iaConvert::convert(const iaColor4f& src, iaColor4c& dst)
    {
        dst._r = static_cast<uint8>(src._r * 255.0f + 0.5f);
        dst._g = static_cast<uint8>(src._g * 255.0f + 0.5f);
        dst._b = static_cast<uint8>(src._b * 255.0f + 0.5f);
        dst._a = static_cast<uint8>(src._a * 255.0f + 0.5f);
    }

    void iaConvert::convert(const iaColor4c& src, iaColor4f& dst)
    {
        dst._r = static_cast<float32>(src._r / 255.0f);
        dst._g = static_cast<float32>(src._g / 255.0f);
        dst._b = static_cast<float32>(src._b / 255.0f);
        dst._a = static_cast<float32>(src._a / 255.0f);
    }

    void iaConvert::convert(const iaVector3I& src, iaVector3f& dst)
    {
        dst._x = static_cast<float32>(src._x);
        dst._y = static_cast<float32>(src._y);
        dst._z = static_cast<float32>(src._z);
    }

	void iaConvert::convert(const iaVector3i& src, iaVector3f& dst)
	{
		dst._x = static_cast<float32>(src._x);
		dst._y = static_cast<float32>(src._y);
		dst._z = static_cast<float32>(src._z);
	}

	void iaConvert::convert(const iaVector2i& src, iaVector2f& dst)
	{
		dst._x = static_cast<float32>(src._x);
		dst._y = static_cast<float32>(src._y);
	}

	void iaConvert::convert(const iaVector2i& src, iaVector2d& dst)
	{
		dst._x = static_cast<float64>(src._x);
		dst._y = static_cast<float64>(src._y);
	}


	void iaConvert::convert(const iaVector3i& src, iaVector3d& dst)
	{
		dst._x = static_cast<float64>(src._x);
		dst._y = static_cast<float64>(src._y);
		dst._z = static_cast<float64>(src._z);
	}

    void iaConvert::convert(const iaVector3f& src, iaVector3I& dst)
    {
        dst._x = static_cast<int64>(src._x);
        dst._y = static_cast<int64>(src._y);
        dst._z = static_cast<int64>(src._z);
    }

    void iaConvert::convert(const iaVector3d& src, iaVector3f& dst)
    {
        dst._x = static_cast<float32>(src._x);
        dst._y = static_cast<float32>(src._y);
        dst._z = static_cast<float32>(src._z);
    }

    void iaConvert::convert(const iaVector3I& src, iaVector3d& dst)
    {
        dst._x = static_cast<float64>(src._x);
        dst._y = static_cast<float64>(src._y);
        dst._z = static_cast<float64>(src._z);
    }

    void iaConvert::convert(const iaVector3d& src, iaVector3I& dst)
    {
        dst._x = static_cast<int64>(src._x);
        dst._y = static_cast<int64>(src._y);
        dst._z = static_cast<int64>(src._z);
    }

    void iaConvert::convert(const iaVector3f& src, iaVector3d& dst)
    {
        dst._x = static_cast<float64>(src._x);
        dst._y = static_cast<float64>(src._y);
        dst._z = static_cast<float64>(src._z);
    }

    void iaConvert::convert(const iaMatrixf& src, iaMatrixd& dst)
    {
        for (int i = 0; i < 16; ++i)
        {
            dst[i] = static_cast<float64>(src[i]);
        }
    }

    void iaConvert::convert(const iaMatrixd& src, iaMatrixf& dst)
    {
        for (int i = 0; i < 16; ++i)
        {
            dst[i] = static_cast<float32>(src[i]);
        }
    }

}
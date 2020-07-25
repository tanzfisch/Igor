// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/data/iaConvert.h>

namespace iaux
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

    void iaConvert::convertRGBtoHSV(const iaColor4f &rgb, iaColor4f &hsv)
    {
        iaColor3f src(rgb._r, rgb._g, rgb._b);
        iaColor3f dst;

        convertRGBtoHSV(src, dst);

        hsv.set(dst._r, dst._g, dst._b, rgb._a);
    }

    void iaConvert::convertRGBtoHSV(const iaColor3f &rgb, iaColor3f &hsv)
    {
        bool maxR = false;
        bool maxG = false;
        bool maxB = false;
        float32 max = 0;

        if (rgb._r > rgb._g)
        {
            if (rgb._r > rgb._b)
            {
                maxR = true;
                max = rgb._r;
            }
            else
            {
                maxB = true;
                max = rgb._b;
            }
        }
        else
        {
            if (rgb._g > rgb._b)
            {
                maxG = true;
                max = rgb._g;
            }
            else
            {
                maxB = true;
                max = rgb._b;
            }
        }

        float32 min = std::min(std::min(rgb._r, rgb._g), rgb._b);
        float32 delta = max - min;

        if (delta == 0)
        {
            hsv._r = 0;
        }
        else if (maxR)
        {
            const float64 tmp = static_cast<float64>(((rgb._g - rgb._b) / delta));
            hsv._r = static_cast<float32>(60.0 * fabs(fmod(tmp, 6.0)));
        }
        else if (maxG)
        {
            hsv._r = static_cast<float32>(60.0 * (((rgb._b - rgb._r) / delta) + 2.0));
        }
        else if (maxB)
        {
            hsv._r = static_cast<float32>(60.0 * (((rgb._r - rgb._g) / delta) + 4.0));
        }

        hsv._r /= 360.0;

        if (max == 0.0)
        {
            hsv._g = 0.0;
        }
        else
        {
            hsv._g = delta / max;
        }

        hsv._b = max;
    }

    void iaConvert::convertRGBtoHSV(const iaColor3c &rgb, iaColor3c &hsv)
    {
        iaColor3f src;
        iaColor3f dst;

        iaConvert::convert(rgb, src);
        convertRGBtoHSV(src, dst);
        iaConvert::convert(dst, hsv);
    }

    void iaConvert::convertRGBtoHSV(const iaColor4c &rgb, iaColor4c &hsv)
    {
        iaColor4f src;
        iaColor4f dst;

        iaConvert::convert(rgb, src);
        convertRGBtoHSV(src, dst);
        iaConvert::convert(dst, hsv);
    }

    void iaConvert::convertHSVtoRGB(const iaColor4f &hsv, iaColor4f &rgb)
    {
        iaColor3f src(hsv._r, hsv._g, hsv._b);
        iaColor3f dst;

        convertHSVtoRGB(src, dst);

        rgb.set(dst._r, dst._g, dst._b, hsv._a);
    }

    void iaConvert::convertHSVtoRGB(const iaColor3f &hsv, iaColor3f &rgb)
    {
        float32 h = hsv._r * 360.0;
        float32 s = hsv._g;
        float32 v = hsv._b;

        float32 c = v * s;
        float32 x = c * (1 - static_cast<float32>(fabs(fmod((static_cast<double>(h) / 60.0), 2.0) - 1.0)));

        float32 m = v - c;

        if (h < 60)
        {
            rgb._r = c;
            rgb._g = x;
            rgb._b = 0;
        }
        else if (h < 120)
        {
            rgb._r = x;
            rgb._g = c;
            rgb._b = 0;
        }
        else if (h < 180)
        {
            rgb._r = 0;
            rgb._g = c;
            rgb._b = x;
        }
        else if (h < 240)
        {
            rgb._r = 0;
            rgb._g = x;
            rgb._b = c;
        }
        else if (h < 300)
        {
            rgb._r = x;
            rgb._g = 0;
            rgb._b = c;
        }
        else if (h <= 360)
        {
            rgb._r = c;
            rgb._g = 0;
            rgb._b = x;
        }

        rgb._r += m;
        rgb._g += m;
        rgb._b += m;
    }

    void iaConvert::convertHSVtoRGB(const iaColor3c &hsv, iaColor3c &rgb)
    {
        iaColor3f src;
        iaColor3f dst;

        iaConvert::convert(hsv, src);
        convertHSVtoRGB(src, dst);
        iaConvert::convert(dst, rgb);
    }

    void iaConvert::convertHSVtoRGB(const iaColor4c &hsv, iaColor4c &rgb)
    {
        iaColor4f src;
        iaColor4f dst;

        iaConvert::convert(hsv, src);
        convertHSVtoRGB(src, dst);
        iaConvert::convert(dst, rgb);
    }
} // namespace iaux
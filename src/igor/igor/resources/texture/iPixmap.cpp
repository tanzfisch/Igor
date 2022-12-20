// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/texture/iPixmap.h>

#include <iaux/data/iaConvert.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <cstring>

namespace igor
{
    iPixmap::iPixmap(uint32 width, uint32 height, iColorFormat colorFormat)
    {
        _colorFormat = colorFormat;
        _width = width;
        _height = height;

        switch (_colorFormat)
        {
        case iColorFormat::RGB:
            _bytesPerPixel = 3;
            break;

        case iColorFormat::RGBA:
            _bytesPerPixel = 4;
            break;

        default:
            _bytesPerPixel = 0;
            con_err("unsupported color format");
            break;
        };

        uint64 size = _width * _height * _bytesPerPixel;

        if (size > 0)
        {
            _data = new uint8[size];
            memset(_data, 0, size);
        }
        else
        {
            con_err("invalid parameters");
        }
    }

    iPixmap::~iPixmap()
    {
        if (_data != nullptr)
        {
            delete[] _data;
            _data = nullptr;
        }
    }

    void iPixmap::setData(uint8 *data)
    {
        if (!data)
            return;
        if (!_bytesPerPixel)
            return;

        uint64 size = _width * _height * _bytesPerPixel;
        if (size > 0)
        {
            if (_data == nullptr)
            {
                _data = new uint8[size];
            }

            memcpy(_data, data, size);
        }
        else
        {
            con_err("inconsistant data");
        }
    }

    uint8 *iPixmap::getData() const
    {
        return _data;
    }

    uint32 iPixmap::getWidth() const
    {
        return _width;
    }

    uint32 iPixmap::getHeight() const
    {
        return _height;
    }

    uint32 iPixmap::getBytesPerPixel() const
    {
        return _bytesPerPixel;
    }

    iColorFormat iPixmap::getColorFormat() const
    {
        return _colorFormat;
    }

    uint32 iPixmap::getPixel(uint32 x, uint32 y) const
    {
        con_assert(x < _width && y < _height, "parameter out of range");
        con_assert(_bytesPerPixel == 4, "only 4 byte per pixel supported in this function");
        con_assert(_data != nullptr, "invalid data");

        return ((uint32 *)_data)[y * _width + x];
    }

    void iPixmap::setPixel(uint32 x, uint32 y, uint32 color)
    {
        con_assert(x < _width && y < _height, "parameter out of range");
        con_assert(_bytesPerPixel == 4, "only 4 byte per pixel supported in this function");
        con_assert(_data != nullptr, "invalid data");

        ((uint32 *)_data)[y * _width + x] = color;
    }

    uint8 iPixmap::getPixel(uint32 x, uint32 y, uint8 colorChannel) const
    {
        con_assert(x < _width && y < _height && colorChannel < _bytesPerPixel, "parameter out of range");
        con_assert(_data != nullptr, "invalid data");

        return _data[(y * _width + x) * _bytesPerPixel + colorChannel];
    }

    void iPixmap::setPixel(uint32 x, uint32 y, uint8 colorChannel, uint8 value)
    {
        con_assert(x < _width && y < _height && colorChannel < _bytesPerPixel, "parameter out of range");
        con_assert(_data != nullptr, "invalid data");

        _data[(y * _width + x) * _bytesPerPixel + colorChannel] = value;
    }

    void iPixmap::setPixelRGB(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b)
    {
        con_assert(x < _width && y < _height, "parameter out of range");
        con_assert(_data != nullptr && _bytesPerPixel == 3, "invalid data");

        _data[(y * _width + x) * _bytesPerPixel + 0] = r;
        _data[(y * _width + x) * _bytesPerPixel + 1] = g;
        _data[(y * _width + x) * _bytesPerPixel + 2] = b;
    }

    void iPixmap::setPixelRGBA(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b, uint8 a)
    {
        con_assert(x < _width && y < _height, "parameter out of range");
        con_assert(_data != nullptr && _bytesPerPixel == 4, "invalid data");

        uint64 pos = (y * _width + x) * _bytesPerPixel;

        _data[pos++] = r;
        _data[pos++] = g;
        _data[pos++] = b;
        _data[pos++] = a;
    }

    void iPixmap::getPixel(float64 x, float64 y, iaColor4f &color) const
    {
        con_assert(_data != nullptr && _bytesPerPixel == 4, "invalid data");
        iaColor4c temp;

        uint64 pos = (static_cast<uint64>(fmod(y, _height) + 0.5) * _width + static_cast<uint64>(fmod(x, _width) + 0.5)) * _bytesPerPixel;

        temp._r = _data[pos++];
        temp._g = _data[pos++];
        temp._b = _data[pos++];
        temp._a = _data[pos++];

        iaConvert::convert(temp, color);
    }

    void iPixmap::getPixelBiLinear(float64 x, float64 y, iaColor4f &color) const
    {
        con_assert(_data != nullptr && _bytesPerPixel == 4, "invalid data");
        iaColor4c c;
        iaColor4f c1;
        iaColor4f c2;
        iaColor4f c3;
        iaColor4f c4;

        uint64 xi = static_cast<uint64>(x);
        uint64 yi = static_cast<uint64>(y);

        float64 dx = x - static_cast<float64>(xi);
        float64 dy = y - static_cast<float64>(yi);

        uint64 pos = (yi * _width + xi) * _bytesPerPixel;

        c._r = _data[pos++];
        c._g = _data[pos++];
        c._b = _data[pos++];
        c._a = _data[pos++];

        iaConvert::convert(c, c1);

        c._r = _data[pos++];
        c._g = _data[pos++];
        c._b = _data[pos++];
        c._a = _data[pos++];

        iaConvert::convert(c, c2);

        c3.lerp(c2, c1, dx);

        yi++;
        pos = (yi * _width + xi) * _bytesPerPixel;

        c._r = _data[pos++];
        c._g = _data[pos++];
        c._b = _data[pos++];
        c._a = _data[pos++];

        iaConvert::convert(c, c1);

        c._r = _data[pos++];
        c._g = _data[pos++];
        c._b = _data[pos++];
        c._a = _data[pos++];

        iaConvert::convert(c, c2);

        c4.lerp(c2, c1, dx);

        color.lerp(c4, c3, dy);
    }

    void iPixmap::getPixel(float64 x, float64 y, iaColor4c &color) const
    {
        con_assert(_data != nullptr && _bytesPerPixel == 4, "invalid data");

        uint64 pos = (static_cast<uint64>(fmod(y, _height) + 0.5) * _width + static_cast<uint64>(fmod(x, _width) + 0.5)) * _bytesPerPixel;

        color._r = _data[pos++];
        color._g = _data[pos++];
        color._b = _data[pos++];
        color._a = _data[pos++];
    }

    void iPixmap::getPixel(float64 x, float64 y, iaColor3f &color) const
    {
        con_assert(_data != nullptr && _bytesPerPixel == 4, "invalid data");
        iaColor3c temp;

        uint64 pos = (static_cast<uint64>(fmod(y, _height) + 0.5) * _width + static_cast<uint64>(fmod(x, _width) + 0.5)) * _bytesPerPixel;

        temp._r = _data[pos++];
        temp._g = _data[pos++];
        temp._b = _data[pos++];

        iaConvert::convert(temp, color);
    }

    void iPixmap::getPixel(float64 x, float64 y, iaColor3c &color) const
    {
        con_assert(_data != nullptr && _bytesPerPixel == 4, "invalid data");

        uint64 pos = (static_cast<uint64>(fmod(y, _height) + 0.5) * _width + static_cast<uint64>(fmod(x, _width) + 0.5)) * _bytesPerPixel;

        color._r = _data[pos++];
        color._g = _data[pos++];
        color._b = _data[pos++];
    }

}; // namespace igor
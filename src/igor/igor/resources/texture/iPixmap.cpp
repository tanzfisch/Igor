// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/texture/iPixmap.h>

#include <igor/resources/iResourceManager.h>
#include <igor/resources/texture/iTextureFactory.h>

#include <iaux/data/iaConvert.h>
#include <iaux/system/iaConsole.h>
#include <iaux/math/iaMath.h>
using namespace iaux;

#include <cstring>

namespace igor
{

    iPixmapPtr iPixmap::createPixmap(uint32 width, uint32 height, iColorFormat colorFormat, uint8 *data)
    {
        return iPixmapPtr(new iPixmap(width, height, colorFormat, data));
    }

    iPixmapPtr iPixmap::loadPixmap(const iaString &filename)
    {
        return iTextureFactory::loadPixmap(filename);
    }

    iPixmap::iPixmap(uint32 width, uint32 height, iColorFormat colorFormat, uint8 *data)
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

        const uint64 size = _width * _height * _bytesPerPixel;
        con_assert(size > 0, "invalid size");
        _data = new uint8[size];

        if (data == nullptr)
        {
            memset(_data, 0, size);
        }
        else
        {
            memcpy(_data, data, size);
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

    const iaColor4c iPixmap::getPixelRGBA(float64 x, float64 y) const
    {
        con_assert(_data != nullptr && _bytesPerPixel == 4, "invalid data");

        uint64 pos = (static_cast<uint64>(fmod(y, _height) + 0.5) * _width + static_cast<uint64>(fmod(x, _width) + 0.5)) * _bytesPerPixel;

        iaColor4c color;
        color._r = _data[pos++];
        color._g = _data[pos++];
        color._b = _data[pos++];
        color._a = _data[pos++];

        return color;
    }

    void iPixmap::setPixel(float64 x, float64 y, const iaColor4c &color)
    {
        con_assert(_data != nullptr && _bytesPerPixel == 4, "invalid data");

        uint64 pos = (static_cast<uint64>(fmod(y, _height) + 0.5) * _width + static_cast<uint64>(fmod(x, _width) + 0.5)) * _bytesPerPixel;

        _data[pos++] = color._r;
        _data[pos++] = color._g;
        _data[pos++] = color._b;
        _data[pos++] = color._a;
    }    

    const iaColor3c iPixmap::getPixelRGB(float64 x, float64 y) const
    {
        con_assert(_data != nullptr && _bytesPerPixel == 3, "invalid data");        

        uint64 pos = (static_cast<uint64>(fmod(y, _height) + 0.5) * _width + static_cast<uint64>(fmod(x, _width) + 0.5)) * _bytesPerPixel;

        iaColor3c color;
        color._r = _data[pos++];
        color._g = _data[pos++];
        color._b = _data[pos++];

        return color;
    }

    void iPixmap::setPixel(float64 x, float64 y, const iaColor3c &color)
    {
        con_assert(_data != nullptr && _bytesPerPixel == 3, "invalid data");

        uint64 pos = (static_cast<uint64>(fmod(y, _height) + 0.5) * _width + static_cast<uint64>(fmod(x, _width) + 0.5)) * _bytesPerPixel;

        _data[pos++] = color._r;
        _data[pos++] = color._g;
        _data[pos++] = color._b;
    }    

}; // namespace igor
// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/data/iaColor4.h>
#include <iaux/math/iaRandom.h>

namespace iaux
{

template <>
iaColor4<float32>::iaColor4()
{
    _r = 1.0;
    _g = 1.0;
    _b = 1.0;
    _a = 1.0;
}

template <>
iaColor4<uint8>::iaColor4()
{
    _r = 0xff;
    _g = 0xff;
    _b = 0xff;
    _a = 0xff;
}

template <>
iaColor4<float32> iaColor4<float32>::random()
{
    return iaColor4<float32>(iaRandom::getNextFloatRange(0.2,0.8), iaRandom::getNextFloatRange(0.2,0.8), iaRandom::getNextFloatRange(0.2,0.8), 1.0);
}

template <>
iaColor4<uint8> iaColor4<uint8>::random()
{
    return iaColor4<uint8>(iaRandom::getNextRange(0x40, 0xd0), iaRandom::getNextRange(0x40, 0xd0), iaRandom::getNextRange(0x40, 0xd0), 0xff);
}
template <>
const iaColor4<float32> iaColor4<float32>::white(1.0, 1.0, 1.0, 1.0);

template <>
const iaColor4<uint8> iaColor4<uint8>::white(0xff, 0xff, 0xff, 0xff);

template <>
const iaColor4<float32> iaColor4<float32>::lightGray(0.75, 0.75, 0.75, 1.0);

template <>
const iaColor4<uint8> iaColor4<uint8>::lightGray(0xbf, 0xbf, 0xbf, 0xff);

template <>
const iaColor4<float32> iaColor4<float32>::gray(0.5, 0.5, 0.5, 1.0);

template <>
const iaColor4<uint8> iaColor4<uint8>::gray(0x80, 0x80, 0x80, 0xff);

template <>
const iaColor4<float32> iaColor4<float32>::darkGray(0.25, 0.25, 0.25, 1.0);

template <>
const iaColor4<uint8> iaColor4<uint8>::darkGray(0x40, 0x40, 0x40, 0xff);

template <>
const iaColor4<float32> iaColor4<float32>::black(0.0, 0.0, 0.0, 1.0);

template <>
const iaColor4<uint8> iaColor4<uint8>::black(0x00, 0x00, 0x00, 0xff);

template <>
const iaColor4<float32> iaColor4<float32>::red(1.0, 0.0, 0.0, 1.0);

template <>
const iaColor4<uint8> iaColor4<uint8>::red(0xff, 0x00, 0x00, 0xff);

template <>
const iaColor4<float32> iaColor4<float32>::green(0.0, 1.0, 0.0, 1.0);

template <>
const iaColor4<uint8> iaColor4<uint8>::green(0x00, 0xff, 0x00, 0xff);

template <>
const iaColor4<float32> iaColor4<float32>::blue(0.0, 0.0, 1.0, 1.0);

template <>
const iaColor4<uint8> iaColor4<uint8>::blue(0x00, 0x00, 0xff, 0xff);

template <>
const iaColor4<float32> iaColor4<float32>::yellow(1.0, 1.0, 0.0, 1.0);

template <>
const iaColor4<uint8> iaColor4<uint8>::yellow(0xff, 0xff, 0x00, 0xff);

template <>
const iaColor4<float32> iaColor4<float32>::magenta(1.0, 0.0, 1.0, 1.0);

template <>
const iaColor4<uint8> iaColor4<uint8>::magenta(0xff, 0x00, 0xff, 0xff);

template <>
const iaColor4<float32> iaColor4<float32>::cyan(0.0, 1.0, 1.0, 1.0);

template <>
const iaColor4<uint8> iaColor4<uint8>::cyan(0x00, 0xff, 0xff, 0xff);

template <>
const iaColor4<float32> iaColor4<float32>::transparent(0.0, 0.0, 0.0, 0.0);

template <>
const iaColor4<uint8> iaColor4<uint8>::transparent(0x00, 0x00, 0x00, 0x00);

}
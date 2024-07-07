// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/data/iaColor3.h>
#include <iaux/math/iaRandom.h>

namespace iaux
{

template <>
iaColor3<float32>::iaColor3()
{
    _r = 1.0;
    _g = 1.0;
    _b = 1.0;
}

template <>
iaColor3<uint8>::iaColor3()
{
    _r = 0xff;
    _g = 0xff;
    _b = 0xff;
}

template <>
iaColor3<float32> iaColor3<float32>::random()
{
    return iaColor3<float32>(iaRandom::getNextFloatRange(0.2,0.8), iaRandom::getNextFloatRange(0.2,0.8), iaRandom::getNextFloatRange(0.2,0.8));
}

template <>
iaColor3<uint8> iaColor3<uint8>::random()
{
    return iaColor3<uint8>(iaRandom::getNextRange(0x40, 0xd0), iaRandom::getNextRange(0x40, 0xd0), iaRandom::getNextRange(0x40, 0xd0));
}

template <>
const iaColor3<float32> iaColor3<float32>::white(1.0, 1.0, 1.0);

template <>
const iaColor3<uint8> iaColor3<uint8>::white(0xff, 0xff, 0xff);

template <>
const iaColor3<float32> iaColor3<float32>::lightGray(0.75, 0.75, 0.75);

template <>
const iaColor3<uint8> iaColor3<uint8>::lightGray(0xb, 0xbf, 0xbf);

template <>
const iaColor3<float32> iaColor3<float32>::gray(0.5, 0.5, 0.5);

template <>
const iaColor3<uint8> iaColor3<uint8>::gray(0x80, 0x80, 0x80);

template <>
const iaColor3<float32> iaColor3<float32>::darkGray(0.25, 0.25, 0.25);

template <>
const iaColor3<uint8> iaColor3<uint8>::darkGray(0x40, 0x40, 0x40);

template <>
const iaColor3<float32> iaColor3<float32>::black(0.0, 0.0, 0.0);

template <>
const iaColor3<uint8> iaColor3<uint8>::black(0x00, 0x00, 0x00);

template <>
const iaColor3<float32> iaColor3<float32>::red(1.0, 0.0, 0.0);

template <>
const iaColor3<uint8> iaColor3<uint8>::red(0xff, 0x00, 0x00);

template <>
const iaColor3<float32> iaColor3<float32>::green(0.0, 1.0, 0.0);

template <>
const iaColor3<uint8> iaColor3<uint8>::green(0x00, 0xff, 0x00);

template <>
const iaColor3<float32> iaColor3<float32>::blue(0.0, 0.0, 1.0);

template <>
const iaColor3<uint8> iaColor3<uint8>::blue(0x00, 0x00, 0xff);

template <>
const iaColor3<float32> iaColor3<float32>::yellow(1.0, 1.0, 0.0);

template <>
const iaColor3<uint8> iaColor3<uint8>::yellow(0xff, 0xff, 0x00);

template <>
const iaColor3<float32> iaColor3<float32>::magenta(1.0, 0.0, 1.0);

template <>
const iaColor3<uint8> iaColor3<uint8>::magenta(0xff, 0x00, 0xff);

template <>
const iaColor3<float32> iaColor3<float32>::cyan(0.0, 1.0, 1.0);

template <>
const iaColor3<uint8> iaColor3<uint8>::cyan(0x00, 0xff, 0xff);

}
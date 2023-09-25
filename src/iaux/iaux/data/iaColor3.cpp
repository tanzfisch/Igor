// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/data/iaColor3.h>

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
const iaColor3<float32> iaColor3<float32>::white(1.0, 1.0, 1.0);

template <>
const iaColor3<uint8> iaColor3<uint8>::white(0xff, 0xff, 0xff);

template <>
const iaColor3<float32> iaColor3<float32>::gray(0.5, 0.5, 0.5);

template <>
const iaColor3<uint8> iaColor3<uint8>::gray(0x80, 0x80, 0x80);

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
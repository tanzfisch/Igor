// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "Tiles.h"

#include <iaConsole.h>
using namespace IgorAux;

Tiles::Tiles(uint32 width, uint32 height)
: _width(width)
, _height(height)
{
    con_assert(0 != _width && 0 != _height, "invalid size");

    _data = new uint32[_width*_height];
    memset(_data, 0, _width*_height*sizeof(uint32));
}

Tiles::~Tiles()
{
    if (nullptr != _data)
    {
        delete[] _data;
    }
}

uint32 Tiles::getWidth()
{
    return _width;
}

uint32 Tiles::getHeight()
{
    return _height;
}

void Tiles::setTile(uint32 x, uint32 y, uint32 value)
{
    con_assert(x < _width && y < _height, "out of bounds");

    _data[y*_width + x] = value;
}

uint32 Tiles::getTile(uint32 x, uint32 y)
{
    con_assert(x < _width && y < _height, "out of bounds");

    return _data[y*_width + x];
}
// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iaRandomNumberGenerator.h>

#include <iaConsole.h>

namespace IgorAux
{

    void iaRandomNumberGenerator::setSeed(uint32 seed)
    {
        _seed = seed;
    }

    // is supposed to do the same as C++11 minstd_rand but maybe I got it wrong
    uint32 iaRandomNumberGenerator::getNext()
    {
        const uint64 _a = 48271;
        const uint64 _c = 0;
        const uint64 _m = 0x7FFFFFFF;

        _seed = (static_cast<uint64>(_seed) * _a + _c) % _m;
        return _seed;
    }
}
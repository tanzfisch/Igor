// Igor game engineiaRandom
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/math/iaRandomNumberGenerator.h>

namespace iaux
{

    iaRandomNumberGenerator::iaRandomNumberGenerator(uint64 seed)
    {
        _seed = seed;
    }

    iaRandomNumberGenerator::~iaRandomNumberGenerator()
    {
    }

    void iaRandomNumberGenerator::setSeed(uint64 seed)
    {
        _seed = seed;
    }

    uint64 iaRandomNumberGenerator::getNext()
    {
        const uint64 _a = 48271;
        const uint64 _c = 0;
        const uint64 _m = 0x7FFFFFFF;

        _seed = (static_cast<uint64>(_seed) * _a + _c) % _m;
        return _seed;
    }

    int64 iaRandomNumberGenerator::getNextRange(int64 min, int64 max)
    {
        con_assert(min <= max, "min has to be smaller then max");

        if(min == max)
        {
            return min;
        }

        return (getNext() % (max - min)) + min;
    }

    int64 iaRandomNumberGenerator::getNextRange(int64 range)
    {
        con_assert(range > 0, "range has to be greater then zero");
        return getNext() % range;
    }

    float64 iaRandomNumberGenerator::getNextFloatRange(float64 min, float64 max)
    {
        con_assert(min < max, "max needs to be greater then min");

        float64 result = getNext() % 1000000 / 1000000.0;
        result *= max - min;
        result += min;

        return result;
    }

    float64 iaRandomNumberGenerator::getNextFloat()
    {
        return getNext() % 1000000 / 1000000.0;
    }

}
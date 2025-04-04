// Igor game engineiaRandom
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/math/iaRandomNumberGenerator.h>

#include <algorithm>

namespace iaux
{

    iaRandomNumberGenerator::iaRandomNumberGenerator(uint64 seed)
    {
        _engine.seed(seed);
    }

    void iaRandomNumberGenerator::setSeed(uint64 seed)
    {
        _engine.seed(seed);
    }

    uint64 iaRandomNumberGenerator::getNext()
    {
        return _distribution(_engine);
    }

    int64 iaRandomNumberGenerator::getNextRange(int64 min, int64 max)
    {
        con_assert(min <= max, "min has to be smaller or equal then max");

        return (getNext() % (max - min + 1)) + min;
    }

    int64 iaRandomNumberGenerator::getNextRangeExponentialDecrease(int64 min, int64 max, float64 lambda)
    {
        const float64 u = getNextFloat();
        const float64 x = -lambda * log(1.0 - u);

        const int64 result = min + ((float64)(max - min + 1) * (1.0 - exp(-lambda * x)));
        return std::clamp(result, min, max);
    }

    int64 iaRandomNumberGenerator::getNextRangeExponentialIncrease(int64 min, int64 max, float64 lambda)
    {
        const float64 u = getNextFloat();
        const float64 x = -lambda * log(1.0 - u);

        const int64 result = min + ((float64)(max - min + 1) * exp(-lambda * x));
        return std::clamp(result, min, max);
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
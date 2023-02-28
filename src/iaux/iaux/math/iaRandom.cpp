// Igor game engineiaRandom
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/math/iaRandom.h>
#include <iaux/system/iaTime.h>

namespace iaux
{
    static iaRandomNumberGenerator _randomNumberGenerator(1337);

    void iaRandom::setSeed(uint64 seed)
    {
        _randomNumberGenerator.setSeed(seed);
    }

    uint64 iaRandom::getNext()
    {
        return _randomNumberGenerator.getNext();
    }

    int64 iaRandom::getNextRange(int64 min, int64 max)
    {
        return _randomNumberGenerator.getNextRange(min, max);
    }

    int64 iaRandom::getNextRangeExponentialDecrease(int64 min, int64 max, float64 lambda)
    {
        return _randomNumberGenerator.getNextRangeExponentialDecrease(min, max , lambda);
    }         

    int64 iaRandom::getNextRangeExponentialIncrease(int64 min, int64 max, float64 lambda)
    {
        return _randomNumberGenerator.getNextRangeExponentialIncrease(min, max , lambda);
    }     

    int64 iaRandom::getNextRange(int64 range)
    {
        return _randomNumberGenerator.getNextRange(range);
    }

    float64 iaRandom::getNextFloat()
    {
        return _randomNumberGenerator.getNextFloat();
    }

    float64 iaRandom::getNextFloatRange(float64 min, float64 max)
    {
        return _randomNumberGenerator.getNextFloatRange(min, max);
    }

} // namespace iaux

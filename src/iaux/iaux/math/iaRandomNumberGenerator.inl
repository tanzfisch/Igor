// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
iaRandomNumberGenerator<T>::iaRandomNumberGenerator(T seed)
{
    _seed = seed;
}

template <class T>
iaRandomNumberGenerator<T>::~iaRandomNumberGenerator()
{
}

template <class T>
void iaRandomNumberGenerator<T>::setSeed(T seed)
{
    _seed = seed;
}

template <class T>
__IGOR_INLINE__ T iaRandomNumberGenerator<T>::getNext()
{
    const uint64 _a = 48271;
    const uint64 _c = 0;
    const uint64 _m = 0x7FFFFFFF;

    _seed = (static_cast<uint64>(_seed) * _a + _c) % _m;
    return _seed;
}
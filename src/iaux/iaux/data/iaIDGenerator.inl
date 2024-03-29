// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
T iaIDGenerator<T>::getNextID()
{
    return _nextID.fetch_add(static_cast<T>(1), std::memory_order_relaxed);
}
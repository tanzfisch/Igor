// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
void* iComponent<T>::getData(uint64 entityID)
{
    con_assert(_data.find(entityID) != _data.end(), "entity " << entityID << " is not linked");
    return reinterpret_cast<void*>(&_data[entityID]);
}

template <class T>
void* iComponent<T>::getData()
{
    return reinterpret_cast<void*>(&_data);
}

template <class T>
void iComponent<T>::linkEntity(uint64 entityID)
{
    con_assert(_data.find(entityID) == _data.end(), "entity " << entityID << " was already linked");
    _data[entityID];
}

template <class T>
void iComponent<T>::unlinkEntity(uint64 entityID)
{
    auto iter = _data.find(entityID);
    con_assert(iter != _data.end(), "entity " << entityID << " was not linked");
    _data.erase(iter);
}
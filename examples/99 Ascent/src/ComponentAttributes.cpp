// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include "ComponentAttributes.h"

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    void* ComponentAttributes::getData(uint64 entityID)
    {
        con_assert(_data.find(entityID) != _data.end(), "entity " << entityID << " is not linked");
        return reinterpret_cast<void*>(&_data[entityID]);
    }

    void* ComponentAttributes::getData()
    {
        return reinterpret_cast<void*>(&_data);
    }

    void ComponentAttributes::linkEntity(uint64 entityID)
    {
        con_assert(_data.find(entityID) == _data.end(), "entity " << entityID << " was already linked");
        _data[entityID];
    }

    void ComponentAttributes::unlinkEntity(uint64 entityID)
    {
        auto iter = _data.find(entityID);
        con_assert(iter != _data.end(), "entity " << entityID << " was not linked");
        _data.erase(iter);
    }

}
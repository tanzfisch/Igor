// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntity.h>

#include <iEntityManager.h>

namespace Igor
{

    uint64 iEntity::_nextID = iEntity::INVALID_ENTITY_ID + 1;
    mutex iEntity::_mutexID;

    iEntity::iEntity()
    {
        _mutexID.lock();
        _id = _nextID++;
        _mutexID.unlock();
    }

    bool iEntity::hasPosition()
    {
        return false;
    }

    uint64 iEntity::getID() const
    {
        return _id;
    }

}
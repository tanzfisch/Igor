// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntity.h>

namespace Igor
{

    const uint64 iEntity::INVALID_ENTITY_ID = 0;
    uint64 iEntity::_nextID = iEntity::INVALID_ENTITY_ID + 1;
    mutex iEntity::_mutexID;

    iEntity::iEntity()
    {
        _mutexID.lock();
        _id = _nextID++;
        _mutexID.unlock();
    }

    uint64 iEntity::getID() const
    {
        return _id;
    }

    iEntityType iEntity::getType() const
    {
        return _type;
    }

}
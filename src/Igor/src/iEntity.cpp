// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntity.h>

#include <iEntityManager.h>

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

        iEntityManager::getInstance().registerEntity(this);
    }

    iEntity::~iEntity()
    {
        iEntityManager::getInstance().unregisterEntity(this);
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
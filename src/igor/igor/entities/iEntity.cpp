// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntity.h>

#include <igor/entities/iEntityManager.h>

namespace igor
{

    const uint64 iEntity::INVALID_ENTITY_ID = 0;
    uint64 iEntity::_nextID = iEntity::INVALID_ENTITY_ID + 1;
    iaMutex iEntity::_mutexID;

    iEntity::iEntity()
    {
        _mutexID.lock();
        _id = _nextID++;
        _mutexID.unlock();

        iEntityManager::getInstance().registerEntity(this);

        _type = iEntityType::Base;
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
// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iSystem.h>

namespace Igor
{

    uint64 iSystem::_nextSystemID = INVALID_SYSTEM_ID + 1;
    mutex iSystem::_mutexID;

    iSystem::iSystem()
    {
        _mutexID.lock();
        _id = _nextSystemID++;
        _mutexID.unlock();
    }

    uint64 iSystem::getID() const
    {
        return _id;
    }

    iEntityManager* iSystem::getEntityManager() const
    {
        return _entityManager;
    }

}
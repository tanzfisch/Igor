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

    void iEntity::kill()
    {
        _delete = true;
    }

    void iEntity::setPosition(const iaVector3d& position)
    {

    }

    iaVector3d iEntity::getPosition() const
    {
        return _sphere._center;
    }

    iSphered iEntity::getSphere() const
    {
        return _sphere;
    }

    uint64 iEntity::getID() const
    {
        return _id;
    }

}
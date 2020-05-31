// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/physics/iPhysicsCollision.h>

#include <igor/physics/iPhysics.h>

namespace Igor
{
    uint64 iPhysicsCollision::_nextCollisionID = INVALID_PHYSICSCOLLISION_ID + 1;

    iPhysicsCollision::iPhysicsCollision(void *collision, uint64 worldID)
    {
        _collision = collision;
        _worldID = worldID;

        _mutex.lock();
        _collisionID = _nextCollisionID++;
        _mutex.unlock();
    }

    uint64 iPhysicsCollision::getWorldID()
    {
        return _worldID;
    }

    uint64 iPhysicsCollision::getID()
    {
        return _collisionID;
    }

    void *iPhysicsCollision::getCollision() const
    {
        return _collision;
    }

} // namespace Igor
// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsCollision.h>

#include <iPhysics.h>

namespace Igor
{
    uint64 iPhysicsCollision::_nextCollisionID = INVALID_PHYSICSCOLLISION_ID + 1;

    iPhysicsCollision::iPhysicsCollision(void* collision)
    {
        _collision = collision;

        _mutex.lock();
        _collisionID = _nextCollisionID++;
        _mutex.unlock();
    }

    uint64 iPhysicsCollision::getID()
    {
        return _collisionID;
    }

    void* iPhysicsCollision::getCollision() const
    {
        return _collision;
    }

}
// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsCollision.h>

#include <iPhysics.h>

namespace Igor
{

    iPhysicsCollision::iPhysicsCollision(void* collision, uint64 collisionID)
    {
        _collision = collision;
        _collisionID = collisionID;
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
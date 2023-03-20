// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/physics/iPhysicsWorld.h>

#include <igor/physics/iPhysics.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    uint64 iPhysicsWorld::_nextID = iPhysicsWorld::INVALID_WORLD_ID + 1;

    iPhysicsWorld::iPhysicsWorld(void *newtonWorld)
        : _newtonWorld(newtonWorld)
    {
        _mutex.lock();
        _id = _nextID++;
        _mutex.unlock();
    }

    void *iPhysicsWorld::getNewtonWorld()
    {
        return _newtonWorld;
    }

    uint64 iPhysicsWorld::getID()
    {
        return _id;
    }

} // namespace igor
// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/physics/iPhysicsWorld.h>

#include <igor/physics/iPhysics.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iaIDGenerator64 iPhysicsWorld::_idGenerator;

    iPhysicsWorld::iPhysicsWorld(void *newtonWorld)
        : _newtonWorld(newtonWorld)
    {
        _id = _idGenerator.getNextID();
    }

    void *iPhysicsWorld::getNewtonWorld() const
    {
        return _newtonWorld;
    }

    iaID64 iPhysicsWorld::getID() const
    {
        return _id;
    }

} // namespace igor
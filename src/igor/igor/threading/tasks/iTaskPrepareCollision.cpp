// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/threading/tasks/iTaskPrepareCollision.h>

#include <igor/physics/iPhysics.h>
#include <iaux/system/iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iTaskPrepareCollision::iTaskPrepareCollision(uint64 collisionConfigID, uint32 priority)
        : iTask(nullptr, priority, false, iTaskContext::Default)
    {
        _collisionConfigID = collisionConfigID;
    }

    void iTaskPrepareCollision::run()
    {
        if (getWorldID() != iPhysicsWorld::INVALID_WORLD_ID)
        {
            iPhysicsCollisionConfig *collisionConfig = iPhysics::getInstance().getCollisionConfig(_collisionConfigID);
            if (collisionConfig != nullptr)
            {
                collisionConfig->finalize(getWorldID());
            }
        }
        else
        {
            con_err("invalid world id");
        }
    }

}; // namespace Igor

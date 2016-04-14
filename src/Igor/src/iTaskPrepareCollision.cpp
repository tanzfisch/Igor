// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskPrepareCollision.h>

#include <iPhysics.h>
#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iTaskPrepareCollision::iTaskPrepareCollision(uint64 collisionConfigID, uint32 priority)
        : iTask(nullptr, priority, false, iTaskContext::PhysicsContext)
    {
        _collisionConfigID = collisionConfigID;
    }

    void iTaskPrepareCollision::run()
    {
        if (getWorldID() != iPhysicsWorld::INVALID_WORLD_ID)
        {
            iPhysicsCollisionConfig* physicsCollisionConfig = iPhysics::getInstance().getCollisionConfig(_collisionConfigID);
            if (physicsCollisionConfig)
            {
                physicsCollisionConfig->finalize(getWorldID());
            }
            else
            {
                con_err("invalid collision config id");
            }
        }
        else
        {
            con_err("invalid world id");
        }

        finishTask();
    }

};

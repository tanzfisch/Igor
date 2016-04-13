// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskPrepareCollision.h>

#include <iPhysics.h>

namespace Igor
{

    iTaskPrepareCollision::iTaskPrepareCollision(uint64 collisionConfigID, uint32 priority)
        : iTask(nullptr, priority, false, iTaskContext::PhysicsContext)
    {
        _collisionConfigID = collisionConfigID;
    }

    void iTaskPrepareCollision::run()
    {
        iPhysicsCollisionConfig* physicsCollisionConfig = iPhysics::getInstance().getCollisionConfig(_collisionConfigID);
        if (physicsCollisionConfig)
        {
            physicsCollisionConfig->finalize();
        }
        finishTask();
    }

};

// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskPrepareCollision.h>

#include <iPhysicsCollisionConfig.h>

namespace Igor
{

    iTaskPrepareCollision::iTaskPrepareCollision(iPhysicsCollisionConfig* collisionConfig, uint32 priority)
        : iTask(nullptr, priority, false, iTaskContext::PhysicsContext)
    {
        _collisionConfig = collisionConfig;
    }

    void iTaskPrepareCollision::run()
    {
        _collisionConfig->finalize();

        finishTask();
    }

};

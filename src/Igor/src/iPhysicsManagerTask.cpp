// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsManagerTask.h>
#include <iNodePhysics.h>

namespace Igor
{

    iPhysicsManagerTask::iPhysicsManagerTask(list<iNode*>& listToUpdate)
        : iTask(nullptr, 0, false, false)
    {
        _listToUpdate = std::move(listToUpdate);
    }

    iPhysicsManagerTask::~iPhysicsManagerTask()
    {

    }

    void iPhysicsManagerTask::run()
    {
        for (auto node : _listToUpdate)
        {
            iNodePhysics* physicsNode = static_cast<iNodePhysics*>(node);
            physicsNode->updatePhysics();
        }

        _isRunning = false;
    }
}

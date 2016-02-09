// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsManagerTask.h>
#include <iNodePhysics.h>
#include <iNodeFactory.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iPhysicsManagerTask::iPhysicsManagerTask(uint32 nodeID)
        : iTask(nullptr, 0, false, false)
    {
        _nodeID = nodeID;
    }

    iPhysicsManagerTask::~iPhysicsManagerTask()
    {

    }

    void iPhysicsManagerTask::run()
    {
        iNodePhysics* node = static_cast<iNodePhysics*>(iNodeFactory::getInstance().getNode(_nodeID));
        if (node != nullptr)
        {
            node->updatePhysics();
        }

        _isRunning = false;
    }
}

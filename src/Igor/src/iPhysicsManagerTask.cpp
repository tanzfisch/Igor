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

    iPhysicsManagerTask::iPhysicsManagerTask(vector<uint32>& listToUpdate)
        : iTask(nullptr, 0, false, false)
    {
        _listToUpdate = std::move(listToUpdate);
    }

    iPhysicsManagerTask::~iPhysicsManagerTask()
    {

    }

    void iPhysicsManagerTask::run()
    {
        for (auto nodeID : _listToUpdate)
        {
            iNodePhysics* node = static_cast<iNodePhysics*>(iNodeFactory::getInstance().getNode(nodeID));
            if (node != nullptr)
            {
                node->updatePhysics();
            }
        }

        _isRunning = false;
    }
}

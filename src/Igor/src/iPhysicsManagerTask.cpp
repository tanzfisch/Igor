// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsManagerTask.h>

namespace Igor
{

    iPhysicsManagerTask::iPhysicsManagerTask(list<iNode*>& listToUpdate)
    {
        _listToUpdate = std::move(listToUpdate);
    }

    iPhysicsManagerTask::~iPhysicsManagerTask()
    {

    }

    void iPhysicsManagerTask::run()
    {

    }
}

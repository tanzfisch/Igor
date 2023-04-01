// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/threading/tasks/iTaskUpdateSceneData.h>

#include <thread>

namespace igor
{

    iTaskUpdateSceneData::iTaskUpdateSceneData(iScenePtr parentScene)
    : iTask(nullptr, iTask::HIGH_PRIORITY, true, iTaskContext::Default)
    , _parentScene(parentScene)
    {
    
    }

    void iTaskUpdateSceneData::run()
    {
        _parentScene->updateData();
    }

    void iTaskUpdateSceneData::abort()
    {
        _parentScene->abortUpdateData();
        iTask::abort();
    }

}; // namespace igor

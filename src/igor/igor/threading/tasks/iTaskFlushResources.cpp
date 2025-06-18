// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/threading/tasks/iTaskFlushResources.h>

#include <igor/resources/iResourceManager.h>
#include <igor/system/iWindow.h>

#include <thread>

namespace igor
{

    iTaskFlushResources::iTaskFlushResources(iWindowPtr window)
        : iTask(window, iTask::TASK_PRIORITY_DEFAULT, true, iTaskContext::RenderContext)
    {
    }

    void iTaskFlushResources::run()
    {
        iResourceManager::getInstance().flush(iResourceCacheMode::Free);
    }

    void iTaskFlushResources::abort()
    {
        iResourceManager::getInstance().interruptFlush();
        setRepeating(false);

        while (isRunning())
        {
            std::this_thread::yield();
        }
    }

}; // namespace igor

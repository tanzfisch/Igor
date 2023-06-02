// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/threading/tasks/iTaskFlushResources.h>

#include <igor/resources/iResourceManager.h>
#include <igor/system/iWindow.h>

#include <thread>

namespace igor
{

    iTaskFlushResources::iTaskFlushResources(iWindowPtr window)
        : iTask(window, iTask::DEFAULT_PRIORITY, true, iTaskContext::RenderContext)
    {
    }

    void iTaskFlushResources::run()
    {
        iResourceManager::getInstance().flush(iResourceCacheMode::Free);
    }

    void iTaskFlushResources::abort()
    {
        iResourceManager::getInstance().interruptFlush();

        while (isRunning())
        {
            std::this_thread::yield();
        }

        setRepeating(false);
    }

}; // namespace igor

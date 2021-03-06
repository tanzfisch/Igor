// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/threading/tasks/iTaskFlushModels.h>

#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/system/iWindow.h>

#include <thread>

namespace igor
{

    iTaskFlushModels::iTaskFlushModels(iWindow *window)
        : iTask(window, iTask::DEFAULT_PRIORITY, true, iTaskContext::Default)
    {
    }

    void iTaskFlushModels::run()
    {
        iModelResourceFactory::getInstance().flush(getWindow(), iResourceCacheMode::Free);
    }

    void iTaskFlushModels::abort()
    {
        iModelResourceFactory::getInstance().interruptFlush();

        while (isRunning())
        {
            std::this_thread::yield();
        }

        setRepeating(false);
    }

}; // namespace igor

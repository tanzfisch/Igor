// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskFlushModels.h>

#include <iModelResourceFactory.h>
#include <iWindow.h>

namespace Igor
{

    iTaskFlushModels::iTaskFlushModels(iWindow* window)
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
            _sleep(1);
        }

        setRepeating(false);
    }

};

// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskFlushTextures.h>

#include <iTextureResourceFactory.h>
#include <iWindow.h>

namespace Igor
{

    iTaskFlushTextures::iTaskFlushTextures(iWindow* window)
        : iTask(window, iTask::DEFAULT_PRIORITY, true, iTaskContext::RenderContext)
    {
    }

    void iTaskFlushTextures::run()
    {
        iTextureResourceFactory::getInstance().flush(iResourceCacheMode::Cache);

        finishTask();
    }

    void iTaskFlushTextures::abort()
    {
        iTextureResourceFactory::getInstance().interruptFlush();

        while (isRunning())
        {
            _sleep(1);
        }

        setRepeating(false);
    }

};

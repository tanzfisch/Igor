// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
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
        iTextureResourceFactory::getInstance().flush(iResourceCacheMode::Free);
    }

    void iTaskFlushTextures::abort()
    {
        iTextureResourceFactory::getInstance().interruptFlush();

        while (isRunning())
        {
            Sleep(1);
        }

        setRepeating(false);
    }

};

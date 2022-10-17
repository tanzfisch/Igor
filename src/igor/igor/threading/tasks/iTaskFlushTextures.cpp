// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/threading/tasks/iTaskFlushTextures.h>

#include <igor/resources/texture/iTextureResourceFactory.h>
#include <igor/system/iWindow.h>

#include <thread>

namespace igor
{

    iTaskFlushTextures::iTaskFlushTextures(iWindow *window)
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
            std::this_thread::yield();
        }

        setRepeating(false);
    }

}; // namespace igor

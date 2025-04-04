// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/threading/tasks/iTaskGenerateThumbnails.h>

#include <igor/resources/texture/iThumbnailCache.h>
#include <igor/resources/iResourceManager.h>

#include <thread>

namespace igor
{
    iTaskGenerateThumbnails::iTaskGenerateThumbnails(iWindowPtr window)
        : iTask(window, iTask::TASK_PRIORITY_LOW, true, iTaskContext::RenderContext)
    {
    }

    void iTaskGenerateThumbnails::run()
    {
        iThumbnailCache::getInstance().generateThumbnails();
    }

    void iTaskGenerateThumbnails::abort()
    {
        iResourceManager::getInstance().interruptFlush();
        setRepeating(false);

        while (isRunning())
        {
            std::this_thread::yield();
        }
    }
}
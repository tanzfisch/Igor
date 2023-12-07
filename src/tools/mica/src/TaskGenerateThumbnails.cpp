// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "TaskGenerateThumbnails.h"

#include "ThumbnailCache.h"

#include <thread>

TaskGenerateThumbnails::TaskGenerateThumbnails(iWindowPtr window)
    : iTask(window, iTask::TASK_PRIORITY_LOW, true, iTaskContext::RenderContext)
{
}

void TaskGenerateThumbnails::run()
{
    ThumbnailCache::getInstance().generateThumbnails();
}

void TaskGenerateThumbnails::abort()
{
    iResourceManager::getInstance().interruptFlush();
    setRepeating(false);

    while (isRunning())
    {
        std::this_thread::yield();
    }
}

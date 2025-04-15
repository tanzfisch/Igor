// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/threading/tasks/iTaskWatchFilesystem.h>

#include <igor/system/iFilesystem.h>
#include <igor/system/iApplication.h>
#include <igor/events/iEventFilesystem.h>

#include <sys/inotify.h>
#include <unistd.h>

namespace igor
{

    iTaskWatchFilesystem::iTaskWatchFilesystem(const iaString &path)
        : iTask(nullptr, iTask::TASK_PRIORITY_LOW, false, iTaskContext::Default), _path(path)
    {
    }

    void iTaskWatchFilesystem::run()
    {
        int inotify = inotify_init1(IN_NONBLOCK);
        if (inotify < 0)
        {
            con_err("failed to init filesystem watch");
            return;
        }

        const size_t bufSize = 4096;
        char buffer[bufSize];
        _path.getData(buffer, bufSize);

        int watch = inotify_add_watch(inotify, buffer, IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO);
        if (watch == -1)
        {
            con_err("initialization to watch path " << _path << " failed");
            close(inotify);
            return;
        }

        con_debug("watching path " << _path);

        _running = true;
        while (_running)
        {
            int length = read(inotify, buffer, bufSize);
            if (length < 0)
            {
                if (errno == EAGAIN)
                {
                    usleep(500000); // sleep 0.5s
                    continue;
                }

                con_err("failed to watch path " << _path);
                break;
            }

            int i = 0;
            while (i < length)
            {
                struct inotify_event *event = (struct inotify_event *)&buffer[i];
                std::string name = event->len ? event->name : "";

                if (event->mask & IN_CREATE)
                {
                    iApplication::getInstance().onEvent(iEventPtr(new iEventFileCreated(name.c_str())));                    
                }
                else if (event->mask & IN_DELETE)
                {
                    iApplication::getInstance().onEvent(iEventPtr(new iEventFileDeleted(name.c_str())));
                }
                else if (event->mask & IN_MOVED_FROM)
                {
                    iApplication::getInstance().onEvent(iEventPtr(new iEventFileMovedFrom(name.c_str())));
                }
                else if (event->mask & IN_MOVED_TO)
                {
                    iApplication::getInstance().onEvent(iEventPtr(new iEventFileMovedTo(name.c_str())));
                }
                else if (event->mask & IN_MODIFY)
                {
                    iApplication::getInstance().onEvent(iEventPtr(new iEventFileChanged(name.c_str())));
                }

                i += sizeof(struct inotify_event) + event->len;
            }
        }

        inotify_rm_watch(inotify, watch);
        close(inotify);

        con_debug("stopped watching path " << _path);
    }

    void iTaskWatchFilesystem::abort()
    {
        _running = false;
        iTask::abort();
    }    

}; // namespace igor

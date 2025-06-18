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

    iTaskWatchFilesystem::iTaskWatchFilesystem(const iaString &path, bool recursive)
        : iTask(nullptr, iTask::TASK_PRIORITY_LOW, false, iTaskContext::Default), _path(path), _recursive(recursive)
    {
    }

    int iTaskWatchFilesystem::watchDir(const iaDirectory &dir)
    {
        const size_t bufSize = 4096;
        char buffer[bufSize];

        dir.getAbsoluteDirectoryName().getData(buffer, bufSize);
        return inotify_add_watch(_inotify, buffer, IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO);
    }

    bool iTaskWatchFilesystem::onWatchDir(const iaString &dir)
    {
        int watch = watchDir(dir);
        if (watch == -1)
        {
            con_err("initialization to watch path " << dir << " failed");
            close(_inotify);
            return false;
        }

        _subdirs[watch] = dir;
        return true;
    }

    void iTaskWatchFilesystem::run()
    {
        _inotify = inotify_init1(IN_NONBLOCK);
        if (_inotify < 0)
        {
            con_err("failed to init filesystem watch");
            return;
        }

        if (!onWatchDir(_path))
        {
            return;
        }

        if (_recursive)
        {
            iaDirectory path(_path);
            for (const auto &subdir : path.getDirectories(true, false))
            {
                if (!onWatchDir(subdir.getAbsoluteDirectoryName()))
                {
                    return;
                }
            }
        }

        con_debug("watching path " << _path);

        _running = true;
        const size_t bufSize = 4096;
        char buffer[bufSize];

        while (_running)
        {
            int length = read(_inotify, buffer, bufSize);
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
                int watch = event->wd;
                auto iterSubDir = _subdirs.find(watch);
                if (iterSubDir == _subdirs.end())
                {
                    con_err("internal error");
                }
                const iaString fullPath = iterSubDir->second + IGOR_PATHSEPARATOR + iaString(name.c_str());

                if (event->mask & IN_CREATE)
                {
                    if (iaDirectory::isDirectory(fullPath))
                    {
                        onWatchDir(fullPath);
                    }

                    iApplication::getInstance().onEvent(iEventPtr(new iEventFileCreated(fullPath)));
                }
                else if (event->mask & IN_DELETE)
                {
                    const auto iter = std::find_if(_subdirs.begin(), _subdirs.end(), [&fullPath](const auto &pair)
                                                   { return pair.second == fullPath; });
                    if (iter != _subdirs.end())
                    {
                        _subdirs.erase(iter);
                    }

                    iApplication::getInstance().onEvent(iEventPtr(new iEventFileDeleted(name.c_str())));
                }
                else if (event->mask & IN_MOVED_FROM)
                {
                    const auto iter = std::find_if(_subdirs.begin(), _subdirs.end(), [&fullPath](const auto &pair)
                                                   { return pair.second == fullPath; });
                    if (iter != _subdirs.end())
                    {
                        _subdirs.erase(iter);
                    }

                    iApplication::getInstance().onEvent(iEventPtr(new iEventFileMovedFrom(name.c_str())));
                }
                else if (event->mask & IN_MOVED_TO)
                {
                    if (iaDirectory::isDirectory(fullPath))
                    {
                        onWatchDir(fullPath);
                    }

                    iApplication::getInstance().onEvent(iEventPtr(new iEventFileMovedTo(name.c_str())));
                }
                else if (event->mask & IN_MODIFY)
                {
                    iApplication::getInstance().onEvent(iEventPtr(new iEventFileChanged(name.c_str())));
                }

                i += sizeof(struct inotify_event) + event->len;
            }
        }

        for (const auto &subdir : _subdirs)
        {
            inotify_rm_watch(_inotify, subdir.first);
        }
        close(_inotify);

        con_debug("stopped watching path " << _path);
    }

    void iTaskWatchFilesystem::abort()
    {
        _running = false;
        iTask::abort();
    }

}; // namespace igor

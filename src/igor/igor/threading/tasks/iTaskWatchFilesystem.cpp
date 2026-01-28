// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
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

    void iTaskWatchFilesystem::unwatchDir(const iaString &path)
    {
        if(_path == path)
        {
            _running = false;
            return;
        }

        const auto iter = std::find_if(_subdirs.begin(), _subdirs.end(), [&path](const auto &pair)
                                       { return pair.second == path; });
        if (iter != _subdirs.end())
        {
            inotify_rm_watch(_inotify, iter->first);
            _subdirs.erase(iter);
        }
        else
        {
            con_warn("path " << path << " was not watched");
        }
    }

    bool iTaskWatchFilesystem::watchDir(const iaString &dir)
    {
        const iaPath path(dir);
        const size_t bufSize = 65536;
        char buffer[bufSize];

        path.getAbsolutePath().getData(buffer, bufSize);
        int watch = inotify_add_watch(_inotify, buffer, IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO | IN_MODIFY | IN_DELETE_SELF | IN_MOVE_SELF);

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

        if (!watchDir(_path))
        {
            return;
        }

        if (_recursive)
        {
            iaPath path(_path);
            for (const auto &subdir : path.getDirectories(true, false))
            {
                if (!watchDir(subdir.getAbsolutePath()))
                {
                    return;
                }
            }
        }

        con_debug("watching path " << _path << (_recursive ? " (recursive)" : ""));

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
                bool isDir = (event->mask & IN_ISDIR);
                int watch = event->wd;
                auto iterSubDir = _subdirs.find(watch);
                if (iterSubDir == _subdirs.end())
                {
                    con_err("internal error. stopped watching folder " << _path);
                    _running = false;
                    break;
                }

                iaString fullPath = iterSubDir->second;

                if (!name.empty())
                {
                    fullPath += iaString(IGOR_PATHSEPARATOR) + iaString(name.c_str());
                }

                if (event->mask & IN_CREATE)
                {
                    if (isDir)
                    {
                        watchDir(fullPath);
                    }

                    iApplication::getInstance().onEvent(iEventPtr(new iEventFileCreated(fullPath)));
                }
                else if (event->mask & IN_DELETE)
                {
                    if (isDir)
                    {
                        unwatchDir(fullPath);
                    }

                    iApplication::getInstance().onEvent(iEventPtr(new iEventFileDeleted(fullPath)));
                }
                else if (event->mask & IN_MOVED_FROM)
                {
                    if (isDir)
                    {
                        unwatchDir(fullPath);
                    }

                    iApplication::getInstance().onEvent(iEventPtr(new iEventFileMovedFrom(fullPath)));
                }
                else if (event->mask & IN_MOVED_TO)
                {
                    if (isDir)
                    {
                        watchDir(fullPath);
                    }

                    iApplication::getInstance().onEvent(iEventPtr(new iEventFileMovedTo(fullPath)));
                }
                else if (event->mask & IN_MODIFY)
                {
                    iApplication::getInstance().onEvent(iEventPtr(new iEventFileChanged(fullPath)));
                }
                else if (event->mask & IN_DELETE_SELF ||
                         event->mask & IN_MOVE_SELF)
                {
                    if (isDir)
                    {
                        unwatchDir(fullPath);
                    }

                    iApplication::getInstance().onEvent(iEventPtr(new iEventFolderDeleted(fullPath)));
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

// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/system/iFilesystem.h>

#include <igor/threading/tasks/iTaskWatchFilesystem.h>
#include <igor/threading/iTaskManager.h>

namespace igor
{

    iFilesystem::iFilesystem()
    {
    }

    void iFilesystem::listenToChanges(const iaString &path)
    {
        if(_watchers.find(path) != _watchers.end())
        {
            con_err("already watching path " << path);
            return;
        }

        _watchers[path] = iTaskManager::getInstance().addTask(new iTaskWatchFilesystem(path));
    }

    void iFilesystem::stopListenToChanges(const iaString& path)
    {
        auto iter = _watchers.find(path);
        if(iter == _watchers.end())
        {
            return;
        }

        const iTaskID id = iter->second;
        _watchers.erase(iter);

        iTaskManager::getInstance().abortTask(id);
    }

} // namespace igor

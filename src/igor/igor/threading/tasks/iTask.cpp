// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/threading/tasks/iTask.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <thread>

namespace igor
{

    iTaskID iTask::_nextTaskID = 1;
    iaMutex iTask::_mutexID;

    iTask::iTask(iWindow *window, uint32 priority, bool isRepeating, iTaskContext taskContext)
    {
        if (taskContext == iTaskContext::RenderContext)
        {
            con_assert(window != nullptr, "zero pointer, need window if render context is required");
        }

        _mutexID.lock();
        _taskID = _nextTaskID++;
        _mutexID.unlock();

        _taskContext = taskContext;
        _isRepeating = isRepeating;
        _priority = priority;
        _window = window;
    }

    void iTask::setWorld(uint64 worldID)
    {
        _worldID = worldID;
    }

    void iTask::setRepeating(bool repeat)
    {
        _isRepeating = repeat;
    }

    uint64 iTask::getWorldID()
    {
        return _worldID;
    }

    iWindow *iTask::getWindow()
    {
        return _window;
    }

    void iTask::finishTask()
    {
        _isRunning = false;
    }

    iTaskID iTask::getID()
    {
        return _taskID;
    }

    uint32 iTask::getPriority()
    {
        return _priority;
    }

    iTaskContext iTask::getContext()
    {
        return _taskContext;
    }

    bool iTask::isRunning()
    {
        return _isRunning;
    }

    bool iTask::isRepeating()
    {
        return _isRepeating;
    }

    void iTask::abort()
    {
        _isRepeating = false;

        while (isRunning())
        {
            std::this_thread::yield();
        }
    }

    void iTask::setPriority(uint32 priority)
    {
        _priority = priority;
    }

}; // namespace igor

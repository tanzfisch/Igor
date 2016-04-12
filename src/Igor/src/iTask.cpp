// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iTask.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    uint64 iTask::_nextTaskID = 1;
    mutex iTask::_mutexID;

    iTask::iTask(iWindow* window, uint32 priority, bool isRepeating, iTaskContext taskContext)
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

    uint64 iTask::getID()
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
            _sleep(1);
        }
    }

    void iTask::setPriority(uint32 priority)
    {
        _priority = priority;
    }

};

// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iTask.h>

#include <windows.h>

namespace Igor
{

    uint64 iTask::_nextTaskID = 1;

    iTask::iTask(iWindow* window, uint32 priority, bool isRepeating, bool needsRenderContext)
    {
        _taskID = _nextTaskID++;

        _needsRenderContext = needsRenderContext;
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

	bool iTask::needsRenderContext()
	{
        return _needsRenderContext;
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

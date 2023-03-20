// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/system/iTimer.h>

#include <igor/system/iApplication.h>

#include <iaux/system/iaClock.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iTimer::iTimer()
    {
        _currentActualTime = _currentTime = iaTime::getNow();
        _timeDelta = 0;
    }

    iTimer::~iTimer()
    {
        if (_timerHandles.size())
        {
            con_warn("possible mem leak. Unregister all timer handles");
        }
    }

    void iTimer::stop()
    {
        _timeRunning = false;
    }

    void iTimer::start()
    {
        _timeRunning = true;
        _currentActualTime = iaTime::getNow() - _timeDelta;
    }

    void iTimer::onUpdate()
    {
        if (!_timeRunning)
        {
            return;
        }

        iaTime now = iaTime::getNow();
        _timeDelta = now - _currentActualTime;
        _currentActualTime = now;

        _currentTime += _timeDelta;

        handleTimerHandles();
    }

    iaTime iTimer::getTime() const
    {
        return _currentTime;
    }

    iaTime iTimer::getTimeDelta() const
    {
        return _timeDelta;
    }

    float64 iTimer::getFPS() const
    {
        return 1.0 / _timeDelta.getSeconds();
    }

    void iTimer::handleTimerHandles()
    {
        _mutexHandleList.lock();
        std::vector<iTimerHandle *> timerHandles(_timerHandles);
        _mutexHandleList.unlock();

        for (auto handle : timerHandles)
        {
            handle->handle(_currentTime);
        }
    }

    void iTimer::insertTimerHandle(const iTimerHandle *timerHandle)
    {
        _mutexHandleList.lock();
        _timerHandles.push_back(const_cast<iTimerHandle *>(timerHandle));
        _mutexHandleList.unlock();
    }

    void iTimer::removeTimerHandle(const iTimerHandle *timerHandle)
    {
        _mutexHandleList.lock();
        auto iter = std::find(_timerHandles.begin(), _timerHandles.end(), timerHandle);

        if (iter != _timerHandles.end())
        {
            _timerHandles.erase(iter);
        }
        _mutexHandleList.unlock();
    }

} // namespace igor
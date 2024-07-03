// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
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
        _gameTime = _realTime = iaTime::getNow();
    }

    iTimer::~iTimer()
    {
        if (_timerHandles.size())
        {
            con_warn("possible memory leak. Unregister all timer handles");
        }
    }

    void iTimer::stop()
    {
        _timeRunning = false;
    }

    void iTimer::start()
    {
        _timeRunning = true;
        _realTime = iaTime::getNow();
    }

    void iTimer::nextFrame()
    {
        iaTime delta = iaTime::getNow() - _realTime;
        _realTime += delta;

        if (!_timeRunning)
        {
            return;
        }

        _timeDeltaIndex = (_timeDeltaIndex + 1) % TIME_DELTAS;
        _timeDeltas[_timeDeltaIndex] = delta;
        _gameTime += delta;
    }

    const iaTime &iTimer::getTime() const
    {
        return _gameTime;
    }

    const iaTime& iTimer::getRealTime() const
    {
        return _realTime;
    }

    iaTime iTimer::getPeakTimeDelta() const
    {
        iaTime result;
        for (int i = 0; i < TIME_DELTAS; ++i)
        {
            if (result < _timeDeltas[i])
            {
                result = _timeDeltas[i];
            }
        }

        return result;
    }

    iaTime iTimer::getAverageTimeDelta() const
    {
        iaTime result;

        int counter = 0;

        for (int i = 0; i < TIME_DELTAS; ++i)
        {
            if (_timeDeltas[i].getMicroseconds() == 0)
            {
                continue;
            }
            result += _timeDeltas[i];
            counter++;
        }

        result /= counter;

        return result;
    }

    const iaTime& iTimer::getTimeDelta() const
    {
        return _timeDeltas[_timeDeltaIndex];
    }

    float64 iTimer::getAverageFPS() const
    {
        const float64 seconds = getAverageTimeDelta().getSeconds();
        return seconds != 0.0 ? 1.0 / seconds : 0.0;
    }

    float64 iTimer::getFPS() const
    {
        const float64 seconds = _timeDeltas[_timeDeltaIndex].getSeconds();
        return seconds != 0.0 ? 1.0 / seconds : 0.0;
    }

    void iTimer::onUpdate()
    {
        _mutexHandleList.lock();
        std::vector<iTimerHandle *> timerHandles(_timerHandles);
        _mutexHandleList.unlock();

        for (auto handle : timerHandles)
        {
            handle->handle(_gameTime);
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
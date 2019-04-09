// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iTimer.h>

#include <iApplication.h>

#include <iaClock.h>
using namespace IgorAux;

namespace Igor
{

	iTimer::iTimer()
	{
		_timeScale = iaClock::getTickScale();
		_lastTime = iaClock::getClockTicks();

		_startTime = _currentTime = _lastTime;
		_timeDelta = 0;
	}

	iTimer::~iTimer()
	{
		if (_timerHandles.size())
		{
			con_err("unregister all timer handles first");
		}
	}

	float64 iTimer::getApplicationTime() const
	{
		return iaClock::getClockMiliseconds();
	}

	void iTimer::handle()
	{
		_currentTime = iaClock::getClockTicks();
		_timeDelta = _currentTime - _lastTime;

		if (_currentTime < _lastTime)
		{
			con_warn("iTimer Overflow!");
		}

		_lastTime = _currentTime;

		handleTimerHandles();
	}

	float64 iTimer::getMilliSecondsDelta() const
	{
		return _timeDelta * _timeScale * __IGOR_SECOND__;
	}

	float64 iTimer::getSecondsDelta() const
	{
		return _timeDelta * _timeScale;
	}

	float32 iTimer::getFPS() const
	{
		return (float32)(1.0f / (_timeDelta * _timeScale));
	}

	float64 iTimer::getMilliSeconds() const
	{
		return _lastTime * _timeScale * __IGOR_SECOND__;
	}

	float64 iTimer::getSeconds() const
	{
		return _lastTime * _timeScale;
	}

	void iTimer::handleTimerHandles()
	{
		float64 time = getMilliSeconds();
		for (uint32 i = 0; i < _timerHandles.size(); i++)
		{
			_timerHandles[i]->handle(time);
		}
	}

	void iTimer::insertTimerHandle(const iTimerHandle* timer_handle)
	{
		_timerHandles.push_back(const_cast<iTimerHandle*>(timer_handle));
	}

	void iTimer::removeTimerHandle(const iTimerHandle *timer_handle)
	{
		auto iter = _timerHandles.begin();
		while (iter != _timerHandles.end())
		{
			if ((*iter) == timer_handle)
			{
				_timerHandles.erase(iter);
				return;
			}

			iter++;
		}
	}

}
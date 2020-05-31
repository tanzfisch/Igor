// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/os/iTimer.h>

#include <igor/os/iApplication.h>

#include <iaux/system/iaClock.h>
#include <iaux/system/iaConsole.h>
using namespace IgorAux;

#include <ctime>

namespace Igor
{

	iTimer::iTimer()
	{
		_startTime = _currentTime = _lastTime = iaClock::getTimeMiliseconds();
		_timeDelta = 0;
	}

	iTimer::~iTimer()
	{
		if (_timerHandles.size())
		{
			con_err("unregister all timer handles first");
		}
	}

	uint32 iTimer::getDay() const
	{
		time_t t = time(nullptr);
		gmtime(&t);
		struct tm *now = localtime(&t);
		return now->tm_mday;
	}

	uint32 iTimer::getMonth() const
	{
		time_t t = time(nullptr);
		gmtime(&t);
		struct tm *now = localtime(&t);
		return now->tm_mon;
	}

	uint32 iTimer::getYear() const
	{
		time_t t = time(nullptr);
		gmtime(&t);
		struct tm *now = localtime(&t);
		return now->tm_year;
	}

	float64 iTimer::getApplicationTime() const
	{
		return iaClock::getTimeMiliseconds();
	}

	void iTimer::handle()
	{
		_currentTime = iaClock::getTimeMiliseconds();
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
		return _timeDelta * 1000 * __IGOR_SECOND__;
	}

	float64 iTimer::getSecondsDelta() const
	{
		return _timeDelta * 1000;
	}

	float32 iTimer::getFPS() const
	{
		return 1000.0f / static_cast<float32>(_timeDelta);
	}

	float64 iTimer::getMilliSeconds() const
	{
		return _lastTime * 1000 * __IGOR_SECOND__;
	}

	float64 iTimer::getSeconds() const
	{
		return _lastTime * 1000;
	}

	void iTimer::handleTimerHandles()
	{
		float64 time = getMilliSeconds();
		std::vector<iTimerHandle *> timerHandles(_timerHandles);
		for (auto handle : timerHandles)
		{
			handle->handle(time);
		}
	}

	void iTimer::insertTimerHandle(const iTimerHandle *timerHandle)
	{
		_timerHandles.push_back(const_cast<iTimerHandle *>(timerHandle));
	}

	void iTimer::removeTimerHandle(const iTimerHandle *timerHandle)
	{
		auto iter = std::find(_timerHandles.begin(), _timerHandles.end(), timerHandle);

		if (iter != _timerHandles.end())
		{
			_timerHandles.erase(iter);
		}
	}

} // namespace Igor
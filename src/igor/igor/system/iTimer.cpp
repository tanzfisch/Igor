// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/system/iTimer.h>

#include <igor/system/iApplication.h>

#include <iaux/system/iaClock.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <ctime>

namespace igor
{

	iTimer::iTimer()
	{
		_startTime = _currentTime = _lastTime = iaTime::now();
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

	iaTime iTimer::getApplicationTime() const
	{
		return iaTime::now();
	}

	void iTimer::handle()
	{
		_currentTime = iaTime::now();
		_timeDelta = _currentTime - _lastTime;

		if (_currentTime < _lastTime)
		{
			con_warn("iTimer Overflow!");
		}

		_lastTime = _currentTime;

		handleTimerHandles();
	}

	iaTime iTimer::getFrameTime() const
	{
		return _currentTime;
	}

	iaTime iTimer::getFrameTimeDelta() const
	{
		return _timeDelta;
	}

	float64 iTimer::getFPS() const
	{
		return 1.0 / _timeDelta.getSeconds();
	}

	void iTimer::handleTimerHandles()
	{
		std::vector<iTimerHandle *> timerHandles(_timerHandles);
		for (auto handle : timerHandles)
		{
			handle->handle(_currentTime);
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

} // namespace igor
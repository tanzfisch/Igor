// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
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
		_startTime = _currentTime = iaTime::getNow();
		_timeDelta = 0;
	}

	iTimer::~iTimer()
	{
		if (_timerHandles.size())
		{
			con_err("unregister all timer handles first");
		}
	}	

	void iTimer::handle()
	{
		iaTime now = iaTime::getNow();
		_timeDelta = now - _currentTime;
		_currentTime = now;

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
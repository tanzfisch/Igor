// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/system/iTimerHandle.h>

#include <igor/system/iTimer.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

	iTimerHandle::iTimerHandle(iTimerTickDelegate timerDelegate, const iaTime &interval, bool oneShot)
	{
		_timerEvent.add(timerDelegate);
		setInterval(interval);
		_oneShot = oneShot;
	}

	iTimerHandle::~iTimerHandle()
	{
		stop();
	}

	void iTimerHandle::restart()
	{
		stop();
		start();
	}

	void iTimerHandle::start()
	{
		if (_playing)
		{
			return;
		}

		_interval = _configuredInterval;

		iTimer::getInstance().insertTimerHandle(this);
		_time = iTimer::getInstance().getTime();
		_playing = true;
	}

	void iTimerHandle::stop()
	{
		if (!_playing)
		{
			return;
		}

		_playing = false;
		iTimer::getInstance().removeTimerHandle(this);
	}

	void iTimerHandle::setInterval(iaTime interval)
	{
		if (interval <= iaTime(0))
		{
			con_err("invalid value for _interval");
			return;
		}

		_configuredInterval = _interval = interval;
		restart();
	}

	iaTime iTimerHandle::getInterval() const
	{
		return _configuredInterval;
	}

	iTimerTickEvent &iTimerHandle::getEventTimerTick()
	{
		return _timerEvent;
	}

	void iTimerHandle::triggerNow()
	{
		_timerEvent(iaTime::getNow());
	}

	void iTimerHandle::handle(iaTime time)
	{
		while (time - _time >= _interval)
		{
			_timerEvent(time);
			if (_oneShot)
			{
				stop();
				break;
			}

			_time += _interval;
		}
	}

}; // namespace igor

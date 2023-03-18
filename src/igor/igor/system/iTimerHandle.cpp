// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/system/iTimerHandle.h>

#include <igor/system/iTimer.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

	iTimerHandle::iTimerHandle(iTimerTickDelegate timerDelegate, const iaTime &interval, bool oneShot)
	{
		_timerEvent.append(timerDelegate);
		setIntervall(interval);
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

		_intervall = _configuredIntervall;

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

	void iTimerHandle::setIntervall(iaTime interval)
	{
		if (interval <= iaTime(0))
		{
			con_err("invalid value for _intervall");
			return;
		}

		_configuredIntervall = _intervall = interval;
		restart();
	}

	iaTime iTimerHandle::getIntervall() const
	{
		return _configuredIntervall;
	}

	void iTimerHandle::registerTimerDelegate(iTimerTickDelegate timerDelegate)
	{
		_timerEvent.append(timerDelegate);
	}

	void iTimerHandle::unregisterTimerDelegate(iTimerTickDelegate timerDelegate)
	{
		_timerEvent.remove(timerDelegate);
	}

	void iTimerHandle::triggerNow()
	{
		_timerEvent(iaTime::getNow());
	}

	void iTimerHandle::handle(iaTime time)
	{
		while (time - _time >= _intervall)
		{
			_timerEvent(time);
			if (_oneShot)
			{
				stop();
				break;
			}

			_time += _intervall;
		}
	}

}; // namespace igor

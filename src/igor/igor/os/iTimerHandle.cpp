// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/os/iTimerHandle.h>

#include <igor/os/iTimer.h>

#include <iaux/system/iaConsole.h>
using namespace IgorAux;

namespace Igor
{

	iTimerHandle::iTimerHandle(iTimerTickDelegate timerDelegate, float64 interval, bool oneShot)
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
		_time = iTimer::getInstance().getMilliSeconds();
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

	void iTimerHandle::setIntervall(float64 interval)
	{
		if (interval <= 0)
		{
			con_err("invalid value for _intervall");
			return;
		}

		_configuredIntervall = _intervall = interval;
		restart();
	}

	float64 iTimerHandle::getIntervall()
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

	void iTimerHandle::handle(float64 time)
	{
		int maxIntervalls = 10;
		while (time - _time >= _intervall)
		{
			_timerEvent();
			if (_oneShot)
			{
				stop();
				break;
			}

			_time += _intervall;

			if (!--maxIntervalls)
			{
				_time = time;
				if (_intervall < _configuredIntervall * 4)
				{
					_intervall *= 2;
					con_warn("Dropping frames. Increasing intervall from " << _configuredIntervall << " to " << _intervall << "ms");
				}
				return;
			}
		}
	}

}; // namespace Igor

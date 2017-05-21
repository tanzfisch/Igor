// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iTimerHandle.h>

#include <iTimer.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

	iTimerHandle::iTimerHandle()
	{
        this->_time = iTimer::getInstance().getMilliSeconds();
        iTimer::getInstance().insertTimerHandle(this);
	}

	iTimerHandle::~iTimerHandle()
	{
        iTimer::getInstance().removeTimerHandle(this);
	}

	void iTimerHandle::restart()
	{
		_time = iTimer::getInstance().getMilliSeconds();
	}

	void iTimerHandle::setIntervall(float64 interval)
	{
		if (interval <= 0)
		{
			con_err("invalid value for _intervall");
			return;
		}

		this->_intervall = interval;
		restart();
	}

	float64 iTimerHandle::getIntervall()
	{
		return _intervall;
	}

	void iTimerHandle::registerTimerDelegate(iTimerTickDelegate timer_delegate)
	{
		_timerEvent.append(timer_delegate);
	}

	void iTimerHandle::unregisterTimerDelegate(iTimerTickDelegate timer_delegate)
	{
		_timerEvent.remove(timer_delegate);
	}

	void iTimerHandle::handle(float64 _time)
	{
		while (_time - this->_time >= _intervall)
		{
			_timerEvent();
			this->_time += _intervall;
		}
	}

};

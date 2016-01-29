// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "iStopWatch.h"
#include "iTimer.h"

namespace Igor
{

	iStopWatch::iStopWatch()
	{
	}

	void iStopWatch::start()
	{
		_startTime = iTimer::getInstance().getTime();
        _running = true;
	}

	void iStopWatch::stop()
	{
		_stopTime = iTimer::getInstance().getTime();
        _running = false;
	}

	float64 iStopWatch::getMilliSeconds()
	{
#ifdef __IGOR_DEBUG__
        _lastMeasuredDelta = _stopTime - _startTime;
#endif
        
        return _stopTime - _startTime;
	}

	float64 iStopWatch::getSeconds()
	{
#ifdef __IGOR_DEBUG__
        _lastMeasuredDelta = _stopTime - _startTime;
#endif

		return (_stopTime - _startTime) / IGOR_SECOND;
	}

    float64 iStopWatch::getCurrentMilliSeconds()
    {
        float64 stopTime = iTimer::getInstance().getTime();
#ifdef __IGOR_DEBUG__
        _lastMeasuredDelta = stopTime - _startTime;
#endif
        return stopTime - _startTime;
    }

    float64 iStopWatch::getCurrentSeconds()
    {
        float64 stopTime = iTimer::getInstance().getTime();
#ifdef __IGOR_DEBUG__
        _lastMeasuredDelta = stopTime - _startTime;
#endif
        return (stopTime - _startTime) / IGOR_SECOND;
    }

    bool iStopWatch::isRunning()
    {
        return _running;
    }

}
// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/os/iStopWatch.h>
#include <igor/os/iTimer.h>

namespace Igor
{

        iStopWatch::iStopWatch()
        {
        }

        void iStopWatch::start()
        {
                _startTime = iTimer::getInstance().getApplicationTime();
                _running = true;
        }

        void iStopWatch::stop()
        {
                _stopTime = iTimer::getInstance().getApplicationTime();
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

                return (_stopTime - _startTime) / __IGOR_SECOND__;
        }

        float64 iStopWatch::getCurrentMilliSeconds()
        {
                float64 stopTime = iTimer::getInstance().getApplicationTime();
#ifdef __IGOR_DEBUG__
                _lastMeasuredDelta = stopTime - _startTime;
#endif
                return stopTime - _startTime;
        }

        float64 iStopWatch::getCurrentSeconds()
        {
                float64 stopTime = iTimer::getInstance().getApplicationTime();
#ifdef __IGOR_DEBUG__
                _lastMeasuredDelta = stopTime - _startTime;
#endif
                return (stopTime - _startTime) / __IGOR_SECOND__;
        }

        bool iStopWatch::isRunning()
        {
                return _running;
        }

} // namespace Igor
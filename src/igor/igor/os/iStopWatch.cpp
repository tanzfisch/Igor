// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/os/iStopWatch.h>
#include <igor/os/iTimer.h>

namespace Igor
{
        void iStopWatch::start()
        {
                _stopTime = _startTime = iaTime::now();
                _running = true;
        }

        void iStopWatch::stop()
        {
                _stopTime = iaTime::now();
                _running = false;
        }

        iaTime iStopWatch::getTime() const
        {
                if (_running)
                {
                        return iaTime::now() - _startTime;
                }
                else
                {
                        return _stopTime - _startTime;
                }
        }

        bool iStopWatch::isRunning() const
        {
                return _running;
        }

} // namespace Igor
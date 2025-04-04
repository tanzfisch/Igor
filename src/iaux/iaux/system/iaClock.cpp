// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/system/iaClock.h>

#ifdef IGOR_WINDOWS
#include <Windows.h>
#endif

#ifdef IGOR_LINUX
#include <time.h>
#endif

#include <iostream>

namespace iaux
{
#ifdef IGOR_WINDOWS
    static LARGE_INTEGER s_frequency;
    static LARGE_INTEGER s_startTime;
#endif

#ifdef IGOR_LINUX
    static int64 s_startTime;
#endif

    float64 iaClock::getTimeSeconds()
    {
        return static_cast<float64>(getTimeMicroseconds()) / 1000000.0;
    }

    float64 iaClock::getTimeMilliseconds()
    {
        return static_cast<float64>(getTimeMicroseconds()) / 1000.0;
    }

    int64 iaClock::getTimeMicroseconds()
    {
#ifdef IGOR_WINDOWS
        LARGE_INTEGER count;
        QueryPerformanceCounter(&count);
        count.QuadPart -= s_startTime.QuadPart;
        uint64 ticks = uint64(count.QuadPart * LONGLONG(1000000) / s_frequency.QuadPart);
        return ticks;
#endif

#ifdef IGOR_LINUX
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        return (int64(ts.tv_sec) * 1000000) + int64(ts.tv_nsec / 1000) - s_startTime;
#endif
    }

    /*! initializes clock
    */
    void iaClock::resetClock()
    {
#ifdef IGOR_WINDOWS
        if (QueryPerformanceFrequency(&s_frequency))
        {
            QueryPerformanceCounter((LARGE_INTEGER *)&s_startTime);
        }
        else
        {
            std::cout << "Error: found no performance counter" << std::endl;
            exit(0);
        }
#endif

#ifdef IGOR_LINUX
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts); // Works on Linux
        s_startTime = uint64(ts.tv_sec) * 1000000 + ts.tv_nsec / 1000;
#endif
    }

} // namespace iaux
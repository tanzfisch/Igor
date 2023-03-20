// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/system/iaClock.h>

#ifdef __IGOR_WINDOWS__
#include <Windows.h>
#endif

#ifdef __IGOR_LINUX__
#include <time.h>
#endif

#include <iostream>

namespace iaux
{
#ifdef __IGOR_WINDOWS__
    static LARGE_INTEGER s_frequency;
    static LARGE_INTEGER s_startTime;
#endif

#ifdef __IGOR_LINUX__
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
#ifdef __IGOR_WINDOWS__
        LARGE_INTEGER count;
        QueryPerformanceCounter(&count);
        count.QuadPart -= s_startTime.QuadPart;
        uint64 ticks = uint64(count.QuadPart * LONGLONG(1000000) / s_frequency.QuadPart);
        return ticks;
#endif

#ifdef __IGOR_LINUX__
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        return (int64(ts.tv_sec) * 1000000) + int64(ts.tv_nsec / 1000) - s_startTime;
#endif
    }

    /*! initializes clock
    */
    void iaClock::resetClock()
    {
#ifdef __IGOR_WINDOWS__
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

#ifdef __IGOR_LINUX__
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts); // Works on Linux
        s_startTime = uint64(ts.tv_sec) * 1000000 + ts.tv_nsec / 1000;
#endif
    }

} // namespace iaux
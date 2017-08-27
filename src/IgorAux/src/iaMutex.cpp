// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iaMutex.h>

#include <iaClock.h>
#include <iaConsole.h>

#include <mutex>
using namespace std;

namespace IgorAux
{

	iaMutex::iaMutex()
	{
		m_handle = static_cast<iaMutexHandle>(new mutex());
	}

	iaMutex::~iaMutex()
	{
		if (m_handle != nullptr)
		{
			delete m_handle;
		}

#ifdef __IGOR_USE_MUTEX_PROFILER__
		if (m_waiting != 0)
		{
			float64 ratio = static_cast<float64>(m_running) / static_cast<float64>(m_waiting);
			con_endl("mutex " << ratio << " " << m_running << ":" << m_waiting);
		}
#endif
	}

	void iaMutex::lock()
	{
#ifdef __IGOR_USE_MUTEX_PROFILER__
		uint64 time = iaClock::getClockTicks();
#endif

		static_cast<mutex*>(m_handle)->lock();

#ifdef __IGOR_USE_MUTEX_PROFILER__
		m_time = iaClock::getClockTicks();
		m_waiting += m_time - time;
#endif
	}

	void iaMutex::unlock()
	{
		static_cast<mutex*>(m_handle)->unlock();
#ifdef __IGOR_USE_MUTEX_PROFILER__
		m_running += iaClock::getClockTicks() - m_time;
#endif
	}

}
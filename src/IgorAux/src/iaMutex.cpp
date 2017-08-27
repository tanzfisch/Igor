// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iaMutex.h>

#include <iaClock.h>
#include <iaSystem.h>
#include <iaConsole.h>

#include <mutex>
using namespace std;

namespace IgorAux
{

	iaMutex::iaMutex()
	{
#ifdef __IGOR_USE_MUTEX_PROFILER__
		getCallStack(m_callStack);
#endif
		m_handle = static_cast<iaMutexHandle>(new mutex());
	}

	iaMutex::~iaMutex()
	{
		if (m_handle != nullptr)
		{
			delete m_handle;
		}

#ifdef __IGOR_USE_MUTEX_PROFILER__
		if (m_waiting != 0 && m_running != 0)
		{
			float64 ratio = static_cast<float64>(m_running) / static_cast<float64>(m_waiting);
			float64 running = m_running * iaClock::getTickScale() * __IGOR_SECOND__;
			float64 waiting = m_waiting * iaClock::getTickScale() * __IGOR_SECOND__;
			if (ratio < 1.0 && 
				waiting > 1000)
			{
				con_warn("bad mutex");
				con_endl(iaForegroundColor::DarkYellow << __IGOR_TAB__ << "ratio  :" << ratio);
				con_endl(iaForegroundColor::DarkYellow << __IGOR_TAB__ << "running:" << iaString::ftoa(running, 6) << "ms");
				con_endl(iaForegroundColor::DarkYellow << __IGOR_TAB__ << "waiting:" << iaString::ftoa(waiting, 6) << "ms");
				for (auto entry : m_callStack)
				{
					con_endl(iaForegroundColor::DarkYellow << __IGOR_TAB__ << entry);
				}
			}
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
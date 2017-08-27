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

		
		float64 maximum = m_waitingMax * iaClock::getTickScale() * __IGOR_SECOND__;
		float64 total = m_waitingTotal * iaClock::getTickScale() * __IGOR_SECOND__;
		float64 average = total / static_cast<float64>(m_lockCount);

		if (maximum > 10 ||
			total > 1000 ||
			average > 2)
		{
			con_warn("bad mutex");
			con_endl(iaForegroundColor::DarkYellow << __IGOR_TAB__ << "locked: " << m_lockCount << " total: " << iaString::ftoa(total, 6) << "ms average: " << iaString::ftoa(average, 6) << "ms max: " << iaString::ftoa(maximum, 6) << "ms");
			
			for (auto entry : m_callStack)
			{
				con_endl(iaForegroundColor::DarkYellow << __IGOR_TAB__ << entry);
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
		m_lockCount++;
		uint64 diff = iaClock::getClockTicks() - time;
		if (diff > m_waitingMax)
		{
			m_waitingMax = diff;
		}
		if (diff > 0)
		{
			m_blockCount++;
		}
		m_waitingTotal += diff;
#endif
	}

	void iaMutex::unlock()
	{
		static_cast<mutex*>(m_handle)->unlock();
	}

}
// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iaMutex.h>

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
	}

	void iaMutex::lock()
	{
		static_cast<mutex*>(m_handle)->lock();
	}

	void iaMutex::unlock()
	{
		static_cast<mutex*>(m_handle)->unlock();
	}

}
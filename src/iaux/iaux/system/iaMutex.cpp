// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/system/iaMutex.h>

#include <mutex>

namespace iaux
{

	iaMutex::iaMutex()
	{
		m_handle = static_cast<iaMutexHandle>(new std::mutex());
	}

	iaMutex::~iaMutex()
	{
		if (m_handle != nullptr)
		{
			delete static_cast<std::mutex *>(m_handle);
		}
	}

	void iaMutex::lock()
	{
		static_cast<std::mutex *>(m_handle)->lock();
	}

	void iaMutex::unlock()
	{
		static_cast<std::mutex *>(m_handle)->unlock();
	}

} // namespace iaux
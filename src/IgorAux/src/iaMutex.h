//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2017 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it   
// under the terms of the GNU Lesser General Public License as published by  
// the Free Software Foundation; either version 3 of the License, or (at   
// your option) any later version.                                           
// 
// This library is distributed in the hope that it will be useful,           
// but WITHOUT ANY WARRANTY; without even the implied warranty of            
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
// Lesser General Public License for more details.                           
// 
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
// 
// contact: martinloga@gmx.de  

#ifndef __IGOR_AUX_MUTEX__
#define __IGOR_AUX_MUTEX__

#include <iaDefines.h>

#define __IGOR_USE_MUTEX_PROFILER__

namespace IgorAux
{

	/*! mutex

	wraps std::mutex
 	*/
	class IgorAux_API iaMutex
	{

		/*! mutex handle definition
		*/
		typedef void* iaMutexHandle;

    public:

		/*! creates a mutex
		*/
        iaMutex();

		/*! destroys a mutex
		*/
        ~iaMutex();

		/*! locks a mutex
		*/
        void lock();

		/*! unlocks a mutex
		*/
        void unlock();

    private:

#ifdef __IGOR_USE_MUTEX_PROFILER__
		uint64 m_waiting = 0;
		uint64 m_running = 0;
		uint64 m_time = 0;
#endif

		/*! handle to mutex

		initialized in ctor
		*/
		iaMutexHandle m_handle;

	};

}

#endif
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

#ifndef __IGOR_AUX_CLOCK__
#define __IGOR_AUX_CLOCK__

#include <iaConsole.h>

namespace IgorAux
{

#ifdef __IGOR_WIN__

	static float64 clockTimeScale = 0;

	/*! initializes clock
	*/
	__IGOR_INLINE__ static void initClock()
	{
		uint64 clockPerformance = 0;
		if (QueryPerformanceFrequency((LARGE_INTEGER*)&clockPerformance))
		{
			clockTimeScale = 1.0 / static_cast<float64>(clockPerformance);
			con_info("Clock", "Scale " << clockTimeScale);
		}
		else
		{
			con_err("this PC doesn't support Hardware Performance Counter");
			exit(0);
		}
	}

	/*! \returns clock time in ticks

	initClock must be called before otherwise result is undefined
	*/
	__IGOR_INLINE__ static uint64 getClockTicks()
	{
		uint64 time;
		QueryPerformanceCounter((LARGE_INTEGER*)&time);
		return time;
	}

	/*! \returns clock time in seconds

	initClock must be called before otherwise result is undefined
	*/
	__IGOR_INLINE__ static float64 getClockSeconds()
	{
		uint64 time;
		QueryPerformanceCounter((LARGE_INTEGER*)&time);
		return static_cast<float64>(time) * clockTimeScale;
	}

	/*! \returns clock time in milliseconds

	initClock must be called before otherwise result is undefined
	*/
	__IGOR_INLINE__ static float64 getClockMiliseconds()
	{
		uint64 time;
		QueryPerformanceCounter((LARGE_INTEGER*)&time);
		return static_cast<float64>(time) * clockTimeScale * __IGOR_SECOND__;
	}

	/*! \returns clock scale from ticks to seconds

	initClock must be called before otherwise result is undefined
	*/
	__IGOR_INLINE__ static float64 getClockScale()
	{
		return clockTimeScale;
	}

#endif

}

#endif
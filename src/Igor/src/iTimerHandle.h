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
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __iRUNTIMEMANAGER__
#define __iRUNTIMEMANAGER__

#include <iaEvent.h>
using namespace IgorAux;

#include <iDefines.h>

namespace Igor
{

	iaEVENT(TimerEvent, TimerDelegate, void, (), ());

	class Igor_API iTimerHandle
	{
		friend class iTimer;

	private:

		/*! the timer event to trigger
		*/
		TimerEvent _timerEvent;

		/*! timer handle intervall in ms
		*/
		float64 _intervall = 20;

		/*! time the handle was triggered last time
		*/
		float64 _time = 0;

	protected:

		/*! calls timer event according to how much time passed by
		*/
		virtual void handle(float64 time);

	public:

		/*! register timer delegate to timer event

		\param timer_delegate timer delegate to register
		*/
		void registerTimerDelegate(TimerDelegate timer_delegate);

		/*! unregister timer delegate from timer event

		\param timer_delegate timer delegate to unregister
		*/
		void unregisterTimerDelegate(TimerDelegate timer_delegate);

		/*! changes timer interval

		\param interval interval in ms

		triggers restart()
		*/
		void setIntervall(float64 interval);

		/*! returns current intervall time
		*/
		float64 getIntervall();

		/*! restarts timer by resetting to current time
		*/
		void restart();

		/*! creates timer handle and registers it to the iTimer

		\param intervall intervall in ms the timer handle event will be called
		*/
		iTimerHandle();

		/*! unregisters from iTimer
		*/
		virtual ~iTimerHandle();

	};

};

#endif

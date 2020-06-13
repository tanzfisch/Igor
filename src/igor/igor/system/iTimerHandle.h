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
// (c) Copyright 2012-2020 by Martin Loga
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

#include <igor/iDefines.h>

#include <iaux/system/iaEvent.h>
#include <iaux/system/iaTime.h>
using namespace iaux;

namespace igor
{

	/*! timer tick event

    called once per interval
    */
	iaEVENT(iTimerTickEvent, iTimerTickDelegate, void, (), ());

	/*! timer handle class
    */
	class Igor_API iTimerHandle
	{

		friend class iTimer;

	public:
		/*! does nothing
		*/
		iTimerHandle() = default;

		/*! creates timer handle and registers it to the iTimer running with given interval

		\param timer_delegate timer delegate to register
		\param intervall the lenght of the intervall
		\param oneShot if true the timed even only occurs once
		*/
		iTimerHandle(iTimerTickDelegate timerDelegate, const iaTime &interval, bool oneShot = false);

		/*! unregisters from iTimer
		*/
		virtual ~iTimerHandle();

		/*! register timer delegate to timer event

		\param timer_delegate timer delegate to register
		*/
		void registerTimerDelegate(iTimerTickDelegate timerDelegate);

		/*! unregister timer delegate from timer event

		\param timer_delegate timer delegate to unregister
		*/
		void unregisterTimerDelegate(iTimerTickDelegate timerDelegate);

		/*! changes timer interval

		\param interval the lenght of the intervall

		triggers restart()
		*/
		void setIntervall(iaTime interval);

		/*! returns current intervall time
		*/
		iaTime getIntervall() const;

		/*! restarts timer by resetting to current time
		*/
		void restart();

		/*! start timer handle
		*/
		void start();

		/*! stop timer handle
		*/
		void stop();

	protected:
		/*! calls timer event according to how much time passed by
		*/
		virtual void handle(iaTime time);

	private:
		/*! the timer event to trigger
		*/
		iTimerTickEvent _timerEvent;

		/*! the intervall in ms in use
		*/
		iaTime _intervall = iaTime::fromMilliseconds(20);

		/*! the configured intervall in ms
		*/
		iaTime _configuredIntervall = iaTime::fromMilliseconds(20);

		/*! time the handle was triggered last time
		*/
		iaTime _time = 0;

		/*! if true timer triggers event only one time
		*/
		bool _oneShot = false;

		/*! if true this timer handle is currently running
		*/
		bool _playing = false;
	};

}; // namespace igor

#endif

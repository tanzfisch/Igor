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
// contact: igorgameengine@protonmail.com

#ifndef __IGOR_TIMER_H__
#define __IGOR_TIMER_H__

#include <igor/system/iTimerHandle.h>
#include <igor/resources/module/iModule.h>

#include <iaux/system/iaTime.h>
using namespace iaux;

#include <vector>

namespace igor
{

    /*! Handles Application time and intervals.

	Meshures time of application cycles based on render frames.
	Can also just meashure current time see getTime()
	*/
    class IGOR_API iTimer : public iModule<iTimer>
    {
        friend class iTimerHandle;
        friend class iApplication;
        friend class iModule<iTimer>;

    public:
        /*! \returns time since application start
        */
        iaTime getApplicationTime() const;

        /*! \returns time at beginning of current frame
        */
        iaTime getFrameTime() const;

        /*! \returns delta time since last frame
        */
        iaTime getFrameTimeDelta() const;

        /*! \returns frame rate based only on one frame's time delta
        */
        float64 getFPS() const;

        /*! \returns todays day
		*/
        uint32 getDay() const;

        /*! \returns current month
		*/
        uint32 getMonth() const;

        /*! \returns current year
		*/
        uint32 getYear() const;

    protected:
        /*! call timer handles events
        */
        void handleTimerHandles();

    private:
        /*! time delta between last and current frame (in miliseconds)
		*/
        iaTime _timeDelta;

        /*! time since timer was initialized (in miliseconds)
		*/
        iaTime _startTime;

        /*! time meshured current frame (in miliseconds)
		*/
        iaTime _currentTime;

        /*! time meshured last frame (in miliseconds)
		*/
        iaTime _lastTime;

        /*! registered timer handles

		\todo why is this pointers?
		*/
        std::vector<iTimerHandle *> _timerHandles;

        /*! registers timer handle to timer
		*/
        void insertTimerHandle(const iTimerHandle *timer_handle);

        /*! unregisters timer handle from timer
		*/
        void removeTimerHandle(const iTimerHandle *timer_handle);

        /*! called by iApplication to handle timer handle events
		*/
        void handle();

        /*! Registers to iApplication and initializes performance counter
		*/
        iTimer();

        /*! Unregisters from iApplication
		*/
        virtual ~iTimer();
    };

} // namespace igor

#endif // __IGOR_TIMER_H__

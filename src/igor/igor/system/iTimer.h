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
// (c) Copyright 2012-2022 by Martin Loga
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
        /*! \returns time at beginning of current frame
         */
        iaTime getTime() const;

        /*! \returns delta time since last frame
         */
        iaTime getTimeDelta() const;

        /*! \returns frame rate based only on one frame's time delta
         */
        float64 getFPS() const;

        /*! stops time and everyting that is triggered by it
        */
        void stop();

        /*! continues time
        */
        void start();

    protected:
        /*! call timer handle events
         */
        void handleTimerHandles();

    private:
        /*! time delta between last and current frame (in miliseconds)
         */
        iaTime _timeDelta;

        /*! time simce application start accounding for start/stop
         */
        iaTime _currentTime;

        /*! actual time since application start
        */
        iaTime _currentActualTime;

        /*! if true time is running
        */
        bool _timeRunning = true;

        /*! registered timer handles
        */
        std::vector<iTimerHandle *> _timerHandles;

        /*! mutex to protect timer handle list
        */
        iaMutex _mutexHandleList;

        /*! registers timer handle to timer
         */
        void insertTimerHandle(const iTimerHandle *timer_handle);

        /*! unregisters timer handle from timer
         */
        void removeTimerHandle(const iTimerHandle *timer_handle);

        /*! called by iApplication to handle timer handle events
         */
        void onUpdate();

        /*! Registers to iApplication and initializes performance counter
         */
        iTimer();

        /*! Unregisters from iApplication
         */
        virtual ~iTimer();
    };

} // namespace igor

#endif // __IGOR_TIMER_H__

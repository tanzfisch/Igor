//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef __IGOR_TIMER__
#define __IGOR_TIMER__

#include <igor/system/iTimerHandle.h>
#include <igor/resources/module/iModule.h>

#include <iaux/system/iaTime.h>
using namespace iaux;

#include <vector>
#include <array>

namespace igor
{
    /*! Handles Application time and intervals.

    Measures time of application cycles based on render frames.
    Can also just measure current time see getTime()
    */
    class IGOR_API iTimer : public iModule<iTimer>
    {
        friend class iTimerHandle;
        friend class iApplication;
        friend class iModule<iTimer>;

    public:
        /*! \returns time at beginning of current frame

        This game time and not real time. Time can differ from real time when ie stop/start the timer
         */
        const iaTime& getTime() const;

        /*! \returns time measured at beginning of each frame since application start
        */
        const iaTime& getRealTime() const;

        /*! \returns delta time since last frame
         */
        const iaTime& getTimeDelta() const;

        /*! \returns average delta time over TIME_DELTAS frames
         */
        iaTime getAverageTimeDelta() const;

        /*! \returns peak delta time over the last TIME_DELTAS frames
        */
        iaTime getPeakTimeDelta() const;

        /*! \returns frame rate based only on one frame's time delta
         */
        float64 getFPS() const;

        /*! \returns average frame rate over TIME_DELTAS frames
         */
        float64 getAverageFPS() const;

        /*! stops time and everything that is triggered by it
         */
        void stop();

        /*! continues time
         */
        void start();

    private:
    
        /*! time deltas count to be stored
        */
        static constexpr uint32 TIME_DELTAS = 500;

        /*! time delta between frames
         */
        std::array<iaTime, TIME_DELTAS> _timeDeltas;

        /*! time delta index
         */
        uint32 _timeDeltaIndex = 0;

        /*! time since application start accounting for start/stop
         */
        iaTime _gameTime;

        /*! actual time since application start
         */
        iaTime _realTime;

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

        /*! measure time at beginning of new frame
         */
        void nextFrame();

        /*! Registers to iApplication and initializes performance counter
         */
        iTimer();

        /*! Unregisters from iApplication
         */
        virtual ~iTimer();
    };

} // namespace igor

#endif // __IGOR_TIMER__

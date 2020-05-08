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
// (c) Copyright 2012-2019 by Martin Loga
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

#ifndef __iTimer__
#define __iTimer__

#include <iTimerHandle.h>

#include <iaSingleton.h>
using namespace IgorAux;

#include <vector>


namespace Igor
{

	/*! Handles Application time and intervals.

	Meshures time of application cycles based on render frames.
	Can also just meashure current time see getTime()
	*/
    class Igor_API iTimer : public iaSingleton<iTimer>
    {
        friend class iTimerHandle;
        friend class iApplication;
		friend class iaSingleton<iTimer>;

    public:

        /*! Absolute time in milliseconds since application start.

        \returns time in milliseconds
        */
        float64 getApplicationTime() const;

        /*! Absolute time in ms at last handle call (aka current frame time)

        This is basically the time from program start to last handle call (right after last render frame)
        */
        float64 getMilliSeconds() const;

        /*! Absolute time in s at last handle call (aka current frame time)

        This is basically the time from program start to last handle call (right after last render frame)
        */
        float64 getSeconds() const;

        /*! delta since last handle call in ms

        Basically the delta between the two last render frames.
        */
        float64 getMilliSecondsDelta() const;

        /*! delta since last handle call in s

        Basically the delta between the two last render frames.
        */
        float64 getSecondsDelta() const;

        /*! Returns handle frame rate. It's equal to the render framerate.
        */
        float32 getFPS() const;

    protected:

        /*! call timer handles events
        */
        void handleTimerHandles();

	private:
		
		/*! if true performance counter is available
		*/
        bool _usingPerformanceCounter;

		/*! time delta between last and current frame (in tics)
		*/
        uint64 _timeDelta = 0;

		/*! time since timer was initialized
		*/
		uint64 _startTime = 0;

		/*! time meshured current frame (in tics)
		*/
        uint64 _currentTime = 0;

		/*! time meshured last frame (in tics)
		*/
        uint64 _lastTime = 0;

		/*! performance frequency
		*/
        uint64 _performance = 0;

		/*! inverse performance frequency 
		*/
        float64 _timeScale = 0.0;

		/*! registered timer handles

		\todo why is this pointers?
		*/
		std::vector<iTimerHandle*> _timerHandles;

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

}

#endif

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

#ifndef __iTimer__
#define __iTimer__

#include <iTimerHandle.h>

#include <iaSingleton.h>
using namespace IgorAux;

#include <vector>
using namespace std;

namespace Igor
{

	/*! Handles Application time and intervals.

	Meshures time of application cycles based on render frames.
	Can also just meashure current time see getTime()
	*/
    class Igor_API iTimer : public iaSingleton<iTimer>
    {
        friend class iTimerHandle;
		friend class iaSingleton<iTimer>;

	private:
		
		/*! if true performance counter is available
		*/
        bool _usingPerformanceCounter;

		/*! time delta between last and current frame (in tics)
		*/
        uint64 _timeDelta = 0;

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
        double _timeScale = 0.0;

		/*! registered timer handles

		\todo why is this pointers?
		*/
		vector<iTimerHandle*> _timerHandles;

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

	protected:

		/*! call timer handles events
		*/
        void handleTimerHandles();

    public:
        
		/*! Absolute current time in milliseconds since program start.

		This is the one you need if you want to meashure time independent from anything.
        
        \returns time in milliseconds
        */
		virtual float64 getTime() const;

		/*! Absolute handle time in ms since program start

		This is basically the time from program start to last handle call (right after last render frame)
		*/
		virtual float64 getMilliSeconds() const;

		/*! Absolute handle time in s since program start
		
		This is basically the time from program start to last handle call (right after last render frame)
		*/
		virtual float64 getSeconds() const;

		/*! delta since last handle call in ms

		Basically the delta between the two last render frames.
		*/
		virtual float64 getMilliSecondsDelta() const;

		/*! delta since last handle call in s

		Basically the delta between the two last render frames.
		*/
		virtual float64 getSecondsDelta() const;

		/*! Returns handle frame rate. It's connected to render framerate.
		*/
		virtual float32 getFPS() const;

    };

}

#endif
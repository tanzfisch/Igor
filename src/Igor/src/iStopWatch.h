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

#ifndef __iSTOPWATCH__
#define __iSTOPWATCH__

#include <iDefines.h>

namespace Igor
{

	/*! use this for meshuring time

	measurement does not exclue the meashure overhead
	*/
	class Igor_API iStopWatch
	{
	private:

		/*! start time in ms
		*/
		float64 _startTime = 0;

		/*! stop time in ms
		*/
		float64 _stopTime = 0;

#ifdef __IGOR_DEBUG__
        /*! the last time we measured

        only interessting for debugging
        */
        float64 _lastMeasuredDelta = 0;
#endif

        /*! true if currently running
        */
        bool _running = false;

	public:

		/*! initializes member variables
		*/
		iStopWatch();

		/*! saves current time as start time in ms
		*/
		void start();					

		/*! saves current time as stop time in ms
		*/
		void stop();					

        /*! \returns true: if currently running
        */
        bool isRunning();

		/*! returns the difference between start and stop time in ms
		*/
		float64 getMilliSeconds();	

		/*! returns the difference between start and stop time in s
		*/
		float64 getSeconds();

        /*! returns the difference between start and current time in ms
        */
        float64 getCurrentMilliSeconds();

        /*! returns the difference between start and current time in s
        */
        float64 getCurrentSeconds();
	};

}

#endif

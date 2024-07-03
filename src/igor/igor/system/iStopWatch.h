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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef __IGOR_STOPWATCH__
#define __IGOR_STOPWATCH__

#include <igor/iDefines.h>

#include <iaux/system/iaTime.h>
using namespace iaux;

namespace igor
{

    /*! use this for meshuring time

	measurement does not exclue the meashure overhead
	*/
    class IGOR_API iStopWatch
    {

    public:
        /*! saves current time as start time in ms
		*/
        void start();

        /*! saves current time as stop time in ms
		*/
        void stop();

        /*! \returns true: if currently running
        */
        bool isRunning() const;

        /*! \returns the difference between start and stop time in ms

		if it is still running it returns the difference between start and current time
		*/
        iaTime getTime() const;

    private:
        /*! start time in ms
		*/
        iaTime _startTime;

        /*! stop time in ms
		*/
        iaTime _stopTime;

        /*! true if currently running
        */
        bool _running = false;
    };

} // namespace igor

#endif

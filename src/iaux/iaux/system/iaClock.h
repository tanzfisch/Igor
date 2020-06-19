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

#ifndef __IAUX_CLOCK__
#define __IAUX_CLOCK__

#include <iaux/iaDefines.h>

namespace iaux
{

    class IgorAux_API iaClock
    {
    public:
        /*! \returns time in microseconds
        */
        static int64 getTimeMicroseconds();

        /*! \returns clock time in seconds

        initClock must be called before otherwise result is undefined
        */
        static float64 getTimeSeconds();

        /*! \returns clock time in milliseconds

        initClock must be called before otherwise result is undefined
        */
        static float64 getTimeMilliseconds();

        /*! initializes clock
        */
        static void resetClock();
    };

} // namespace iaux

#endif

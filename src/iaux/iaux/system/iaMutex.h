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
// (c) Copyright 2012-2025 by Martin Loga
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

#ifndef IAUX_MUTEX_H
#define IAUX_MUTEX_H

#include <iaux/iaDefines.h>

namespace iaux
{

    /*! mutex

	wraps std::mutex
 	*/
    class IAUX_API iaMutex
    {
        /*! mutex handle definition
		*/
        typedef void *iaMutexHandle;

    public:
        /*! creates a mutex
		*/
        iaMutex();

        /*! destroys a mutex
		*/
        ~iaMutex();

        /*! locks a mutex
		*/
        void lock();

        /*! unlocks a mutex
		*/
        void unlock();

    private:
        /*! handle to mutex

		initialized in ctor
		*/
        iaMutexHandle m_handle;
    };

} // namespace iaux

#endif // IAUX_MUTEX_H
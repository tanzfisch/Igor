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

#ifndef __iPHYSICSWORLD__
#define __iPHYSICSWORLD__

#include <igor/iDefines.h>

#include <iaux/system/iaMutex.h>
using namespace iaux;

namespace igor
{

    class Igor_API iPhysicsWorld
    {

        friend class iPhysics;

    public:
        /*! defines the invalid world id
        */
        static const uint64 INVALID_WORLD_ID = 0;

        /*! \returns body ID
        */
        uint64 getID();

        /*! \returns pointer to newton world
        */
        void *getNewtonWorld();

    private:
        /*! world id
        */
        uint64 _id = INVALID_WORLD_ID;

        /*! next world id
        */
        static uint64 _nextID;

        /*! mutex to save the id generation
        */
        iaMutex _mutex;

        /*! handle to newton world
        */
        void *_newtonWorld = nullptr;

        /*! initializes members

        \param newtonWorld handle to newton world
        */
        iPhysicsWorld(void *newtonWorld);

        /*! does nothing
        */
        virtual ~iPhysicsWorld() = default;
    };

} // namespace igor

#endif
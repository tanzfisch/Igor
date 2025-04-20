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

#ifndef __IGOR_PHYSICSWORLD__
#define __IGOR_PHYSICSWORLD__

#include <igor/iDefines.h>

#include <iaux/data/iaIDGenerator.h>
using namespace iaux;

namespace igor
{

    /*! world id definition
    */
    typedef iaID64 iWorldID;

    class IGOR_API iPhysicsWorld
    {

        friend class iPhysics;

    public:
        /*! defines the invalid world id
         */
        static const iWorldID INVALID_WORLD_ID = IGOR_INVALID_ID;

        /*! \returns body ID
         */
        iWorldID getID() const;

        /*! \returns pointer to newton world
         */
        void *getNewtonWorld() const;

    private:
        /*! world id
         */
        iWorldID _id = INVALID_WORLD_ID;

        /*! id generator
         */
        static iaIDGenerator64 _idGenerator;

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

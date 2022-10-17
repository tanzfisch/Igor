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

#ifndef __IGOR_THREAD_H__
#define __IGOR_THREAD_H__

#include <iaux/system/iaThread.h>
using namespace iaux;

#include <igor/iDefines.h>
#include <igor/physics/iPhysicsWorld.h>

namespace igor
{

    /*! igor thread
    */
    class iThread : public iaThread
    {

    public:
        /*! does nothing
        */
        iThread() = default;

        /*! does nothing
        */
        ~iThread() = default;

        /*! \returns world id

        \todo this is weird. we need to find an other solution. maybe some sort of thread context in form of user data
        */
        uint64 getWorld() const;

    protected:
        /*! init function will be called as first by the thread
        */
        void init() override;

        /*! deinit function is called last by the thread. right before join
        */
        void deinit() override;

    private:
        /*! the world id
        */
        uint64 _worldID = iPhysicsWorld::INVALID_WORLD_ID;

    };

}; // namespace igor

#endif

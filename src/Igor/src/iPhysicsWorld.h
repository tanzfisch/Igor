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
// (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __iPHYSICSWORLD__
#define __iPHYSICSWORLD__

#include <iDefines.h>

#include <mutex>
using namespace std;

namespace Igor
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
        void* getNewtonWorld();

    private:

        /*! world id
        */
        uint64 _id = INVALID_WORLD_ID;

        /*! next world id
        */
        static uint64 _nextID;

        /*! mutex to save the id generation
        */
        mutex _mutex;

        /*! handle to newton world
        */
        void* _newtonWorld = nullptr;

        /*! initializes members

        \param newtonWorld handle to newton world
        */
        iPhysicsWorld(void* newtonWorld);

        /*! does nothing
        */
        virtual ~iPhysicsWorld() = default;

    };

}

#endif
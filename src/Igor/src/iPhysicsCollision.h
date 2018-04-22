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
// (c) Copyright 2012-2018 by Martin Loga
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

#ifndef __iPHYSICSCOLLISION__
#define __iPHYSICSCOLLISION__

#include <iDefines.h>
#include <iPhysicsWorld.h>

#include <iaMutex.h>
using namespace IgorAux;

namespace Igor
{

    /*! wrapper for the NewtonCollision handle
    */
    class iPhysicsCollision
    {

        friend class iPhysics;
        
    public:

        static const uint64 INVALID_PHYSICSCOLLISION_ID = 0;

        /*! \returns pointer to newton collision
        */
        void* getCollision() const;

        /*! \returns collision ID
        */
        uint64 getID();

        /*! \returns world ID
        */
        uint64 getWorldID();

    private:

        /*! next collision id
        */
        static uint64 _nextCollisionID;

        /*! mutex to protec the id generation
        */
        iaMutex _mutex;

        /*! id of collision object
        */
        uint64 _collisionID = INVALID_PHYSICSCOLLISION_ID;

        /*! the handle to the newton collision
        */
        void* _collision = nullptr;

        /*! id of the world this collision was created with
        */
        uint64 _worldID = iPhysicsWorld::INVALID_WORLD_ID;

        /*! initializes members

        \param collision handle to newton collision
        \param world the world this collision was created in
        */
        iPhysicsCollision(void* collision, uint64 worldID);

        /*! does nothing
        */
        virtual ~iPhysicsCollision() = default;

    };
    
}

#endif

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

#ifndef __iPHYSICSCOLLISION__
#define __iPHYSICSCOLLISION__

#include <iDefines.h>

namespace Igor
{

    /*! wrapper for the NewtonCollision handle
    */
    class iPhysicsCollision
    {

        friend class iPhysics;
        
    public:

        /*! \returns pointer to newton collision
        */
        void* getCollision() const;

        /*! \returns collision ID
        */
        uint64 getID();

    private:

        /*! id of collision object
        */
        uint64 _collisionID = 0;

        /*! the handle to the newton collision
        */
        void* _collision = nullptr;

        /*! initializes members

        \param collision handle to newton collision
        */
        iPhysicsCollision(void* collision, uint64 collisionID);

        /*! does nothing
        */
        virtual ~iPhysicsCollision() = default;

    };
    
}

#endif
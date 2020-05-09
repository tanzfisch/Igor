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

#ifndef __iPHYSICSJOINTUPVECTOR__
#define __iPHYSICSJOINTUPVECTOR__

#include <iDefines.h>

namespace Igor
{

    /*! wrapper for the NewtonJoint handle
    */
    class iPhysicsJointUpVector
    {

        friend class iPhysics;
        
    public:

        /*! \returns pointer to newton joint
        */
        void* getJoint() const;

        /*! \returns joint ID
        */
        uint64 getID();

    private:

        /*! id of joint object
        */
        uint64 _jointID = 0;

        /*! the handle to the newton joint
        */
        void* _joint = nullptr;

        /*! initializes members

        \param collision handle to newton collision
        */
        iPhysicsJointUpVector(void* joint, uint64 jointID);

        /*! does nothing
        */
        virtual ~iPhysicsJointUpVector() = default;

    };
    
}

#endif

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

#ifndef __iENTITYPOSITIONED__
#define __iENTITYPOSITIONED__

#include <iEntity.h>

#include <iaVector3.h>
#include <iSphere.h>
using namespace IgorAux;

namespace Igor
{

    /*! specialized entity that has a position
    */
    class Igor_API iEntityPositioned : public iEntity
    {

        friend class iEntityManager;

    public:

        /*! \returns true if entity has component position
        */
        bool hasPosition() const;

        /*! sets position of entity

        \param position new position to set
        */
        virtual void setPosition(const iaVector3d& position) = 0;

        /*! \returns current position 
        */
        iaVector3d getPosition() const;

        /*! \returns sphere of entity
        */
        iSphered getSphere() const;

    protected:

        /*! current position
        */
        iSphered _sphere;

        /*! does nothing
        */
        iEntityPositioned() = default;

        /*! does nothing
        */
        virtual ~iEntityPositioned() = default;

    };

}

#endif
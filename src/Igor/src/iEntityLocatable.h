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

#ifndef __iENTITYLOCATABLE__
#define __iENTITYLOCATABLE__

#include <iEntity.h>
#include <iSphere.h>

#include <iaVector3.h>
using namespace IgorAux;

namespace Igor
{

    /*! enetity that can be located by it's position
    */
    class Igor_API iEntityLocatable : public iEntity
    {

        friend class iEntityManager;

    public:

        /*! \returns current position sphere of entity
        */
        iSphered getSphere() const;

        /*! does nothing
        */
        iEntityLocatable() = default;

        /*! does nothing
        */
        virtual ~iEntityLocatable() = default;

    protected:

        /*! entity's handle
        */
        virtual void handle() = 0;

        /*! \returns current position
        */
        virtual iaVector3d getCurrentPos() = 0;

    private:

        /*! position sphere
        */
        iSphered _sphere;

    };

}

#endif
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

#ifndef __iENTIYTRANSFORMCONTROL__
#define __iENTIYTRANSFORMCONTROL__

#include <iControl.h>
#include <iEntity.h>
#include <iKeyCodeMap.h>

#include <iaMatrix.h>
using namespace IgorAux;

#include <map>
using namespace std;

namespace Igor
{

    /*! gives direct control over a iNodeTransform via keyboard an mouse
    */
    class Igor_API iEntityTransformControl : public iControl
    {

    public:

        /*! initializes member variables

        \param initDefaultLayout if true a default keyboard layout will ne initialized
        */
        iEntityTransformControl(bool initDefaultLayout = true);

        /*! release handles
        */
        ~iEntityTransformControl();

        /*! \retruns id of node that is controlled
        */
        uint32 getEntity();

        /*! sets the id fo the node to be controlled
        */
        void setEntity(uint32 entityID);

    private:

        /*! the id of the node to control
        */
        uint32 _entityID = iEntity::INVALID_ENTITY_ID;

        /*! called every frame
        */
        void onHandle();

    };

}
#endif

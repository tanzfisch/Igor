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

#ifndef __iNODETRANSFORMCONTROL__
#define __iNODETRANSFORMCONTROL__

#include <iControl.h>
#include <iNode.h>
#include <iKeyCodeMap.h>

#include <iaMatrix.h>
using namespace IgorAux;

#include <map>
using namespace std;

namespace Igor
{

    class iNodeTransform;

    /*! gives direct control over a iNodeTransform via keyboard an mouse
    */
    class Igor_API iNodeTransformControl : public iControl
    {

    public:

        /*! initializes member variables

        \param initDefaultLayout if true a default keyboard layout will ne initialized
        */
        iNodeTransformControl(bool initDefaultLayout = true);

        /*! release handles
        */
        ~iNodeTransformControl();

        /*! \retruns id of node that is controlled
        */
        uint32 getNode();

        /*! sets the id fo the node to be controlled
        */
        void setNode(uint32 nodeID);

    private:

        /*! the id of the node to control
        */
        uint32 _nodeID = iNode::INVALID_NODE_ID;

        /*! helper matrix for controling the heading
        */
        iaMatrixd _heading;

        /*! helper matrix for controling the pitch
        */
        iaMatrixd _pitch;

        /*! helper matrix for controling the roll
        */
        iaMatrixd _roll;

        /*! called every frame
        */
        void onHandle();

        /*! called when mouse moves
        \param x1 last horizontal position of mouse
        \param y1 last vertical position of mouse
        \param x2 current horizontal position of mouse
        \param y2 current vertical position of mouse
        */
        void onMouseMove(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window);

    };

}
#endif

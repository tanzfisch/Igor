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

#ifndef __PLAYER__
#define __PLAYER__

#include <iControl.h>
#include <iNode.h>
#include <iEntity.h>
using namespace Igor;

namespace Igor
{
    class iScene;
    class iNodeCamera;
    class iNodeTransform;
    class iNodeLODTrigger;
    class iNodeTransformControl;
    class iEntityTransformControl;
}

class Player : public iControl
{

public:

    Player(iScene* scene);
    ~Player();

    void printPosition();

    iNodeLODTrigger* getLODTrigger();

    void getMatrix(iaMatrixf& matrix) const;

private:

    iScene* _scene = nullptr;

    uint32 _entityID = iEntity::INVALID_ENTITY_ID;
    iNodeTransform* _transformNode = nullptr;
    iNodeTransform* _orientationNode = nullptr;
    iNodeCamera* _cameraNode = nullptr;
    iNodeLODTrigger* _camLODTriggerNode = nullptr;

    iPhysicsBody* _playerBody = nullptr;
    iEntity* _playerEntity = nullptr;

    /*! helper matrix for controling the heading
    */
    iaMatrixf _heading;

    /*! helper matrix for controling the pitch
    */
    iaMatrixf _pitch;

    /*! helper matrix for controling the roll
    */
    iaMatrixf _roll;

    float32 _headingAngle = 0;
    float32 _pitchAngle = 0;

    void onApplyForceAndTorque(iPhysicsBody* body, float32 timestep, int threadIndex);

    /*! called every frame
    */
    void onHandle();

    void onMouseMove(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window);

};

#endif
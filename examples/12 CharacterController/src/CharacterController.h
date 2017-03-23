//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/                     ) )     ((
//                                           (_(       \)
//    (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __CHARACTERCONTROLLER__
#define __CHARACTERCONTROLLER__

#include <iNode.h>
#include <iPhysicsBody.h>
using namespace Igor;

namespace Igor
{
    class iPhysicsJoint;
    class iPhysicsCollision;
}

class CharacterController
{

    enum class CharacterControllerState
    {
        Air,
        Jumped,
        Floor
    };

public:

    CharacterController(iNode* parent, int64 materiaID, const iaMatrixd& startMatrix);

    virtual ~CharacterController();

    void setForce(const iaVector3d& force);
    iaVector3d getForce() const;

    iNodeTransform* getRootNode() const;

    iNodeTransform* getHeadTransform() const;

private:

    float64 _characterHeight = 2.0;
    float64 _characterRadius = 0.3;

    static constexpr float64 _headHeight = 0.65;

    static constexpr float64 _stepHeight = 0.3;
    static constexpr float64 _mass = 100;

    CharacterControllerState _state = CharacterControllerState::Air;

    iaVector3d _navigationForce;

    uint64 _bodyID = iPhysicsBody::INVALID_PHYSICSBODY_ID;

    iPhysicsCollision* _collisionCast = nullptr;

    uint32 _rootTransformNodeID = iNode::INVALID_NODE_ID;
    uint32 _headTransformNodeID = iNode::INVALID_NODE_ID;

    float64 getContactPoint(iaVector3d& point, iaVector3d& normal);
    void iterate(iaVector3d& correctionForce);

    unsigned onRayPreFilter(iPhysicsBody* body, iPhysicsCollision* collision, const void* userData);

    void onApplyForceAndTorque(iPhysicsBody* body, float32 timestep);

    void onSubmitConstraints(iPhysicsJoint* joint, float32 timestep);

};

#endif

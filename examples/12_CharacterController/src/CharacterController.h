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
//    (c) Copyright 2014-2020 by Martin Loga
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
// contact: igorgameengine@protonmail.com

#ifndef __CHARACTERCONTROLLER_H__
#define __CHARACTERCONTROLLER_H__

#include <igor/igor.h>
using namespace igor;

class CharacterController
{

public:
    /*! character state 
    */
    enum class State
    {
        Air,
        Jumped,
        Floor
    };

    CharacterController(iNodePtr parent, int64 materiaID, const iaMatrixd &startMatrix);

    virtual ~CharacterController();

    void setForce(const iaVector3d &force);
    iaVector3d getForce() const;

    iNodeTransform *getRootNode() const;

    iNodeTransform *getHeadingTransform() const;
    iNodeTransform *getPitchTransform() const;

    iNodeTransform *getHeadTransform() const;

    iNodeTransform *getLeftSholderTransform() const;
    iNodeTransform *getRightSholderTransform() const;

    State getState() const;

private:
    float64 _characterHeight = 1.8;
    float64 _characterRadius = 0.3;

    /// the height of a step that carachter can walk without jumping
    float64 _stepHeight = 0.3;
    float64 _targetHeight = _stepHeight + 0.1;
    float64 _headHeight = 1.8;
    float64 _mass = 100;

    State _state = State::Air;

    iaVector3d _navigationForce;

    uint64 _bodyID = iPhysicsBody::INVALID_PHYSICSBODY_ID;

    iPhysicsCollision *_collisionCast = nullptr;

    uint32 _physicsTransformNodeID = iNode::INVALID_NODE_ID;

    uint32 _upperBodyTransformNodeID = iNode::INVALID_NODE_ID;

    uint32 _headingTransformNodeID = iNode::INVALID_NODE_ID;
    uint32 _pitchTransformNodeID = iNode::INVALID_NODE_ID;

    uint32 _headTransformNodeID = iNode::INVALID_NODE_ID;
    uint32 _leftShoulderTransformNodeID = iNode::INVALID_NODE_ID;
    uint32 _rightShoulderTransformNodeID = iNode::INVALID_NODE_ID;

    float64 getFloorContactPoint(iaVector3d &point, iaVector3d &normal);

    unsigned onRayPreFilter(iPhysicsBody *body, iPhysicsCollision *collision, const void *userData);

    void onApplyForceAndTorque(iPhysicsBody *body, float64 timestep);

    void onSubmitConstraints(iPhysicsJoint *joint, float64 timestep);
};

#endif // __CHARACTERCONTROLLER_H__

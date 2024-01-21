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
//    (c) Copyright 2012-2024 by Martin Loga
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

#ifndef IGOR_CHARACTERCONTROLLER_H
#define IGOR_CHARACTERCONTROLLER_H

#include <igor/igor.h>
using namespace igor;

/*! character controller
 */
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

    /*! init character controller

    \param parent the parenting node in the scene
    \param materialID physics material of character body
    \param matrix initial position/orientation of character
    */
    CharacterController(iNodePtr parent, int64 materiaID, const iaMatrixd &matrix);

    /*! cleanup
     */
    virtual ~CharacterController();

    /*! sets a force to be applied next tick

    \param force the force to apply
    */
    void setForce(const iaVector3d &force);

    /*! \returns the force to be applied next tick
     */
    iaVector3d getForce() const;

    /*! \returns root transform node of character
     */
    iNodeTransform *getRootNode() const;

    /*! \returns heading transform node of character
     */
    iNodeTransform *getHeadingTransform() const;

    /*! \returns pitch transform node of character
     */
    iNodeTransform *getPitchTransform() const;

    /*! \returns head transform node of character
     */
    iNodeTransform *getHeadTransform() const;

    /*! \returns left shoulder transform node of character
     */
    iNodeTransform *getLeftShoulderTransform() const;

    /*! \returns right shoulder transform node of character
     */
    iNodeTransform *getRightShoulderTransform() const;

    /*! \returns current state od character
     */
    State getState() const;

private:
    /*! the height of the character
     */
    float64 _characterHeight = 1.8;

    /*! the radius of the character
     */
    float64 _characterRadius = 0.3;

    /*! the height of a step that character can walk without jumping
     */
    float64 _stepHeight = 0.3;

    /*! target height
     */
    float64 _targetHeight = _stepHeight + 0.1;

    /*! head height
     */
    float64 _headHeight = 1.8;

    /*! mass of body
     */
    float64 _mass = 100;

    /*! state of character
     */
    State _state = State::Air;

    /*! force to be applied next tick
     */
    iaVector3d _navigationForce;

    /*! physics body handle
     */
    uint64 _bodyID = iPhysicsBody::INVALID_PHYSICSBODY_ID;

    /*! collision caster
     */
    iPhysicsCollision *_collisionCast = nullptr;

    /*! root transform node physics handle
     */
    uint32 _physicsTransformNodeID = iNode::INVALID_NODE_ID;

    /*! upper body physics handle
     */
    uint32 _upperBodyTransformNodeID = iNode::INVALID_NODE_ID;

    /*! heading transform node physics handle
     */
    uint32 _headingTransformNodeID = iNode::INVALID_NODE_ID;

    /*! pitch transform node physics handle
     */
    uint32 _pitchTransformNodeID = iNode::INVALID_NODE_ID;

    /*! head transform node physics handle
     */
    uint32 _headTransformNodeID = iNode::INVALID_NODE_ID;

    /*! left shoulder transform node physics handle
     */
    uint32 _leftShoulderTransformNodeID = iNode::INVALID_NODE_ID;

    /*! right shoulder transform node physics handle
     */
    uint32 _rightShoulderTransformNodeID = iNode::INVALID_NODE_ID;

    /*! gets floor contact point

    \returns height above ground
    \param[out] point contact point
    \param[out] normal contact normal
    */
    float64 getFloorContactPoint(iaVector3d &point, iaVector3d &normal);

    /*! helper to filter out bodies
     */
    unsigned onRayPreFilter(iPhysicsBody *body, iPhysicsCollision *collision, const void *userData);

    /*! apply force and torque per tick

    \param body the body that is affected
    \param timestep the time step
    */
    void onApplyForceAndTorque(iPhysicsBody *body, float64 timestep);

    /*! apply/submit constraints to the characters body

    \param joint the affected joint
    \param timestep the time step
    */
    void onSubmitConstraints(iPhysicsJoint *joint, float64 timestep);
};

#endif // IGOR_CHARACTERCONTROLLER_H

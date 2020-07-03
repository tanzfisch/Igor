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

#ifndef __PHYSICSEXAMPLE_H__
#define __PHYSICSEXAMPLE_H__

#include <ExampleBase.h>

/*! physics example
*/
class PhysicsExample : public ExampleBase
{

public:
    /*! init
    */
    PhysicsExample();

    /*! does nothing
    */
    ~PhysicsExample() = default;

private:
    /*! controls wether the physics simulation is running or not
    */
    bool _running = false;

    /*! camera heading transform
    */
    iNodeTransform *_cameraHeading = nullptr;

    /*! camera pitch transform
    */
    iNodeTransform *_cameraPitch = nullptr;

    /*! camera distance to origin transform
    */
    iNodeTransform *_cameraTranslation = nullptr;

    /*! list of physics bodies create for later cleanup
    */
    std::vector<uint64> _bodyIDs;

    /*! physics callback to apply force and torque to the physics bodies

    \param body the body affected
    \param timestep the time passed
    */
    void onApplyForceAndTorque(iPhysicsBody *body, float32 timestep);

    /*! initialize scene
    */
    void onInit() override;

    /*! clean up scene
    */
    void onDeinit() override;

    /*! renders orthogonal stuff
    */
    void onRenderOrtho() override;

    /*! called on any other event

    \param event the event to handle
    */
    void onEvent(iEvent &event) override;

    /*! called when key was pressed

    \param event the event to handle
    */
    bool onKeyDown(iKeyDownEvent_TMP &event);

    /*! handles mouse move event

    \param event the mouse move event
    \returns true if consumed
    */
    bool onMouseMoveEvent(iMouseMoveEvent_TMP &event);

    /*! handles mouse wheel event

    \param event the mouse wheel event
    \returns true if consumed
    */
    bool onMouseWheelEvent(iMouseWheelEvent_TMP &event);
};

#endif // __PHYSICSEXAMPLE_H__

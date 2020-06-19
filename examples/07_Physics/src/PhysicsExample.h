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

#ifndef __OPENGL3D_EXAMPLE_H__
#define __OPENGL3D_EXAMPLE_H__

#include <ExampleBase.h>

#include <igor/system/iTimerHandle.h>
using namespace igor;

namespace igor
{
    class iNodeTransform;
    class iPhysicsBody;
} // namespace igor

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

    /*! handles mouse moved event

    \param from last mouse position
    \param to current mouse position
    \param window the window the coordinates are related to
    */
    void onMouseMovedFull(const iaVector2i &from, const iaVector2i &to, iWindow *window) override;

    /*! handles mouse wheel event

    \param d mouse wheel delta
    */
    void onMouseWheel(int32 d) override;

    /*! initialize scene
    */
    void init() override;

    /*! clean up scene
    */
    void deinit() override;

    /*! renders orthogonal stuff
    */
    void onRenderOrtho() override;

    /*! handle keyboard events

    \param key the key pressed
    */
    void onKeyDown(iKeyCode key) override;
};

#endif // __OPENGL3D_EXAMPLE_H__

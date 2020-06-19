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

#ifndef __EXAMPLE_CHARACTER_CONTROLLER_H__
#define __EXAMPLE_CHARACTER_CONTROLLER_H__

#include <ExampleBase.h>

#include "CharacterController.h"

#include <igor/resources/material/iMaterial.h>
using namespace igor;

namespace igor
{
    class iPhysicsBody;
} // namespace igor

class ExampleCharacterController : public ExampleBase
{

public:
    /*! init
    */
    ExampleCharacterController();

    /*! does nothing
    */
    ~ExampleCharacterController() = default;

private:
    /*! character input flags
    */
    struct InputFlags
    {
        bool _forward = false;
        bool _backward = false;
        bool _left = false;
        bool _right = false;
        bool _up = false;
        bool _down = false;

        bool _jump = false;
        bool _crouch = false;

        bool _shootPrimary = false;
    };

    /*! flat to control weather or not the mouse stays trapped
    */
    bool _captureMouse = true;

    /*! character input flags
    */
    InputFlags _inputFlags;

    /*! material definition for the sky box
    */
    iMaterialID _materialSkyBox = iMaterial::INVALID_MATERIAL_ID;

    /*! the character controller
    */
    CharacterController *_characterController = nullptr;

    /*! physics material for terrain
    */
    iPhysicsMaterialID _terrainMaterialID = 0;

    /*! physics material for entity / character
    */
    iPhysicsMaterialID _entityMaterialID = 0;

    /*! physics material for bullet
    */
    iPhysicsMaterialID _bulletMaterialID = 0;

    /*! handles initializing the level after the models where loaded
    */
    void onModelReady(uint64 modelNodeID);

    /*! recursively creates collisions for all meshes in giben sub scene

    \param node the current node in sub scene
    */
    void makeCollisions(iNodePtr node);

    /*! creates a physics box at given position

    \param pos the given position
    */
    void createBox(const iaVector3d &pos);

    /*! applies force and torque to moveable boxes in scene

    \param body the body affected
    \param timestep the physics time
    */
    void onApplyForceAndTorqueBox(iPhysicsBody *body, float32 timestep);

    /*! called on key pressed event

    \param key the key code of the pressed key
    */
    void onKeyDown(iKeyCode key) override;

    /*! called on key released event

    \param key the key that was released
    */
    void onKeyUp(iKeyCode key) override;

    /*! handle called once per frame
    */
    void onPreDraw() override;

    /*! called when the mouse was moved

    \param from last mouse position
    \param to current mouse position
    \param window the window the coordinates are related to
    */
    void onMouseMovedFull(const iaVector2i &from, const iaVector2i &to, iWindow *window) override;

    /*! called when mouse wheel was turned

    \param d mouse wheel delta
    */
    void onMouseWheel(int32 d) override;

    /*! handles mouse key down events

	\param keyCode the key code of the pressed button
	*/
    void onMouseKeyDown(iKeyCode keyCode) override;

    /*! handles mouse key up events

	\param keyCode the key code of the released button
	*/
    void onMouseKeyUp(iKeyCode keyCode) override;

    /*! called by orthogonal view
    */
    void onRenderOrtho() override;

    /*! deinit example
    */
    void deinit() override;

    /*! init example
    */
    void init() override;
};

#endif // __EXAMPLE_CHARACTER_CONTROLLER_H__

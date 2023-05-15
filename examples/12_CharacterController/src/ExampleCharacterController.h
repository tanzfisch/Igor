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

#ifndef __EXAMPLECHARACTERCONTROLLER__
#define __EXAMPLECHARACTERCONTROLLER__

#include <ExampleBase.h>

#include "CharacterController.h"

class ExampleCharacterController : public ExampleBase
{

public:
    /*! init

    \param window the given window
    */
    ExampleCharacterController(iWindowPtr window);

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
    bool _captureMouse = false;

    /*! character input flags
    */
    InputFlags _inputFlags;

    /*! material definition for the sky box
    */
    iMaterialPtr _materialSkyBox;

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
    void onApplyForceAndTorqueBox(iPhysicsBody *body, float64 timestep);

    /*! handle called once per frame
    */
    void onUpdate() override;

    /*! called by orthogonal view
    */
    void onRenderOrtho() override;

    /*! deinit example
    */
    void onDeinit() override;

    /*! init example
    */
    void onInit() override;

    /*! called on any other event

    \param event the event to handle
    */
    void onEvent(iEvent &event) override;

    /*! handles mouse key down event

    \param event the mouse key down event
    \returns true if consumed
    */
    bool onMouseKeyDownEvent(iEventMouseKeyDown &event);

    /*! handles mouse key up event

    \param event the mouse key up event
    \returns true if consumed
    */
    bool onMouseKeyUpEvent(iEventMouseKeyUp &event);

    /*! handles mouse move event

    \param event the mouse move event
    \returns true if consumed
    */
    bool onMouseMoveEvent(iEventMouseMove &event);

    /*! called when key was pressed

    \param event the event to handle
    */
    bool onKeyDown(iEventKeyDown &event);

    /*! called when key was released

    \param event the event to handle
    */
    bool onKeyUp(iEventKeyUp &event);
};

#endif // __EXAMPLECHARACTERCONTROLLER__

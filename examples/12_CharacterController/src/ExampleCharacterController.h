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
// contact: martinloga@gmx.de

#ifndef __EXAMPLE3D__
#define __EXAMPLE3D__

#include <igor/igor.h>
#include <igor/system/iWindow.h>
#include <igor/graphics/iView.h>
#include <igor/system/iTimerHandle.h>
#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/profiler/iProfilerVisualizer.h>
using namespace igor;

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{
    class iScene;
    class iNodeTransform;
    class iNodeLight;
    class iNodeSwitch;
    class iTextureFont;
    class iTaskFlushModels;
    class iTaskFlushTextures;
    class iNodeLODTrigger;
    class iNodeLODSwitch;
    class iNodeModel;
    class iPhysicsBody;
    class iPhysicsJoint;
    class iTexture;
} // namespace igor

#include "CharacterController.h"

class ExampleCharacterController
{

public:
    /*! init
    */
    ExampleCharacterController();

    /*! deinit
    */
    virtual ~ExampleCharacterController();

    /*! run example
    */
    void run();

private:

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

    InputFlags _inputFlags;

    /*! the window
    */
    iWindow _window;

    /*! visualizes statistics
    */
    iProfilerVisualizer _profilerVisualizer;

    /*! the view we render 3D to
    */
    iView _view;

    /*! the view we render 2D to
    */
    iView _viewOrtho;

    /*! the scene holding our 3d nodes
    */
    iScene *_scene = nullptr;

    /*! async loading of models
    */
    uint64 _taskFlushModels = iTask::INVALID_TASK_ID;

    /*! async loading of textures
    */
    uint64 _taskFlushTextures = iTask::INVALID_TASK_ID;

    /*! texture fon we use to render statistics
    */
    iTextureFont *_font = nullptr;

    void onModelReady(uint64 modelNodeID);
    void makeCollisions(iNodePtr node);

    /*! creates a physics box at given position

    \param pos the given position
    */
    void createBox(const iaVector3d& pos);

    /*! material definition for the sky box
    */
    uint64 _materialSkyBox = iMaterial::INVALID_MATERIAL_ID;

    CharacterController *_characterController = nullptr;

    /*! material for igor logo
    */
    uint64 _materialWithTextureAndBlending = iMaterial::INVALID_MATERIAL_ID;

    /*! igor logo
    */
    iTexturePtr _igorLogo = nullptr;

    int64 _terrainMaterialID = 0;
    int64 _entityMaterialID = 0;
    int64 _bulletMaterialID = 0;

    /*! called on key pressed event

    \param key the key code of the pressed key
    */
    void onKeyPressed(iKeyCode key);

    /*! called on key released event

    \param key the key that was released
    */
    void onKeyReleased(iKeyCode key);

    /*! handle called once per frame
    */
    void onHandle();

    /*! called when window was closed
    */
    void onWindowClosed();

    /*! called when window was resized

    \param clientWidth the client rectangle width
    \param clientHeight the client rectangle height
    */
    void onWindowResized(int32 clientWidth, int32 clientHeight);

    /*! called when the mouse was moved

    \param from last mouse position
    \param to current mouse position
    \param window the window the coordinates are related to
    */
    void onMouseMoved(const iaVector2i &from, const iaVector2i &to, iWindow *window);

    /*! called when mouse wheel was turned

    \param d mouse wheel delta
    */
    void onMouseWheel(int32 d);

    /*! handles mouse key down events

	\param keyCode the key code of the pressed button
	*/
    void onMouseKeyDown(iKeyCode keyCode);

    void onMouseKeyUp(iKeyCode keyCode);

    /*! called by orthogonal view
    */
    void onRenderOrtho();

    /*! draw igor logo
    */
    void drawLogo();

    /*! deinit example
    */
    void deinit();

    /*! init example
    */
    void init();

    void onApplyForceAndTorqueBox(iPhysicsBody *body, float32 timestep);
};

#endif

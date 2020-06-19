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

#ifndef __OPENGL3DEXAMPLE__
#define __OPENGL3DEXAMPLE__

#include <igor/igor.h>
#include <igor/system/iWindow.h>
#include <igor/graphics/iView.h>
#include <igor/system/iTimerHandle.h>
#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/system/iKeyboard.h>
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
    class iTextureFont;
    class iEntity;
    class iTaskFlushModels;
    class iTaskFlushTextures;
    class iPhysicsBody;
    class iPhysicsJoint;
    class iTexture;
}

/*! physics example
*/
class PhysicsExample
{

public:

    /*! init
    */
    PhysicsExample();

    /*! deinit
    */
    virtual ~PhysicsExample();

    /*! run example
    */
    void run();

private:

    /*! the window
    */
	iWindow _window;

    /*! visualizes statistics
    */
    iProfilerVisualizer _profilerVisualizer;

    /*! view to render the scene with
    */
	iView _view;

    /*! ortogonal view to render statistics with
    */
    iView _viewOrtho;

    /*! controls wether the physics simulation is running or not
    */
    bool _running = false;

    /*! handle for the flush model task
    */
    uint64 _flushModelsTask = iTask::INVALID_TASK_ID;

    /*! handle for the flush textures task
    */
    uint64 _flushTexturesTask = iTask::INVALID_TASK_ID;

    /*! font handle for statistics
    */
    iTextureFont* _font = nullptr;

    /*! the scene
    */
	iScene* _scene = nullptr;

    /*! camera heading transform
    */
	iNodeTransform* _cameraHeading = nullptr;

    /*! camera pitch transform
    */
    iNodeTransform* _cameraPitch = nullptr;

    /*! camera distance to origin transform
    */
    iNodeTransform* _cameraTranslation = nullptr;

    /*! list of physics bodies create for later cleanup
    */
    std::vector<uint64> _bodyIDs;

    /*! sky box material ID
    */
	uint64 _materialSkyBox = 0;

    /*! material for igor logo
    */
    uint64 _materialWithTextureAndBlending = iMaterial::INVALID_MATERIAL_ID;

    /*! igor logo
    */
    iTexturePtr _igorLogo = nullptr;

    /*! physics callback to apply force and torque to the physics bodies

    \param body the body affected
    \param timestep the time passed
    */
    void onApplyForceAndTorque(iPhysicsBody* body, float32 timestep);

    /*! handle keyboard events

    \param key the key pressed
    */
	void onKeyPressed(iKeyCode key);

    /*! handles window close evnt
    */
	void onWindowClosed();

    /*! handles mouse moved event

    \param from last mouse position
    \param to current mouse position
    \param window the window the coordinates are related to
    */
    void onMouseMoved(const iaVector2i& from, const iaVector2i& to, iWindow* window);

    /*! handles mouse wheel event

    \param d mouse wheel delta
    */
    void onMouseWheel(int32 d);

    /*! initialize scene
    */
    void init();

    /*! clean up scene
    */
	void deinit();

    /*! run scene handle
    */
	void onHandle();

    /*! renders orthogonal stuff
    */
    void onRenderOrtho();

    /*! renders igor logo
    */
    void drawLogo();

};

#endif

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

#ifndef __PARTICLES__
#define __PARTICLES__

#include <Igor.h>
#include <iWindow.h>
#include <iView.h>
#include <iTimerHandle.h>
#include <iModelResourceFactory.h>
#include <iKeyCodeMap.h>
using namespace Igor;

#include <iaMatrix.h>
using namespace IgorAux;

namespace Igor
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
}

class Particles
{

private:

    /*! the window
    */
	iWindow _window;

    /*! the view we render 3D to
    */
	iView _view;

    /*! the view we render 2D to
    */
    iView _viewOrtho;

    /*! the scene holding our 3d nodes
    */
    iScene* _scene = nullptr;

    /*! texture fon we use to render statistics
    */
    iTextureFont* _font = nullptr;

    /*! id to transform node used for manipulating the heading of the camera
    */
    uint32 _cameraHeading = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the pitch of the camera
    */
    uint32 _cameraPitch = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the distance of the camera to the origin
    */
    uint32 _cameraTranslation = iNode::INVALID_NODE_ID;

    /*! timer handle to control the movement of the light source over time
    */
    iTimerHandle* _animationTimingHandle = nullptr;

    /*! material definition for the sky box
    */
    uint64 _materialSkyBox = 0;

    /*! particles material
    */
    uint64 _particlesMaterial = 0;

    /*! id of task to load textures
    */
    uint64 _taskFlushTexturesID = iTask::INVALID_TASK_ID;

    /*! id of circle emitter ID
    */
    uint64 _circleEmitterID = iNode::INVALID_NODE_ID;

    /*! id of circle emitter transform id
    */
    uint64 _circlEmitterTransformID = iNode::INVALID_NODE_ID;

    /*! called on key pressed event

    \param key the key code of the pressed key
    */
	void onKeyPressed(iKeyCode key);

    /*! called when window was closed
    */
	void onWindowClosed();

    /*! called when window was resized

    \param clientWidth the client rectangle width
    \param clientHeight the client rectangle height
    */
    void onWindowResized(int32 clientWidth, int32 clientHeight);

    /*! called when the mouse was moved

    \param x1 last horrizontal position
    \param y1 last vertical position
    \param x2 current horrizontal position
    \param y2 current vertical position
    \param window corresponding window
    */
    void onMouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window);

    /*! called when mouse wheel was turned

    \param d mouse wheel delta
    */
    void onMouseWheel(int32 d);

    /*! called by orthogonal view
    */
    void onRenderOrtho();

    /*! called by timer
    */
    void onTimer();

    /*! deinit example
    */
	void deinit();

    /*! init example
    */
	void init();

public:

    /*! init
    */
    Particles();

    /*! deinit
    */
	virtual ~Particles();

    /*! run example
    */
	void run();

};

#endif

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

#ifndef __LSYSTEMS__
#define __LSYSTEMS__

#include <Igor.h>
#include <iWindow.h>
#include <iView.h>
#include <iTimerHandle.h>
#include <iModelResourceFactory.h>
#include <iKeyCodeMap.h>
#include <iLSystem.h>
#include <iMaterial.h>
#include <iTexture.h>
#include <iSkeleton.h>
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
    class iNodeModel;
}

/*! the LSystems example class
*/
class LSystems
{

public:

    /*! init
    */
    LSystems();

    /*! deinit
    */
    virtual ~LSystems();

    /*! run example
    */
    void run();

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

    /*! id of flush model task
    */
    uint64 _flushModelsTask = iTask::INVALID_TASK_ID;

    /*! id of flush textures task
    */
    uint64 _flushTexturesTask = iTask::INVALID_TASK_ID;

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
    
    /*! counter to switch between styles of L-Systems
    */
    uint32 _styleCounter = 0;

    /*! the L-System
    */
    iLSystem _lSystem;

    /*! segment lenght
    */
    float32 _segmentLength = 0;

    /*! angle between bones
    */
    float32 _angle = 0;

    /*! color of trunk
    */
    iaColor3f _trunkColor;

    /*! color of branch
    */
    iaColor3f _branchColor;

    /*! color of bud
    */
    iaColor3f _budColor;

    /*! color of flower
    */
    iaColor3f _flowerColor;

    /*! color of leaf
    */
    iaColor3f _leafColor;

    /*! group node where we put the L-System model nodes in so we can delete them all at once
    */
    uint32 _groupNodeID = iNode::INVALID_NODE_ID;

    /*! generates L-Systems
    */
    void generateLSystems();

    /*! init L-System with style 1
    */
    void initStyle1();

    /*! init L-System with style 2
    */
    void initStyle2();

    /*! init L-System with style 3
    */
    void initStyle3();

    /*! generates plant at specified position

    \param matrix matrix to position the plant
    \param axiom start string to generate L-System
    \param iterations iteration count
    \param seed random seed
    */
    void generatePlant(const iaMatrixf& matrix, const iaString& axiom, uint32 iterations, uint64 seed);

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

    /*! deinit example
    */
	void deinit();

    /*! init example
    */
	void init();

};

#endif

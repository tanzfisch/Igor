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

#ifndef __EXAMPLE3D__
#define __EXAMPLE3D__

#include <Igor.h>
#include <iWindow.h>
#include <iView.h>
#include <iTimerHandle.h>
#include <iModelResourceFactory.h>
#include <iKeyCodeMap.h>
#include <iMaterial.h>
#include <iStatisticsVisualizer.h>
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
    class iTexture;
}

class ExampleInstancing
{

private:

    /*! the window
    */
	iWindow _window;

    /*! visualizes statistics
    */
    iStatisticsVisualizer _statisticsVisualizer;

    /*! the view we render 3D to
    */
	iView _view;

    /*! the view we render 2D to
    */
    iView _viewOrtho;

    /*! the scene holding our 3d nodes
    */
    iScene* _scene = nullptr;

    /*! async loading of models
    */
    uint64 _taskFlushModels = iTask::INVALID_TASK_ID;

    /*! async loading of textures
    */
    uint64 _taskFlushTextures = iTask::INVALID_TASK_ID;

    /*! texture fon we use to render statistics
    */
    iTextureFont* _font = nullptr;

    /*! id to transform node used for manipulating the heading of the camera
    */
    uint64 _cameraHeading = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the pitch of the camera
    */
    uint64 _cameraPitch = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the distance of the camera to the origin
    */
    uint64 _cameraTranslation = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the direction of the light relative to the origin
    */
    uint64 _directionalLightRotate = iNode::INVALID_NODE_ID;

    /*! id to cat model node
    */
    uint64 _catModel = iNode::INVALID_NODE_ID;

    /*! timer handle to control the movement of the light source over time
    */
    iTimerHandle* _animationTimingHandle = nullptr;

    /*! material definition for the sky box
    */
    uint64 _materialSkyBox = iMaterial::INVALID_MATERIAL_ID;

    /*! material for igor logo
    */
    uint64 _materialWithTextureAndBlending = iMaterial::INVALID_MATERIAL_ID;

    /*! instancing material
    */
    uint64 _materialWithInstancing = iMaterial::INVALID_MATERIAL_ID;

    /*! igor logo
    */
    iTexturePtr _igorLogo = nullptr;

    uint64 _modelNodeID = 0;

    void onModelReady(uint64 modelNodeID);

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

    \param from last mouse position
    \param to current mouse position
    \param window the window the coordinates are related to
    */
    void onMouseMoved(const iaVector2i& from, const iaVector2i& to, iWindow* window);

    /*! called when mouse wheel was turned

    \param d mouse wheel delta
    */
    void onMouseWheel(int32 d);

    /*! called by orthogonal view
    */
    void onRenderOrtho();

    /*! draw igor logo
    */
    void drawLogo();

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
    ExampleInstancing();

    /*! deinit
    */
	virtual ~ExampleInstancing();

    /*! run example
    */
	void run();

};

#endif

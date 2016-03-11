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

class Example3D
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

    /*! async loading of models
    */
    iTaskFlushModels* _taskFlushModels = nullptr;

    /*! async loading of textures
    */
    iTaskFlushTextures* _taskFlushTextures = nullptr;

    /*! texture fon we use to render statistics
    */
    iTextureFont* _font = nullptr;

    /*! id to transform node used for manipulating the heading of all objects in scene
    */
    uint32 _allObjectsHeading = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the pitch of all objects in scene
    */
    uint32 _allObjectsPitch = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the heading of the camera
    */
    uint32 _cameraHeading = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the pitch of the camera
    */
    uint32 _cameraPitch = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the distance of the camera to the origin
    */
    uint32 _cameraTranslation = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the distance of the light to the origin
    */
    uint32 _directionalLightTranslate = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the direction of the light relative to the origin
    */
    uint32 _directionalLightRotate = iNode::INVALID_NODE_ID;

    /*! id to light node
    */
    uint32 _lightNode = iNode::INVALID_NODE_ID;

    /*! timer handle to control the movement of the light source over time
    */
    iTimerHandle* _animationTimingHandle = nullptr;

    /*! id to crate model node
    */
    uint32 _crateModel = iNode::INVALID_NODE_ID;

    /*! id to cat model node
    */
    uint32 _catModel = iNode::INVALID_NODE_ID;

    /*! id to teapot model node
    */
    uint32 _teapotModel = iNode::INVALID_NODE_ID;

    /*! id to switch node
    */
    uint32 _switchNode = iNode::INVALID_NODE_ID;

    /*! 
    */
    int32 _activeNode = 0;

    /*! id to lod trigger node
    */
    uint32 _LODTrigger = iNode::INVALID_NODE_ID;

    /*! id to lod switch node
    */
    uint32 _LODSwitch = iNode::INVALID_NODE_ID;

    /*! material definition for the sky box
    */
    int32 _materialSkyBox = 0;

	void onKeyPressed(iKeyCode key);
	void onWindowClosed();
    void onWindowResized(int32 clientWidth, int32 clientHeight);

    void onMouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window);
    void onMouseWheel(int32 d);

    void onRenderOrtho();
    void onTimer();

	void deinit();
	void init();

public:

    Example3D();
	virtual ~Example3D();

	void run();

};

#endif

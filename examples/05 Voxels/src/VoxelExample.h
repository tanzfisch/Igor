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

#ifndef __VOXELEXAMPLE__
#define __VOXELEXAMPLE__

#include <Igor.h>
#include <iWindow.h>
#include <iView.h>
#include <iTimerHandle.h>
#include <iModelResourceFactory.h>
#include <iKeyboard.h>
#include <iPerlinNoise.h>
using namespace Igor;

#include <iaMatrix.h>
using namespace IgorAux;

namespace Igor
{
	class iScene;
	class iNodeTransform;
	class iNodeLight;
    class iTextureFont;
    class iVoxelData;
    class iContouringCubes;
    class iMeshBuilder;
    class iMesh;
    class iTargetMaterial;
    class iNodeTransformControl;
}

/*! the voxel example
*/
class VoxelExample
{

public:

    /*! init
    */
    VoxelExample();

    /*! deinit
    */
    virtual ~VoxelExample();

    /*! runs application loop
    */
    void run();

private:

    /*! window to render in
    */
	iWindow _window;

    /*! view within the window
    */
	iView _view;

    /*! an other view to render orthogonal projected stuff
    */
    iView _viewOrtho;

    /*! font to display framerate
    */
    iTextureFont* _font = nullptr;

    /*! the scene
    */
	iScene* _scene = nullptr;

    /*! heading of camera
    */
    uint32 _cameraHeading = iNode::INVALID_NODE_ID;

    /*! pitch of camera
    */
    uint32 _cameraPitch = iNode::INVALID_NODE_ID;

    /*! the model that represents our voxel mesh
    */
    uint32 _voxelMeshModel = iNode::INVALID_NODE_ID;

    /*! transform node for voxel mesh
    */
    uint32 _voxelMeshTransform = iNode::INVALID_NODE_ID;

    /*! voxel mesh material
    */
    int32 _voxelMeshMaterialID = 0;

    /*! the actual voxel data
    */
    iVoxelData* _voxelData = nullptr;

    /*! material for the sky box
    */
    int32 _materialSkyBox = 0;

    /*! perlin noise generator
    */
    iPerlinNoise _perlinNoise;

    /*! flag to show loading text
    */
    bool _loading = true;

    /*! loading text material
    */
    int32 _materialWithTextureAndBlending = 0;

    /*! id of flush model task
    */
    uint64 _flushModelTask = iTask::INVALID_TASK_ID;
    
    /*! id of flush textures task
    */
    uint64 _flushTexturesTask = iTask::INVALID_TASK_ID;

    /*! called when ESC key was pressed
    */
	void onKeyESCPressed();

    /*! called when space key was released
    */
    void onKeySpacePressed();

    /*! called when window was closed
    */
	void onWindowClosed();

    /*! called before every frame
    */
    void onHandle();

    /*! called when window was resized

    \param clientWidth new client width of window
    \param clientHeight new client height of window
    */
    void onWindowResized(int32 clientWidth, int32 clientHeight);

    /*! called when mouse was moved
    
    \param x1 last horrizontal coordinate
    \param y1 last vertical coordinate
    \param x2 current horrizontal coordinate
    \param y2 current vertical coordinate
    \param window the window the coordinates are related to
    */
    void onMouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window);

    /*! called when orthogonal view was rendred
    */
    void onRenderOrtho();

    /*! initialize everything
    */
    void init();

    /*! initialize window and views
    */
	void initViews();

    /*! initialize scene
    */
	void initScene();

    /*! register some callbacks
    */
    void registerHandles();

    /*! deinitialize everything
    */
    void deinit();

    /*! unregister the callbacks
    */
    void unregisterHandles();

    /*! generate some voxel data and meshs
    */
    void generateVoxelData();

};

#endif

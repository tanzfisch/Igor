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

class VoxelExample
{
private:

	iWindow _window;
	iView _view;
    iView _viewOrtho;

    iTextureFont* _font = nullptr;
	iScene* _scene = nullptr;

    iNodeTransform* _cameraHeading = nullptr;
    iNodeTransform* _cameraPitch = nullptr;
    iNodeTransform* _cameraTranslation = nullptr;
    iNodeCamera* _camera = nullptr;

    iNodeTransform* _lightTranslate = nullptr;
    iNodeTransform* _lightRotate = nullptr;
    iNodeLight* _lightNode = nullptr;

    iNodeModel* _voxelMeshModel = nullptr;

    int32 _materialSkyBox = 0;
    int32 _voxelMeshMaterialID = 0;

    iVoxelData* _voxelData = nullptr;
    uint32 _seed = 0;
    
	void keyESCPressed();
    void keySpaceReleased();
	void windowClosed();
    void windowResized(int32 clientWidth, int32 clientHeight);
    void mouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window);
	void deinit();
	void init();
    void generateVoxelData();
    void renderOrtho();
	void registerHandles();
	void unregisterHandles();
	void initViews();
	void initScene();

public:

	VoxelExample();
	virtual ~VoxelExample();

	void run();

};

#endif

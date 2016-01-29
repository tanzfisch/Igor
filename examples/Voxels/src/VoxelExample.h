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

#include "VoxelTerrainGenerator.h"

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
    class iNodeLODTrigger;
}

class Player;

class VoxelExample
{
private:

	iWindow _window;
	iView _view;
    iView _viewOrtho;

    Player* _player = nullptr;

    iTextureFont* _font = nullptr;

	iScene* _scene = nullptr;

    bool _renderOctree = false;

    iNodeTransform* _markerTranform = nullptr;
    iNodeTransform* _markerScale = nullptr;
    iNodeModel* _marker = nullptr;

    int64 _toolSize = 3;
    int64 _toolDensity = 255;

    iNodeTransform* _lightTranslate = nullptr;
    iNodeTransform* _lightRotate = nullptr;
    iNodeLight* _lightNode = nullptr;

    uint32 _materialWithTextureAndBlending = 0;
    uint32 _octreeMaterial = 0;
    int32 _materialSkyBox = 0;

    VoxelTerrainGenerator* _voxelTerrainGenerator = nullptr;

	void keyPressed(iKeyCode key);
	void windowClosed();
    void windowResized(int32 clientWidth, int32 clientHeight);
    void mouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window);
    void mouseDown(iKeyCode key);
    void mouseWheel(int d);
	void deinit();
	void init();

	void render();
    void handle();
    void renderOrtho();

	void registerHandles();
	void unregisterHandles();
	void initViews();
	void initScene();
	void initPlayer();
    void initVoxelData();

    void updateMarkerPosition();

public:

	VoxelExample();
	virtual ~VoxelExample();

	void run();

};

#endif

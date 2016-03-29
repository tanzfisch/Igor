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

#ifndef __OPENGL3DEXAMPLE__
#define __OPENGL3DEXAMPLE__

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
    class iEntity;
    class iTaskFlushModels;
    class iTaskFlushTextures;
    class iPhysicsBody;
}

class PhysicsExample
{
private:

	iWindow _window;
	iView _view;
    iView _viewOrtho;
    iaMatrixf _modelViewOrtho;
    iTaskFlushModels* _taskFlushModels = nullptr;
    iTaskFlushTextures* _taskFlushTextures = nullptr;

    iTextureFont* _font = nullptr;

	iScene* _scene = nullptr;
	iNodeTransform* _cameraHeading = nullptr;
    iNodeTransform* _cameraPitch = nullptr;
    iNodeTransform* _cameraTranslation = nullptr;
    iNodeTransform* _directionalLightTranslate = nullptr;
    iNodeTransform* _directionalLightRotate = nullptr;
    iNodeLight* _lightNode = nullptr;

    vector<uint64> _bodyIDs;

	TimerDelegate _timerDelegate;

    uint32 _materialWithTextureAndBlending = 0;
    uint32 _octreeMaterial = 0;

    float32 _camPitch = -0.4;
    float32 _camHeading = 0.2;

	int32 _materialSkyBox = 0;

    void onApplyForceAndTorque(iPhysicsBody* body, float32 timestep, int threadIndex);

	void keyPressed(iKeyCode key);
	void windowClosed();
    void mouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window);
    void mouseWheel(int32 d);
	void deinit();
    void init();

    void updateCameraPosition();
	void handle();
    void renderOrtho();

public:

	PhysicsExample();
	virtual ~PhysicsExample();

	void run();

};

#endif

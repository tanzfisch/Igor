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

    iNodeTransform* _allObjectsHeading = nullptr;
    iNodeTransform* _allObjectsPitch = nullptr;

	iNodeTransform* _cameraHeading = nullptr;
    iNodeTransform* _cameraPitch = nullptr;
    iNodeTransform* _cameraTranslation = nullptr;

    iNodeTransform* _directionalLightTranslate = nullptr;
    iNodeTransform* _directionalLightRotate = nullptr;
    iNodeLight* _lightNode = nullptr;

    iNodeModel* _crateModel = nullptr;
    iNodeModel* _catModel = nullptr;
    iNodeModel* _teapotModel = nullptr;

    iNodeSwitch* _switchNode = nullptr;
    int32 _activeNode = 0;

    iNodeLODTrigger* _LODTrigger = nullptr;
    iNodeLODSwitch* _LODSwitch = nullptr;

	iTimerHandle* _animationTimingHandle = nullptr;

    int32 _materialSkyBox = 0;

	void keyPressed(iKeyCode key);
	void handleAnimation();

	void windowClosed();
    void windowResized(int32 clientWidth, int32 clientHeight);

    void mouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window);
    void mouseWheel(int32 d);
	void deinit();
	void init();

    void renderInfo();

public:

    Example3D();
	virtual ~Example3D();

	void run();

};

#endif

//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __MODELVIEWER__
#define __MODELVIEWER__

#include <Igor.h>
#include <iWindow.h>
#include <iView.h>
#include <iTimerHandle.h>
#include <iModelResourceFactory.h>
#include <iKeyboard.h>
#include <iaMatrix.h>
#include <iWidgetManager.h>
#include <iFileDialog.h>
using namespace Igor;

namespace Igor
{
    class iScene;
    class iNodeTransform;
    class iNodeLight;
    class iTextureFont;

    class iWidgetDefaultTheme;
    class iWidgetDialog;
    class iWidgetScroll;
    class iMessageBox;
    class iRenderStatistics;
}

class PropertiesDialog;
class MenuDialog;

class ModelViewer
{

public:

    ModelViewer();
    virtual ~ModelViewer();

    void run(iaString fileName);

private:

    iWindow _window;
    iView _view;
    iView _viewOrtho;
    iaMatrixf _modelViewOrtho;

    iTextureFont* _font = nullptr;
    iScene* _scene = nullptr;

    iFileDialog* _fileDialog = nullptr;
    iMessageBox* _messageBox = nullptr;

    iNodeTransform* _cameraCOI = nullptr;
    iNodeTransform* _cameraHeading = nullptr;
    iNodeTransform* _cameraPitch = nullptr;
    iNodeTransform* _cameraTranslation = nullptr;
    iNodeTransform* _transformModel = nullptr;

    iNodeTransform* _directionalLightTranslate = nullptr;
    iNodeTransform* _directionalLightRotate = nullptr;
    iNodeLight* _lightNode = nullptr;

    iWidgetDefaultTheme* _widgetTheme = nullptr;

    PropertiesDialog* _propertiesDialog = nullptr;
    MenuDialog* _menuDialog = nullptr;
    iRenderStatistics* _renderStatistics = nullptr;

    float32 _camDistance = 0;
    float32 _camMinDistance = 0;
    float32 _camMaxDistance = 0;

    uint64 _taskFlushTextures = 0;

    iNode* _groupNode = nullptr;

    int32 _materialSkyBox;

    bool _mouseKey0Pressed = false;
    bool _mouseKey1Pressed = false;
    bool _mouseKey2Pressed = false;
    bool _mouseKey3Pressed = false;
    bool _mouseKey4Pressed = false;

	uint32 _cursorNodeID = iNode::INVALID_NODE_ID;

    void onKeyPressed(iKeyCode key);
    void onWindowClosed();

    void onLoadFile();
    void onImportFile(uint32 nodeID);
    void onImportFileReference(uint32 nodeID);
    void onSaveFile();
    void onExitModelViewer();

    void onAddTransformation(uint32 atNodeID);
    void onAddGroup(uint32 atNodeID);
    void onAddEmitter(uint32 atNodeID);
    void onAddParticleSystem(uint32 atNodeID);
    void onAddSwitch(uint32 atNodeID);

    void onAddMaterial();

    void onMouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window);
    void onMouseWheel(int32 d);
    void onMouseKeyDown(iKeyCode key);
    void onMouseKeyUp(iKeyCode key);

    void onWindowResize(int32 clientWidth, int32 clientHeight);

    void deinit();
    void init(iaString fileName);
    void updateCamDistance();

    void onFileLoadDialogClosed(iFileDialogReturnValue fileDialogReturnValue);
    void onImportFileDialogClosed(iFileDialogReturnValue fileDialogReturnValue);
    void onImportFileReferenceDialogClosed(iFileDialogReturnValue fileDialogReturnValue);
    void onFileSaveDialogClosed(iFileDialogReturnValue fileDialogReturnValue);

    void forceLoadingNow();
    void initGUI();
    void deinitGUI();

    void handle();
    void render();
    void renderOrtho();

};

#endif

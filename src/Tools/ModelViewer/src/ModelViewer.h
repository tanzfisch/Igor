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

#include "Manipulator.h"

#include <Igor.h>
#include <iWindow.h>
#include <iView.h>
#include <iTimerHandle.h>
#include <iModelResourceFactory.h>
#include <iKeyboard.h>
#include <iaMatrix.h>
#include <iWidgetManager.h>
#include <iDialogFileSelect.h>
#include <iStatisticsVisualizer.h>
using namespace Igor;

namespace Igor
{
    class iScene;
    class iNodeTransform;
    class iNodeLight;
    class iTextureFont;

    class iWidgetDefaultTheme;
    class iDialog;
    class iWidgetScroll;
    class iDialogMessageBox;
    class iRenderStatistics;

    class iNodeSkyBox;
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
    iView _viewManipulator;
    iView _viewOrtho;
    iaMatrixd _modelViewOrtho;

    iTextureFont* _font = nullptr;
    iScene* _scene = nullptr;
    iScene* _sceneManipulator = nullptr;

    iDialogFileSelect* _fileDialog = nullptr;
    iDialogMessageBox* _messageBox = nullptr;

    // TODO need some classes handling different types of cameras
    iNodeTransform* _cameraCOI = nullptr;
    iNodeTransform* _cameraHeading = nullptr;
    iNodeTransform* _cameraPitch = nullptr;
    iNodeTransform* _cameraTranslation = nullptr;
    iNodeTransform* _transformModel = nullptr;
    iNodeCamera* _camera = nullptr;

    iNodeTransform* _cameraCOIUI = nullptr;
    iNodeTransform* _cameraHeadingUI = nullptr;
    iNodeTransform* _cameraPitchUI = nullptr;
    iNodeTransform* _cameraTranslationUI = nullptr;
    iNodeTransform* _transformModelUI = nullptr;
    iNodeCamera* _cameraUI = nullptr;


    iNodeSkyBox* _skyBoxNode = nullptr;

    iNodeTransform* _directionalLightTranslate = nullptr;
    iNodeTransform* _directionalLightRotate = nullptr;
    iNodeLight* _lightNode = nullptr;

    iWidgetDefaultTheme* _widgetTheme = nullptr;

    PropertiesDialog* _propertiesDialog = nullptr;
    MenuDialog* _menuDialog = nullptr;
    iRenderStatistics* _renderStatistics = nullptr;

    float32 _camDistance = 0;

    uint64 _taskFlushTextures = 0;

    iNode* _groupNode = nullptr;

    uint64 _materialSkyBox;
    uint64 _materialManipulator;
    uint64 _materialBoundingBox;
    uint64 _materialCelShading;

    uint32 _selectedNodeID = iNode::INVALID_NODE_ID;

    iStatisticsVisualizer _statisticsVisualizer;

    Manipulator* _manipulator = nullptr;

    void updateManipulator();
    uint64 selectManipulator();
    void pickcolorID();

    void onGraphViewSelectionChanged(uint64 nodeID);

    void onKeyPressed(iKeyCode key);
    void centerCamOnSelectedNode();
    void onWindowClosed();

    void onLoadFile();
    void onImportFile(uint64 nodeID);
    void onImportFileReference(uint64 nodeID);
    void onSaveFile();
    void onExitModelViewer();

    void onAddTransformation(uint64 atNodeID);
    void onAddGroup(uint64 atNodeID);
    void onAddEmitter(uint64 atNodeID);
    void onAddParticleSystem(uint64 atNodeID);
    void onAddSwitch(uint64 atNodeID);

    void onAddMaterial();

    void onMouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window);
    void onMouseWheel(int32 d);
    void onMouseKeyDown(iKeyCode key);
    void onMouseKeyUp(iKeyCode key);

    void onWindowResize(int32 clientWidth, int32 clientHeight);

    void deinit();
    void init(iaString fileName);
    void updateCamDistanceTransform();
    void centerCamOnNode(iNode* node);

    void onFileLoadDialogClosed(iFileDialogReturnValue fileDialogReturnValue);
    void onImportFileDialogClosed(iFileDialogReturnValue fileDialogReturnValue);
    void onImportFileReferenceDialogClosed(iFileDialogReturnValue fileDialogReturnValue);
    void onFileSaveDialogClosed(iFileDialogReturnValue fileDialogReturnValue);

    void forceLoadingNow(iNodeModel* modelNode);
    void initGUI();
    void deinitGUI();

    void handle();
    void renderNodeSelected(uint64 nodeID);
    void render();
    void renderManipulator();
    void renderOrtho();

};

#endif

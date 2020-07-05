#if 0

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
// (c) Copyright 2014-2020 by Martin Loga
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
// contact: igorgameengine@protonmail.com

#ifndef __MICA_H__
#define __MICA_H__

#include "Manipulator.h"
#include "Outliner.h"
#include "PropertiesDialog.h"
#include "widget3d/Widget3D.h"

#include <igor/igor.h>
using namespace igor;

class Mica
{

public:
    /*! nothing to do
	*/
    Mica() = default;

    /*! deinit resources
	*/
    virtual ~Mica();

    /*! run mica
	*/
    void run(iaString fileName);

private:
    /*! main window of mica
	*/
    iWindow _window;

    /*! main scene view
	*/
    iView _view;

    /*! UI view
	*/
    iView _viewOrtho;

    /*! 3d UI view
	*/
    iView _viewWidget3D;

    /*! font used for widget display
	*/
    iTextureFont *_font = nullptr;

    /*! main scene
	*/
    iScene *_scene = nullptr;

    /*! 3d widget scene eg used for the manipulator
	*/
    iScene *_sceneWidget3D = nullptr;

    /*! file open/close dialog 
	*/
    iDialogFileSelectPtr _fileDialog = nullptr;

    // TODO need some classes handling different types of cameras
    iNodeTransform *_cameraCOI = nullptr;
    iNodeTransform *_cameraHeading = nullptr;
    iNodeTransform *_cameraPitch = nullptr;
    iNodeTransform *_cameraTranslation = nullptr;
    iNodeCamera *_camera = nullptr;
    float32 _camDistance = 0;

    /*! the default sky box
	*/
    iNodeSkyBox *_defaultSkyBox = nullptr;

    // TODO need to handle light differently
    iNodeTransform *_directionalLightTranslate = nullptr;
    iNodeTransform *_directionalLightRotate = nullptr;
    iNodeLight *_lightNode = nullptr;

    /*! instance of the widget theme
	*/
    iWidgetDefaultTheme *_widgetTheme = nullptr;

    /*! the properties dialog or editor
	*/
    PropertiesDialog *_propertiesDialog = nullptr;

    /*! the outliner
	*/
    Outliner *_outliner = nullptr;

    /*! id of textures flush task
	*/
    uint64 _taskFlushTextures = 0;

    /*! the node that contains the editable part of the scene or workspace
	*/
    iNodePtr _workspace = nullptr;

    /*! material for orientation plane 
	*/
    uint64 _materialOrientationPlane;

    /*! material for bounding box display 
	*/
    uint64 _materialBoundingBox;

    /*! cel shading material for selecting nodes in the scene
	*/
    uint64 _materialCelShading;

    /*! id of currently selected node 
	*/
    uint32 _selectedNodeID = iNode::INVALID_NODE_ID;

    /*! visualizer for profiler data
	*/
    iProfilerVisualizer _profilerVisualizer;

    /*! manipulator
	*/
    Manipulator *_manipulator = nullptr;

    /*! pointer to active 3d widget
	*/
    Widget3D *_widget3D = nullptr;

    /*! clear resources
	*/
    void deinit();

    /*! initializes mica

	\param filename optional filename to open to start with
	*/
    void init(iaString filename);

    /*! empties the workspace
	*/
    void clearWorkspace();

    /*! reset manipulator mode to none
	*/
    void resetManipulatorMode();

    /*! sets the manipulator mode on currently selected node 
	but only if it is a transform node otherwise its set to none

	\param modifierMode the modifier mode to set
	*/
    void setManipulatorMode(ManipulatorMode modifierMode);

    /*! handle for graph view selection change event

	\param nodeID the id of the selected node
	*/
    void onGraphViewSelectionChanged(uint64 nodeID);

    /*! handle for keyboard dow event
	*/
    void onKeyDown(iKeyCode key);

    /*! handle for window closed event
	*/
    void onWindowClosed();

    /*! handle for load file event
	*/
    void onLoadFile();

    /*! handle for import file to existing scene event
	*/
    void onImportFile();

    /*! hande for import file by reference to existing scene event
	*/
    void onImportFileReference();

    /*! handle for save file event
	*/
    void onSaveFile();

    /*! hanlde for exist mica event
	*/
    void onExitMica();

    void onAddMaterial();

    void onMouseMoved(const iaVector2i &from, const iaVector2i &to, iWindow *window);
    void onMouseWheel(int32 d);
    void onMouseKeyDown(iKeyCode key);
    void onMouseKeyUp(iKeyCode key);

    void onWindowResize(int32 clientWidth, int32 clientHeight);

    void onFileLoadDialogClosed(iDialogPtr dialog);
    void onImportFileDialogClosed(iDialogPtr dialog);
    void onImportFileReferenceDialogClosed(iDialogPtr dialog);
    void onFileSaveDialogClosed(iDialogPtr dialog);

    void forceLoadingNow(iNodeModel *modelNode);
    void initGUI();
    void deinitGUI();

    void handle();
    void renderNodeSelected(uint64 nodeID);
    void render();
    void renderOrtho();
    void renderOrientationPlane();

    iModelDataInputParameter *createDataInputParameter();
};

#endif // __MICA_H__

#endif
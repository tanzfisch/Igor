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

#ifndef __UILAYER__
#define __UILAYER__

#include "Workspace.h"
#include "Outliner.h"
#include "PropertiesEditor.h"

class UILayer : public iLayerWidgets
{

public:
    /*! nothing to do
	*/
    UILayer(iWindowPtr window, int32 zIndex, WorkspacePtr workspace);

    /*! deinit resources
	*/
    ~UILayer();

private:
    /*! the mica workspace
	*/
    WorkspacePtr _workspace;

    /*! the outliner
	*/
    Outliner *_outliner = nullptr;

    /*! the properties dialog or editor
	*/
    PropertiesEditor *_propertiesDialog = nullptr;

    /*! file open/close dialog 
	*/
    iDialogFileSelectPtr _fileDialog = nullptr;

    /*! if true refresh ui next frame
    */
    bool _refresh = false;

    /*! empties the workspace
	*/
    void clearWorkspace();

    /*! handle for graph view selection change event

	\param nodeID the id of the selected node
	*/
    void onGraphViewSelectionChanged(uint64 nodeID);

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

    /*! called when model was done loading

    \param modelNodeID the node id of the model done loading
    */
    void onFileLoadModelReady(uint64 modelNodeID);

    /*! hanlde for exist mica event
	*/
    void onExitMica();

    void onAddMaterial();
    void onLoadMaterial();

    void onFileLoadDialogClosed(iDialogPtr dialog);
    void onImportFileDialogClosed(iDialogPtr dialog);
    void onImportFileReferenceDialogClosed(iDialogPtr dialog);
    void onFileSaveDialogClosed(iDialogPtr dialog);
    void onLoadMaterialFileDialogClosed(iDialogPtr dialog);

    iModelDataInputParameter *createDataInputParameter();

    /*! init ui
	*/
    void onInit() override;

    /*! clear resources
	*/
    void onDeinit() override;

    /*! called on application pre draw event
    */
    void onUpdate() override;

    /*! called on any other event
    */
    void onEvent(iEvent &event) override;

    /*! called when key was pressed

    \param event the event to handle
    */
    bool onKeyDown(iEventKeyDown &event);

    /*! triggered when node was added to scene

    \param event the event to handle
    */
    bool onNodeAddedToScene(iEventNodeAddedToScene &event);

    /*! triggered when node was removed from scene

    \param event the event to handle
    */
    bool onNodeRemovedFromScene(iEventNodeRemovedFromScene &event);

    /*! triggered when selection in scene changed

    \param event the event handle
    */
    bool onSceneSelectionChanged(iEventSceneSelectionChanged &event);
};

#endif // __UILAYER__

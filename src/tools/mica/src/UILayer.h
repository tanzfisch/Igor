//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef MICA_UILAYER_H
#define MICA_UILAYER_H

#include "outliner/Outliner.h"
#include "asset_browser/AssetBrowser.h"
#include "MainDialog.h"
#include "properties_editor/PropertiesEditor.h"
#include "viewport/Viewport.h"

/*! ui layer handling all widget things
 */
class UILayer : public iLayerWidgets
{

public:
    /*! init members
     */
    UILayer(iWindowPtr window, int32 zIndex);

    /*! nothing to do
     */
    ~UILayer() = default;

private:
    /*! the main dialog
     */
    MainDialog *_mainDialog = nullptr;

    /*! entities outliner
     */
    Outliner *_outliner = nullptr;

    /*! the asset browser
     */
    AssetBrowser *_assetBrowser = nullptr;

    /*! view port dialog
     */
    Viewport *_viewport = nullptr;

    /*! the properties dialog or editor
     */
    PropertiesEditor *_propertiesDialog = nullptr;

    /*! file open/close dialog
     */
    iDialogFileSelect _fileDialog;

    /*! opens the create project dialog
     */
    void onCreateProject();

    /*! opens the load project dialog
     */
    void onLoadProject();

    /*! saves the current project
     */
    void onSaveProject();

    /*! handles closing the create project dialog

    \param dialog the calling widget
    */
    void onCreateProjectDialogClosed(iDialogPtr dialog);

    /*! handles closing the load project dialog

    \param dialog the calling widget
    */
    void onLoadProjectDialogClosed(iDialogPtr dialog);

    /*! called when outliner selection changed

    \param sceneID the selected scene
    \param entityIDs selected entities in given scene
     */
    void onOutlinerSelectionChanged(const iEntitySceneID &sceneID, const std::vector<iEntityID> &entityIDs);

    /*! init ui
     */
    void onInit() override;

    /*! clear resources
     */
    void onDeinit() override;

    /*! called on any other event
     */
    void onEvent(iEvent &event) override;

    /*! called when key was pressed

    \param event the event to handle
    */
    bool onKeyDown(iEventKeyDown &event);
};

#endif // MICA_UILAYER_H

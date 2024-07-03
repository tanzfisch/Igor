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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef MICA_SCENE_OUTLINER_H
#define MICA_SCENE_OUTLINER_H

#include "../Workspace.h"
#include "UserControlGraphView.h"

IGOR_EVENT_DEFINITION(ImportFile, void);
IGOR_EVENT_DEFINITION(ImportFileReference, void);
IGOR_EVENT_DEFINITION(ExitMica, void);

/*! scene data outliner
*/
class SceneOutliner : public iDialog
{

public:
    SceneOutliner(WorkspacePtr workspace);
    ~SceneOutliner();

    void refresh();

    void registerOnImportFile(ImportFileDelegate importFileDelegate);
    void unregisterOnImportFile(ImportFileDelegate importFileDelegate);

    void registerOnImportFileReference(ImportFileReferenceDelegate importFileReferenceDelegate);
    void unregisterOnImportFileReference(ImportFileReferenceDelegate importFileReferenceDelegate);

    void registerOnGraphSelectionChanged(GraphSelectionChangedDelegate graphSelectionChangedDelegate);
    void unregisterOnGraphSelectionChanged(GraphSelectionChangedDelegate graphSelectionChangedDelegate);

    void addModel();

private:
    /*! the mica workspace
    */
    WorkspacePtr _workspace;

    ImportFileEvent _importFile;
    ImportFileReferenceEvent _importFileReference;
    ExitMicaEvent _exitMica;

    GraphSelectionChangedEvent _graphSelectionChanged;

    iWidgetGridLayout *_grid = nullptr;

    UserControlGraphView *_userControlGraphView = nullptr;

    iDialogMessageBox *_messageBox = nullptr;
    iDialogDecisionBox *_decisionBoxModelRef = nullptr;

    uint32 _copiedNodeID = 0;
    uint32 _cutNodeID = 0;

    void initGUI();
    void deinitGUI();

    void deinitGraphView();
    void initGraphView();

    void onCreateProject(const iWidgetPtr source);
    void onLoadProject(const iWidgetPtr source);
    void onSaveProject(const iWidgetPtr source);

    void onLoadFile(const iWidgetPtr source);
    void onSaveFile(const iWidgetPtr source);

    void onDelete(const iWidgetPtr source);

    void onAddModelDecision(iDialogPtr dialog);
    void onAddTransformation(uint64 addAt);
    void onAddSwitch(uint64 addAt);
    void onAddGroup(uint64 addAt);
    void onAddEmitter(uint64 addAt);
    void onAddParticleSystem(uint64 addAt);

    void onCopy(const iWidgetPtr source);
    void onPaste(const iWidgetPtr source);
    void onCut(const iWidgetPtr source);

    void onGraphSelectionChanged(uint64 nodeID);

    void onGraphViewSelected(const iWidgetPtr source);
    void onMaterialViewSelected(const iWidgetPtr source);
};

#endif // MICA_SCENE_OUTLINER_H

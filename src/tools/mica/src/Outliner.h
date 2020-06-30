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

#ifndef __OUTLINER_H__
#define __OUTLINER_H__

#include <igor/ui/dialogs/iDialog.h>
using namespace igor;

#include "usercontrols/UserControlGraphView.h"
#include "usercontrols/UserControlMaterialView.h"

namespace igor
{
    class iNode;
    class iWidgetGrid;
    class iWidgetScroll;
    class iWidgetButton;
    class iWidgetGroupBox;
    class iWidgetSpacer;
    class iDialogMessageBox;
    class iDialogDecisionBox;
    class iWidgetCheckBox;
} // namespace igor

iaEVENT(LoadFile, LoadFileDelegate, (), ());
iaEVENT(ImportFile, ImportFileDelegate, (), ());
iaEVENT(ImportFileReference, ImportFileReferenceDelegate, (), ());
iaEVENT(SaveFile, SaveFileDelegate, (), ());
iaEVENT(ExitMica, ExitMicaDelegate, (), ());
iaEVENT(CopyNode, CopyNodeDelegate, (uint64 nodeID), (nodeID));
iaEVENT(PasteNode, PasteNodeDelegate, (uint64 nodeID), (nodeID));
iaEVENT(CutNode, CutNodeDelegate, (uint64 nodeID), (nodeID));

enum class ViewType
{
    GraphView,
    MaterialView
};

/*! menu dialog
*/
class Outliner : public iDialog
{

    friend class iWidgetManager;

public:
    Outliner();
    ~Outliner();

    void setRootNode(iNodePtr root);
    void setSelectedNode(iNodePtr node);
    void refreshView();

    void registerOnLoadFile(LoadFileDelegate loadFileDelegate);
    void unregisterOnLoadFile(LoadFileDelegate loadFileDelegate);

    void registerOnImportFile(ImportFileDelegate importFileDelegate);
    void unregisterOnImportFile(ImportFileDelegate importFileDelegate);

    void registerOnImportFileReference(ImportFileReferenceDelegate importFileReferenceDelegate);
    void unregisterOnImportFileReference(ImportFileReferenceDelegate importFileReferenceDelegate);

    void registerOnSaveFile(SaveFileDelegate saveFileDelegate);
    void unregisterOnSaveFile(SaveFileDelegate saveFileDelegate);

    void registerOnExitMica(ExitMicaDelegate exitMicaDelegate);
    void unregisterOnExitMica(ExitMicaDelegate exitMicaDelegate);

    void registerOnCopyNode(CopyNodeDelegate copyNodeDelegate);
    void unregisterOnCopyNode(CopyNodeDelegate copyNodeDelegate);

    void registerOnPasteNode(PasteNodeDelegate pasteNodeDelegate);
    void unregisterOnPasteNode(PasteNodeDelegate pasteNodeDelegate);

    void registerOnCutNode(CutNodeDelegate cutNodeDelegate);
    void unregisterOnCutNode(CutNodeDelegate cutNodeDelegate);

    void registerOnGraphSelectionChanged(GraphSelectionChangedDelegate graphSelectionChangedDelegate);
    void unregisterOnGraphSelectionChanged(GraphSelectionChangedDelegate graphSelectionChangedDelegate);

    void registerOnAddMaterial(AddMaterialDelegate addMaterialDelegate);
    void unregisterOnAddMaterial(AddMaterialDelegate addMaterialDelegate);

    void registerOnMaterialSelectionChanged(MaterialSelectionChangedDelegate materialSelectionChangedDelegate);
    void unregisterOnMaterialSelectionChanged(MaterialSelectionChangedDelegate materialSelectionChangedDelegate);

    void deleteSelected();
    void duplicateSelected();
    void copySelected();
    void cutSelected();
    void pasteSelected();

    void fileOpen();
    void fileSave();

    void addModel();

private:
    LoadFile _loadFile;
    ImportFile _importFile;
    ImportFileReference _importFileReference;
    SaveFile _saveFile;
    ExitMica _exitMica;

    GraphSelectionChanged _graphSelectionChanged;

    AddMaterial _addMaterial;
    MaterialSelectionChanged _materialSelectionChanged;

    CopyNode _copyNode;
    CutNode _cutNode;
    PasteNode _pasteNode;

    iWidgetGrid *_grid = nullptr;

    UserControlGraphView *_userControlGraphView = nullptr;
    UserControlMaterialView *_userControlMaterialView = nullptr;

    iDialogMessageBox *_messageBox = nullptr;
    iDialogDecisionBox *_decisionBoxModelRef = nullptr;

    ViewType _currentView = ViewType::GraphView;

    uint32 _copiedNodeID = 0;
    uint32 _cutNodeID = 0;

    uint32 _rootNodeID = 0;

    void setViewType(ViewType viewType);

    void initGUI();
    void deinitGUI();

    void deinitGraphView();
    void initGraphView();

    void deinitMaterialView();
    void initMaterialView();

    void onLoadFile(const iWidgetPtr source);
    void onSaveFile(const iWidgetPtr source);
    void onExitMica(const iWidgetPtr source);

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
    void onAddMaterial();
    void onMaterialSelectionChanged(uint64 materialID);

    void onGraphViewSelected(const iWidgetPtr source);
    void onMaterialViewSelected(const iWidgetPtr source);
};

#endif // __OUTLINER_H__

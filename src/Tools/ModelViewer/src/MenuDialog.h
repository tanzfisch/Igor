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

#ifndef __MENUDIALOG__
#define __MENUDIALOG__

#include <iWidgetUserDialog.h>
using namespace Igor;

#include "UserControlGraphView.h"
#include "UserControlProperties.h"

namespace Igor
{
    class iNode;
    class iWidgetDialog;
    class iWidgetGrid;
    class iWidgetScroll;
    class iWidgetButton;
    class iWidgetGroupBox;
    class iWidgetSpacer;
    class iMessageBox;
    class iDecisionBox;
}

iaEVENT(LoadFile, LoadFileDelegate, void, (), ());
iaEVENT(ImportFile, ImportFileDelegate, void, (), ());
iaEVENT(ImportFileReference, ImportFileReferenceDelegate, void, (), ());
iaEVENT(SaveFile, SaveFileDelegate, void, (), ());
iaEVENT(ExitModelViewer, ExitModelViewerDelegate, void, (), ());

iaEVENT(AddTransformation, AddTransformationDelegate, void, (uint32 nodeID), (nodeID));
iaEVENT(AddGroup, AddGroupDelegate, void, (uint32 nodeID), (nodeID));
iaEVENT(AddSwitch, AddSwitchDelegate, void, (uint32 nodeID), (nodeID));

iaEVENT(CopyNode, CopyNodeDelegate, void, (uint32 nodeID), (nodeID));
iaEVENT(PasteNode, PasteNodeDelegate, void, (uint32 nodeID), (nodeID));
iaEVENT(CutNode, CutNodeDelegate, void, (uint32 nodeID), (nodeID));

/*! menu dialog
*/
class MenuDialog : public iWidgetUserDialog
{
public:

    MenuDialog();
    ~MenuDialog();

    void setRootNode(iNode* root);
    void updateGraph();

    void registerOnLoadFile(LoadFileDelegate loadFileDelegate);
    void unregisterOnLoadFile(LoadFileDelegate loadFileDelegate);

    void registerOnImportFile(ImportFileDelegate importFileDelegate);
    void unregisterOnImportFile(ImportFileDelegate importFileDelegate);

    void registerOnImportFileReference(ImportFileReferenceDelegate importFileReferenceDelegate);
    void unregisterOnImportFileReference(ImportFileReferenceDelegate importFileReferenceDelegate);

    void registerOnSaveFile(SaveFileDelegate saveFileDelegate);
    void unregisterOnSaveFile(SaveFileDelegate saveFileDelegate);

    void registerOnExitModelViewer(ExitModelViewerDelegate exitModelViewerDelegate);
    void unregisterOnExitModelViewer(ExitModelViewerDelegate exitModelViewerDelegate);

    void registerOnAddTransformation(AddTransformationDelegate addTransformationDelegate);
    void unregisterOnAddTransformation(AddTransformationDelegate addTransformationDelegate);

    void registerOnAddGroup(AddGroupDelegate addGroupDelegate);
    void unregisterOnAddGroup(AddGroupDelegate addGroupDelegate);

    void registerOnAddSwitch(AddSwitchDelegate addSwitchDelegate);
    void unregisterOnAddSwitch(AddSwitchDelegate addSwitchDelegate);

    void registerOnCopyNode(CopyNodeDelegate copyNodeDelegate);
    void unregisterOnCopyNode(CopyNodeDelegate copyNodeDelegate);

    void registerOnPasteNode(PasteNodeDelegate pasteNodeDelegate);
    void unregisterOnPasteNode(PasteNodeDelegate pasteNodeDelegate);

    void registerOnCutNode(CutNodeDelegate cutNodeDelegate);
    void unregisterOnCutNode(CutNodeDelegate cutNodeDelegate);

private:

    LoadFile _loadFile;
    ImportFile _importFile;
    ImportFileReference _importFileReference;
    SaveFile _saveFile;
    ExitModelViewer _exitModelViewer;
    AddTransformation _addTransformation;
    AddGroup _addGroup;
    AddSwitch _addSwitch;
    CopyNode _copyNode;
    CutNode _cutNode;
    PasteNode _pasteNode;

    iMessageBox* _messageBox = nullptr;
    iDecisionBox* _decisionBoxModelRef = nullptr;

    iWidgetGrid* _grid = nullptr;
    iWidgetGrid* _gridButtons = nullptr;
    iWidgetGrid* _gridButtons1 = nullptr;
    iWidgetGrid* _gridButtons2 = nullptr;
    iWidgetButton* _exitButton = nullptr;
    iWidgetButton* _loadButton = nullptr;
    iWidgetButton* _saveButton = nullptr;
    iWidgetSpacer* _spacer1 = nullptr;
    iWidgetSpacer* _spacer2 = nullptr;
    iWidgetButton* _cutButton = nullptr;
    iWidgetButton* _copyButton = nullptr;
    iWidgetButton* _pasteButton = nullptr;
    iWidgetButton* _deleteButton = nullptr;
    iWidgetButton* _addModelButton = nullptr;
    iWidgetButton* _addSwitchButton = nullptr;
    iWidgetButton* _addTransformationButton = nullptr;
    iWidgetButton* _addGroupButton = nullptr;

    vector<iWidget*> _allwidgets;

    UserControlGraphView* _userControlGraphView = nullptr;
    UserControlProperties* _userControlProperties = nullptr;

    uint32 _copiedNodeID = 0;
    uint32 _cutNodeID = 0;

    void initGUI();
    void deinitGUI();

    void onLoadFile(iWidget* source);
    void onSaveFile(iWidget* source);
    void onExitModelViewer(iWidget* source);

    void onDelete(iWidget* source);

    void onAddModel(iWidget* source);
    void onAddModelDecision(bool ok, int32 selection);
    void onAddTransformation(iWidget* source);
    void onAddSwitch(iWidget* source);
    void onAddGroup(iWidget* source);

    void onCopy(iWidget* source);
    void onPaste(iWidget* source);
    void onCut(iWidget* source);

    void onGraphViewSelectionChanged();
    void onStructureChanged();

};


#endif

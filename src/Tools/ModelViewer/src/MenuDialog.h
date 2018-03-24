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

#include <iDialog.h>
using namespace Igor;

#include "UserControlGraphView.h"
#include "UserControlMaterialView.h"

namespace Igor
{
    class iNode;
    class iDialog;
    class iWidgetGrid;
    class iWidgetScroll;
    class iWidgetButton;
    class iWidgetGroupBox;
    class iWidgetSpacer;
    class iDialogMessageBox;
    class iDialogDecisionBox;
	class iWidgetCheckBox;
}

iaEVENT(LoadFile, LoadFileDelegate, void, (), ());
iaEVENT(ImportFile, ImportFileDelegate, void, (uint64 nodeID), (nodeID));
iaEVENT(ImportFileReference, ImportFileReferenceDelegate, void, (uint64 nodeID), (nodeID));
iaEVENT(SaveFile, SaveFileDelegate, void, (), ());
iaEVENT(ExitModelViewer, ExitModelViewerDelegate, void, (), ());

iaEVENT(CopyNode, CopyNodeDelegate, void, (uint64 nodeID), (nodeID));
iaEVENT(PasteNode, PasteNodeDelegate, void, (uint64 nodeID), (nodeID));
iaEVENT(CutNode, CutNodeDelegate, void, (uint64 nodeID), (nodeID));

enum class ViewType
{
    GraphView,
    MaterialView
};

/*! menu dialog
*/
class MenuDialog : public iDialog
{

public:

    /*! creates instance of this widget type
    */
    static iDialog* createInstance();

    void setRootNode(iNode* root);
    void setSelectedNode(iNode* node);
    void refreshView();

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

    void registerOnAddEmitter(AddEmitterDelegate addEmitterDelegate);
    void unregisterOnAddEmitter(AddEmitterDelegate addEmitterDelegate);

    void registerOnAddParticleSystem(AddParticleSystemDelegate addParticleSystemDelegate);
    void unregisterOnAddParticleSystem(AddParticleSystemDelegate addParticleSystemDelegate);

    void registerOnAddSwitch(AddSwitchDelegate addSwitchDelegate);
    void unregisterOnAddSwitch(AddSwitchDelegate addSwitchDelegate);

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

private:

    LoadFile _loadFile;
    ImportFile _importFile;
    ImportFileReference _importFileReference;
    SaveFile _saveFile;
    ExitModelViewer _exitModelViewer;
    
	AddTransformation _addTransformation;
    AddGroup _addGroup;
    AddEmitter _addEmitter;
    AddParticleSystem _addParticleSystem;
    AddSwitch _addSwitch;
	GraphSelectionChanged _graphSelectionChanged;

	AddMaterial _addMaterial;
	MaterialSelectionChanged _materialSelectionChanged;

    CopyNode _copyNode;
    CutNode _cutNode;
    PasteNode _pasteNode;

    iDialogMessageBox* _messageBox = nullptr;

    iWidgetGrid* _grid = nullptr;
    iWidgetGrid* _gridButtons = nullptr;

	iWidgetGrid* _gridRadioButtons = nullptr;
	iWidgetCheckBox* _checkBoxGraph = nullptr;
	iWidgetCheckBox* _checkBoxMaterial = nullptr;

    iWidgetButton* _exitButton = nullptr;
    iWidgetButton* _loadButton = nullptr;
    iWidgetButton* _saveButton = nullptr;
    iWidgetSpacer* _spacer1 = nullptr;
    iWidgetSpacer* _spacer2 = nullptr;
    iWidgetButton* _cutButton = nullptr;
    iWidgetButton* _copyButton = nullptr;
    iWidgetButton* _pasteButton = nullptr;
    iWidgetButton* _deleteButton = nullptr;

    vector<iWidget*> _allwidgets;

    UserControlGraphView* _userControlGraphView = nullptr;
    UserControlMaterialView* _userControlMaterialView = nullptr;

	iDialogDecisionBox* _decisionBoxModelRef = nullptr;

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

    void onLoadFile(iWidget* source);
    void onSaveFile(iWidget* source);
    void onExitModelViewer(iWidget* source);

    void onDelete(iWidget* source);
    
	void onAddModel(uint64 addAt);
	void onAddModelDecision(bool ok, int32 selection);
	void onAddTransformation(uint64 addAt);
	void onAddSwitch(uint64 addAt);
	void onAddGroup(uint64 addAt);
	void onAddEmitter(uint64 addAt);
	void onAddParticleSystem(uint64 addAt);

    void onCopy(iWidget* source);
    void onPaste(iWidget* source);
    void onCut(iWidget* source);

    void onGraphSelectionChanged(uint64 nodeID);
	void onAddMaterial();
	void onMaterialSelectionChanged(uint64 materialID);

    void onGraphViewSelected(iWidget* source);
    void onMaterialViewSelected(iWidget* source);

    MenuDialog();
    ~MenuDialog();

};


#endif


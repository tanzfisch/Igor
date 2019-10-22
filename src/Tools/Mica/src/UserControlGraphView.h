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

#ifndef __USERCONTROLGRAPHVIEW__
#define __USERCONTROLGRAPHVIEW__

#include <iaString.h>
#include <iaEvent.h>
using namespace IgorAux;

#include <iUserControl.h>
#include <iNodeVisitor.h>
#include <iDefines.h>
#include <iNode.h>
using namespace Igor;

#include <sstream>
#include <vector>
#include <map>


namespace Igor
{
    class iWidgetGrid;
    class iWidget;
    class iWidgetScroll;
    class iWidgetGroupBox;
	class iWidgetButton;
    class iDialogIndexMenu;
    typedef iDialogIndexMenu* iDialogIndexMenuPtr;
    class iDialog;
    typedef iDialog* iDialogPtr;
}

iaEVENT(AddModel, AddModelDelegate, void, (uint64 nodeID), (nodeID));
iaEVENT(AddTransformation, AddTransformationDelegate, void, (uint64 nodeID), (nodeID));
iaEVENT(AddGroup, AddGroupDelegate, void, (uint64 nodeID), (nodeID));
iaEVENT(AddSwitch, AddSwitchDelegate, void, (uint64 nodeID), (nodeID));
iaEVENT(AddEmitter, AddEmitterDelegate, void, (uint64 nodeID), (nodeID));
iaEVENT(AddParticleSystem, AddParticleSystemDelegate, void, (uint64 nodeID), (nodeID));
iaEVENT(GraphSelectionChanged, GraphSelectionChangedDelegate, void, (uint64 nodeID), (nodeID));

class UserControlGraphView : public iUserControl, public iNodeVisitor
{

public:

	UserControlGraphView();
	~UserControlGraphView();

    void setSelectedNode(uint64 nodeID);
    uint64 getSelectedNode() const;
    void setRootNode(uint64 root);
    uint64 getRootNode();
    void refresh();

    void registerOnSelectionChange(GraphSelectionChangedDelegate selectionChangeDelegate);
    void unregisterOnSelectionChange(GraphSelectionChangedDelegate selectionChangeDelegate);

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
    
	void registerOnAddModel(AddModelDelegate addModelDelegate);
	void unregisterOnAddModel(AddModelDelegate addModelDelegate);

private:

	AddModel _addModel;
	AddTransformation _addTransformation;
	AddGroup _addGroup;
	AddEmitter _addEmitter;
	AddParticleSystem _addParticleSystem;
	AddSwitch _addSwitch;
	GraphSelectionChanged _graphSelectionChanged;

    uint64 _root = iNode::INVALID_NODE_ID;
	GraphSelectionChanged _selectionChange;

    int32 _indentation = 0;
    bool _firstNode = true;

    uint64 _selectedNode = iNode::INVALID_NODE_ID;

    iWidgetGrid* _gridGraph = nullptr;

    iDialogIndexMenuPtr _dialogMenu = nullptr;

    void clearGraph();

    iaString getIconTexture(iNodeType type);

    void initGUI();

    void OnSelectionChange(iWidgetPtr widget);
    void OnContextMenu(iWidgetPtr widget);
    void OnContextMenuClose(iDialogPtr dialog);

	void onAddModel(const iWidgetPtr source);
	void onAddTransformation(const iWidgetPtr source);
	void onAddSwitch(const iWidgetPtr source);
	void onAddGroup(const iWidgetPtr source);
	void onAddEmitter(const iWidgetPtr source);
	void onAddParticleSystem(const iWidgetPtr source);

    void preTraverse() override;
    bool preOrderVisit(iNodePtr node, iNodePtr nextSibling) override;
    void postOrderVisit(iNodePtr node) override;
    void postTraverse() override;

};

#endif

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

#ifndef __USERCONTROLGRAPHVIEW_H__
#define __USERCONTROLGRAPHVIEW_H__

#include "../Workspace.h"

#include <sstream>
#include <vector>
#include <map>

iaSIGNAL(GraphSelectionChanged, GraphSelectionChangedDelegate, (uint64 nodeID), (nodeID));

class Outliner;

/*! the user control graph view
*/
class UserControlGraphView : public iUserControl, public iNodeVisitor
{

public:
    UserControlGraphView(WorkspacePtr workspace, Outliner *outliner);
    ~UserControlGraphView();

    /*! sets the selected node

    \param nodeID the node to select
    */
    void setSelectedNode(iNodeID nodeID);

    /*! \returns selected node id
    */
    iNodeID getSelectedNode() const;

    /*! sets root node for graph view

    \param root the root node id
    */
    void setRootNode(iNodeID root);

    /*! \returns root node id
    */
    iNodeID getRootNode();

    /*! refresh the graph view
    */
    void refresh();

    void registerOnSelectionChange(GraphSelectionChangedDelegate selectionChangeDelegate);
    void unregisterOnSelectionChange(GraphSelectionChangedDelegate selectionChangeDelegate);

private:
    /*! the mica workspace
    */
    WorkspacePtr _workspace;

    Outliner *_outliner = nullptr;

    GraphSelectionChanged _graphSelectionChanged;

    uint64 _root = iNode::INVALID_NODE_ID;
    GraphSelectionChanged _selectionChange;

    int32 _indentation = 0;
    bool _firstNode = true;

    uint64 _selectedNode = iNode::INVALID_NODE_ID;

    iWidgetGrid *_gridGraph = nullptr;

    /*! the scene graphs context menu
    */
    iDialogMenuPtr _graphContextMenu = nullptr;

    /*! \returns a new action context
    */
    iActionContextPtr getContext();

    void clearGraph();

    iaString getIconTexture(iNodeType type);

    void initGUI();

    void onSceneChanged();

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

#endif // __USERCONTROLGRAPHVIEW_H__

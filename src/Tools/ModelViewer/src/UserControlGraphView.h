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

#include <iWidgetUserControl.h>
#include <iNodeVisitor.h>
#include <iDefines.h>
#include <iNode.h>
using namespace Igor;

#include <sstream>
#include <vector>
#include <map>
using namespace std;

namespace Igor
{
    class iWidgetGrid;
    class iWidget;
    class iWidgetScroll;
    class iWidgetGroupBox;
}

iaEVENT(SelectionChange, SelectionChangeDelegate, void, (), ());

class UserControlGraphView : public iWidgetUserControl, public iNodeVisitor
{

public:

    UserControlGraphView();
    virtual ~UserControlGraphView();

    uint32 getSelectedNode();
    void setRootNode(uint32 root);
    uint32 getRootNode();
    void updateGraph();

    iWidget* getWidget();
    
    void registerOnSelectionChange(SelectionChangeDelegate selectionChangeDelegate);
    void unregisterOnSelectionChange(SelectionChangeDelegate selectionChangeDelegate);
    
private:

    uint32 _root = iNode::INVALID_NODE_ID;
    SelectionChange _selectionChange;

    iWidgetGroupBox* _groupBox = nullptr;
    iWidgetGrid* _grid = nullptr;
    iWidgetScroll* _scroll = nullptr;

    vector<iWidget*> _widgets;

    int32 _indentation = 0;
    bool _firstNode = true;

    uint32 _selectedNode = iNode::INVALID_NODE_ID;

    vector<uint32*> _userData;

    void clear();

    iaString getIconTexture(iNodeType type);

    void OnSelectionChange(iWidget* widget);

    void initGUI();
    void deinitGUI();

    virtual void preTraverse();
    virtual bool preOrderVisit(iNode* node);
    virtual void postOrderVisit(iNode* node);
    virtual void postTraverse();

};

#endif

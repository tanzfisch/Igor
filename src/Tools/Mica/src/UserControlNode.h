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

#ifndef __USERCONTROLNODE__
#define __USERCONTROLNODE__

#include <iaEvent.h>
using namespace IgorAux;

#include <iNode.h>

#include <iUserControl.h>
using namespace Igor;

namespace Igor
{
    class iWidgetGrid;
    class iWidgetTextEdit;
    class iWidgetLabel;
    class iWidget;
    class iWidgetCheckBox;
}

iaEVENT(NameChangedEvent, NameChangedDelegate, void, (), ());

class UserControlNode : public iUserControl
{
public:

	UserControlNode();
	~UserControlNode() = default;
	
    void setNode(uint32 id);
    uint32 getNode();

    void registerNameChangeDelegate(NameChangedDelegate nameChangedDelegate);
    void unregisterNameChangeDelegate(NameChangedDelegate nameChangedDelegate);

private:

    NameChangedEvent _nameChangedEvent;

    iWidgetGrid* _grid = nullptr;
    iWidgetLabel* _labelName = nullptr;
    iWidgetLabel* _labelActive = nullptr;
    iWidgetCheckBox* _checkBoxActive = nullptr;
    iWidgetTextEdit* _textName = nullptr;

    uint32 _nodeId = 0;

    void onNameChanged(iWidgetPtr source);

    void updateGUI();
    void updateNode();

    void initGUI();



};

#endif
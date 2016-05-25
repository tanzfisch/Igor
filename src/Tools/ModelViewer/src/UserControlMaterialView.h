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

#ifndef __USERCONTROLMATERIALVIEW__
#define __USERCONTROLMATERIALVIEW__

#include <iaEvent.h>
using namespace IgorAux;

#include <iNodeModel.h>
#include <iWidgetUserControl.h>
using namespace Igor;

namespace Igor
{
    class iWidgetGrid;
    class iWidgetTextEdit;
    class iWidgetLabel;
    class iWidgetButton;
    class iUserControlColorChooser;
    class iWidgetSlider;
    class iWidgetNumberChooser;
    class iWidgetSelectBox;
}

iaEVENT(AddMaterial, AddMaterialDelegate, void, (), ());

class UserControlMaterialView : public iWidgetUserControl
{
public:

    UserControlMaterialView();
    ~UserControlMaterialView();

    void refresh();
    iWidget* getWidget();

    void registerOnAddMaterial(AddMaterialDelegate addMaterialDelegate);
    void unregisterOnAddMaterial(AddMaterialDelegate addMaterialDelegate);

private:

    AddMaterial _addMaterial;

    iWidget* _rootWidget = nullptr;
    iWidgetGrid* _gridGraph = nullptr;
    vector<uint32*> _userData;

    vector<iWidget*> _allWidgets;
    vector<iWidget*> _gridEntryWidgets;

    uint32 _nodeId = 0;

    void initGUI();
    void deinitGUI();

    void clear();

    void OnSelectionChange(iWidget* widget);

    void onAddMaterial(iWidget* source);
};

#endif
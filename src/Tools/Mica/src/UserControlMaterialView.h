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
#include <iUserControl.h>
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
iaEVENT(MaterialSelectionChanged, MaterialSelectionChangedDelegate, void, (uint64 materialID), (materialID));

class UserControlMaterialView : public iUserControl
{
public:

	UserControlMaterialView();
	~UserControlMaterialView() = default;

    void refresh();

    void registerOnAddMaterial(AddMaterialDelegate addMaterialDelegate);
    void unregisterOnAddMaterial(AddMaterialDelegate addMaterialDelegate);

	void registerOnMaterialSelectionChanged(MaterialSelectionChangedDelegate materialSelectionChangedDelegate);
	void unregisterOnMaterialSelectionChanged(MaterialSelectionChangedDelegate materialSelectionChangedDelegate);

private:

	MaterialSelectionChanged _materialSelectionChanged;
    AddMaterial _addMaterial;

    iWidgetGrid* _gridGraph = nullptr;

	uint32 _selectedMaterial = 0;

    void initGUI();

    void clear();

    void OnSelectionChange(iWidgetPtr widget);

    void onAddMaterial(iWidgetPtr source);

};

#endif
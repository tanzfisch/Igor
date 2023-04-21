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

#ifndef __USERCONTROLMATERIALVIEW_H__
#define __USERCONTROLMATERIALVIEW_H__

#include <igor/igor.h>
using namespace iaux;
using namespace igor;

IGOR_SIGNAL(AddMaterial, AddMaterialDelegate, (), ());
IGOR_SIGNAL(LoadMaterial, LoadMaterialDelegate, (), ());
IGOR_SIGNAL(MaterialSelectionChanged, MaterialSelectionChangedDelegate, (const iMaterialID &materialID), (materialID));

class UserControlMaterialView : public iUserControl
{
public:
    UserControlMaterialView();
    ~UserControlMaterialView() = default;

    void refresh();

    void registerOnAddMaterial(AddMaterialDelegate addMaterialDelegate);
    void unregisterOnAddMaterial(AddMaterialDelegate addMaterialDelegate);

    void registerOnLoadMaterial(LoadMaterialDelegate delegate);
    void unregisterOnLoadMaterial(LoadMaterialDelegate delegate);

    void registerOnMaterialSelectionChanged(MaterialSelectionChangedDelegate materialSelectionChangedDelegate);
    void unregisterOnMaterialSelectionChanged(MaterialSelectionChangedDelegate materialSelectionChangedDelegate);

private:
    MaterialSelectionChanged _materialSelectionChanged;
    AddMaterial _addMaterial;
    LoadMaterial _loadMaterial;

    iWidgetGrid *_gridGraph = nullptr;

    /*! id of current selected material
    */
    iMaterialID _selectedMaterial;

    void initGUI();

    void clear();

    void OnSelectionChange(iWidgetPtr widget);

    void onAddMaterial(const iWidgetPtr source);
    void onLoadMaterial(const iWidgetPtr source);
};

#endif // __USERCONTROLMATERIALVIEW_H__
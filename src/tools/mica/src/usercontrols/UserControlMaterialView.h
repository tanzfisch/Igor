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

#ifndef __USERCONTROLMATERIALVIEW__
#define __USERCONTROLMATERIALVIEW__

#include <igor/igor.h>
using namespace iaux;
using namespace igor;

IGOR_EVENT_DEFINITION(AddMaterial, void);
IGOR_EVENT_DEFINITION(LoadMaterial, void);
IGOR_EVENT_DEFINITION(ResourceSelectionChanged, void, const iResourceID &);

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

    void registerOnResourceSelectionChanged(ResourceSelectionChangedDelegate materialSelectionChangedDelegate);
    void unregisterOnResourceSelectionChanged(ResourceSelectionChangedDelegate materialSelectionChangedDelegate);

private:
    ResourceSelectionChangedEvent _materialSelectionChanged;
    AddMaterialEvent _addMaterial;
    LoadMaterialEvent _loadMaterial;

    iWidgetGridLayout *_gridGraph = nullptr;

    /*! id of current selected material
    */
    iMaterialID _selectedMaterial;

    void initGUI();

    /*! clears the widget back to default
    */
    void clear() override;

    void OnSelectionChange(iWidgetPtr widget);

    void onAddMaterial(const iWidgetPtr source);
    void onLoadMaterial(const iWidgetPtr source);
};

#endif // __USERCONTROLMATERIALVIEW__
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

#ifndef __PROPERTIESDIALOG__
#define __PROPERTIESDIALOG__

#include <igor/ui/dialogs/iDialog.h>
using namespace igor;

#include "UserControlProperties.h"

/*! menu dialog
*/
class PropertiesDialog : public iDialog
{

	friend class iWidgetManager;

public:

    /*! init UI
    */
	PropertiesDialog();

    /*! does nothing
    */
	~PropertiesDialog() = default;

    void registerPropertiesChangedDelegate(PropertiesChangedDelegate propertiesChangedDelegate);
    void unregisterPropertiesChangedDelegate(PropertiesChangedDelegate propertiesChangedDelegate);

    void registerStructureChangedDelegate(StructureChangedDelegate structureChangedDelegate);
    void unregisterStructureChangedDelegate(StructureChangedDelegate structureChangedDelegate);

    void onGraphViewSelectionChanged(uint64 nodeID);
	void onMaterialSelectionChanged(uint64 materialID);

private:

    PropertiesChangedEvent _propertiesChangedEvent;
    StructureChangedEvent _structureChangedEvent;

    UserControlProperties* _userControlProperties = nullptr;

    /*! init UI
    */
    void initGUI();
    
    void onStructureChanged();


};


#endif


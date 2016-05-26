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

#ifndef __USERCONTROLMATERIAL__
#define __USERCONTROLMATERIAL__

#include <iNodeMesh.h>
#include <iaColor4.h>
using namespace IgorAux;

#include <iWidgetUserControl.h>
#include <iUserControlColorChooser.h>
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
}

iaEVENT(MaterialNameChangedEvent, MaterialNameChangedDelegate, void, (), ());

class UserControlMaterial : public iWidgetUserControl
{
public:

    UserControlMaterial();
    ~UserControlMaterial();

    iWidget* getWidget();

    void setMaterial(uint32 id);
    uint32 getMaterial();

    void registerNameChangeDelegate(MaterialNameChangedDelegate nameChangedDelegate);
    void unregisterNameChangeDelegate(MaterialNameChangedDelegate nameChangedDelegate);

private:

    MaterialNameChangedEvent _materialNameChangedEvent;

    iWidgetGrid* _grid = nullptr;
    iWidgetTextEdit* _textName = nullptr;
    iWidgetNumberChooser* _renderingOrder = nullptr;

    vector<iWidget*> _allWidgets;

    uint32 _materialID = 0;

    void onTextChangedName(iWidget* source);
    void onChangedRenderOrder(iWidget* source);

    void updateGUI();
    void updateMaterial();

    void initGUI();
    void deinitGUI();

};

#endif
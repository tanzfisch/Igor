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

#ifndef __USERCONTROLPROPERTIES__
#define __USERCONTROLPROPERTIES__

namespace Igor
{
    class iWidgetDialog;
    class iWidgetGrid;
    class iWidgetScroll;
    class iWidgetGroupBox;
    class iWidgetSpacer;
}

#include <iNode.h>
#include <iWidgetUserControl.h>
using namespace Igor;

#include <iaEvent.h>
using namespace IgorAux;


class UserControlLight;
class UserControlMesh;
class UserControlTransformation;
class UserControlNode;
class UserControlModel;
class UserControlEmitter;
class UserControlParticleSystem;

iaEVENT(PropertiesChangedEvent, PropertiesChangedDelegate, void, (), ());
iaEVENT(StructureChangedEvent, StructureChangedDelegate, void, (), ());

class UserControlProperties : public iWidgetUserControl
{
public:

    UserControlProperties();
    ~UserControlProperties();

    void setNode(uint32 id);
    uint32 getNode();
    iWidgetDialog* getDialog();

    void clear();

    iWidget* getWidget();

    void registerPropertiesChangedDelegate(PropertiesChangedDelegate propertiesChangedDelegate);
    void unregisterPropertiesChangedDelegate(PropertiesChangedDelegate propertiesChangedDelegate);

    void registerStructureChangedDelegate(StructureChangedDelegate structureChangedDelegate);
    void unregisterStructureChangedDelegate(StructureChangedDelegate structureChangedDelegate);

private:

    PropertiesChangedEvent _propertiesChangedEvent;
    StructureChangedEvent _structureChangedEvent;

    iNodeType _currentNodeType = iNodeType::Undefined;

    void initGUI();
    void deinitGUI();

    void initTransformNode();
    void deinitTransformNode();

    void initLightNode();
    void deinitLightNode();

    void initMeshNode();
    void deinitMeshNode();

    void initModel();
    void deinitModel();

    void initEmitter();
    void deinitEmitter();

    void initParticleSystem();
    void deinitParticleSystem();

    void onNodeNameChanged();

    uint32 _nodeID = 0;

    iWidgetDialog* _dialog = nullptr;
    iWidgetScroll* _scroll = nullptr;
    iWidgetGrid* _grid = nullptr;
    iWidgetGroupBox* _groupBox = nullptr;
    iWidgetSpacer* _spacer = nullptr;

    UserControlTransformation* _userControlTransformation = nullptr;
    UserControlLight* _userControlLight = nullptr;
    UserControlMesh* _userControlMesh = nullptr;
    UserControlModel* _userControlModel = nullptr;
    UserControlEmitter* _userControlEmitter = nullptr;
    UserControlParticleSystem* _userControlParticleSystem = nullptr;
    UserControlNode* _userControlNode = nullptr;

};


#endif

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

#ifndef __USERCONTROLPROPERTIES__
#define __USERCONTROLPROPERTIES__

#include <igor/igor.h>
using namespace igor;
using namespace iaux;

class UserControlLight;
class UserControlMesh;
class UserControlTransformation;
class UserControlNode;
class UserControlModel;
class UserControlEmitter;
class UserControlParticleSystem;
class UserControlMaterial;

IGOR_EVENT_DEFINITION(PropertiesChanged, void);
IGOR_EVENT_DEFINITION(StructureChanged, void);

enum class PropertyType
{
    Undefined,
    Node,
    Material
};

class UserControlProperties : public iUserControl
{
public:
    /*! init ui
    */
    UserControlProperties();

    /*! does nothing
    */
    ~UserControlProperties() = default;

    void setNode(const iNodeID &id);
    void setMaterial(const iMaterialID& materialID);

    void clear();

    void registerPropertiesChangedDelegate(PropertiesChangedDelegate propertiesChangedDelegate);
    void unregisterPropertiesChangedDelegate(PropertiesChangedDelegate propertiesChangedDelegate);

    void registerStructureChangedDelegate(StructureChangedDelegate structureChangedDelegate);
    void unregisterStructureChangedDelegate(StructureChangedDelegate structureChangedDelegate);

private:
    PropertiesChangedEvent _propertiesChangedEvent;
    StructureChangedEvent _structureChangedEvent;

    iNodeType _currentNodeType = iNodeType::Undefined;

    void initGUI();

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

    void initMaterial();
    void deinitMaterial();

    void onNameChanged();

    void cleanUpPropertyUI();

    iNodeID _nodeID;
    iMaterialID _materialID;
    PropertyType _propertyType = PropertyType::Undefined;

    // iDialog* _dialog = nullptr;
    iWidgetGrid *_grid = nullptr;

    UserControlTransformation *_userControlTransformation = nullptr;
    UserControlLight *_userControlLight = nullptr;
    UserControlMesh *_userControlMesh = nullptr;
    UserControlModel *_userControlModel = nullptr;
    UserControlEmitter *_userControlEmitter = nullptr;
    UserControlParticleSystem *_userControlParticleSystem = nullptr;
    UserControlNode *_userControlNode = nullptr;
    UserControlMaterial *_userControlMaterial = nullptr;
};

#endif // __USERCONTROLPROPERTIES__

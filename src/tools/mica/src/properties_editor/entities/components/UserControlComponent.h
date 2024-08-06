//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef USERCONTROL_COMPONENT_H
#define USERCONTROL_COMPONENT_H

#include "../../../MicaDefines.h"

#include <igor/igor.h>
using namespace iaux;
using namespace igor;

class UserControlComponent : public iUserControl
{
public:
    /*! init user control

    \param scene the given scene
    \param entity the given entity
    \param typeName name of component
    \param parent the optional parent widget
    */
    UserControlComponent(const iEntitySceneID &scene, const iEntityID &entity, const iaString &typeName, const iWidgetPtr parent = nullptr);

    /*! does nothing
     */
    virtual ~UserControlComponent() = default;

    /*! init ui
     */
    virtual void init();

    /*! update ui with node data
     */
    virtual void update();

    /*! update entity
     */
    virtual void updateComponent();

protected:
    /*! scene id
     */
    iEntitySceneID _sceneID;

    /*! entity id
     */
    iEntityID _entityID;

    /*! name of component type
     */
    iaString _typeName;

    /*! flag to prevent endless update loop
     */
    bool _ignoreUpdate = false;

    /*! main layout
     */
    iWidgetBoxLayoutPtr _layout = nullptr;
};

#endif // USERCONTROL_COMPONENT_H
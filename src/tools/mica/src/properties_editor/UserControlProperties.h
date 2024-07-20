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

#ifndef USERCONTROL_PROPERTIES_H
#define USERCONTROL_PROPERTIES_H

#include <igor/igor.h>
using namespace igor;

/*! user control properties
 */
class UserControlProperties : public iUserControl
{
public:
    /*! property type definition
     */
    enum class PropertyType
    {
        Resource,
        Scene, // aka prefab
        Entity
    };

    /*! init ui for nodes

    \param nodeID the node to represent
    \param parent the parent widget (optional)
    */
    UserControlProperties(iNodeID nodeID, const iWidgetPtr parent = nullptr);

    /*! init ui with id

    \param propertyType type of property
    \param id the id chain of what we are looking at
    \param parent the parent widget (optional)
    */
    UserControlProperties(PropertyType propertyType, const std::vector<iaUUID> &id, const iWidgetPtr parent = nullptr);

    /*! does nothing
     */
    virtual ~UserControlProperties() = default;

private:
    /*! layout widget
     */
    iWidgetBoxLayoutPtr _layout = nullptr;

    /*! init common part of UI
     */
    void initUI();

    /*! initializes node UI
     */
    void initNodeUI(iNodeID nodeID);

    /*! initializes resource UI

    \param resourceID id of resource to display
    */
    void initResourceUI(const iResourceID &resourceID);

    /*! initializes entity UI

    \param sceneID the scene id
    */
    void initSceneUI(const iEntitySceneID &sceneID);

    /*! initializes entity UI

    \param sceneID the scene id
    \param entityID the entity id (must be in given scene)
    */
    void initEntityUI(const iEntitySceneID &sceneID, const iEntityID &entityID);
};

#endif // USERCONTROL_PROPERTIES_H
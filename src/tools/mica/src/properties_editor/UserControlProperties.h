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

#ifndef __USERCONTROL_PROPERTIES__
#define __USERCONTROL_PROPERTIES__

#include <igor/igor.h>
using namespace igor;

/*! user control properties
*/
class UserControlProperties : public iUserControl
{
public:
    /*! init ui for nodes

    \param nodeID the node to represent
    */
    UserControlProperties(iNodeID nodeID, const iWidgetPtr parent);

    /*! init ui for resource

    \param resourceID the resource to represent
    */
    UserControlProperties(const iResourceID &resourceID, const iWidgetPtr parent);

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

    /*! initializes material UI
    */
    void initResourceUI(const iResourceID &resourceID);
};

#endif // __USERCONTROL_PROPERTIES__

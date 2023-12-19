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

#ifndef __PROPERTIES_EDITOR__
#define __PROPERTIES_EDITOR__

#include "UserControlProperties.h"

/*! properties dialog
 */
class PropertiesEditor : public iDialog
{

    friend class iWidgetManager;

public:
    /*! init UI
     */
    PropertiesEditor();

    /*! does nothing
     */
    ~PropertiesEditor() = default;

    /*! sets selection of a node id

    \param nodeID the node id to be selected
    */
    void setSelection(iNodeID nodeID);

    /*! sets selection of a resource id

    \param resourceID the id to be selected
    */
    void setSelection(const iResourceID &resourceID);

private:

    /*! scroll widget
    */
    iWidgetScrollPtr _scroll = nullptr;

    /*! user control properties
    */
    UserControlProperties *_userControlProperties = nullptr;

    /*! init UI
     */
    void initGUI();

    /*! delete current properties UI
    */
    void deinitProperties();
};

#endif // __PROPERTIES_EDITOR__

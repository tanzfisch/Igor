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
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef __USERCONTROLMODEL__
#define __USERCONTROLMODEL__

#include "UserControlNode.h"

class UserControlModel : public UserControlNode
{
public:
    /*! user control

    \param nodeID the node to init with
    \param parent parent widget
    */
    UserControlModel(iNodeID nodeID, const iWidgetPtr parent = nullptr);

    /*! does nothing
     */
    ~UserControlModel() = default;

    /*! update ui with node data
     */
    void update() override;

    /*! init ui
     */
    void init() override;

private:
    iWidgetGridLayout *_grid = nullptr;

    iWidgetLabel *_labelFilename = nullptr;
    iWidgetLineTextEdit *_textFilename = nullptr;

};

#endif
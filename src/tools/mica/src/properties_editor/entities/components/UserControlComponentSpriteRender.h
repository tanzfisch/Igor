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

#ifndef USERCONTROL_COMPONENT_SPRITE_RENDER_H
#define USERCONTROL_COMPONENT_SPRITE_RENDER_H

#include "UserControlComponent.h"

class UserControlComponentSpriteRender : public UserControlComponent
{
public:
    /*! init user control

    \param scene the given scene
    \param entity the given entity
    \param parent the optional parent widget
    */
    UserControlComponentSpriteRender(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent = nullptr);

    /*! does nothing
     */
    virtual ~UserControlComponentSpriteRender() = default;

    /*! init ui
     */
    void init() override;

    /*! update ui with node data
     */
    void update() override;

    /*! update entity
     */
    void updateComponent() override;

private:
    /*! called after values changed
     */
    void onValueChanged(iWidgetPtr source);
};

#endif // USERCONTROL_COMPONENT_SPRITE_RENDER_H
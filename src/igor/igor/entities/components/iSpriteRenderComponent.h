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

#ifndef IGOR_SPRITE_RENDER_COMPONENT_H
#define IGOR_SPRITE_RENDER_COMPONENT_H

#include <igor/entities/iEntity.h>

namespace igor
{

    /*! sprite render component
     */
    class iSpriteRenderComponent : public iEntityComponent
    {

    public:
        /*! sprite render mode
         */
        enum class iRenderMode
        {
            Simple,
            Tiled
        };

        /*! ctor

        \param name the name of this component
        */
        iSpriteRenderComponent(iSpritePtr sprite, const iaVector2d &size = iaVector2d(1.0, 1.0), const iaColor4f &color = iaColor4f::white, int32 zIndex = 0, iRenderMode renderMode = iRenderMode::Simple, const iaString &name = "sprite render");

        /*! sprite to render
         */
        iSpritePtr _sprite;

        /*! render size
         */
        iaVector2d _size = {1.0, 1.0};

        /*! color to render sprite with
         */
        iaColor4f _color = iaColor4f::white;

        /*! specifies the render order within a layer
         */
        int32 _zIndex = 0;

        /*! sprite render mode
         */
        iRenderMode _renderMode = iRenderMode::Simple;

        /*! index of the sprite frame to render
         */
        uint32 _frameIndex = 0;
    };

}

#endif //  IGOR_2D_BODY_COMPONENT_H
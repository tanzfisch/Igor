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

#ifndef IGOR_SPRITE_RENDER_COMPONENT_H
#define IGOR_SPRITE_RENDER_COMPONENT_H

#include <igor/entities/iEntity.h>
#include <igor/resources/sprite/iSprite.h>

namespace igor
{

    /*! sprite render component
     */
    class iSpriteRenderComponent : public iEntityComponent
    {

    public:
        /*! default ctor
         */
        iSpriteRenderComponent() = default;

        /*! creates instance of this component type
         */
        static iEntityComponent *createInstance();

        /*! \returns type name of component
         */
        static const iaString &getTypeName();

        /*! sprite render mode
         */
        enum class iRenderMode
        {
            Simple,
            Tiled
        };

        /*! ctor
         */
        iSpriteRenderComponent(iSpritePtr sprite, const iaVector2d &size = iaVector2d(1.0, 1.0), const iaColor4f &color = iaColor4f::white, int32 zIndex = 0, iRenderMode renderMode = iRenderMode::Simple);

        /*! \returns a set of info strings
         */
        std::vector<iaString> getInfo() const override;

        /*! sets the sprite

        \param sprite the sprite to use
        */
        void setSprite(iSpritePtr sprite);

        /*! \returns the sprite
         */
        IGOR_INLINE iSpritePtr getSprite() const
        {
            return _sprite;
        }

        /*! sets the size

        \param size the new size
        */
        void setSize(const iaVector2d &size);

        /*! \returns the size
         */
        IGOR_INLINE const iaVector2d &getSize() const
        {
            return _size;
        }

        /*! sets the color

        \param color the new color
        */
        void setColor(const iaColor4f &color);

        /*! \returns the color
         */
        IGOR_INLINE const iaColor4f &getColor() const
        {
            return _color;
        }

        /*! sets z index

        \param zIndex the z index
        */
        void setZIndex(int32 zIndex);

        /*! \returns the z index
         */
        IGOR_INLINE int32 getZIndex() const
        {
            return _zIndex;
        }

        /*! sets render mode

        \param mode the render mode to set
        */
        void setRenderMode(iSpriteRenderComponent::iRenderMode mode);

        /*! \returns the render mode
         */
        IGOR_INLINE iSpriteRenderComponent::iRenderMode getRenderMode() const
        {
            return _renderMode;
        }

        /*! sets frame index

        \param frameIndex the frame index
        */
        void setFrameIndex(int32 frameIndex);

        /*! \returns the frame index
         */
        IGOR_INLINE int32 getFrameIndex() const
        {
            return _frameIndex;
        }

    private:
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

        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override;
    };

}

#endif //  IGOR_2D_BODY_COMPONENT_H
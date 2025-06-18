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

#ifndef IGOR_USERCONTROL_SPRITE_H
#define IGOR_USERCONTROL_SPRITE_H

#include <igor/ui/user_controls/iUserControl.h>

#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetPicture.h>

namespace igor
{

    /*! User control sprite
     */
    class IGOR_API iUserControlSprite : public iUserControl
    {

    public:
        /*! ctor initializes member variables

        \param parent the optional parent
        */
        iUserControlSprite(const iWidgetPtr parent = nullptr);

        /*! clean up
         */
        ~iUserControlSprite();

        /*! sets the sprite id

        \param spriteID the given sprite id
        */
        void setID(iResourceID spriteID);

        /*! \returns sprite id
         */
        iResourceID getID() const;

    private:
        /*! sprite id
         */
        iResourceID _spriteID;

        /*! label sprite id
         */
        iWidgetLabelPtr _labelID;

        /*! sprite alias
         */
        iWidgetLabelPtr _labelAlias;

        /*! label texture id
         */
        iWidgetLabelPtr _labelTextureID;

        /*! texture alias
         */
        iWidgetLabelPtr _labelTextureAlias;

        /*! displays the texture
         */
        iWidgetPicturePtr _picture;

        /*! initialize gui elements
         */
        void initGUI();

        /*! drag move handle

        \param drag the drag data
        \param mousePos the current mouse pos
        */
        void onDragMove(iDrag &drag, const iaVector2f &mousePos) override;

        /*! drop handle

        \param drag the drag data
        \param mousePos the current mouse pos
        */
        void onDrop(const iDrag &drag, const iaVector2f &mousePos) override;
    };

    /*! user control sprite pointer definition
     */
    typedef iUserControlSprite *iUserControlSpritePtr;

} // namespace igor

#endif // IGOR_USERCONTROL_SPRITE_H
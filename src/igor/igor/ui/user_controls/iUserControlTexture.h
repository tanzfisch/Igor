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

#ifndef IGOR_USERCONTROL_TEXTURE_H
#define IGOR_USERCONTROL_TEXTURE_H

#include <igor/ui/user_controls/iUserControl.h>

#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetPicture.h>
#include <igor/resources/texture/iTexture.h>

namespace igor
{

    /*! User control texture chooser
     */
    class IGOR_API iUserControlTexture : public iUserControl
    {

    public:
        /*! ctor initializes member variables

        \param parent the optional parent
        */
        iUserControlTexture(const iWidgetPtr parent = nullptr);

        /*! clean up
         */
        ~iUserControlTexture();

        /*! sets the texture id

        \param textureID the given texture id
        */
        void setID(iResourceID textureID);

        /*! \returns texture id
         */
        iResourceID getID() const;

    private:
        /*! texture id
         */
        iResourceID _textureID;

        /*! label texture id
         */
        iWidgetLabelPtr _labelID;

        /*! texture alias
         */
        iWidgetLabelPtr _labelAlias;

        /*! displays the selected texture
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

    /*! user control texture chooser pointer definition
     */
    typedef iUserControlTexture *iUserControlTexturePtr;

} // namespace igor

#endif // IGOR_USERCONTROL_TEXTURE_H
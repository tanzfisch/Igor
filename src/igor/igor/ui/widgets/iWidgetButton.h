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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __IGOR_WIDGETBUTTON_H__
#define __IGOR_WIDGETBUTTON_H__

#include <igor/ui/widgets/iWidget.h>
#include <igor/resources/texture/iTexture.h>

#include <iaux/data/iaString.h>
using namespace iaux;

#include <memory>

namespace igor
{

    /*! button widget
    */
    class Igor_API iWidgetButton : public iWidget
    {

    public:
        /*! ctor initializes member variables

		\param parent the optional parent
		*/
        iWidgetButton(const iWidgetPtr parent = nullptr);

        /*! release texture
		*/
        virtual ~iWidgetButton();

        /*! sets text of the widget

        \param text new text to set
        */
        void setText(const iaString &text);

        /*! \returns text of the widget
        */
        const iaString &getText() const;

        /*! sets horrizotnal text aligment

        \param align the horizontal allignment of the text
        */
        void setHorizontalTextAlignment(iHorizontalAlignment align);

        /*! \returns horizontal text alignment
        */
        iHorizontalAlignment getHorizontalTextAlignment() const;

        /*! sets vertical text aligment

        \param valign the vertical allignment of the text
        */
        void setVerticalTextAlignment(iVerticalAlignment valign);

        /*! \returns vertical text alignment
        */
        iVerticalAlignment getVerticalTextAlignment() const;

        /*! sets the texture to use for the button

        \param texturePath path to texture
        */
        void setTexture(const iaString &texturePath);

        /*! \returns texture path
        */
        const iaString &getTexture() const;

    protected:
        /*! the horizontal alignment
        */
        iHorizontalAlignment _horizontalTextAlignment = iHorizontalAlignment::Center;

        /*! the vertical alignment
        */
        iVerticalAlignment _verticalTextAlignment = iVerticalAlignment::Center;

        /*! current text of the button
        */
        iaString _text;

        /*! the buttons texture
        */
        iTexturePtr _texture;

        /*! texture path
        */
        iaString _texturePath;

        /*! updates size based on it's content
        */
        void calcMinSize();

        /*! draws the button
        */
        void draw();
    };

    /*! widget button pointer definition
	*/
    typedef iWidgetButton *iWidgetButtonPtr;
} // namespace igor

#endif // __IGOR_WIDGETBUTTON_H__

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

#ifndef IGOR_WIDGETBUTTON_H
#define IGOR_WIDGETBUTTON_H

#include <igor/ui/widgets/iWidget.h>
#include <igor/ui/actions/iAction.h>
#include <igor/resources/texture/iTexture.h>

#include <memory>

namespace igor
{

    /*! button widget
    */
    class IGOR_API iWidgetButton : public iWidget
    {

    public:
        /*! ctor initializes member variables

		\param parent the optional parent
		*/
        iWidgetButton(const iWidgetPtr parent = nullptr);

        /*! does nothing
		*/
        virtual ~iWidgetButton() = default;

        /*! sets the action this widget is associated with

        \param action the action to be set
        \param context the optional action context
        */
        void setAction(const iActionPtr action, const iActionContextPtr context = nullptr);

        /*! sets the action this widget is associated with by action name

        \param actionName the action to be set
        \param context the optional action context
        */
        void setAction(const iaString &actionName, const iActionContextPtr context);

        /*! \returns action that is associated with this widget
        */
        iActionPtr getAction() const;

        /*! \returns action context
        */
        iActionContextPtr getActionContext() const;

        /*! sets text of the widget

        \param text new text to set
        */
        void setText(const iaString &text);

        /*! \returns text of the widget
        */
        const iaString &getText() const;

        /*! sets horizontal text alignment

        \param align the horizontal alignment of the text
        */
        void setHorizontalTextAlignment(iHorizontalAlignment align);

        /*! \returns horizontal text alignment
        */
        iHorizontalAlignment getHorizontalTextAlignment() const;

        /*! sets vertical text alignment

        \param valign the vertical alignment of the text
        */
        void setVerticalTextAlignment(iVerticalAlignment valign);

        /*! \returns vertical text alignment
        */
        iVerticalAlignment getVerticalTextAlignment() const;

        /*! sets the background texture to use for the whole button by name

        \param textureAlias alias or id to texture
        */
        void setBackgroundTexture(const iaString &textureAlias);

        /*! sets the background texture to use for the whole button

        \param texture the texture
        */
        void setBackgroundTexture(iTexturePtr texture);

        /*! \returns background texture
        */
        iTexturePtr getBackgroundTexture() const;

        /*! sets the icon texture by name

        the icon is rendered in the foreground.
        if there is no text then centred
        if there is text then on the opposite side of the text        

        \param iconAlias alias or id to icon texture
        */
        void setIcon(const iaString &iconAlias);

        /*! sets the icon texture

        the icon is rendered in the foreground.
        if there is no text then centred
        if there is text then on the opposite side of the text        

        \param texture the texture to use
        */
        void setIcon(iTexturePtr texture);

        /*! \returns icon
        */
        iTexturePtr getIcon() const;

        /*! sets the button checkable

        \param checkable if true button is checkable
        */
        void setCheckable(bool checkable);

        /*! \returns true if button is checkable
        */
        bool isCheckable() const;

        /*! sets button checked

        \param check if true button is checked
        */
        void setChecked(bool check);

        /*! \returns true if button is checked
        */
        bool isChecked() const;

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

        /*! the background texture
        */
        iTexturePtr _texture;

        /*! the icon texture
        */
        iTexturePtr _iconTexture;

        /*! the action
        */
        iActionPtr _action;

        /*! the action context
        */
        iActionContextPtr _actionContext;        

        /*! if true button is checkable
        */
        bool _checkable = false;

        /*! if true button is checked
        */
        bool _checked = false;

        /*! internal click handler
        */
        void onInternalClick(const iWidgetPtr source);

        /*! updates size based on it's content
        */
        void calcMinSize() override;

        /*! draws the widget
         */ 
        void draw() override;
    };

    /*! widget button pointer definition
	*/
    typedef iWidgetButton *iWidgetButtonPtr;
} // namespace igor

#endif // IGOR_WIDGETBUTTON_H
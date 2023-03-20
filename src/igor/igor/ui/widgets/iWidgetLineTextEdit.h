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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_WIDGETLINETEXTEDIT__
#define __IGOR_WIDGETLINETEXTEDIT__

#include <igor/ui/widgets/iWidget.h>

namespace igor
{

    /*! single line text edit widget
	*/
    class IGOR_API iWidgetLineTextEdit : public iWidget
    {

    public:
        /*! initializes member variables

		\param parent optional parent
		*/
        iWidgetLineTextEdit(const iWidgetPtr parent = nullptr);

        /*! deinitializes member variables
		*/
        virtual ~iWidgetLineTextEdit() = default;

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

        /*! sets the text max length in characters

		\param chars number of characters
		*/
        void setMaxTextLength(int32 chars);

        /*! \returns the max length of text in characters
		*/
        int32 getMaxTextLength();

        /*! sets text edit write protected

		\param writeProtected true: text edit will be write protected
		*/
        void setWriteProtected(bool writeProtected);

        /*! \retruns if text edit is write protected
		*/
        bool isWriteProtected();

        /*! set up that the change event will only triggered if you press enter
		*/
        void setChangeEventAtOnce();

        /*! set up that the change event will be triggered for every change in the text field
		*/
        void setChangeEventOnEnterAndLostFocus();

        /*! sets the text

		\param text the text to be set
		*/
        void setText(const iaString &text);

        /*! \returns the text
		*/
        const iaString &getText() const;

        /*! sets cursor pos

		cursor pos will be clamped by current text size

		\param cursorPos the new cursor pos
		*/
        void setCursorPos(uint64 cursorPos);

        /* \returns current cursor pos
		*/
        uint64 getCursorPos() const;

        /*! increase cursor pos by one
		*/
        void incCursorPos();

        /*! decrease cursor pos by one
		*/
        void decCursorPos();

    protected:
        /*! the horizontal alignment
		*/
        iHorizontalAlignment _horizontalTextAlignment = iHorizontalAlignment::Left;

        /*! the vertical alignment
		*/
        iVerticalAlignment _verticalTextAlignment = iVerticalAlignment::Center;

        /*! flat to handle change event trigger behaviour
		*/
        bool _triggerChangeAtOnce = false;

        /*! the text
		*/
        iaString _text;

        /*! backup text
		*/
        iaString _textBackup;

        /*! handles incomming acsii codes from keyboard

		\param c the incomming character from keyboard
		*/
        virtual bool handleASCII(uint8 c) override;

        /*! handles incomming released key events

		\param key the released key
		*/
        virtual bool handleKeyDown(iKeyCode key) override;

        /*! handles incomming released key events

		\param key the released key
		*/
        virtual bool handleKeyUp(iKeyCode key) override;

        /*! handles gained kayboard focus
		*/
        virtual void handleGainedKeyboardFocus() override;

        /*! handles lost keyboard focus
		*/
        virtual void handleLostKeyboardFocus() override;

        /*! triggers a change event if the content has changed
		*/
        void handleChanges();

    private:
        /*! position of cursor within the text in characters
		*/
        uint64 _cursorPos = 0;

        /*! cursor position in pixel
		*/
        int64 _cursorPosPix = 0;

        /*! scroll offset of text display in pixel
		*/
        int64 _scrollOffset = 0;

        /*! max text lenght in characters
		*/
        int32 _maxTextLenght = 20;

        /*! if true text field is write protected
		*/
        bool _writeProtected = false;

        /*! updates size based on it's content
		*/
        void calcMinSize() override;

        /*! draws the text edit widget
		*/
        void draw();

        /*! updates the cursor position and scroll offset
		*/
        void updateMetrics();
    };

} // namespace igor

#endif // __IGOR_WIDGETLINETEXTEDIT__

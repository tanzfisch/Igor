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
// (c) Copyright 2014-2015 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __iWIDGETTEXTEDIT__
#define __iWIDGETTEXTEDIT__

#include <iWidget.h>

namespace Igor
{

    /*! text edit widget

    Example:
    \ref Widgets/src/WidgetsExample.cpp "Widgets usage example"

    */
	class Igor_API iWidgetTextEdit : public iWidget
	{

        /*! needs to be friend because it's the factory that creates this widget
        */
        friend class iWidgetManager;

	public:

        /*! stes width of widget

        \param width new width of widget
        */
		void setWidth(int32 width);

        /*! stes height of widget

        \param height new height of widget
        */
        void setHeight(int32 height);

        /*! sets horrizotnal text aligment

        \param align the horrizontal allignment of the text
        */
		void setHorrizontalTextAlignment(iHorrizontalAlignment align);

        /*! \returns horrizontal text alignment
        */
        iHorrizontalAlignment getHorrizontalTextAlignment() const;

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
		void setChangeEventOnEnterAndLosFocus();

        /*! sets the text

        \param text the text to be set
        */
		void setText(const iaString& text);

        /*! \returns the text
        */
		const iaString& getText() const;

	protected:

        /*! the horrizontal alignment
        */
		iHorrizontalAlignment _horrizontalTextAlignment = iHorrizontalAlignment::Left;

        /*! the vertical alignment
        */
		iVerticalAlignment _verticalTextAlignment = iVerticalAlignment::Center;

        /*! flat to handle change event trigger behaviour
        */
		bool _triggerChangeAtOnce = false;

        /*! the text
        */
		iaString _text;

        /*! max text lenght in characters
        */
		int32 _maxTextLenght = 8;

        /*! if true text field is write protected
        */
        bool _writeProtected = false;

        /*! updates widget
        */
		void update();

        /*! draws widget
        */
		void draw();

	private:

        /*! handles incomming acsii codes from keyboard

        \param c the incomming character from keyboard
        */
        bool handleASCII(uint8 c);

        /*! initializes member variables
        */
		iWidgetTextEdit();

        /*! deinitializes member variables
        */
		virtual ~iWidgetTextEdit() = default;
	};

}

#endif
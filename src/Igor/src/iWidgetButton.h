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

#ifndef __iWIDGETBUTTON__
#define __iWIDGETBUTTON__

#include <iWidget.h>

#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    class iTexture;

    /*! button widget

    Example:
    \ref Widgets/src/WidgetsExample.cpp "Widgets usage example"

    */
	class Igor_API iWidgetButton : public iWidget
	{

        /*! needs to be friend because it's the factory that creates this widget
        */
		friend class iWidgetManager;

	public:

        /*! sets the width of the widget

        \param width new width
        */
		void setWidth(int32 width);

        /*! sets the height of the widget

        \param height new height
        */
        void setHeight(int32 height);

        /*! sets text of the widget

        \param text new text to set
        */
		void setText(const iaString& text);

        /*! \returns text of the widget
        */
		const iaString& getText() const;

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

        /*! sets the texture to use for the button

        \param texturePath path to texture
        */
        void setTexture(const iaString& texturePath);

        /*! \returns texture path
        */
        const iaString& getTexture() const;

	protected:

        /*! the horrizontal alignment
        */
        iHorrizontalAlignment _horrizontalTextAlignment = iHorrizontalAlignment::Center;

        /*! the vertical alignment
        */
        iVerticalAlignment _verticalTextAlignment = iVerticalAlignment::Center;

        /*! current text of the button
        */
        iaString _text;

        /*! the buttons texture
        */
        shared_ptr<iTexture> _texture = nullptr;

        /*! texture path
        */
        iaString _texturePath;

        /*! updates dimensions of button
        */
		void update();

        /*! draws the button
        */
        void draw();

	private:

        /*! ctor initializes member variables
        */
		iWidgetButton();

        /*! release texture
        */
		virtual ~iWidgetButton();
	};
}

#endif
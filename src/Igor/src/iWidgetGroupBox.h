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
// (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __iWIDGETGROUPBOX__
#define __iWIDGETGROUPBOX__

#include <iWidget.h>

#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    class iWidgetUserControl;

    /*! group box widget

    Example:
    \ref Widgets/src/WidgetsExample.cpp "Widgets usage example"

    */
	class Igor_API iWidgetGroupBox : public iWidget
	{

        /*! needs to be friend because it's the factory that creates this widget
        */
        friend class iWidgetManager;

	public:

        /*! sets text of widget

        \param text new text
        */
		void setText(const iaString& text);

        /*! \returns text of widget
        */
        const iaString& getText() const;

        /*! sets boder size (something like a margin)

        \param border size of border
        */
		void setBorder(int32 border);

        /*! \returns border size of widget
        */
		int32 getBorder();

    private:

        /*! border size
        */
		int32 _border = 4;

        /*! group box text
        */
		iaString _text = "...";

		/*! draws the button

		\param parentPosX parent absolute horrizontal position
		\param parentPosY parent absolute vertical position
		*/
		void draw(int32 parentPosX, int32 parentPosY);

        /*! updates widget
        */
        void update();

        /*! ctor initializes member variables
        */
		iWidgetGroupBox();

        /*! does nothing
        */
		~iWidgetGroupBox() = default;

	};
}

#endif

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

#ifndef __iWIDGETDIALOG__
#define __iWIDGETDIALOG__

#include <iWidget.h>

namespace Igor
{

    class iWidgetUserControl;

    /*! dialog widget

    Example:
    \ref Widgets/src/WidgetsExample.cpp "Widgets usage example"

    \todo need more than one apperance type of dialogs e.g. for iWidgetSelectBox

    */
    class Igor_API iWidgetDialog : public iWidget
    {

        /*! needs to be friend because it's the factory that creates this widget
        */
        friend class iWidgetManager;

    public:

        /*! set horrizontal position of dialog and horrizontal alignment to absolute

        \param x horrizontal position
        */
        void setX(int32 x);

        /*! set vertical position of dialog and vertical alignment to absolute

        \param y vertical position
        */
        void setY(int32 y);

        /*! set size of border

        \param border border size
        */
        void setBorder(int32 border);

        /*! \retruns border size
        */
        int32 getBorder();

    private:

        /*! horrizontal position relative to parent if horrizontal alignment is absolute
        */
        int32 _offsetX = 0;

        /*! vertical position relative to parent if horrizontal alignment is absolute
        */
        int32 _offsetY = 0;

        /*! size of border
        */
        int32 _border = 1;

        /*! update the widget
        */
        void update();

		/*! draws the button

		\param parentPosX parent absolute horrizontal position
		\param parentPosY parent absolute vertical position
		*/
		void draw(int32 parentPosX, int32 parentPosY);

        /*! ctor initializes member variables and registers mouse events
        */
        iWidgetDialog();

        /*! dtor unregisters mouse events
        */
        virtual ~iWidgetDialog();

    };
}

#endif

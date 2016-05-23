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

#ifndef __iWIDGETUSERDIALOG__
#define __iWIDGETUSERDIALOG__

#include <iWidget.h>

namespace Igor
{

    class iWidgetDialog;

    /*! user dialog base class
    */
    class Igor_API iWidgetUserDialog
    {

    public:

        /*! set horrizontal position of dialog and horrizontal alignment to absolute

        \param x horrizontal position
        */
        void setX(int32 x);

        /*! set vertical position of dialog and vertical alignment to absolute

        \param y vertical position
        */
        void setY(int32 y);

		/*! sets the configured width

		\param width configured width
		*/
		void setWidth(int32 width);

		/*! sets the configured height

		\param width configured height
		*/
		void setHeight(int32 height);

        /*! registers delegate to click event (click is left mouse button)

        \param clickDelegate the delegate to register
        */
        void registerOnClickEvent(iClickDelegate clickDelegate);

        /*! registers delegate to double click event

        \param doubleClickDelegate the delegate to register
        */
        void registerOnDoubleClickEvent(iDoubleClickDelegate doubleClickDelegate);

        /*! registers delegate to mouse over event

        \param mouseOverDelegate the delegate to register
        */
        void registerOnMouseOverEvent(iMouseOverDelegate mouseOverDelegate);

        /*! registers delegate to mouse off event (oposite of mouse over)

        \param mouseOffDelegate the delegate to register
        */
        void registerOnMouseOffEvent(iMouseOffDelegate mouseOffDelegate);

        /*! registers delegate to content change event

        \param changeDelegate the delegate to register
        */
        void registerOnChangeEvent(iChangeDelegate changeDelegate);

        /*! registers delegate to keyboard focus event

        \param focusDelegate the delegate to register
        */
        void registerOnFocusEvent(iFocusDelegate focusDelegate);

        /*! unregisters delegate from click event (click is left mouse button)

        \param clickDelegate the delegate to unregister
        */
        void unregisterOnClickEvent(iClickDelegate clickDelegate);

        /*! unregisters delegate from double click event

        \param doubleClickDelegate the delegate to unregister
        */
        void unregisterOnDoubleClickEvent(iDoubleClickDelegate doubleClickDelegate);

        /*! unregisters delegate mouse over event

        \param mouseOverDelegate the delegate to unregister
        */
        void unregisterOnMouseOverEvent(iMouseOverDelegate mouseOverDelegate);

        /*! unregisters delegate mouse off event

        \param mouseOffDelegate the delegate to unregister
        */
        void unregisterOnMouseOffEvent(iMouseOffDelegate mouseOffDelegate);

        /*! unregisters delegate content change event

        \param changeDelegate the delegate to unregister
        */
        void unregisterOnChangeEvent(iChangeDelegate changeDelegate);

        /*! unregisters delegate keyboard focus event

        \param focusDelegate the delegate to unregister
        */
        void unregisterOnFocusEvent(iFocusDelegate focusDelegate);

        /*! \returns true if visible
        */
        bool isVisible();

        /*! \returns true if active aka enabled
        */
        bool isActive();

        /*! set widget visible

        \parma visible boolean to set is visible or invisible
        */
        void setVisible(bool visible = true);

        /*! set widget active aka enabled

        \param active boolean to set is active or inactive
        */
        void setActive(bool active = true);

        /*! creating the dialog
        */
        iWidgetUserDialog();

        /*! destroying the dialog
        */
        ~iWidgetUserDialog();

    protected:

        /*! \returns root widget that is supposed to be integrated in a bigger form
        */
        iWidgetDialog* getDialog();

    private:

        /*! the dialog
        */
        iWidgetDialog* _dialog = nullptr;

    };

}

#endif


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

#ifndef __iDIALOGMENU__
#define __iDIALOGMENU__

#include <iWidgetUserDialog.h>

#include <iaEvent.h>
#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    class iWidgetGrid;
    class iWidgetLabel;

    iaEVENT(iDialogMenuCloseEvent, iDialogMenuCloseDelegate, void, (int32 value), (value));

    /*! the message box
    */
    class Igor_API iDialogMenu : public iWidgetUserDialog
    {
    public:

        /*! does nothing
        */
        iDialogMenu() = default;

        /*! deinitializes gui
        */
        ~iDialogMenu();

		/*! opens dialog

		\param texts the texts to put in the selection list
		\param closeDelegate delegate for closing event
		*/
        void show(vector<iaString>& texts, iDialogMenuCloseDelegate closeDelegate);

    private:

        /*! the close event
        */
        iDialogMenuCloseEvent _selectBoxCloseEvent;

        /*! the return value of the selection box

        -1 stands for cancel
        */
        int32 _returnValue = -1;

        /*! over all grid
        */
        iWidgetGrid* _grid = nullptr;
        
        /*! all widgets
        */
        vector<iWidget*> _allWidgets;

        /*! handles change event

        \param source the source of the event (should be the grid)
        */
        void onChange(iWidget* source);

		/*! handle mouse off event

		\param source the source of the event (should be the dialog)
		*/
		void onMouseOff(iWidget* source);

        /*! closes the dialog and sends closed event

        will be triggered by any button
        */
        void close();

        /*! initializes the gui

        \param message the message text
        \param buttons the button configuration
        */
        void initGUI(vector<iaString>& texts);

        /*! deinitializes the gui elements
        */
        void deinitGUI();

    };

}

#endif
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

#include <iWidgetDialog.h>

#include <iaEvent.h>
#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    class iWidgetGrid;
    class iWidgetLabel;

    iaEVENT(iDialogMenuCloseEvent, iDialogMenuCloseDelegate, void, (int32 index), (index));

    /*! pull down menu
    */
    class Igor_API iDialogMenu : public iWidgetDialog
    {

        friend class iWidgetManager;

    public:

		/*! opens dialog

        leave the pictures list empty if you don't want pictures or call the alternative implementation of show

		\param texts the texts to put in the selection list
        \param pictures paths to textures used as icons next to the text (optional)
		\param closeDelegate delegate for closing event
		*/
        void show(vector<iaString>& texts, vector<iaString>& pictures, iDialogMenuCloseDelegate closeDelegate);

        /*! opens dialog

        \param texts the texts to put in the selection list
        \param closeDelegate delegate for closing event
        */
        void show(vector<iaString>& texts, iDialogMenuCloseDelegate closeDelegate);

        /*! sets the height of an entry

        it's interpreted as max height and width for pictures

        \param height the entry height in pixel
        */
        void setEntryHeight(int32 height);

        /*! \returns height of an entry
        */
        int32 getEntryHeight() const;

    private:

        /*! menu entry height in pixel
        */
        int32 _entryHeight = 20;

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

		/*! handle mouse off click event

        \param source the source of that event
		*/
        void onMouseOffClick(iWidget* source);

        /*! closes the dialog and sends closed event

        will be triggered by any button
        */
        void close();

        /*! initializes the gui

        \param texts the texts for the menu
        */
        void initGUI(vector<iaString>& texts);

        /*! initializes the gui

        \param texts the texts for the menu
        \param pictures the pictures for the menu
        */
        void initGUI(vector<iaString>& texts, vector<iaString>& pictures);

        /*! deinitializes the gui elements
        */
        void deinitGUI();

        /*! does nothing
        */
        iDialogMenu() = default;

        /*! deinitializes gui
        */
        ~iDialogMenu();

        /*! creates instance of this widget type
        */
        static iWidgetDialog* createInstance();

    };

}

#endif
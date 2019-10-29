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
// (c) Copyright 2012-2019 by Martin Loga
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

#ifndef __iDIALOG_INDEX_MENU__
#define __iDIALOG_INDEX_MENU__

#include <iDialog.h>

#include <iaEvent.h>
#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

	iaEVENT(iDialogIndexMenuCloseEvent, iDialogIndexMenuCloseDelegate, void, (int32 index), (index));

	/*! pull down menu
	*/
	class Igor_API iDialogIndexMenu : public iDialog
	{

	public:

		/*! does nothing
		*/
		iDialogIndexMenu() = default;

		/*! deinitializes gui
		*/
		virtual ~iDialogIndexMenu() = default;

        /*! \returns the widgets type
        */
        virtual iWidgetType getWidgetType() const override;

		/*! opens dialog

		leave the pictures list empty if you don't want pictures or call the alternative implementation of show

		\param texts the texts to put in the selection list
		\param pictures paths to textures used as icons next to the text (optional)
		\param dialogCloseDelegate delegate for closing event
		*/
        void open(iDialogCloseDelegate dialogCloseDelegate, std::vector<iaString>& texts, std::vector<iaString>& pictures = std::vector<iaString>());

        /*! \returns selected menu entry index

        returns -1 if nothing was selected
        */
        int32 getSelectionIndex() const;

	private:
        
		/*! the close event
		*/
		iDialogIndexMenuCloseEvent _selectBoxCloseEvent;

		/*! the return value of the selection box

		-1 stands for cancel
		*/
		int32 _returnValue = -1;
		
		/*! handles change event

		\param source the source of the event (should be the grid)
		*/
		void onChange(const iWidgetPtr source);

		/*! handle mouse off click event

		\param source the source of that event
		*/
		void onMouseOffClick(const iWidgetPtr source);

		/*! initializes the gui

		\param texts the texts for the menu
		*/
		void initGUI(std::vector<iaString>& texts);

		/*! initializes the gui

		\param texts the texts for the menu
		\param pictures the pictures for the menu
		*/
		void initGUI(std::vector<iaString>& texts, std::vector<iaString>& pictures);

	};

    /*! dialog menu pointer definition
    */
    typedef iDialogIndexMenu* iDialogIndexMenuPtr;

}

#endif
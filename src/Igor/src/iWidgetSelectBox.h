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

#ifndef __iWIDGETSELECTBOX__
#define __iWIDGETSELECTBOX__

#include <iWidget.h>

namespace Igor
{

    class iDialog;
    typedef iDialog* iDialogPtr;
	class iTextureFont;
	class iWidgetManager;
	class iDialogIndexMenu;

	/*! select box widget
	*/
	class Igor_API iWidgetSelectBox : public iWidget
	{

	public:

		/*! ctro initializes member variables

		\param parent optional parent
		*/
		iWidgetSelectBox(const iWidgetPtr parent = nullptr);

		/*! clean up
		*/
		virtual ~iWidgetSelectBox();

		/*! adds entry to select box

		\param entryText the entry text
		\param userData optional pointer to user data
		*/
		void addSelectionEntry(const iaString& entryText, void* userData = nullptr);

		/*! clears all entries

		Attention! if you used user data you have to delete it your self
		*/
		void clear();

		/*! sets selection by index
		*/
		void setSelection(uint32 index);

		/*! \returns ammount of select box entries
		*/
		uint32 getSelectionEntryCount() const;

		/*! \returns index of selected entry
		*/
		uint32 getSelectedIndex() const;

		/*! \returns text of selected entry
		*/
		iaString getSelectedValue() const;

		/*! \returns user data pointer of selected entry
		*/
		void* getSelectedUserData() const;

	private:

		/*! list of selct box entries
		*/
		std::vector<std::pair<iaString, void*>> _entries;

		/*! current selected index

		-1 stand for no selection
		*/
		int32 _currentSelection = -1;

		/*! true: if the mouse cursor is over the button
		*/
		bool _mouseOver = false;

		/*! true: if the select box is unfoldet
		*/
		bool _openSelectBox = false;

		/*! select box dialog used for the unfold effect
		*/
		iDialogIndexMenu* _selectBox = nullptr;

		/*! appearance state of the button
		*/
		iWidgetState _buttonAppearanceState = iWidgetState::Standby;

		/*! triggered when selection box closed

        \param dialog source of this event
		*/
		void onSelectBoxClosed(iDialogPtr dialog);

		/*! handles incomming mouse wheel event

		\param d mouse wheel delta
		\returns true: if event was consumed and therefore ignored by the parent
		*/
		bool handleMouseWheel(int32 d);

		/*! handles incomming mouse key down events

		\param key the key that was pressed
		\returns true: if event was consumed and therefore will be ignored by the parent
		*/
		bool handleMouseKeyDown(iKeyCode key);

		/*! handles mouse key up events

		\param key the key that was pressed
		\returns true: if event was consumed and therefore will be ignored by the parent
		*/
		bool handleMouseKeyUp(iKeyCode key);

		/*! handles incomming mouse move events

		\param pos mouse position
		*/
		void handleMouseMove(const iaVector2i& pos);

		/*! updates size based on it's content
		*/
		void calcMinSize();

		/*! renders widget
		*/
		void draw();

	};
}

#endif


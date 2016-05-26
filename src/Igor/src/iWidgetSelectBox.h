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

#ifndef __iWIDGETSELECTBOX__
#define __iWIDGETSELECTBOX__

#include <iWidget.h>

namespace Igor
{

	class iTextureFont;
	class iWidgetManager;
    class iSelectBox;

	/*!

	\todo replace stringstream

    Example:
    \ref Widgets/src/WidgetsExample.cpp "Widgets usage example"

	*/
	class Igor_API iWidgetSelectBox : public iWidget
	{

        /*! needs to be friend because it's the factory that creates this widget
        */
        friend class iWidgetManager;

	public:

        void clear();
        void appendEntry(const iaString& entryText, void* userData = nullptr);
		
		void setSelection(uint32 key);

		uint32 getSelectedKey() const;
		iaString getSelectedValue() const;
        void* getSelectedUserData() const;

	private:

        vector<pair<iaString, void*>> _entries;

        int32 _key = -1;

        iRectanglef _buttonRectangle;

        bool _mouseOverButton = false;

        bool _openSelectBox = false;

        iSelectBox* _selectBox = nullptr;

        iWidgetAppearanceState _buttonAppearanceState = iWidgetAppearanceState::Standby;

        void onSelectionChanged(int32 value);

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

        \param x horrizontal mouse position
        \param y horrizontal mouse position
        */
        void handleMouseMove(int32 x, int32 y);

        /*! updates widget
        */
		void update();

        /*! renders widget
        */
        void draw(int32 parentPosX, int32 parentPosY);

        /*! ctro initializes member variables
        */
		iWidgetSelectBox();

        /*! clean up
        */
		virtual ~iWidgetSelectBox();
	};
}

#endif


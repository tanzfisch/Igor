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

#ifndef __iDIALOGMANAGER__
#define __iDIALOGMANAGER__

#include <iDefines.h>
#include <iaSingleton.h>

#include <iWidget.h>

#include <vector>
#include <map>
using namespace std;

namespace Igor
{

	class iWidgetBaseTheme;
    class iWidgetDialog;

    /*! manages the widgets in use

    is also a factory that creates and destroys all the widgets

    Example:
    \ref Widgets/src/WidgetsExample.cpp "Widgets usage example"

    */
	class Igor_API iWidgetManager : public iaSingleton<iWidgetManager>
	{

        friend class iaSingleton<iWidgetManager>;

	public:
		
        /*! creates a widget of given type

        \param type type of widget to create
        */
		iWidget* createWidget(iWidgetType type);        

        /*! destroyes widget

        \param widget the widget to destroy
        */
		void destroyWidget(iWidget* widget);

        /*! destroyes widget by id

        \param id id of the widget to be destroyed
        */
        void destroyWidget(uint64 id);

        /*! \returns widget by id

        \param id id of widget
        */
        iWidget* getWidget(uint64 id);

        /*! \returns the theme in use
        */
		iWidgetBaseTheme* getTheme();

        /*! sets the theme to use

        \param theme the theme to use
        */
		void setTheme(iWidgetBaseTheme *theme);

        /*! sets the desktop dimensions

        you should update this e.g. when the window size changes

        \param width width of desktop
        \param height height of desktop
        */
		void setDesktopDimensions(uint32 width, uint32 height);

        /*! \returns dektop width
        */
		uint32 getDesktopWidth() const;

        /*! \returns dektop height
        */
        uint32 getDesktopHeight() const;

        /*! draws the widgets if theme is defined

        needs to be called by application

        make sure you have set up an orthogonal coordinate system with the same dimensions as set via setDesktopDimensions
        */
        void draw();

        /*! updates recursively all widgets
        */
        void update();

        /*! set this widget exclusively modal
        */
        void setModal(iWidgetDialog* dialog);

        /*! \returns current modal widget
        */
        iWidgetDialog* getModal();

        /*! \returns true: if widget is modal

        \param dialog the dialog to check if it is modal
        */
        bool isModal(iWidgetDialog* dialog);

        /*! reset modal flag
        */
        void resetModal();

        /*! register delegate to "redirected" mouse double click event

        \param doubleClickDelegate the delegate to register
        */
        void registerMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate doubleClickDelegate);

        /*! unregister delegate from "redirected" mouse key down event

        \param doubleClickDelegate the delegate to unregister
        */
        void unregisterMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate doubleClickDelegate);

        /*! register delegate to "redirected" mouse key down event

        \param keydown_delegate the delegate to register
        */
        void registerMouseKeyDownDelegate(iMouseKeyDownDelegate keydown_delegate);

        /*! unregister delegate from "redirected" mouse key down event

        \param keydown_delegate the delegate to unregister
        */
        void unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate keydown_delegate);

        /*! register delegate to "redirected" mouse key up event

        \param keyup_delegate the delegate to register
        */
        void registerMouseKeyUpDelegate(iMouseKeyUpDelegate keyup_delegate);

        /*! unregister delegate from "redirected" mouse key up event

        \param keyup_delegate the delegate to unregister
        */
        void unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate keyup_delegate);

        /*! register delegate to "redirected" mouse move full data event

        \param move_delegate the delegate to register
        */
        void registerMouseMoveFullDelegate(iMouseMoveFullDelegate move_delegate);

        /*! unregister delegate from "redirected" mouse move full data event

        \param move_delegate the delegate to unregister
        */
        void unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate move_delegate);

        /*! register delegate to "redirected" mouse move event

        \param move_delegate the delegate to register
        */
        void registerMouseMoveDelegate(iMouseMoveDelegate move_delegate);

        /*! unregister delegate from "redirected" mouse move event

        \param move_delegate the delegate to unregister
        */
        void unregisterMouseMoveDelegate(iMouseMoveDelegate move_delegate);

        /*! register delegate to "redirected" mouse wheel event

        \param wheel_delegate the delegate to register
        */
        void registerMouseWheelDelegate(iMouseWheelDelegate wheel_delegate);

        /*! unregister delegate from "redirected" mouse wheel event

        \param wheel_delegate the delegate to unregister
        */
        void unregisterMouseWheelDelegate(iMouseWheelDelegate wheel_delegate);

	private:

        /*! modal marker
        */
        iWidgetDialog* _modal = nullptr;

        /*! mouse key down event
        */
        iMouseKeyDownEvent _keyDownEvent;

        /*! mouse key up event
        */
        iMouseKeyUpEvent _keyUpEvent;

        /*! mouse double click event
        */
        iMouseKeyDoubleClickEvent _doubleClickEvent;

        /*! mouse move event
        */
        iMouseMoveFullEvent _moveFullEvent;

        /*! mouse move event
        */
        iMouseMoveEvent _moveEvent;

        /*! mouse wheel event
        */
        iMouseWheelEvent _wheelEvent;

        /*! pointer to current theme
        */
		iWidgetBaseTheme* _currentTheme = nullptr;

        /*! list of all widgets
        */
		map<uint64, iWidget*> _widgets;

        /*! list of all dialogs
        */
        vector<iWidgetDialog*> _dialogs;

        /*! list of widgets to delete
        */
        vector<iWidget*> _toDelete;

        /*! current desktop width
        */
		uint32 _desktopWidth = 0;

        /*! current desktop height
        */
		uint32 _desktopHeight = 0;

        void updateWidget(iWidget* widget);

        /*! destroyes widgets
        */
        void destroyWidgets();

        /*! handle for mouse key down event

        \param key mouse key pressed
        */
        void onMouseKeyDown(iKeyCode key);

        /*! handle for mouse key up event

        \param key mouse key released
        */
        void onMouseKeyUp(iKeyCode key);

        /*! handle for mouse double click

        \param key contains which key was pressed
        */
        void onMouseDoubleClick(iKeyCode key);

        /*! handle for mouse move event

        \param x1 last horrizontal mouse position
        \param y1 last vertical mouse position
        \param x2 current horrizontal mouse position
        \param y2 current vertical mouse position
        \param window parent window of mouse event
        */
        void onMouseMove(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window);

        /*! handle for mouse wheel event

        \param d mouse wheel delta
        */
        void onMouseWheel(int32 d);

        /*! internal handler for incomming keyboard events
        */
        void onASCII(char c);

        /*! registers to mouse and keyboard evetns
        */
        void registerIOEvents();

        /*! unregister from mouse and keyboard evetns
        */
        void unregisterIOEvents();

        /*! init
        */
        iWidgetManager();

        /*! checks for mem leaks and releases all left over widgets
        */
        virtual ~iWidgetManager();

	};

}

#endif

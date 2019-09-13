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

#ifndef __iDIALOGMANAGER__
#define __iDIALOGMANAGER__

#include <iDefines.h>
#include <iKeyboard.h>
#include <iWidget.h>

#include <iaSingleton.h>

#include <vector>
#include <unordered_map>
#include <deque>


namespace Igor
{

	class iWidgetBaseTheme;
    class iDialog;

    iaDELEGATE(iInstanciateWidgetDelegate, iWidget*, (), ());
    iaDELEGATE(iInstanciateDialogDelegate, iDialog*, (), ());

    /*! manages the widgets in use and is a singleton
    */
	class Igor_API iWidgetManager : public iaSingleton<iWidgetManager>
	{

        /*! needs to be friends with singleton base class in order to be a singleton
        */
        friend class iaSingleton<iWidgetManager>;

		friend class iWidget;
		friend class iDialog;
        
	public:

		/*! shows tooltip at given position

		\param pos the position to show the tooltip
		\param text the text of the tooltip
		*/
		void showTooltip(const iaVector2i& pos, const iaString& text);

		/*! hides the tooltip
		*/
		void hideTooltip();

        /*! \returns widget by id

        \param id id of widget
        */
        iWidget* getWidget(uint64 id);

        /*! \returns dialog by id

        \param id id of dialog
        */
        iDialog* getDialog(uint64 id);

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

        /*! set this widget exclusively modal
        */
        static void setModal(iDialog* dialog);

        /*! \returns current modal widget
        */
        static iDialog* getModal();

        /*! \returns true: if widget is modal

        \param dialog the dialog to check if it is modal
        */
        static bool isModal(iDialog* dialog);

        /*! reset modal flag
        */
        static void resetModal();

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

		/*! register delegate to "redirected" key down event

		\param delegate the delegate to register
		*/
		void registerKeyDownDelegate(iKeyDownDelegate delegate);

		/*! unregister delegate from "redirected" key down event

		\param delegate the delegate to unregister
		*/
		void unregisterKeyDownDelegate(iKeyDownDelegate delegate);

		/*! register delegate to "redirected" key up event

		\param delegate the delegate to register
		*/
		void registerKeyUpDelegate(iKeyUpDelegate delegate);

		/*! unregister delegate from "redirected" key up event

		\param delegate the delegate to unregister
		*/
		void unregisterKeyUpDelegate(iKeyUpDelegate delegate);

	private:

        /*! registered widget types
        */
        std::map<uint64, iInstanciateWidgetDelegate> _widgetTypes; // TODO use unordered map

        /*! registered dialog types
        */
        std::map<uint64, iInstanciateDialogDelegate> _dialogTypes;

        /*! modal marker
        */
        static iDialog* _modal;

        /*! mouse key down event
        */
        iMouseKeyDownEvent _mouseKeyDownEvent;

        /*! mouse key up event
        */
        iMouseKeyUpEvent _mouseKeyUpEvent;

		/*! keyboard down event
		*/
		iKeyDownEvent _keyDownEvent;

		/*! keyboard up event
		*/
		iKeyUpEvent _keyUpEvent;

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
		std::unordered_map<uint64, iWidget*> _widgets;

        /*! list of all dialogs
        */
        std::unordered_map<uint64, iDialog*> _dialogs;

        /*! current desktop width
        */
		uint32 _desktopWidth = 0;

        /*! current desktop height
        */
		uint32 _desktopHeight = 0;

		/*! tooltip position
		*/
		iaVector2i _tooltipPos;

		/*! tooltip text
		*/
		iaString _tooltipText;

		/*! registers widget to WidgetManager so we can track if all widgets got destroyed at shutdown

		\param widget the widget to track
		*/
		void registerWidget(iWidget* widget);

		/*! unregister widget from WidgetManager so we don't track this one anymore

		\param widget the widget to not track anymore
		*/
		void unregisterWidget(iWidget* widget);

		/*! registers dialog to WidgetManager so we can track if all dialogs got destroyed at shutdown

		\param dialog the dialog to track
		*/
		void registerDialog(iDialog* dialog);

		/*! unregister dialog from WidgetManager so we don't track this one anymore

		\param dialog the dialog to not track anymore
		*/
		void unregisterDialog(iDialog* dialog);

        /*! traverse widget tree and updates positions and sizes

        \param widget current widget to update
        */
        void traverseContentSize(iWidget* widget);

        /*! traverse widget tree and updates alignment
        */
        void traverseAlignment(iWidget* widget, int32 offsetX, int32 offsetY, int32 clientRectWidth, int32 clientRectHeight);

        /*! updates recursively all widgets
        */
        void onHandle();
		
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

        \param x1 last horizontal mouse position
        \param y1 last vertical mouse position
        \param x2 current horizontal mouse position
        \param y2 current vertical mouse position
        \param window parent window of mouse event
        */
        void onMouseMove(const iaVector2i& from, const iaVector2i& to, iWindow* window);

        /*! handle for mouse wheel event

        \param d mouse wheel delta
        */
        void onMouseWheel(int32 d);

        /*! internal handler for incomming keyboard events

        \param c the ascii character
        */
        void onASCII(const char c);

        /*! handles key down event
        */
        void onKeyDown(iKeyCode key);

        /*! handles key up event
        */
        void onKeyUp(iKeyCode key);

        /*! registers to mouse and keyboard evetns
        */
        void registerHandles();

        /*! unregister from mouse and keyboard evetns
        */
        void unregisterHandles();

        /*! init
        */
        iWidgetManager();

        /*! checks for mem leaks and releases all left over widgets
        */
        virtual ~iWidgetManager();

	};

#include <iWidgetManager.inl>

}

#endif

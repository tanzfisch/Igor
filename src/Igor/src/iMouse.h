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
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __iMOUSE__
#define __iMOUSE__

#include <iaEvent.h>
#include <iKeyCodeMap.h>
#include <iRectangle.h>
#include <iaVector2.h>
#include <iOSEventListener.h>

#define WM_MOUSEWHEEL 0x020A
#define WHEEL_DELTA 120

namespace Igor
{

	/*! mouse key down event

	will be called for a any pressed mouse key
    (only the first 5 keys of the mouse)

    \pram key the key code of the key that was pressed
	*/
	iaEVENT(iMouseKeyDownEvent, iMouseKeyDownDelegate, void, (iKeyCode key), (key));

	/*! mouse key up event

	will be called for a any released mouse key
    (only the first 5 keys of the mouse)

    \pram key the key code of the key that was released
	*/
	iaEVENT(iMouseKeyUpEvent, iMouseKeyUpDelegate, void, (iKeyCode key), (key));

    /*! mouse double click event

    will be called for a any mouse key that does a double click

    \pram key the key code of the key that was double clicked
    */
    iaEVENT(iMouseKeyDoubleClickEvent, iMouseKeyDoubleClickDelegate, void, (iKeyCode key), (key));

    /*! mouse move event with simple data

    mouse coordinates have their origin in the upper left corner of the parenting window

    \param x current horrizontal coordinate of mouse cursor
    \param y current vertical coordinate of mouse cursor
    */
    iaEVENT(iMouseMoveEvent, iMouseMoveDelegate, void, (int32 x, int32 y), (x, y));

	/*! mouse move event with full data

    mouse coordinates have their origin in the upper left corner of the parenting window

    \param x1 last horrizontal coordinate
    \param y1 last vertical coordinate
    \param x2 current horrizontal coordinate
    \param y2 current vertical coordinate
    \param window the window the coordinates are related to
    */
	iaEVENT(iMouseMoveFullEvent, iMouseMoveFullDelegate, void, (int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window), (x1, y1, x2, y2, window));

	/*! mouse wheel event

    \param d mouse wheel delta
	*/
	iaEVENT(iMouseWheelEvent, iMouseWheelDelegate, void, (int32 d), (d));

	/*! Mouse (Singleton)

	Example:
	\ref Mouse/src/MouseExample.cpp "Mouse usage example"

	\todo double click
	*/
	class Igor_API iMouse : public iOSEventListener, public iaSingleton<iMouse>
	{

		friend class iaSingleton<iMouse>;

	private:

        /*! size of buffer needed to receive raw input messages
        */
		uint32 _rawInputBufferSize = 0;

        /*! pointer to the buffer to receive raw input messages
        */
		uint8* _rawInputBuffer = nullptr;

        /*! configuration of raw input device
        */
		RAWINPUTDEVICE _rawInputDevice;

        /*! x position of mouse in pixel from the last update message
        */
		int32 _xPos = 0;

        /*! y position of mouse in pixel from the last update message
        */
        int32 _yPos = 0;

        /*! old x position of mouse in pixel
        */
        int32 _xPosOld = 0;

        /*! old y position of mouse in pixel
        */
        int32 _yPosOld = 0;

        /*! wheel delta from the last update message
        */
		int32 _wheelDelta = 0;

        /*! right mouse button state since the last update message
        */
		bool _rButton = false;

        /*! left mouse button state since the last update message
        */
        bool _lButton = false;

        /*! middle mouse button state since the last update message
        */
        bool _mButton = false;

        /*! state of mouse button 4 since the last update message
        */
		bool _button4 = false;

        /*! state of mouse button 5 since the last update message
        */
        bool _button5 = false;

        /*! if true the next mouse move event will be ignored.

        This is used e.g. to center the mouse cursor
        */
		bool _ignoreNextMoveEvent = false;

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

		/*! handles incomming OS messages
		*/
		bool onOSEvent(const void *data);

		/*! registers raw input device for mouse usage
		*/
		bool registerDevice();

	private:

		/*! initializes member variables
		*/
		iMouse();

		/*! clean up and release memory
		*/
		virtual ~iMouse();

	public:

		/*! register delegate to mouse double click event

		\param doubleClickDelegate the delegate to register
		*/
		void registerMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate doubleClickDelegate);

		/*! unregister delegate from mouse key down event

		\param doubleClickDelegate the delegate to unregister
		*/
		void unregisterMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate doubleClickDelegate);

        /*! register delegate to mouse key down event

        \param keydown_delegate the delegate to register
        */
        void registerMouseKeyDownDelegate(iMouseKeyDownDelegate keydown_delegate);

        /*! unregister delegate from mouse key down event

        \param keydown_delegate the delegate to unregister
        */
        void unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate keydown_delegate);

		/*! register delegate to mouse key up event

		\param keyup_delegate the delegate to register
		*/
		void registerMouseKeyUpDelegate(iMouseKeyUpDelegate keyup_delegate);

		/*! unregister delegate from mouse key up event

		\param keyup_delegate the delegate to unregister
		*/
		void unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate keyup_delegate);

		/*! register delegate to mouse move full data event

		\param move_delegate the delegate to register
		*/
		void registerMouseMoveFullDelegate(iMouseMoveFullDelegate move_delegate);

		/*! unregister delegate from mouse move full data event

		\param move_delegate the delegate to unregister
		*/
		void unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate move_delegate);

        /*! register delegate to mouse move event

        \param move_delegate the delegate to register
        */
        void registerMouseMoveDelegate(iMouseMoveDelegate move_delegate);

        /*! unregister delegate from mouse move event

        \param move_delegate the delegate to unregister
        */
        void unregisterMouseMoveDelegate(iMouseMoveDelegate move_delegate);

		/*! register delegate to mouse wheel event

		\param wheel_delegate the delegate to register
		*/
		void registerMouseWheelDelegate(iMouseWheelDelegate wheel_delegate);

		/*! unregister delegate from mouse wheel event

		\param wheel_delegate the delegate to unregister
		*/
		void unregisterMouseWheelDelegate(iMouseWheelDelegate wheel_delegate);

		/*! set mouse to center of currently focused window
		*/
		virtual void setCenter(bool supressMoveEvent = false);

		/*! set mouse to a position relative to currently focussed window

		\param x horrizontal position of the mouse cursor in pixel
		\param y vertical position of the mouse cursor in pixel
		*/
		virtual void setPosition(int32 x, int32);

		/*! switches the cursor on or of

		\param show true: cursor is visible; false: cursor is invisible
		*/
		virtual void showCursor(bool show);

        /*! \returns the current mouse position relative to the parent window in pixel.
        */
        iaVector2i getPos();

        /*! \returns the delta mouse movement since last mouse update.
        */
        iaVector2i getPosDelta();

        /*! \returns the the mouse wheel delta
        */
        int32 getWheelDelta();

        /*! \returns true: right mouse button is pressed; false: right mouse button is not pressed
        */
        bool getRightButton();

        /*! \returns true: left mouse button is pressed; false: left mouse button is not pressed
        */
        bool getLeftButton();

        /*! \returns true: middle mouse button is pressed; false: middle mouse button is not pressed
        */
        bool getMiddleButton();

        /*! \returns true: mouse button 4 is pressed; false: mouse button 4 is not pressed
        */
        bool getButton4();

        /*! \returns true: mouse button 5 is pressed; false: mouse button 5 is not pressed
        */
        bool getButton5();
	};

	/*! \example Mouse/src/MouseExample.cpp
	Mouse usage example.

	\include Mouse/src/MouseExample.h
	*/

};

#endif

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
// (c) Copyright 2012-2020 by Martin Loga
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
// contact: igorgameengine@protonmail.com

#ifndef __iMOUSE__
#define __iMOUSE__

#include <iaux/system/iaEvent.h>
#include <igor/data/iRectangle.h>
#include <iaux/math/iaVector2.h>
#include <igor/system/iOSEventListener.h>

namespace igor
{

    class iMouseImpl;

    /*! mouse key down event

	will be called for a any pressed mouse key
    (only the first 5 keys of the mouse)

    \param key the key code of the key that was pressed    
	*/
    iaEVENT(iMouseKeyDownEvent, iMouseKeyDownDelegate, void, (iKeyCode key), (key));

    /*! mouse key up event

	will be called for a any released mouse key
    (only the first 5 keys of the mouse)

    \pram key the key code of the key that was released
	*/
    iaEVENT(iMouseKeyUpEvent, iMouseKeyUpDelegate, void, (iKeyCode key), (key));

    /*! mouse clicked event. triggered on moue up after there was a mouse down

    \param key the key code of the key that was pressed
    */
    iaEVENT(iMouseClickEvent, iMouseClickDelegate, void, (iKeyCode key), (key));

    /*! mouse double click event

    will be called for a any mouse key that does a double click

    \pram key the key code of the key that was double clicked
    */
    iaEVENT(iMouseKeyDoubleClickEvent, iMouseKeyDoubleClickDelegate, void, (iKeyCode key), (key));

    /*! mouse move event with simple data

    mouse coordinates have their origin in the upper left corner of the parenting window

    \param pos current mouse position
    */
    iaEVENT(iMouseMoveEvent, iMouseMoveDelegate, void, (const iaVector2i &pos), (pos));

    /*! mouse move event with full data

    mouse coordinates have their origin in the upper left corner of the parenting window

    \param from last mouse position
    \param to current mouse position
    \param window the window the coordinates are related to
    */
    iaEVENT(iMouseMoveFullEvent, iMouseMoveFullDelegate, void, (const iaVector2i &from, const iaVector2i &to, iWindow *window), (from, to, window));

    /*! mouse wheel event

    \param d mouse wheel delta
	*/
    iaEVENT(iMouseWheelEvent, iMouseWheelDelegate, void, (int32 d), (d));

    /*! Mouse (Singleton)

    \todo use ClipCursor to kleep the cursor inside the window
	*/
    class Igor_API iMouse : public iOSEventListener, public iaSingleton<iMouse>
    {

        friend class iaSingleton<iMouse>;

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

        \param keydownDelegate the delegate to register
        */
        void registerMouseKeyDownDelegate(iMouseKeyDownDelegate keydownDelegate);

        /*! unregister delegate from mouse key down event

        \param keydownDelegate the delegate to unregister
        */
        void unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate keydownDelegate);

        /*! register delegate to mouse key up event

		\param keyupDelegate the delegate to register
		*/
        void registerMouseKeyUpDelegate(iMouseKeyUpDelegate keyupDelegate);

        /*! unregister delegate from mouse key up event

		\param keyupDelegate the delegate to unregister
		*/
        void unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate keyupDelegate);

        /*! register delegate to mouse key clicked event

        \param clickDelegate the delegate to register
        */
        void registerMouseClickDelegate(iMouseClickDelegate clickDelegate);

        /*! unregister delegate from mouse key up event

        \param clickDelegate the delegate to unregister
        */
        void unregisterMouseClickDelegate(iMouseClickDelegate clickDelegate);

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

        \param supressMoveEvent if true supress next mouse event
		*/
        void setCenter();

        /*! set mouse to a position relative to currently focussed window

		\param x horizontal position of the mouse cursor in pixel
		\param y vertical position of the mouse cursor in pixel
        \param supressMoveEvent if true supress next mouse event
		*/
        void setPosition(int32 x, int32);

        /*! set mouse to a position relative to currently focussed window

        \param pos horizontal and vertical position of the mouse cursor in pixel
        \param supressMoveEvent if true supress next mouse event
        */
        void setPosition(const iaVector2i &pos);

        /*! switches the cursor on or of

		\param show true: cursor is visible; false: cursor is invisible
		*/
        virtual void showCursor(bool show);

        /*! \returns the current mouse position relative to the parent window in pixel.

        origin is top left 
        */
        iaVector2i getPos();

        /*! \returns the delta mouse movement since last mouse update.
        */
        iaVector2i getPosDelta();

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

        /*! \retruns pointer to corresponding window
		if it returns nullptr the listener was not registred to a window yet
		*/
        iWindow *getWindow() const override;

    private:
        /*! pimpl
        */
        iMouseImpl *_impl = nullptr;

        /*! handles incomming OS messages
        */
        bool onOSEvent(const void *data) override;

        /*! does what ever necessairy to get the device running

		\param data os specific data (see iOSEventregisterData)
		*/
        bool initDevice(const void *data) override;

        /*! unregisters device from listening
		*/
        void deinitDevice() override;

        /*! initializes member variables
        */
        iMouse();

        /*! clean up and release memory
        */
        virtual ~iMouse();
    };

    /*! \example Mouse/src/MouseExample.cpp
	Mouse usage example.

	\include Mouse/src/MouseExample.h
	*/

}; // namespace igor

#endif

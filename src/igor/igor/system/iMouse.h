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

#ifndef __IGOR_MOUSE_H__
#define __IGOR_MOUSE_H__

#include <igor/data/iRectangle.h>
#include <igor/system/iOSEventListener.h>

#include <iaux/math/iaVector2.h>
using namespace iaux;

namespace igor
{

    class iMouseImpl;

    /*! Mouse (Singleton)

    \todo use ClipCursor to kleep the cursor inside the window
	*/
    class Igor_API iMouse : public iOSEventListener, public iaSingleton<iMouse>
    {

        friend class iaSingleton<iMouse>;

    public:
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

#endif // __IGOR_MOUSE_H__

//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef IGOR_MOUSE_H
#define IGOR_MOUSE_H

#include <igor/system/iOSEventListener.h>
#include <igor/resources/module/iModule.h>

#include <iaux/data/iaRectangle.h>
#include <iaux/math/iaVector2.h>
using namespace iaux;

namespace igor
{

    /*! mouse cursor type definition
    */
    enum class iMouseCursorType
    {
        Arrow,  // default
        ArrowLeftEdge,
        ArrowRightEdge,
        ArrowTopEdge,
        ArrowBottomEdge,
        ArrowTopLeftCorner,
        ArrowTopRightCorner,
        ArrowBottomLeftCorner,
        ArrowBottomRightCorner,
        VeticalSplit,
        HorizontalSplit
    };

    class iMouseImpl;

    /*! Mouse (Singleton)
	*/
    class IGOR_API iMouse : public iOSEventListener, public iModule<iMouse>
    {

        friend class iModule<iMouse>;

    public:
        /*! set mouse to center of currently focused window

        \param suppressMoveEvent if true suppress next mouse event
		*/
        void setCenter();

        /*! set mouse to a position relative to currently focussed window

		\param x horizontal position of the mouse cursor in pixel
		\param y vertical position of the mouse cursor in pixel
		*/
        void setPosition(float32 x, float32 y);

        /*! set mouse to a position relative to currently focussed window

        \param pos horizontal and vertical position of the mouse cursor in pixel
        */
        void setPosition(const iaVector2f &pos);        

        /*! switches the cursor on or off

		\param show true: cursor is visible; false: cursor is invisible
		*/
        void hideCursor(bool hide);

        /*! sets cursor type

        \param cursorType the cursor type to set
        */
        void setCursor(iMouseCursorType cursorType);

        /*! resets cursor type to default
        */
        void resetCursor();

        /*! \returns the current mouse position relative to the parent window in pixel.

        origin is top left 
        */
        const iaVector2f& getPos() const;

        /*! \returns the delta mouse movement since last mouse update.
        */
        iaVector2f getPosDelta() const;

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

        /*! \returns pointer to corresponding window
		
        If it returns nullptr the listener was not registered to a window yet
		*/
        iWindowPtr getWindow() const override;

    private:
        /*! pimpl
        */
        iMouseImpl *_impl = nullptr;

        /*! handles incoming OS messages
        */
        bool onOSEvent(const void *data) override;

        /*! does what ever necessary to get the device running

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

    /*! \example examples/02_Mouse/MouseExample.cpp
	Mouse usage example.

	\include examples/02_Mouse/MouseExample.h
	*/

}; // namespace igor

#endif // IGOR_MOUSE_H

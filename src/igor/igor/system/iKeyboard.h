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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __IGOR_KEYBOARD_H__
#define __IGOR_KEYBOARD_H__

#include <igor/system/iWindow.h>
#include <igor/system/iOSEventListener.h>

namespace igor
{
    class iKeyboardImpl;

    /*! Handles keyboard specific os events and triggers Igor events.

	It also stores key states for later use
	*/
    class IGOR_API iKeyboard : public iOSEventListener, public iModule<iKeyboard>
    {

        friend class iModule<iKeyboard>;

    public:
        /*! \returns true: if a specified key is pressed; false if the specified key is not pressed

        you should not use that. better style is to use events

        \param keyCode the specified key
        */
        bool getKey(iKeyCode keyCode);

        /*! \returns true: if any key is pressed; false: if no key is pressed

        you should not use that. better style is to use events
        */
        bool keyPressed();

        /*! returns key code

		\param keycode key name
		\return key code
		*/
        static iKeyCode getKeyCode(const iaString &keyname);

        /*! \retruns pointer to corresponding window
		if it returns nullptr the listener was not registred to a window yet
		*/
        iWindow *getWindow() const override;

    private:
        /*! pimpl
		*/
        iKeyboardImpl *_impl;

        /*! called by a window if an os event occours
		*/
        bool onOSEvent(const void *data) override;

        /*! does what ever necessairy to get the device running

		\param window handle to the window
		\param data os specific data (see iOSEventregisterData)
		*/
        bool initDevice(const void *data) override;

        /*! unregisters device from listening
		*/
        void deinitDevice() override;

        /*! initializes member variables
		*/
        iKeyboard();

        /*! releases all delegates if not done by application
		*/
        virtual ~iKeyboard();
    };

    /*! \example Keyboard/src/KeyboardExample.cpp
    Keyboard usage example.

    \include Keyboard/src/KeyboardExample.h
    */

    /*! stream operator

	\param stream the destination
	\param keyCode the key code to print
	\returns the resulting stream
	*/
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iKeyCode &keyCode);

}; // namespace igor

#endif // __IGOR_KEYBOARD_H__

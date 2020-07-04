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

#ifndef __IGOR_EVENTKEYBOARD_H__
#define __IGOR_EVENTKEYBOARD_H__

#include <igor/system/events/iEvent.h>

namespace igor
{

    class iWindow;

    /*! key down event
    */
    class Igor_API iKeyDownEvent_TMP : public iEvent
    {
    public:
        /*! init members

        \param window the window this event came from
        \param key the key code used in this event
        */
        iKeyDownEvent_TMP(iWindow *window, const iKeyCode key);

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns the key code
        */
        iKeyCode getKey() const;

        IGOR_EVENT_CLASS_TYPE(iKeyDownEvent_TMP)

    private:
        /*! the key code
        */
        iKeyCode _key;
    };

    /*! key up event
    */
    class Igor_API iKeyUpEvent_TMP : public iEvent
    {
    public:
        /*! init members

        \param window the window this event came from
        \param key the key code used in this event
        */
        iKeyUpEvent_TMP(iWindow *window, const iKeyCode key);

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns the key code
        */
        iKeyCode getKey() const;

        IGOR_EVENT_CLASS_TYPE(iKeyUpEvent_TMP)

    private:
        /*! the key code
        */
        iKeyCode _key;
    };

    /*! key ascii event
    */
    class Igor_API iKeyASCIIEvent_TMP : public iEvent
    {
    public:
        /*! init members

        \param window the window this event came from
        \param key the key code used in this event
        */
        iKeyASCIIEvent_TMP(iWindow *window, const char character);

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns the key code
        */
        char getChar() const;

        IGOR_EVENT_CLASS_TYPE(iKeyASCIIEvent_TMP)

    private:
        /*! the ascii value
        */
        char _character;
    };

}; // namespace igor

#endif // __IGOR_EVENTKEYBOARD_H__
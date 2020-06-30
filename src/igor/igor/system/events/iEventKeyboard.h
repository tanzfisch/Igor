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

    /*! key down event
    */
    class Igor_API iKeyDownEvent : public iEvent
    {
    public:
        /*! init members

        \param key the key code used in this event
        */
        iKeyDownEvent(const iKeyCode key);

        /*! does nothing
        */
        virtual ~iKeyDownEvent() = default;

        /*! \returns event type
        */
        iEventType getEventType() const override;

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns the key code
        */
        iKeyCode getKey() const;

    private:
        /*! the key code
        */
        iKeyCode _key;
    };

    /*! key up event
    */
    class Igor_API iKeyUpEvent : public iEvent
    {
    public:
        /*! init members

        \param key the key code used in this event
        */
        iKeyUpEvent(const iKeyCode key);

        /*! does nothing
        */
        virtual ~iKeyUpEvent() = default;

        /*! \returns event type
        */
        iEventType getEventType() const override;

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns the key code
        */
        iKeyCode getKey() const;

    private:
        /*! the key code
        */
        iKeyCode _key;
    };

    /*! key ascii event
    */
    class Igor_API iKeyASCIIEvent : public iEvent
    {
    public:
        /*! init members

        \param key the key code used in this event
        */
        iKeyASCIIEvent(const char key);

        /*! does nothing
        */
        virtual ~iKeyASCIIEvent() = default;

        /*! \returns event type
        */
        iEventType getEventType() const override;

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns the key code
        */
        char getKey() const;

    private:
        /*! the ascii value
        */
        char _key;
    };

}; // namespace igor

#endif // __IGOR_EVENTKEYBOARD_H__
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

#ifndef __IGOR_EVENTMOUSE_H__
#define __IGOR_EVENTMOUSE_H__

#include <igor/system/events/iEvent.h>

namespace igor
{

    class iWindow;

    /*! mouse key down event
    */
    class Igor_API iMouseKeyDownEvent : public iEvent
    {
    public:
        /*! init members

        \param key the key code used in this event
        */
        iMouseKeyDownEvent(const iKeyCode key);

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

    /*! mouse key up event
    */
    class Igor_API iMouseKeyUpEvent : public iEvent
    {
    public:
        /*! init members

        \param key the key code used in this event
        */
        iMouseKeyUpEvent(const iKeyCode key);

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

    /*! mouse key double click event
    */
    class Igor_API iMouseKeyDoubleClickEvent : public iEvent
    {
    public:
        /*! init members

        \param key the key code used in this event
        */
        iMouseKeyDoubleClickEvent(const iKeyCode key);

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

    /*! mouse wheel event
    */
    class Igor_API iMouseWheelEvent : public iEvent
    {
    public:
        /*! init members

        \param key the key code used in this event
        */
        iMouseWheelEvent(const int32 wheelDelta);

        /*! \returns event type
        */
        iEventType getEventType() const override;

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns the key code
        */
        int32 getWheelDelta() const;

    private:
        /*! the key code
        */
        int32 _wheelDelta = 0;
    };

    /*! mouse move event
    */
    class Igor_API iMouseMoveEvent : public iEvent
    {
    public:
        /*! init members

        \param key the key code used in this event
        */
        iMouseMoveEvent(const iaVector2i &from, const iaVector2i &to, iWindow *window);

        /*! \returns event type
        */
        iEventType getEventType() const override;

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns the key code
        */
        int32 getWheelDelta() const;

    private:
        /*! last mouse position
        */
        iaVector2i _from;

        /*! current mouse position
        */
        iaVector2i _to;

        /*! window handle
        */
        iWindow *_window;
    };

}; // namespace igor

#endif // __IGOR_EVENTMOUSE_H__

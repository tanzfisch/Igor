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
    class Igor_API iMouseKeyDownEvent_TMP : public iEvent
    {
    public:
        /*! init members

        \param key the key code used in this event
        */
        iMouseKeyDownEvent_TMP(const iKeyCode key);

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns the key code
        */
        iKeyCode getKey() const;

        IGOR_EVENT_CLASS_TYPE(iMouseKeyDownEvent_TMP)

    private:
        /*! the key code
        */
        iKeyCode _key;
    };

    /*! mouse key up event
    */
    class Igor_API iMouseKeyUpEvent_TMP : public iEvent
    {
    public:
        /*! init members

        \param key the key code used in this event
        */
        iMouseKeyUpEvent_TMP(const iKeyCode key);

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns the key code
        */
        iKeyCode getKey() const;

        IGOR_EVENT_CLASS_TYPE(iMouseKeyUpEvent_TMP)

    private:
        /*! the key code
        */
        iKeyCode _key;
    };

    /*! mouse key double click event
    */
    class Igor_API iMouseKeyDoubleClickEvent_TMP : public iEvent
    {
    public:
        /*! init members

        \param key the key code used in this event
        */
        iMouseKeyDoubleClickEvent_TMP(const iKeyCode key);

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns the key code
        */
        iKeyCode getKey() const;

        IGOR_EVENT_CLASS_TYPE(iMouseKeyDoubleClickEvent_TMP)

    private:
        /*! the key code
        */
        iKeyCode _key;
    };

    /*! mouse wheel event
    */
    class Igor_API iMouseWheelEvent_TMP : public iEvent
    {
    public:
        /*! init members

        \param wheelDelta the wheel delta
        */
        iMouseWheelEvent_TMP(const int32 wheelDelta);

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns the wheel delta
        */
        int32 getWheelDelta() const;

        IGOR_EVENT_CLASS_TYPE(iMouseWheelEvent_TMP)

    private:
        /*! the wheel delta
        */
        int32 _wheelDelta = 0;
    };

    /*! mouse move event
    */
    class Igor_API iMouseMoveEvent_TMP : public iEvent
    {
    public:
        /*! init members

        \param key the key code used in this event
        */
        iMouseMoveEvent_TMP(const iaVector2i &from, const iaVector2i &to, iWindow *window);

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns window handle
        */
        iWindow *getWindow() const;

        /*! \returns mouse position
        */
        const iaVector2i &getPosition() const;

        /*! \returns last mouse position
        */
        const iaVector2i &getLastPosition() const;

        IGOR_EVENT_CLASS_TYPE(iMouseMoveEvent_TMP)

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

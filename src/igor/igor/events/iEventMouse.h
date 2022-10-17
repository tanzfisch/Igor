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

#ifndef __IGOR_EVENTMOUSE_H__
#define __IGOR_EVENTMOUSE_H__

#include <igor/events/iEvent.h>

namespace igor
{

    class iWindow;

    /*! mouse key down event
    */
    class IGOR_API iEventMouseKeyDown : public iEvent
    {
    public:
        /*! init members

        \param window the window this event came from
        \param key the key code used in this event
        */
        iEventMouseKeyDown(iWindow *window, const iKeyCode key);

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns information to event
        */
        virtual const iaString getInfo() const override;

        /*! \returns the key code
        */
        iKeyCode getKey() const;

        IGOR_EVENT_CLASS_TYPE(iEventMouseKeyDown)

    private:
        /*! the key code
        */
        iKeyCode _key;
    };

    /*! mouse key up event
    */
    class IGOR_API iEventMouseKeyUp : public iEvent
    {
    public:
        /*! init members

        \param window the window this event came from
        \param key the key code used in this event
        */
        iEventMouseKeyUp(iWindow *window, const iKeyCode key);

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns information to event
        */
        virtual const iaString getInfo() const override;

        /*! \returns the key code
        */
        iKeyCode getKey() const;

        IGOR_EVENT_CLASS_TYPE(iEventMouseKeyUp)

    private:
        /*! the key code
        */
        iKeyCode _key;
    };

    /*! mouse key double click event
    */
    class IGOR_API iEventMouseKeyDoubleClick : public iEvent
    {
    public:
        /*! init members

        \param window the window this event came from
        \param key the key code used in this event
        */
        iEventMouseKeyDoubleClick(iWindow *window, const iKeyCode key);

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns information to event
        */
        virtual const iaString getInfo() const override;

        /*! \returns the key code
        */
        iKeyCode getKey() const;

        IGOR_EVENT_CLASS_TYPE(iEventMouseKeyDoubleClick)

    private:
        /*! the key code
        */
        iKeyCode _key;
    };

    /*! mouse wheel event
    */
    class IGOR_API iEventMouseWheel : public iEvent
    {
    public:
        /*! init members

        \param window the window this event came from
        \param wheelDelta the wheel delta
        */
        iEventMouseWheel(iWindow *window, const int32 wheelDelta);

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns information to event
        */
        virtual const iaString getInfo() const override;

        /*! \returns the wheel delta
        */
        int32 getWheelDelta() const;

        IGOR_EVENT_CLASS_TYPE(iEventMouseWheel)

    private:
        /*! the wheel delta
        */
        int32 _wheelDelta = 0;
    };

    /*! mouse move event
    */
    class IGOR_API iEventMouseMove : public iEvent
    {
    public:
        /*! init members

        \param window the window this event came from
        \param from the old mouse position
        \param to the new mouse position
        */
        iEventMouseMove(iWindow *window, const iaVector2i &from, const iaVector2i &to);

        /*! \returns event kind mask
        */
        iEventKindMask getEventKindMask() const override;

        /*! \returns information to event
        */
        virtual const iaString getInfo() const override;

        /*! \returns mouse position
        */
        const iaVector2i &getPosition() const;

        /*! \returns last mouse position
        */
        const iaVector2i &getLastPosition() const;

        IGOR_EVENT_CLASS_TYPE(iEventMouseMove)

    private:
        /*! last mouse position
        */
        iaVector2i _from;

        /*! current mouse position
        */
        iaVector2i _to;
    };

}; // namespace igor

#endif // __IGOR_EVENTMOUSE_H__

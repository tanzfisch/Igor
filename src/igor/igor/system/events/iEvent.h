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

#ifndef __IGOR_EVENT_H__
#define __IGOR_EVENT_H__

#include <igor/iDefines.h>

namespace igor
{
    /*! event type definitions
    */
    enum class iEventType
    {
        iKeyDownEvent,
        iKeyUpEvent,
        iKeyASCIIEvent,

        iMouseKeyDownEvent,
        iMouseKeyUpEvent,
        iMouseKeyDoubleClickEvent,
        iMouseMoveEvent,
        iMouseWheelEvent
    };

    /*! event kind definition
    */
    enum class iEventKind
    {
        None = 0,
        Input = 1,
        Keyboard = 2,
        Mouse = 4,
        Window = 8
    };

    /*! define event kind mask
    */
    typedef uint32 iEventKindMask;

    /*! event base class
    */
    class iEvent
    {
    public:
        /*! init members
        */
        iEvent() = default;

        /*! does nothing
        */
        virtual ~iEvent() = default;

        /*! \returns true if event is of given kind

        \param kind the given kind
        */
        bool isOfKind(const iEventKind kind);

        /*! \return information string from this event
        */
        virtual iaString getInfo() const;

        /*! \returns event type
        */
        virtual iEventType getEventType() const = 0;

        /*! \returns event kind mask
        */
        virtual iEventKindMask getEventKindMask() const = 0;

        /*! \returns event type name
        */
        virtual const iaString &getName() const = 0;

        /*! \returns true if already consumed
        */
        bool isConsumed() const;

    private:
        /*! if true event was already consumed
        */
        bool _consumed = false;
    };

}; // namespace igor

#endif // __IGOR_EVENT_H__
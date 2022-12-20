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

#ifndef __IGOR_EVENTWINDOW_H__
#define __IGOR_EVENTWINDOW_H__

#include <igor/events/iEvent.h>

namespace igor
{

    class iWindow;

    /*! window close event

    called after window was closed
    */
    class IGOR_API iEventWindowClose : public iEvent
    {
    public:
        /*! init members

        \param window the window that was closed
        */
        iEventWindowClose(iWindowPtr window);

        IGOR_EVENT_CLASS_TYPE(iEventWindowClose)
        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::Window)
    };

    /*! window open event

    called after window was opened
    */
    class IGOR_API iEventWindowOpen : public iEvent
    {
    public:
        /*! init members

        \param window the window that was opened
        */
        iEventWindowOpen(iWindowPtr window);

        IGOR_EVENT_CLASS_TYPE(iEventWindowOpen)
        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::Window)
    };

    /*! window resize event
    */
    class IGOR_API iEventWindowResize : public iEvent
    {
    public:
        /*! init members

        \param window the window that was resized
        \param width width of the window
        \param height height of the window
        */
        iEventWindowResize(iWindowPtr window, int32 width, int32 height);

        /*! \returns information to event
        */
        virtual const iaString getInfo() const override;

        /*! \returns width of the window
        */
        int32 getWidth() const;

        /*! \returns height of the window
        */
        int32 getHeight() const;

        IGOR_EVENT_CLASS_TYPE(iEventWindowResize)
        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::Window)

    private:
        /*! width of the window
        */
        int32 _width;

        /*! height of the window
        */
        int32 _height;
    };

}; // namespace igor

#endif // __IGOR_EVENTKEYBOARD_H__
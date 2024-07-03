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

#ifndef __IGOR_DEFINES_LINUX__
#define __IGOR_DEFINES_LINUX__

#ifdef IGOR_LINUX

#include <igor/iDefines.h>

#include <glad/glad.h>
#include <GL/glx.h>
#include <GL/glext.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#ifdef XF86VMODE_FOUND
#include <X11/extensions/xf86vmode.h>
#endif
#include <X11/cursorfont.h>

namespace igor
{
    class iWindow;

    /*! helper data set for windows specific messages
    */
    struct iOSEvent
    {
        /*! window handle
        */
        Display *_display = nullptr;

        /*! event message
        */
        XEvent _event;

        /*! forgot what that was
        */
        XVisualInfo *_visual = nullptr;
    };

    /*! helper class to pass through some data from window to listener during registration
    */
    struct iOSEventregisterData
    {
        /*! the window the events will come from
        */
        iWindowPtr _window;

        /*! display instance
        */
        Display *_display = nullptr;

        /*! window instance
        */
        Window _xWindow;
    };

} // namespace igor

#endif // IGOR_LINUX

#endif // __IGOR_DEFINES_LINUX__
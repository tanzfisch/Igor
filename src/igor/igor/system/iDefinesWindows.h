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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_DEFINES_WINDOWS__
#define __IGOR_DEFINES_WINDOWS__

#ifdef IGOR_WINDOWS

#include <igor/iDefines.h>
#include <windows.h>
#include <windowsx.h>
#define WM_MOUSEWHEEL 0x020A
#define WHEEL_DELTA 120

#include <glad/glad.h>
#include <gl/gl.h>

namespace igor
{
    class iWindow;

    /*! helper data set for windows specific messages
    */
    struct iOSEvent
    {
        /*! window handle
        */
        HWND _hWnd;

        /*! message Id
        */
        UINT _uMsg;

        /*! first part of message
        */
        WPARAM _wParam;

        /*! second part of message
        */
        LPARAM _lParam;
    };

    /*! helper class to pass through some data from window to listener during registration
    */
    struct iOSEventregisterData
    {
        /*! the window the events will come from
        */
        iWindowPtr _window;
    };

} // namespace igor

#endif // IGOR_WINDOWS

#endif // __IGOR_DEFINES_WINDOWS__

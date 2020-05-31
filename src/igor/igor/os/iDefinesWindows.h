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
// contact: martinloga@gmx.de

#ifndef __iDEFINES_WINDOWS__
#define __iDEFINES_WINDOWS__

#ifdef __IGOR_WINDOWS__

#include <igor/iDefines.h>
#include <GLee.h>
#include <windows.h>
#include <windowsx.h>
#define WM_MOUSEWHEEL 0x020A
#define WHEEL_DELTA 120

namespace Igor
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
        iWindow *_window;
    };

} // namespace Igor

#endif // __IGOR_WINDOWS__

#endif // __iDEFINES_WINDOWS__

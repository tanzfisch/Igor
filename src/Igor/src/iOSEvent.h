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
// (c) Copyright 2012-2018 by Martin Loga
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

#ifndef __iOSEVENT__
#define __iOSEVENT__

#include <iDefines.h>

#ifdef __IGOR_WIN__

	#include <windows.h>

	/*! helper data set for windows specific messages
	*/
	struct iOSEventWin32
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

#endif

#ifdef __LINUX__

	#include <X11/Xlib.h>
	#include <GL/glx.h>

	/*! helper data set for windows specific messages

	\deprecated currently thers is no Linux support in Igor anymore
	*/
	struct iOSEventLinux
	{
		/*! window handle
		*/
		Display* _xdisplay;

		/*! event message 
		*/
		XEvent _xevent;

		/*! forgot what that was		
		*/
		XVisualInfo* _xvisual;
	};

#endif

#endif


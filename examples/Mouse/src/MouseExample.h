//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/                     ) )     ((
//                                           (_(       \)
//    (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __MOUSEEXAMPLE__
#define __MOUSEEXAMPLE__

#include <iWindow.h>
#include <iView.h>
#include <iKeyboard.h>
using namespace Igor;

class MouseExample
{

private:

	iView view;
	iWindow window;

    void mouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window);
	void mouseKeyDown(iKeyCode key);
	void mouseKeyUp(iKeyCode key);
	void mouseWheel(int32 d);
	void keyboardESCKeyDown();
	void closeWindow();

	void init();
	void deinit();

public:

	void run();

	MouseExample();
	~MouseExample();

};

#endif
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
#include <iKeyCodeMap.h>
using namespace Igor;

class MouseExample
{

public:

    /*! run the example
    */
    void run();

    /*! init example
    */
    MouseExample();

    /*! deinit example
    */
    ~MouseExample();
    
private:

    /*! the window we need to receive io events
    */
	iWindow _window;

    /*! called when mouse was moved
    \param x1 last horrizontal coordinate
    \param y1 last vertical coordinate
    \param x2 current horrizontal coordinate
    \param y2 current vertical coordinate
    \param window the window the coordinates are related to
    */
    void onMouseMovedFull(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window);

    /*! called when any mouse key was pressed

    \pram key the key code of the key that was pressed
    */
	void onMouseKeyDown(iKeyCode key);

    /*! called when any mouse key was released

    \pram key the key code of the key that was pressed
    */
    void onMouseKeyUp(iKeyCode key);

    /*! called when mouse wheel was turned

    \param d mouse wheel delta
    */
	void onMouseWheel(int32 d);

    /*! called when ESC key was pressed
    */
	void onKeyESCDown();

    /*! called when window was closed
    */
	void onCloseWindow();

    /*! init example
    */
	void init();

    /*! deinit example
    */
	void deinit();

};

#endif

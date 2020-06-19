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
//    (c) Copyright 2014-2020 by Martin Loga
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

#ifndef __MOUSEEXAMPLE_H__
#define __MOUSEEXAMPLE_H__

#include <ExampleBase.h>
using namespace igor;

#include <iaux/math/iaVector2.h>
using namespace iaux;

class MouseExample : public ExampleBase
{

public:
    /*! init example
    */
    MouseExample();

    /*! does nothing
    */
    ~MouseExample() = default;

private:
    /*! called when mouse was moved
    \param from last mouse position
    \param to current mouse position
    \param window the window the coordinates are related to
    */
    void onMouseMovedFull(const iaVector2i &from, const iaVector2i &to, iWindow *window) override;

    /*! called when any mouse key was pressed

    \pram key the key code of the key that was pressed
    */
    void onMouseKeyDown(iKeyCode key) override;

    /*! called when any mouse key was released

    \param key the key code of the key that was pressed
    */
    void onMouseKeyUp(iKeyCode key) override;

    /*! called when mouse was double clicked

    \param key the key that was double clicked
    */
    void onMouseDoubleClick(iKeyCode key) override;

    /*! called when mouse wheel was turned

    \param d mouse wheel delta
    */
    void onMouseWheel(int32 d) override;
};

#endif // __MOUSEEXAMPLE_H__

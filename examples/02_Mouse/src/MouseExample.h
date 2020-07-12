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

class MouseExample : public ExampleBase
{

public:
    /*! init example

    \param window pass the window we work with
    */
    MouseExample(iWindow *window);

    /*! does nothing
    */
    ~MouseExample() = default;

private:
    /*! called on any other event

    \param event the event to handle
    */
    void onEvent(iEvent &event) override;

    /*! handles mouse key down event

    \param event the mouse key down event
    \returns true if consumed
    */
    bool onMouseKeyDownEvent(iEventMouseKeyDown &event);

    /*! handles mouse key up event

    \param event the mouse key up event
    \returns true if consumed
    */
    bool onMouseKeyUpEvent(iEventMouseKeyUp &event);

    /*! handles mouse key double click event

    \param event the mouse key double click event
    \returns true if consumed
    */
    bool onMouseKeyDoubleClickEvent(iEventMouseKeyDoubleClick &event);

    /*! handles mouse move event

    \param event the mouse move event
    \returns true if consumed
    */
    bool onMouseMoveEvent(iEventMouseMove &event);

    /*! handles mouse wheel event

    \param event the mouse wheel event
    \returns true if consumed
    */
    bool onMouseWheelEvent(iEventMouseWheel &event);
};

#endif // __MOUSEEXAMPLE_H__

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

#ifndef __KEYBOARDEXAMPLE_H__
#define __KEYBOARDEXAMPLE_H__

#include <ExampleBase.h>

/*! keyboard input example
*/
class KeyboardExample : public ExampleBase
{

public:
    /*! init example
    */
    KeyboardExample();

    /*! does nothing
    */
    ~KeyboardExample() = default;

private:
    /*! switches beween ascii output or single key output
    */
    bool _outputSwitch = true;

    /*! called when key was pressed

    \param event the event to handle
    */
    bool onKeyDown(iKeyDownEvent_TMP &event);

    /*! called when key was released

    \param event the event to handle
    */
    bool onKeyUp(iKeyUpEvent_TMP &event);

    /*! called on ascii input

    \param event the event to handle
    */
    bool onKeyASCIIInput(iKeyASCIIEvent_TMP &event);

    /*! initialize example
    */
    void onInit() override;

    /*! called on any other event
    */
    void onEvent(iEvent &event) override;
};

#endif // __KEYBOARDEXAMPLE_H__
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
// contact: martinloga@gmx.de

#ifndef __KEYBOARDEXAMPLE__
#define __KEYBOARDEXAMPLE__

#include <ExampleBase.h>
using namespace igor;

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

protected:
    /*! initialize example
    */
    void init() override;

    /*! deinitialize example
    */
    void deinit() override;

private:

    /*! switches beween ascii output or single key output
    */
    bool _outputSwitch;

    /*! called when key was pressed

    \param key the keycode of the pressed key
    */
    void onKeyPressed(iKeyCode key);

    /*! called when key was released

    \param key the keycode of the released key
    */
    void onKeyReleased(iKeyCode key);

    /*! called on ascii input

    \param c the ascii value
    */
    void onKeyASCIIInput(char c);

    /*! called when ESC key 
    */
    void onKeyESCPressed();
};

#endif

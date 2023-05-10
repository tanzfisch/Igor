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

#ifndef __FIRSTSTEPS_H__
#define __FIRSTSTEPS_H__

#include <ExampleBase.h>

/*! the first steps example class

it shows how to register to the application handle event.
*/
class FirstSteps : public ExampleBase
{

public:
    /*! init example
    */
    FirstSteps();

    /*! does nothing
    */
    ~FirstSteps() = default;

private:
    /*! let's count something
    */
    uint32 _counter = 0;

    /*! called every frame
    */
    void onUpdate() override;

    /*! initialize example
    */
    virtual void onInit() override;

    /*! deinitialize example
    */
    virtual void onDeinit() override;
};

#endif // __FIRSTSTEPS_H__

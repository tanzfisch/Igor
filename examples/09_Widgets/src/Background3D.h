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

#ifndef __BACKGROUND3D__
#define __BACKGROUND3D__

#include <ExampleBase.h>

// basically just copied the 3d example here
class Background3D : public ExampleBase
{

public:
    /*! initializes the example

    \param window the given window
    */
    Background3D(iWindowPtr window);

    /*! nothing to do
    */
    ~Background3D() = default;

private:
    /*! id to transform node of cat
    */
    iNodeID _catTransformID = iNode::INVALID_NODE_ID;

    /*! timer handle to control the movement of the light source over time
    */
    iTimerHandlePtr _animationTimingHandle = nullptr;

    /*! called by timer

    \param time the time
    */
    void onUpdate(const iaTime &time);

    /*! deinit example
    */
    void onDeinit() override;

    /*! init example
    */
    void onInit() override;

};

#endif // __BACKGROUND3D__

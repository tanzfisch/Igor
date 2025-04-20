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
//    (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef __EXAMPLE_SOUND__
#define __EXAMPLE_SOUND__

#include <ExampleBase.h>

/*! example for playing back sound
*/
class ExampleSound : public ExampleBase
{
public:
    /*! init example

    \param window the given window
    */
    ExampleSound(iWindowPtr window);

    /*! nothing to do
    */
    ~ExampleSound() = default;

private:
    /*! sound resource beep
    */
    iSoundPtr _soundBeep;

    /*! sound resource left right
    */
    iSoundPtr _soundLeftRight;

    /*! the transform the manipulates the audio source position
    */
    iNodeID _rotate = iNode::INVALID_NODE_ID;

    /*! timer handle to control the movement of the cat
    */
    iTimerHandle *_timerHandle = nullptr;

    /*! triggered by timer

    \param time the time
    */
    void onUpdate(const iaTime &time);

    /*! initialize example
    */
    void onInit() override;

    /*! deinitialize example
    */
    void onDeinit() override;
};

#endif // __EXAMPLE_SOUND__

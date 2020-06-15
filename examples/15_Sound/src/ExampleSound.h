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

#ifndef __EXAMPLE_SOUND_H__
#define __EXAMPLE_SOUND_H__

#include <ExampleBase.h>

#include <igor/resources/iResource.h>
#include <igor/audio/iAudio.h>
#include <igor/system/iTimerHandle.h>
using namespace igor;

/*! example for playing back sound
*/
class ExampleSound : public ExampleBase
{
public:
    /*! init example
    */
    ExampleSound();

    /*! nothing to do
    */
    ~ExampleSound() = default;

private:

    /*! sound resource beep
    */
    iResourcePtr _soundBeep;

    /*! sound resource left right
    */
    iResourcePtr _soundLeftRight;

    /*! the transform the manipulates the audio source position
    */
    iNodeID _rotate = iNode::INVALID_NODE_ID;

    /*! timer handle to control the movement of the cat
    */
    iTimerHandle* _timerHandle = nullptr;

    /*! triggered by timer
    */
    void onTimer();

    /*! initialize example
    */
    void init() override;

    /*! deinitialize example
    */
    void deinit() override;

};

#endif // __EXAMPLE_SOUND_H__

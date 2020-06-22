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

#ifndef __EXAMPLE_INSTANCING_H__
#define __EXAMPLE_INSTANCING_H__

#include <ExampleBase.h>

#include <igor/resources/material/iMaterial.h>
#include <igor/system/iTimerHandle.h>
using namespace igor;

class ExampleInstancing : public ExampleBase
{

public:
    /*! init members
    */
    ExampleInstancing();

    /*! does nothing
    */
    ~ExampleInstancing() = default;

private:
    /*! id to transform node used for manipulating the heading of the camera
    */
    uint64 _cameraHeading = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the pitch of the camera
    */
    uint64 _cameraPitch = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the distance of the camera to the origin
    */
    uint64 _cameraTranslation = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the direction of the light relative to the origin
    */
    uint64 _directionalLightRotate = iNode::INVALID_NODE_ID;

    /*! id to cat model node
    */
    uint64 _catModel = iNode::INVALID_NODE_ID;

    /*! timer handle to control the movement of the light source over time
    */
    iTimerHandlePtr _animationTimingHandle = nullptr;

    /*! instancing material
    */
    uint64 _materialWithInstancing = iMaterial::INVALID_MATERIAL_ID;

    /*! called when model was loaded

    \param modelNodeID the node id of the loaded model
    */
    void onModelReady(uint64 modelNodeID);

    /*! called by timer
    */
    void onTimer();

    /*! called when the mouse was moved

    \param from last mouse position
    \param to current mouse position
    \param window the window the coordinates are related to
    */
    void onMouseMovedFull(const iaVector2i &from, const iaVector2i &to, iWindow *window) override;

    /*! called when mouse wheel was turned

    \param d mouse wheel delta
    */
    void onMouseWheel(int32 d) override;

    /*! deinit example
    */
    void deinit() override;

    /*! init example
    */
    void init() override;
};

#endif // __EXAMPLE_INSTANCING_H__

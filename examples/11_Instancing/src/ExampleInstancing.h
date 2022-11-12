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

#ifndef __EXAMPLEINSTANCING_H__
#define __EXAMPLEINSTANCING_H__

#include <ExampleBase.h>

class ExampleInstancing : public ExampleBase
{

public:
    /*! init members

    \param window the given window
    */
    ExampleInstancing(iWindowPtr window);

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
    uint64 _materialWithInstancingA = iMaterial::INVALID_MATERIAL_ID;
    uint64 _materialWithInstancingB = iMaterial::INVALID_MATERIAL_ID;

    /*! perlin noise generator
    */
    iPerlinNoise _perlinNoise;

    /*! called when model was loaded

    \param modelNodeID the node id of the loaded model
    */
    void onModelReadyA(uint64 modelNodeID);
    void onModelReadyB(uint64 modelNodeID);

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

    /*! called on any other event

    \param event the event to handle
    */
    void onEvent(iEvent &event) override;

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

#endif // __EXAMPLEINSTANCING_H__

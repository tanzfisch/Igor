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

#ifndef __EXAMPLE3D_H__
#define __EXAMPLE3D_H__

#include <ExampleBase.h>

class Example3D : public ExampleBase
{

public:
    /*! initializes the example

    \param window the given window
    */
    Example3D(iWindow *window);

    /*! nothing to do
    */
    ~Example3D() = default;

private:
    /*! id to transform node used for manipulating the heading of all objects in scene
    */
    iNodeID _allObjectsHeading = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the pitch of all objects in scene
    */
    iNodeID _allObjectsPitch = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the heading of the camera
    */
    iNodeID _cameraHeading = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the pitch of the camera
    */
    iNodeID _cameraPitch = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the distance of the camera to the origin
    */
    iNodeID _cameraTranslation = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the direction of the light relative to the origin
    */
    iNodeID _directionalLightRotate = iNode::INVALID_NODE_ID;

    /*! timer handle to control the movement of the light source over time
    */
    iTimerHandlePtr _animationTimingHandle = nullptr;

    /*! id to crate model node
    */
    iNodeID _crateModel = iNode::INVALID_NODE_ID;

    /*! id to cat model node
    */
    iNodeID _catModel = iNode::INVALID_NODE_ID;

    /*! id to teapot model node
    */
    iNodeID _teapotModel = iNode::INVALID_NODE_ID;

    /*! id to switch node
    */
    iNodeID _switchNode = iNode::INVALID_NODE_ID;

    /*! index of switch position
    */
    iNodeID _activeNode = 0;

    /*! id to lod trigger node
    */
    iNodeID _LODTrigger = iNode::INVALID_NODE_ID;

    /*! id to lod switch node
    */
    iNodeID _LODSwitch = iNode::INVALID_NODE_ID;

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

    /*! handles mouse key down event

    \param event the mouse key down event
    \returns true if consumed
    */
    bool onMouseKeyDownEvent(iEventMouseKeyDown &event);

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

    /*! called when key was pressed

    \param event the event to handle
    */
    bool onKeyDown(iEventKeyDown &event);
};

#endif // __EXAMPLE3D_H__

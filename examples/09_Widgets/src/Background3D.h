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

#ifndef __BACKGROUND3D_H__
#define __BACKGROUND3D_H__

#include <igor/igor.h>
using namespace igor;
using namespace iaux;

class Background3D : public iLayer
{

public:
    /*! initializes the example

    \param window the given window
    */
    Background3D(iWindow *window);

    /*! nothing to do
    */
    ~Background3D() = default;

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

    /*! the scene
    */
    iScene *_scene;

    /*! the view we render 3D to
    */
    iView _view;

    /*! called by timer
    */
    void onTimer();

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
    bool onMouseKeyDownEvent(iMouseKeyDownEvent_TMP &event);

    /*! handles mouse move event

    \param event the mouse move event
    \returns true if consumed
    */
    bool onMouseMoveEvent(iMouseMoveEvent_TMP &event);

    /*! handles mouse wheel event

    \param event the mouse wheel event
    \returns true if consumed
    */
    bool onMouseWheelEvent(iMouseWheelEvent_TMP &event);
};

#endif // __BACKGROUND3D_H__

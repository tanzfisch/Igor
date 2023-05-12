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

#ifndef __LSYSTEMSEXAMPLE_H__
#define __LSYSTEMSEXAMPLE_H__

#include <ExampleBase.h>

/*! the LSystems example class
*/
class LSystems : public ExampleBase
{

public:
    /*! init

    \param window the given window
    */
    LSystems(iWindowPtr window);

    /*! does nothing
    */
    ~LSystems() = default;

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

    /*! timer handle to control the movement of the light source over time
    */
    iTimerHandle *_animationTimingHandle = nullptr;

    /*! counter to switch between styles of L-Systems
    */
    uint32 _styleCounter = 0;

    /*! incarnation counter
    */
    uint32 _incarnation = 1;

    /*! the L-System
    */
    iLSystem _lSystem;

    /*! segment length
    */
    float32 _segmentLength = 0;

    /*! angle between bones
    */
    float32 _angle = 0;

    /*! color of trunk
    */
    iaColor3f _trunkColor;

    /*! color of branch
    */
    iaColor3f _branchColor;

    /*! color of bud
    */
    iaColor3f _budColor;

    /*! color of flower
    */
    iaColor3f _flowerColor;

    /*! color of leaf
    */
    iaColor3f _leafColor;

    /*! node ids of plants in progress
    */
    std::vector<uint64> _plantsInProgress;

    /*! group node where we put the L-System model nodes in so we can delete them all at once
    */
    uint64 _groupNodeID = iNode::INVALID_NODE_ID;

    /*! \returns true if all plants are done creating
    */
    bool checkIfDone();

    /*! generates L-Systems
    */
    void generateLSystems();

    /*! init L-System with style 1
    */
    void initStyle1();

    /*! init L-System with style 2
    */
    void initStyle2();

    /*! init L-System with style 3
    */
    void initStyle3();

    /*! generates plant at specified position

    \param matrix matrix to position the plant
    \param axiom start string to generate L-System
    \param iterations iteration count
    \param seed random seed
    \retruns id of generated plant node
    */
    uint64 generatePlant(const iaMatrixd &matrix, const iaString &axiom, uint32 iterations, uint64 seed);

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

    /*! called when key was pressed

    \param event the event to handle
    */
    bool onKeyDown(iEventKeyDown &event);
};

#endif // __LSYSTEMSEXAMPLE_H__

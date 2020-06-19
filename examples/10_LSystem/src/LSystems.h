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

#ifndef __LSYSTEMS_EXAMPLE_H__
#define __LSYSTEMS_EXAMPLE_H__

#include <ExampleBase.h>

#include <igor/system/iTimerHandle.h>
#include <igor/generation/iLSystem.h>
using namespace igor;

#include <iaux/math/iaMatrix.h>
using namespace iaux;

/*! the LSystems example class
*/
class LSystems : public ExampleBase
{

public:
    /*! init
    */
    LSystems();

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

    /*! segment lenght
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

    /*! called on key pressed event

    \param key the key code of the pressed key
    */
    void onKeyDown(iKeyCode key) override;
};

#endif // __LSYSTEMS_EXAMPLE_H__

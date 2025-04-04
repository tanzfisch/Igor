//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin Loga
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

#ifndef __CAMERAARC__
#define __CAMERAARC__

#include <igor/igor.h>
using namespace igor;

/*! arc controlled camera
*/
class CameraArc
{
public:
    /*! init members

    \param node the node where to insert the camera
    */
    CameraArc(iNodePtr node);

    /*! \return current heading in rad
    */
    float64 getHeading() const;

    /*! sets new heading

    \param heading the new heading
    */
    void setHeading(float64 heading);

    /*! \return current pitch in rad
    */
    float64 getPitch() const;

    /*! sets new pitch

    \param pitch the new pitch
    */
    void setPitch(float64 pitch);

    /*! \return current distance to center of interest
    */
    float64 getDistance() const;

    /*! sets new distance to center of interest

    \param distance the new distance
    */
    void setDistance(float64 distance);

    /*! sets the center of interest

    \param coi the new center of interest
    */
    void setCenterOfInterest(const iaVector3d &coi);

    /*! \returns center of interest
    */
    iaVector3d getCenterOfInterest() const;

    /*! \returns camera node id
    */
    iNodeID getCameraNode() const;

    /*! \returns camera distance node id
    */
    iNodeID getCameraDistanceNode() const;

    /*! returns world transformation of camera

    \param[out] matrix the returing world matrix of the camera
    */
    void getWorldTransformation(iaMatrixd &matrix) const;

private:
    /*! center of interest node
    */
    iNodeID _cameraCOI = iNode::INVALID_NODE_ID;

    /*! camera heading node
    */
    iNodeID _cameraHeading = iNode::INVALID_NODE_ID;

    /*! camera pitch node
    */
    iNodeID _cameraPitch = iNode::INVALID_NODE_ID;

    /*! camera distance to center of interes node
    */
    iNodeID _cameraDistance = iNode::INVALID_NODE_ID;

    /*! camera node
    */
    iNodeID _camera = iNode::INVALID_NODE_ID;

    /*! heading
    */
    float64 _heading = 0.0;

    /*! pitch
    */
    float64 _pitch = 0.0;

    /*! distance to center of interest
    */
    float64 _distance = 10.0;
};

/*! camera arc pointer definition
*/
typedef CameraArc *CameraArcPtr;

#endif // __CAMERAARC__

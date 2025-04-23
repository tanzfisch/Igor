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
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef MICA_CAMERA_ARC_H
#define MICA_CAMERA_ARC_H

#include <igor/igor.h>
using namespace igor;

/*! arc controlled camera
 */
class CameraArc
{
public:
    /*! init members

    \param entitySceneID the scene to use this camera in
    \param entityID the anchor entity to attach this camera to
    */
    CameraArc(const iEntitySceneID &entitySceneID, const iEntityID &entityID);

    ~CameraArc();

    /*! \return current heading in rad
     */
    float64 getHeading() const;

    /*! sets new heading

    \param heading the new heading in rad
    */
    void setHeading(float64 heading);

    /*! \return current pitch in rad
     */
    float64 getPitch() const;

    /*! sets new pitch

    \param pitch the new pitch in rad
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

    /*! returns world transformation of camera

    \param[out] matrix the returing world matrix of the camera
    */
    void getWorldTransformation(iaMatrixd &matrix) const;

private:

    /*! entity scene to use
    */
    iEntitySceneID _entitySceneID = iEntitySceneID::getInvalid();

    /*! anchor entity
    */
    iEntityID _entityID = iEntityID::getInvalid();

    /*! center of interest node
     */
    iEntityID _cameraCOI = iEntityID::getInvalid();

    /*! camera heading node
     */
    iEntityID _cameraHeading = iEntityID::getInvalid();

    /*! camera pitch node
     */
    iEntityID _cameraPitch = iEntityID::getInvalid();

    /*! camera distance to center of interest node
     */
    iEntityID _cameraDistance = iEntityID::getInvalid();

    /*! camera node
     */
    iEntityID _camera = iEntityID::getInvalid();
};

/*! camera arc pointer definition
 */
typedef CameraArc *CameraArcPtr;

#endif // MICA_CAMERA_ARC_H

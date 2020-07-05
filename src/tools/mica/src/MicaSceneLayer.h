
//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2014-2020 by Martin Loga
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

#ifndef __MICASCENELAYER_H__
#define __MICASCENELAYER_H__

#include "Workspace.h"

class MicaSceneLayer : public iLayer
{

public:
    /*! init members
	*/
    MicaSceneLayer(iWindow *window, int32 zIndex);

    /*! deinit resources
	*/
    ~MicaSceneLayer();

    /*! \returns node at given screen position
	\param x horizonral screen position
	\param y vertical screen position
	*/
    iNodePtr getNodeAt(int32 x, int32 y);

    /*! frames the view on current selection
	*/
    void frameOnSelection();

    /*! returns camera transformation matrix

    \param[out] matrix the returned matrix
    */
    void getCamTransformation(iaMatrixd &matrix) const;

    /*! returns the camera center of interest

    \param[out] matrix the returned matrix
    */
    void getCamCOI(iaMatrixd &matrix) const;

    /*! rotates the camera by given values

    \param heading the heading change in rad
    \param pitch the pitch change in rad
    */
    void rotateCamera(float64 heading, float64 pitch);

    /*! translate the center of interesst of the camera by given vector

    \param translate the given vector
    */
    void translateCOI(const iaVector3d &translate);

private:
    /*! main scene view
	*/
    iView _view;

    /*! the workspace inclusing the scene
    */
    Workspace _workspace;

    // TODO need some classes handling different types of cameras
    iNodeTransform *_cameraCOI = nullptr;
    iNodeTransform *_cameraHeading = nullptr;
    iNodeTransform *_cameraPitch = nullptr;
    iNodeTransform *_cameraTranslation = nullptr;
    iNodeCamera *_camera = nullptr;
    float32 _camDistance = 0;

    // TODO need to handle light differently
    iNodeTransform *_directionalLightTranslate = nullptr;
    iNodeTransform *_directionalLightRotate = nullptr;
    iNodeLight *_lightNode = nullptr;

    /*! clear resources
	*/
    void onDeinit() override;

    /*! init layer
    */
    void onInit() override;

    /*! called on application pre draw event
    */
    void onPreDraw() override;

    /*! updates camera transform based on configured distance
    */
    void updateCamDistanceTransform();
};

#endif // __MICASCENELAYER_H__

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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __IGOR_CAMERA_H__
#define __IGOR_CAMERA_H__

#include <iaux/data/iaString.h>
using namespace iaux;

#include <iaux/math/iaMatrix.h>
#include <igor/scene/nodes/iNode.h>

#include <vector>
#include <map>

namespace igor
{

    class iScene;

    /*! camera node
    */
    class IGOR_API iNodeCamera : public iNode
    {

        friend class iNodeVisitorUpdateTransform;
        friend class iNodeManager;

    public:
        /*! returns world matrix of camera

        \param matrix in out value to return the world matrix
        */
        void getWorldMatrix(iaMatrixd &matrix);

        /*! returns the view matrix of the camera

        \param viewmatrix in out value to return the cameras view matrix
        */
        void getViewMatrix(iaMatrixd &viewmatrix);

    protected:
        /*! matrix including position an orientation of this camera in world coordinates
        */
        iaMatrixd _worldMatrix;

        /*! unregisters camera from scene if it belonged to a scene before
        */
        virtual void onPreSetScene() override;

        /*! registers to new scene
        */
        virtual void onPostSetScene() override;

        /*! set the world matrix of the camera

        \param matrix world matrix for the camera
        */
        void onUpdateTransform(iaMatrixd &matrix);

    private:
        /*! init member variables
        */
        iNodeCamera();

        /*! copy ctor
        */
        iNodeCamera(iNodeCamera *node);

        /*! dtor
		*/
        virtual ~iNodeCamera();
    };

    /*! camera node pointer definition
    */
    typedef iNodeCamera *iNodeCameraPtr;

}; // namespace igor

#endif // __IGOR_CAMERA_H__

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

#ifndef IGOR_RENDER_ENGINE_H
#define IGOR_RENDER_ENGINE_H

#include <igor/entities/iEntityScene.h>
#include <igor/data/iFrustum.h>
#include <igor/renderer/buffers/iInstancingBuffer.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/mesh/iMesh.h>

#include <iaux/math/iaMatrix.h>

namespace igor
{

    /*! feeds the renderer with pre filtered data
     */
    class iRenderEngine
    {
    public:
        /*! does nothing
         */
        iRenderEngine() = default;

        /*! does nothing
         */
        virtual ~iRenderEngine() = default;

        /*! sets the scene to render

        \param sceneID id of scene to render
         */
        void setSceneID(const iEntitySceneID &sceneID);

        /*! \returns scene id
        */
        const iEntitySceneID& getSceneID() const;

        /*! sets the current camera

        \param cameraID ID of entity that contains iCameraComponent and iTransformComponent
        */
        void setCameraID(const iEntityID &cameraID);

        /*! \returns camera id
        */
       const iEntityID& getCameraID() const;

        /*! add mesh for render queue

        \param mesh entity that contains iTransformComponent and iMeshRenderComponent
        */
        void addMesh(iEntityPtr mesh);

        /*! renders given data

        \param viewport the given viewport to render in
        */
        void render();

        /*! \returns current frustum

        valid after render/setupCamera
        */
        const iFrustumd &getFrustum() const;

        /*! set frustum

        \param frustum the frustum to set
        */
        void setFrustum(const iFrustumd &frustum);

        /*! set frustum by matrix

        \param matrix the matrix for the frustum
        */
        void setFrustum(const iaMatrixd &matrix);

    private:
        /*! the scene to render
         */
        iEntitySceneID _sceneID = iEntitySceneID::getInvalid();

        /*! camera ID
         */
        iEntityID _cameraID = iEntityID::getInvalid();

        /*! current frustum
         */
        iFrustumd _frustum;

        struct iInstaningPackage
        {
            iInstancingBufferPtr _buffer;
            iMaterialPtr _material; // TODO needs to be part of the buffer so individual instances can have different colors etc
        };

        /*! bringing all nodes using the same material together for more efficient rendering
         */
        struct iMaterialGroup
        {
            /*! the shader used
             */
            iShaderPtr _shader;

            /*! optional instancing buffers per mesh that is using the same material
             */
            std::unordered_map<iMeshPtr, iInstaningPackage> _instancing;
        };

        /*! render nodes
         */
        std::vector<iMaterialGroup> _materialGroups;

        /*! renders all mesh instances
         */
        void renderInstances();
    };
} // namespace igor

#endif // IGOR_RENDER_ENGINE_H
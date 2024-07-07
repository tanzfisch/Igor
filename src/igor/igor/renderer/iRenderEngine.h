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
// (c) Copyright 2012-2024 by Martin Loga
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
         */
        void setScene(iEntityScenePtr scene);

        /*! sets the current camera

        \param camera entity that contains iCameraComponent and iTransformComponent
        */
        void setCamera(iEntityPtr camera);

        /*! add mesh for render queue

        \param mesh entity that contains iTransformComponent and iMeshRenderComponent
        */
        void addMesh(iEntityPtr mesh);

        /*! setup camera for render

        \param camera the camera to use for setup
        \param viewport the viewport given by the parent view
        */
        void setupCamera(const iaRectanglei &viewport);

        /*! renders given data

        \param viewport the given viewport to render in
        */
        void render();

        /*! \returns current frustum

        valid after render/setupCamera
        */
        const iFrustumd &getFrustum() const;

    private:
        /*! the scene to render
         */
        iEntityScenePtr _scene = nullptr;

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
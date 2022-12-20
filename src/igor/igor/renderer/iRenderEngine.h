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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __IGOR_RENDERENGINE_H__
#define __IGOR_RENDERENGINE_H__

#include <igor/resources/material/iMaterialGroup.h>
#include <igor/scene/iScene.h>
#include <igor/resources/profiler/iProfiler.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/scene/nodes/iNodeCamera.h>

#include <vector>
#include <unordered_map>

namespace igor
{

    /*! does control the render loop
     */
    class IGOR_API iRenderEngine
    {
    public:
        /*! does nothing
        */
        iRenderEngine() = default;

        /*! does nothing
         */
        virtual ~iRenderEngine() = default;

        /*! defines if bounding boxes are shown or not

        \param boundingBox if true bounding boxes are shown
        */
        void setBoundingBoxVisible(bool boundingBox = true);

        /*! \returns true if bounding boxes are shown
         */
        bool isBoundingBoxVisible() const;

        /*! defines if octree is shown or not

        \param octree if true octree is shown
        */
        void setOctreeVisible(bool octree = true);

        /*! \returns true if octree is shown
         */
        bool isOctreeVisible() const;

        /*! sets if the nodes should be rendered with solely their colorid

        \param enabled if true nodes will be rendered with their color id
        */
        void setColorIDRendering(bool enabled = true);

        /*! \returns if nodes will be rendered with their color id
         */
        bool isColorIDRendering() const;

        /*! sets the scene to render with

        \param scene scene to render
        */
        void setScene(iScenePtr scene);

        /*! \returns pointer to scene
         */
        iScenePtr getScene() const;

        /*! sets current camera by id

        \param cameraID the camery id
        */
        void setCurrentCamera(iNodeID cameraID);

        /*! \returns current camera id
         */
        iNodeID getCurrentCamera() const;

        /*! culls and renders
         */
        void render();

    private:
        /*! current camera
         */
        iNodeCamera *_currentCamera = nullptr;

        /*! flag if rendering uses wireframe
         */
        bool _showWireframe = false;

        /*! flag if bounding boxes are drawn
         */
        bool _showBoundingBoxes = false;

        /*! flag if octree will be rendered
         */
        bool _showOctree = false;

        /*! flag if true color ids get rendered
         */
        bool _renderColorID = false;

        /*! material groups
         */
        std::unordered_map<iMaterialPtr, iMaterialGroup> _materialGroups;

        /*! handle to scene
         */
        iScenePtr _scene = nullptr;

        /*! temporary list of nodes that where filtered by the culling process
         */
        std::vector<iNodeID> _cullResult;

        /*! cull scene relative to specified camera

        \param camera the specified camera
        */
        void cullScene(iNodeCameraPtr camera);

        /*! updates material groups
         */
        void updateMaterialGroups();

        /*! draw scene relative to specified camera

        \param camera the specified camera
        */
        void drawScene();

        /*! draws everyting by using it's color id
         */
        void drawColorIDs();

    };
} // namespace igor

#endif // __IGOR_RENDERENGINE_H__
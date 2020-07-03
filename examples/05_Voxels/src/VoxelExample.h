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

#ifndef __VOXELEXAMPLE_H__
#define __VOXELEXAMPLE_H__

#include <ExampleBase.h>

/*! the voxel example
*/
class VoxelExample : public ExampleBase
{

public:
    /*! init
    */
    VoxelExample();

    /*! does nothing
    */
    ~VoxelExample() = default;

private:
    /*! heading of camera
    */
    uint64 _cameraHeading = iNode::INVALID_NODE_ID;

    /*! pitch of camera
    */
    uint64 _cameraPitch = iNode::INVALID_NODE_ID;

    /*! the model that represents our voxel mesh
    */
    uint64 _voxelMeshModel = iNode::INVALID_NODE_ID;

    /*! transform node for voxel mesh
    */
    uint64 _voxelMeshTransform = iNode::INVALID_NODE_ID;

    /*! voxel mesh material
    */
    uint64 _voxelMeshMaterialID = iMaterial::INVALID_MATERIAL_ID;

    /*! the actual voxel data
    */
    iVoxelData *_voxelData = nullptr;

    /*! perlin noise generator
    */
    iPerlinNoise _perlinNoise;

    /*! flag to show loading text
    */
    bool _loading = true;

    /*! measure some time
    */
    iaTime _time;

    /*! random number generator
    */
    iaRandomNumberGeneratoru _rand;

    /*! incarnation of mesh
    */
    uint32 _incarnation = 1;

    /*! material definition for the sky box
    */
    iMaterialID _materialSkyBox = iMaterial::INVALID_MATERIAL_ID;

    /*! initialize scene
    */
    void initScene();

    /*! register some callbacks
    */
    void registerHandles();

    /*! unregister the callbacks
    */
    void unregisterHandles();

    /*! generate some voxel data and meshs
    */
    void generateVoxelData();

    /*! triggers mesh generation
    */
    void prepareMeshGeneration();

    /*! called before every frame
    */
    void onPreDraw() override;

    /*! initialize everything
    */
    void onInit() override;

    /*! deinitialize everything
    */
    void onDeinit() override;

    /*! called by orthogonal view
    */
    void onRenderOrtho() override;

    /*! called on any other event

    \param event the event to handle
    */
    void onEvent(iEvent &event) override;

    /*! called when key was pressed

    \param event the event to handle
    */
    bool onKeyDown(iKeyDownEvent_TMP &event);

    /*! handles mouse move event

    \param event the mouse move event
    \returns true if consumed
    */
    bool onMouseMoveEvent(iMouseMoveEvent_TMP &event);
};

#endif // __VOXELEXAMPLE_H__

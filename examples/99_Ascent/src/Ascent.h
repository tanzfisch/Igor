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

#ifndef __ASCENT__
#define __ASCENT__

#include <igor/igor.h>

class Ascent : public iLayer
{

public:
    /*! init members

    \param window the given window
    */
    Ascent(iWindow *window);

    /*! does nothing
     */
    ~Ascent() = default;

private:
    /*! 3d view
     */
    iView _view;

    /*! orthogonal view for HUD
     */
    iView _viewOrtho;

    /*! generate some random numbers
     */
    iaRandomNumberGeneratoru _rand;

    /*! need some noise
     */
    iPerlinNoise _perlinNoise;

    /*! the render scene
     */
    iScenePtr _scene = nullptr;

    /*! the voxel terrain
    */
    iVoxelTerrainPtr _voxelTerrain = nullptr;

    /*! font for HUD
     */
    iTextureFont *_font = nullptr;

    /*! flush models task handle
     */
    uint64 _taskFlushModels = iTask::INVALID_TASK_ID;

    /*! flush textures task handle
     */
    uint64 _taskFlushTextures = iTask::INVALID_TASK_ID;

    iPhysicsMaterialID _terrainMaterialID = iPhysicsMaterial::INVALID_PHYSICSMATERIAL_ID;
    iPhysicsMaterialID _entityMaterialID = iPhysicsMaterial::INVALID_PHYSICSMATERIAL_ID;
    iPhysicsMaterialID _bulletMaterialID = iPhysicsMaterial::INVALID_PHYSICSMATERIAL_ID;

    bool _captureMouse = true;
    bool _loading = true;
    bool _activeControls = false;

    uint64 _toolSize = 3;
    uint8 _toolDensity = 0;

    iaVector2f _mouseDelta;
    iaVector3f _weaponPos;

    uint32 _materialWithTextureAndBlending = iMaterial::INVALID_MATERIAL_ID;
    uint32 _octreeMaterial = iMaterial::INVALID_MATERIAL_ID;
    int32 _materialSkyBox = iMaterial::INVALID_MATERIAL_ID;

    iaMutex _hitListMutex;
    std::vector<std::pair<uint64, uint64>> _hitList;

    std::vector<iSphered> _metaballs;
    std::vector<iSphered> _holes;
    

    int _enemyCount = 0;

    void outlineLevelStructure();
    void deinitVoxelData();
    void initVoxelData();
    void onGenerateVoxelData(iVoxelBlockInfo *voxelBlockInfo);
    void onVoxelDataGenerated(iVoxelBlockPropsInfo voxelBlockInfo);

    bool getTerrainIntersectionPoint(iaVector3I &intersection);
    void dig(iaVector3I position, uint64 toolSize, uint8 toolDensity);

    void onRenderOrtho();

    void initViews();
    void initScene();
    void initPlayer();

    void initPhysics();
    void onContactTerrainBullet(iPhysicsBody *body0, iPhysicsBody *body1);
    void onContact(iPhysicsBody *body0, iPhysicsBody *body1);

    /*! called when added to layer stack
     */
    void onInit() override;

    /*! called when removed from layer stack
     */
    void onDeinit() override;

    /*! called on application pre draw event
     */
    void onPreDraw() override;

    /*! called on any other event
     */
    void onEvent(iEvent &event) override;

    /*! handles mouse key down event

    \param event the mouse key down event
    \returns true if consumed
    */
    bool onMouseKeyDownEvent(iEventMouseKeyDown &event);

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

    /*! called when key was released

    \param event the event to handle
    */
    bool onKeyUp(iEventKeyUp &event);

    /*! handle window resize event

    \param event the window resize event
    \returns true if consumed
    */
    bool onWindowResize(iEventWindowResize &event);
};

#endif

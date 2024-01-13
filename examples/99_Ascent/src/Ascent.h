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

#include <ExampleBase.h>

// replace with entity later
struct TempKey
{
    bool _left = false;
    bool _right = false;
    bool _front = false;
    bool _back = false;
    bool _up = false;
    bool _down = false;
    bool _rollLeft = false;
    bool _rollRight = false;
};

class Ascent : public ExampleBase
{

public:
    /*! initializes the example

    \param window the given window
    */
    Ascent(iWindowPtr window);

    /*! nothing to do
     */
    ~Ascent() = default;

private:
    /*! voxel mesh material
     */
    iShaderPtr _voxelMeshMaterial;

    /*! voxel terrain
     */
    std::unique_ptr<iVoxelTerrain> _voxelTerrain;

    /*! makes noise
     */
    iPerlinNoise _perlinNoise;

    /*! list of balls to generate base shape of terrain
    */
    std::vector<iaSphered> _metaballs;

    /*! list of inverted balls to generate base shape of terrain
    */
    std::vector<iaSphered> _holes;

    // TODO
    uint64 _terrainMaterialID;
    uint64 _entityMaterialID;
    uint64 _bulletMaterialID;    

    // TODO 
    TempKey _keyInput;
    iNodeTransformPtr _cameraTransform;

    /*! init voxel data
     */
    void initVoxelData(uint32 lodTriggerID);

    void outlineLevelStructure();

    void onGenerateVoxelData(iVoxelBlockInfo *voxelBlockInfo);
    void onVoxelDataGenerated(iVoxelBlockPropsInfo voxelBlockInfo);

    bool getTerrainIntersectionPoint(iaVector3I &intersection);
    void dig(iaVector3I position, uint64 toolSize, uint8 toolDensity);

    void handleHitList();

    void onRender();

    void initScene();

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
    void onUpdate() override;

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
};

#endif // __ASCENT__

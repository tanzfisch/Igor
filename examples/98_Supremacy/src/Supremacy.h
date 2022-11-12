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

#ifndef __SUPREMACY__
#define __SUPREMACY__

#include "Defines.h"

/*! Supremacy
 */
class Supremacy : public iLayer
{

public:

    /*! ctor
    */
    Supremacy(iWindowPtr window);

    /*! does nothing
    */
    ~Supremacy() = default;

private:
    using Quadtree = iQuadtree<float64, iEntityID>;
    using QuadtreeObject = iQuadtreeObject<float64, iEntityID>;
    using QuadtreeObjectPtr = std::shared_ptr<iQuadtreeObject<float64, iEntityID>>;
    using QuadtreeObjects = std::vector<std::shared_ptr<iQuadtreeObject<float64, iEntityID>>>;

    /*! the view we render 2D to
     */
    iView _viewOrtho;

    /*! material id of a non textured material
     */
    uint64 _materialWithTextureAndBlending = iMaterial::INVALID_MATERIAL_ID;

    /*! entity scene
     */
    iEntityScene _entityScene;

    /*! handle to player entity
     */
    iEntity _player;

    /*! viewport entity which follows the player
     */
    iEntity _viewport;

    /*! random numbers
     */
    iaRandomNumberGeneratoru _rand;

    /*! update timer
     */
    iTimerHandlePtr _updateTimerHandle;

    /*! async loading of textures
     */
    iTaskID _taskFlushTextures = iTask::INVALID_TASK_ID;

    /*! quadtree
     */
    Quadtree _quadtree;

    /*! floor shadow
     */
    iTexturePtr _shadow;

    /*! texture font we use to display texts
     */
    iTextureFontPtr _font;

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

    /*! called when render ortho viewport
     */
    void onRenderOrtho();

    /*! renders HUD
    */
    void onRenderHUD();

    /*! game logic intervall

    \param time the time
     */
    void onUpdate(const iaTime &time);

    /*! called when key was pressed

    \param event the event to handle
    */
    bool onKeyDown(iEventKeyDown &event);

    /*! called when key was released

    \param event the event to handle
    */
    bool onKeyUp(iEventKeyUp &event);

    /*! \returns random direction
     */
    iaVector2d getRandomDir();

    iEntity createPlayer();

    iEntity createViewport(iEntityID targetID);

    void createUnit(const iaVector2d &pos, uint32 party, iEntityID target);

    void onUpdateQuadtreeSystem();

    void onUpdateMovementControlSystem();

    void onUpdateFollowTargetSystem();

    void onUpdatePositionSystem();

    void onUpdateRangeSystem();

    void onUpdateOrientationSystem();

    void onUpdateCleanUpTheDeadSystem();

    void onUpdateWeaponSystem();

    void aquireTargetFor(iEntity &entity);

    void fire(const iaVector2d &from, const iaVector2d &dir, uint32 party, float64 damage, float64 speed, float64 range, WeaponType waponType);

    void updateViewRectangleSystem();

    /*! query a circle on the quardtree while wrapping arround at the edges like a doughnut

    \param circle the query circle
    \param hits the resulting list of entitties and relatice to the circle center positions
    */
    void doughnutQuery(const iaCircled &circle, std::vector<std::pair<iEntityID, iaVector2d>> &hits);

    bool intersectDoughnut(const iaVector2d &position, const iaRectangled &rectangle, iaVector2d &offset);
    bool intersectDoughnut(const iaVector2d &position, const iaCircled &circle, iaVector2d &offset);
};

#endif // __SUPREMACY__

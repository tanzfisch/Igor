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

#ifndef __SUPREMACY_GAMELAYER__
#define __SUPREMACY_GAMELAYER__

#include "Defines.h"
#include "UpgradeDialog.h"
#include "ShopDialog.h"

#include <tinyxml.h>

/*! GameLayer
 */
class GameLayer : public iLayer
{

public:
    /*! ctor
     */
    GameLayer(iWindowPtr window);

    /*! does nothing
     */
    ~GameLayer() = default;

private:
    /*! the view we render 2D to
     */
    iView _viewOrtho;

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

    iTimerHandlePtr _statsTimerHandle;
    iTimerHandlePtr _spawnTimerHandle;

    /*! async loading of textures
     */
    iTaskID _taskFlushTextures = iTask::INVALID_TASK_ID;

    /*! quadtree
     */
    iQuadtreed _quadtree;

    /*! floor shadow
     */
    iTexturePtr _shadow;

    /*! floor
     */
    iTexturePtr _backgroundTexture;

    /*! texture font we use to display texts
     */
    iTextureFontPtr _font;

    /*! coin texture
     */
    iTexturePtr _coin;

    iTexturePtr _damage;

    iTexturePtr _attackSpeed;

    iTexturePtr _walkSpeed;

    BuildingType _currentBuilding = BuildingType::None;

    /*! if true game logic is on hold
     */
    bool _gamePause = false;

    bool _levelUp = false;

    std::vector<uint32> _expLvl;

    std::vector<UpgradeConfiguration> _upgrades;
    std::vector<ShopItem> _shopItems;

    UpgradeDialog *_levelUpDialog = nullptr;
    ShopDialog *_shopDialog = nullptr;

    void upgrade(iEntity entity, const UpgradeConfiguration &upgradeConfiguration);

    void onCloseLevelUpDialog(iDialogPtr dialog);
    void onCloseShopDialog(iDialogPtr dialog);

    void onOpenBuilding(BuildingType buildingType);

    void pause();
    void play();

    void resetKeyboardInput();

    void openShop();

    /*! called when added to layer stack
     */
    void onInit() override;

    void initExpLvlTable();

    void loadSpecs(const iaString &filename);
    void readUpgrades(TiXmlElement *upgrades);
    void readShopItems(TiXmlElement *shopItems);

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

    void onRenderStats();

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

    void createShop(const iaVector2d &pos);

    void createObject(const iaVector2d &pos, uint32 party, ObjectType objectType);

    void onUpdateQuadtreeSystem();

    void onUpdateMovementControlSystem();

    void onUpdateFollowTargetSystem();

    void onUpdatePositionSystem();

    void onUpdateCollisionSystem();

    void onUpdatePickupSystem(iEntity &entity);

    BuildingType onCheckForBuildingsNearBy(iEntity &entity);

    void onUpdateRangeSystem();

    void onUpdateOrientationSystem();

    void onUpdateCleanUpTheDeadSystem();

    void onUpdateWeaponSystem();

    void onSpawnStuff(const iaTime &time);

    void onUpdateStats(const iaTime &time);

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

    float64 calcLevel(uint32 experience);
    void addExperience(iEntity &entity, float64 experience);

    /*! called on level up
     */
    void onLevelUp();

    struct GameStats
    {
        float32 _playerDamage = 0.0;
        float32 _playerExperience = 0.0;
        float32 _playerLevel = 0.0;
        float32 _playerCoins = 0.0;
        float32 _enemyHealth = 0.0;
    };

    std::vector<GameStats> _stats;

    std::set<iEntityID> _deleteQueue;
};

#endif // __SUPREMACY_GAMELAYER__

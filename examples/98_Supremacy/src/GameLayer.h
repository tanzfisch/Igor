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
//    (c) Copyright 2012-2024 by Martin Loga
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

struct EnemyClass
{
    iaString _name;
    iaString _description;
    iaString _texture;
    float32 _damage;
    float32 _health;
    float32 _size;
    float32 _speed;
    float32 _xpDrop;
    float32 _coinDrop;
};

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
    iEntityScenePtr _entityScene = nullptr;

    /*! handle to player entity
     */
    iEntityID _player;

    /*! handle to player entity
     */
    iEntityID _shop;

    /*! viewport entity which follows the player
     */
    iEntityID _camera;

    /*! update timer
     */
    iTimerHandlePtr _updateTimerHandle = nullptr;

    /*! stats timer
     */
    iTimerHandlePtr _statsTimerHandle = nullptr;

    /*! spawn timer
     */
    iTimerHandlePtr _spawnUnitsTimerHandle = nullptr;

    /*! spawn shop timer
     */
    iTimerHandlePtr _spawnShopTimerHandle = nullptr;

    /*! floor shadow
     */
    iSpritePtr _shadow = nullptr;

    /*! shield sprite
     */
    iSpritePtr _shield = nullptr;

    /*! rage sprite
     */
    iSpritePtr _rage = nullptr;

    /*! texture font we use to display texts
     */
    iTextureFontPtr _font = nullptr;

    /*! coin texture
     */
    iSpritePtr _coin = nullptr;

    /*! damage sprite
     */
    iSpritePtr _damage = nullptr;

    /*! attack speed sprite
     */
    iSpritePtr _attackSpeed = nullptr;

    /*! walk speed sprite
     */
    iSpritePtr _walkSpeed = nullptr;

    /*! bounce animation
     */
    iAnimationPtr _bounceAnimation = nullptr;

    /*! shop idle animation
     */
    iAnimationPtr _shopIdleAnimation = nullptr;

    /*! coin spin animation
     */
    iAnimationPtr _coinSpinAnimation = nullptr;

    /*! building type to spawn next
     */
    BuildingType _currentBuilding = BuildingType::None;

    /*! if true level up
     */
    bool _levelUp = false;

    /*! experience levels
     */
    std::vector<uint32> _expLvl;

    /*! all available upgrades to character
     */
    std::vector<UpgradeConfiguration> _upgrades;

    /*! all available shop items
     */
    std::vector<ShopItem> _shopItems;

    /*! all available weapons
     */
    std::map<iaString, WeaponConfiguration> _weapons;

    /*! all available enemies
     */
    std::vector<EnemyClass> _enemies;

    /*! level up dialog
     */
    UpgradeDialog *_levelUpDialog = nullptr;

    /*! shop dialog
     */
    ShopDialog *_shopDialog = nullptr;

    /*! upgrade a given entity

    \param entity the entity to upgrade
    \param upgradeConfiguration the upgrade definition
    */
    void upgrade(iEntityPtr entity, const UpgradeConfiguration &upgradeConfiguration);

    /*! handle level dialog close event
     */
    void onCloseLevelUpDialog(iDialogPtr dialog);

    /*! handle shop dialog close event
     */
    void onCloseShopDialog(iDialogPtr dialog);

    /*! opens a building of given type

    \param buildingType the given building type
    */
    void onOpenBuilding(BuildingType buildingType);

    /*! open a shop
     */
    void openShop();

    /*! called when added to layer stack
     */
    void onInit() override;

    /*! calculate experience levels
     */
    void initExpLvlTable();

    /*! load specs from config file

    \param filename the given config file
    */
    void loadSpecs(const iaString &filename);

    /*! load upgrades from xml handle

    \param upgrades the xml handle
    */
    void readUpgrades(TiXmlElement *upgrades);

    /*! load shop items from xml handle

    \param shopItems the xml handle
    */
    void readShopItems(TiXmlElement *shopItems);

    /*! load enemies from xml handle

    \param enemies the xml handle
    */
    void readEnemies(TiXmlElement *enemies);

    /*! called when removed from layer stack
     */
    void onDeinit() override;

    /*! called on application pre draw event
     */
    void onUpdate() override;

    /*! called on any other event
     */
    void onEvent(iEvent &event) override;

    /*! called when render ortho viewport
     */
    void onRenderOrtho();

    /*! renders HUD
     */
    void onRenderHUD();

    /*! render player hud
     */
    void onRenderPlayerHUD();

    /*! render stats for debugging
     */
    void onRenderStats();

    /*! game logic interval

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
    iaVector3d getRandomDir();

    /*! create the player

    \returns the created player
    */
    iEntityID createPlayer();

    /*! create the camera

    \returns the created camera
    */
    iEntityID createCamera();

    /*! crate the background
     */
    void createBackground();

    /*! creates a unit. usually enemies

    \param pos the position to create at
    \param party the party ie enemy, friend etc.
    \param target giving the unit a target
    \param enemyClass the class of the unit
    */
    void createUnit(const iaVector2f &pos, uint32 party, iEntityID target, const EnemyClass &enemyClass);

    /*! handles player movement
     */
    void onPlayerMovementBehaviour(iEntityPtr entity, std::any &userData);

    /*! handles camera movement
     */
    void onCameraFollowPlayer(iEntityPtr entity, std::any &userData);

    /*! aquire target
     */
    void onAquireTarget(iEntityPtr entity, std::any &userData);

    /*! update orientation of projectiles
     */
    void onUpdateProjectileOrientation(iEntityPtr entity, std::any &userData);

    /*! check for collisions
     */
    void onCheckCollision(iEntityPtr entity, std::any &userData);

    /*! aim/follow target
     */
    void onFollowTarget(iEntityPtr entity, std::any &userData);

    /*! check for collisions
     */
    void onUpdateCollision(iEntityPtr entity, std::any &userData);

    /*! fire weapon if possible
     */
    void onUpdateWeapon(iEntityPtr entity, std::any &userData);

    /*! collision check
     */
    void onUpdateRange(iEntityPtr entity, std::any &userData);

    /*! create the shop
     */
    void createShop();

    /*! lift the shop
     */
    void onLiftShop();

    /*! create a coin

    \param pos position to create the coin at
    \param party friend or foe coin
    \param objectType what kind of coins
    */
    void createCoin(const iaVector2f &pos, uint32 party, ObjectType objectType);

    /*! fire

    \param from origin to fire from
    \param dir direction to fire at
    \param party friend/foe projectile
    \param weapon the weapon to use
    \param modifier modifiers on the weapon to use
    */
    void fire(const iaVector2d &from, const iaVector2d &dir, uint32 party, const WeaponComponent *weapon, const ModifierComponent *modifier);

    /*! spawns stuff
     */
    void onSpawnStuff(const iaTime &time);

    /*! land the shop
     */
    void onLandShop(const iaTime &time);

    /*! query a circle on the quardtree while wrapping arround at the edges like a doughnut

    \param circle the query circle
    \param hits the resulting list of entitties and relatice to the circle center positions
    */
    void doughnutQuery(const iaCircled &circle, std::vector<std::pair<iEntityID, iaVector2d>> &hits);

    /*! rectangle collision detection in a doughnut world
     */
    bool intersectDoughnut(const iaVector2d &position, const iaRectangled &rectangle, iaVector2d &offset);

    /*! circle collision detection in a doughnut world
     */
    bool intersectDoughnut(const iaVector2d &position, const iaCircled &circle, iaVector2d &offset);

    /*! calculate level based on experience

    \param experience the given experience
    \returns xp level
    */
    float32 calcLevel(uint32 experience);

    /*! add experience to given entity

    \param entity the given entity
    \param experience the experience to add
    */
    void addExperience(iEntityPtr entity, float32 experience);

    /*! called on level up
     */
    void onLevelUp();

    /*! some debugging stats
     */
    struct GameStats
    {
        float32 _playerDamage = 0.0;
        float32 _playerExperience = 0.0;
        float32 _playerLevel = 0.0;
        float32 _playerCoins = 0.0;
        float32 _enemyCount = 0.0;
        float32 _enemyHealth = 0.0;
    };

    std::vector<GameStats> _stats;
};

#endif // __SUPREMACY_GAMELAYER__

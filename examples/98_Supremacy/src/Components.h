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

#ifndef __SUPREMACY_COMPONENTS__
#define __SUPREMACY_COMPONENTS__

#include <igor/igor.h>
using namespace igor;

struct RangeComponent
{
    /*! max range
     */
    float32 _maxRange;

    /*! distance traveled
     */
    float32 _distanceTraveled = 0.0;
};

struct OrientationComponent
{
    /*! direction the entity is pointing at
    */
    iaVector2f _direction;

    /*! if true orientation follows current velocity
    */
    bool followVelocity;
};

struct AngularVelocityComponent
{
    /*! angular velocity in rad per frame
    */
    float32 _velocity;
};

struct QuadtreeObjectComponent
{
    iQuadtreef::ObjectPtr _object;
};

struct VelocityComponent
{
    /*! normalized vector to discribe the direction
    */
    iaVector2f _direction;

    /*! speed in units per frame
    */
    float32 _speed;

    // TODO does this need to be in here?
    bool _nonBlockable = false;
};

struct HealthComponent
{
    float32 _health = 0;
    bool _destroyOnImpact = false;
};

struct PickupComponent
{
    bool _canBePickedUp = true;
};

struct HealComponent
{
    float32 _heal = 0;
};

struct DamageComponent
{
    float32 _damage = 0.0;
};

struct PartyComponent
{
    uint32 _partyID = 0;
};

struct ExperienceComponent
{
    float32 _experience = 0.0;
    float32 _level = 1.0;
};

struct CoinsComponent
{
    float32 _coins = 0;
};

struct ExperienceGainComponent
{
    float32 _experience = 0.0;
};

struct CoinGainComponent
{
    float32 _coins = 0.0;
};

struct VisualComponent
{
    bool _castShadow;
    bool _scaleAnimation;
    iaTime _timerOffset;    
};

struct TargetComponent
{
    iEntityID _targetID;
    bool _inRange = false;
    bool _followTarget = true;
};

struct MovementControlComponent
{
    bool _up = false;
    bool _down = false;
    bool _left = false;
    bool _right = false;
};

struct ViewportComponent
{
    iaRectanglef _viewport;
    iaVector2f _targetOffset;
    iEntityID _targetID;
};

/*! weapon types
 */
enum class WeaponType
{
    Knife,
    Shotgun,
    Minigun,
    RocketLauncher
};

struct WeaponComponent
{
    iaString _texture;
    float32 _size;
    uint32 _projectileCount;
    float32 _accuracy;
    float32 _angularVelocity;

    /*! base damage of this weapon
     */
    float32 _damage;

    /*! base speed of this weapon's projectiles
     */
    float32 _speed;

    /*! base range of this weapon
     */
    float32 _range;

    /*! attack speed interval
     */
    iaTime _attackInterval;

    /*! if true unit must stand still so it can fire
     */
    bool _standStillToFire;

    bool _returnToSender;

    /*! offset to unit position to fire from
     */
    iaVector2f _offset;

    /*! keeps track of cool down
     */
    iaTime _time;
};

enum class ObjectType
{
    Coin
};

struct ModifierComponent
{
    float32 _damageFactor;
    float32 _attackIntervalFactor;
    float32 _criticalHitChanceFactor;
    float32 _criticalHitDamageFactor;
    float32 _splashDamageRangeFactor;
    float32 _walkSpeedFactor;
    float32 _projectileSpeedFactor;
    float32 _projectileRangeFactor;
};

enum class BuildingType
{
    None,
    Shop
};

struct BuildingComponent
{
    BuildingType _type;
};

#endif // __SUPREMACY_COMPONENTS__
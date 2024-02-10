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

#ifndef SUPREMACY_COMPONENTS_H
#define SUPREMACY_COMPONENTS_H

#include <igor/igor.h>
using namespace igor;

class RangeComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    RangeComponent(float64 rangeLeft, const iaVector3d &lastPosition, const iaString &name = "range")
        : iEntityComponent(name), _rangeLeft(rangeLeft), _lastPosition(lastPosition)
    {
    }

    /*! max range
     */
    float64 _rangeLeft = 0.0;

    /*! last position
     */
    iaVector3d _lastPosition;
};

class AngularVelocityComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    AngularVelocityComponent(float64 velocity, const iaString &name = "angular velocity")
        : iEntityComponent(name), _velocity(velocity)
    {
    }

    /*! angular velocity in rad per frame
     */
    float32 _velocity;
};

class HealthComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    HealthComponent(float64 health, bool destroyOnImpact = false, const iaString &name = "health")
        : iEntityComponent(name), _health(health), _destroyOnImpact(destroyOnImpact)
    {
    }

    float32 _health = 0;
    bool _destroyOnImpact = false;
};

class PickupComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    PickupComponent(bool canBePickedUp = true, const iaString &name = "pickup")
        : iEntityComponent(name), _canBePickedUp(canBePickedUp)
    {
    }

    bool _canBePickedUp;
};

class HealComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    HealComponent(float32 heal, const iaString &name = "heal")
        : iEntityComponent(name), _heal(heal)
    {
    }

    float32 _heal;
};

class DamageComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    DamageComponent(float32 damage, const iaString &name = "damage")
        : iEntityComponent(name), _damage(damage)
    {
    }

    float32 _damage;
};

class ExperienceComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    ExperienceComponent(float32 experience = 0.0, float32 level = 1.0, const iaString &name = "experience")
        : iEntityComponent(name), _experience(experience), _level(level)
    {
    }

    float32 _experience;
    float32 _level;
};

class CoinsComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    CoinsComponent(float32 coins = 0.0, const iaString &name = "coins")
        : iEntityComponent(name), _coins(coins)
    {
    }

    float32 _coins;
};

class ExperienceGainComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    ExperienceGainComponent(float32 experience, const iaString &name = "experience gain")
        : iEntityComponent(name), _experience(experience)
    {
    }

    float32 _experience;
};

class CoinGainComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    CoinGainComponent(float32 coins, const iaString &name = "coin gain")
        : iEntityComponent(name), _coins(coins)
    {
    }

    float32 _coins;
};

class TargetComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    TargetComponent(iEntityID targetID = iEntityID::getInvalid(), bool inRange = false, bool followTarget = true, const iaString &name = "target")
        : iEntityComponent(name), _targetID(targetID), _inRange(inRange), _followTarget(followTarget)
    {
    }

    iEntityID _targetID;
    bool _inRange = false;
    bool _followTarget = true;
};

class MovementControlComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    MovementControlComponent(const iaString &name = "movement control")
        : iEntityComponent(name)
    {
    }

    bool _up = false;
    bool _down = false;
    bool _left = false;
    bool _right = false;
};

class ViewportComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    ViewportComponent(const iaString &name = "viewport")
        : iEntityComponent(name)
    {
    }

    iaRectanglef _viewport;
    iaVector2f _targetOffset;
    iEntityID _targetID = iEntityID::getInvalid();
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

struct WeaponConfiguration
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
    iaVector2d _offset;
};

class WeaponComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    WeaponComponent(const WeaponConfiguration &config, const iaString &name = "weapon")
        : iEntityComponent(name), _config(config)
    {
    }

    WeaponConfiguration _config;

    /*! keeps track of cool down
     */
    iaTime _time;    
};

enum class ObjectType
{
    Coin
};

struct ModifierConfiguration
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

class ModifierComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    ModifierComponent(const ModifierConfiguration &config, const iaString &name = "modifier")
        : iEntityComponent(name), _config(config)
    {
    }

    ModifierConfiguration _config;
};

enum class BuildingType
{
    None,
    Shop
};

class BuildingComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    BuildingComponent(BuildingType type, const iaString &name = "building")
        : iEntityComponent(name), _type(type)
    {
    }

    BuildingType _type;
};

#endif // SUPREMACY_COMPONENTS_H
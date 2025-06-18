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
//    (c) Copyright 2012-2025 by Martin A. Loga
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

/*! I love parties <3
 */
class PartyComponent : public iEntityComponent
{
public:
    /*! ctor

    \param name the name of this component
    */
    PartyComponent(uint32 partyID = 0, const iaString &name = "party")
        : _partyID(partyID)
    {
    }

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new PartyComponent();
    }

    /*! party id
     */
    uint32 _partyID = 0;

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        PartyComponent *component = new PartyComponent();
        component->_partyID = _partyID;
        return component;
    }
};

class RangeComponent : public iEntityComponent
{
public:
    /*! default ctor
     */
    RangeComponent() = default;

    /*! ctor

    \param name the name of this component
    */
    RangeComponent(float64 rangeLeft, const iaVector3d &lastPosition)
        : _rangeLeft(rangeLeft), _lastPosition(lastPosition)
    {
    }

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new RangeComponent();
    }

    /*! max range
     */
    float64 _rangeLeft = 0.0;

    /*! last position
     */
    iaVector3d _lastPosition;

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        RangeComponent *component = new RangeComponent();
        component->_rangeLeft = _rangeLeft;
        component->_lastPosition = _lastPosition;
        return component;
    }
};

class AngularVelocityComponent : public iEntityComponent
{
public:
    /*! default ctor
     */
    AngularVelocityComponent() = default;

    /*! ctor
     */
    AngularVelocityComponent(float64 velocity)
        : _velocity(velocity)
    {
    }

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new AngularVelocityComponent();
    }

    /*! angular velocity in rad per frame
     */
    float32 _velocity;

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        AngularVelocityComponent *component = new AngularVelocityComponent();
        component->_velocity = _velocity;
        return component;
    }
};

class HealthComponent : public iEntityComponent
{
public:
    /*! default ctor
     */
    HealthComponent() = default;

    /*! ctor
     */
    HealthComponent(float64 health, bool destroyOnImpact = false)
        : _health(health), _destroyOnImpact(destroyOnImpact)
    {
    }

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new HealthComponent();
    }

    float32 _health = 0;
    bool _destroyOnImpact = false;

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        HealthComponent *component = new HealthComponent();
        component->_health = _health;
        component->_destroyOnImpact = _destroyOnImpact;
        return component;
    }
};

class PickupComponent : public iEntityComponent
{
public:
    /*! default ctor
     */
    PickupComponent() = default;

    /*! ctor
     */
    PickupComponent(bool canBePickedUp)
        : _canBePickedUp(canBePickedUp)
    {
    }

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new PickupComponent();
    }

    bool _canBePickedUp;

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        PickupComponent *component = new PickupComponent();
        component->_canBePickedUp = _canBePickedUp;
        return component;
    }
};

class HealComponent : public iEntityComponent
{
public:
    /*! default ctor
     */
    HealComponent() = default;

    /*! ctor
     */
    HealComponent(float32 heal)
        : _heal(heal)
    {
    }

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new HealComponent();
    }

    float32 _heal;

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        HealComponent *component = new HealComponent();
        component->_heal = _heal;
        return component;
    }
};

class DamageComponent : public iEntityComponent
{
public:
    /*! default ctor
     */
    DamageComponent() = default;

    /*! ctor
     */
    DamageComponent(float32 damage)
        : _damage(damage)
    {
    }

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new DamageComponent();
    }

    float32 _damage;

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        DamageComponent *component = new DamageComponent();
        component->_damage = _damage;
        return component;
    }
};

class ExperienceComponent : public iEntityComponent
{
public:
    /*! default ctor
     */
    ExperienceComponent() = default;

    /*! ctor
     */
    ExperienceComponent(float32 experience, float32 level)
        : _experience(experience), _level(level)
    {
    }

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new ExperienceComponent();
    }

    float32 _experience = 0;
    float32 _level = 0;

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        ExperienceComponent *component = new ExperienceComponent();
        component->_experience = _experience;
        component->_level = _level;
        return component;
    }
};

class CoinsComponent : public iEntityComponent
{
public:
    /*! default ctor
     */
    CoinsComponent() = default;

    /*! ctor
     */
    CoinsComponent(float32 coins)
        : _coins(coins)
    {
    }

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new CoinsComponent();
    }

    float32 _coins = 0.0;

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        CoinsComponent *component = new CoinsComponent();
        component->_coins = _coins;
        return component;
    }
};

class ExperienceGainComponent : public iEntityComponent
{
public:
    /*! default ctor
     */
    ExperienceGainComponent() = default;

    /*! ctor
     */
    ExperienceGainComponent(float32 experience)
        : _experience(experience)
    {
    }

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new ExperienceGainComponent();
    }

    float32 _experience;

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        ExperienceGainComponent *component = new ExperienceGainComponent();
        component->_experience = _experience;
        return component;
    }
};

class CoinGainComponent : public iEntityComponent
{
public:
    /*! default ctor
     */
    CoinGainComponent() = default;

    /*! ctor
     */
    CoinGainComponent(float32 coins)
        : _coins(coins)
    {
    }

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new CoinGainComponent();
    }

    float32 _coins;

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        CoinGainComponent *component = new CoinGainComponent();
        component->_coins = _coins;
        return component;
    }
};

class TargetComponent : public iEntityComponent
{
public:
    /*! default ctor
     */
    TargetComponent() = default;

    /*! ctor
     */
    TargetComponent(iEntityID targetID, bool inRange = false, bool followTarget = true)
        : _targetID(targetID), _inRange(inRange), _followTarget(followTarget)
    {
    }

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new TargetComponent();
    }

    iEntityID _targetID;
    bool _inRange = false;
    bool _followTarget = true;

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        TargetComponent *component = new TargetComponent();
        component->_targetID = _targetID;
        component->_inRange = _inRange;
        component->_followTarget = _followTarget;
        return component;
    }
};

class MovementControlComponent : public iEntityComponent
{
public:
    /*! default ctor
     */
    MovementControlComponent() = default;

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new MovementControlComponent();
    }

    bool _up = false;
    bool _down = false;
    bool _left = false;
    bool _right = false;

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        MovementControlComponent *component = new MovementControlComponent();
        component->_up = _up;
        component->_down = _down;
        component->_left = _left;
        component->_right = _right;
        return component;
    }
};

class ViewportComponent : public iEntityComponent
{
public:
    /*! default ctor
     */
    ViewportComponent() = default;

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new ViewportComponent();
    }

    iaRectanglef _viewport;
    iaVector2f _targetOffset;
    iEntityID _targetID = iEntityID::getInvalid();

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        ViewportComponent *component = new ViewportComponent();
        component->_viewport = _viewport;
        component->_targetOffset = _targetOffset;
        component->_targetID = _targetID;
        return component;
    }
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
    /*! default ctor
     */
    WeaponComponent() = default;

    /*! ctor

    \param name the name of this component
    */
    WeaponComponent(const WeaponConfiguration &config, const iaString &name = "weapon")
        : _config(config)
    {
    }

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new WeaponComponent();
    }

    WeaponConfiguration _config;

    /*! keeps track of cool down
     */
    iaTime _time;

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        WeaponComponent *component = new WeaponComponent();
        component->_config = _config;
        component->_time = _time;
        return component;
    }
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
    /*! default ctor
     */
    ModifierComponent() = default;

    /*! ctor

    \param name the name of this component
    */
    ModifierComponent(const ModifierConfiguration &config, const iaString &name = "modifier")
        : _config(config)
    {
    }

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new ModifierComponent();
    }

    ModifierConfiguration _config;

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        ModifierComponent *component = new ModifierComponent();
        component->_config = _config;
        return component;
    }
};

enum class BuildingType
{
    None,
    Shop
};

class BuildingComponent : public iEntityComponent
{
public:
    /*! default ctor
     */
    BuildingComponent() = default;

    /*! ctor

    \param name the name of this component
    */
    BuildingComponent(BuildingType type, const iaString &name = "building")
        : _type(type)
    {
    }

    /*! creates instance of this component type
     */
    static iEntityComponent *createInstance()
    {
        return new BuildingComponent();
    }

    BuildingType _type;

private:
    /*! \returns a copy of this component
     */
    iEntityComponentPtr getCopy() override
    {
        BuildingComponent *component = new BuildingComponent();
        component->_type = _type;
        return component;
    }
};

#endif // SUPREMACY_COMPONENTS_H
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

struct PositionComponent
{
    /*! current position
     */
    iaVector2d _position;
};

struct RangeComponent
{
    /*! max range
     */
    float64 _maxRange;

    /*! distance traveled
     */
    float64 _distanceTraveled = 0.0;
};

struct OrientationComponent
{
    /*! direction the entity is pointing at
    */
    iaVector2d _direction;

    /*! if true orientation follows current velocity
    */
    bool followVelocity;
};

struct AngularVelocityComponent
{
    /*! angular velocity in rad per frame
    */
    float64 _velocity;
};

struct SizeComponent
{
    float64 _size;
};

struct QuadtreeObjectComponent
{
    std::shared_ptr<iQuadtreeObject<float64, iEntityID>> _object;
};

struct VelocityComponent
{
    /*! normalized vector to discribe the direction
    */
    iaVector2d _direction;

    /*! speed in units per frame
    */
    float64 _speed;

    // TODO does this need to be in here?
    bool _nonBlockable = false;
};

struct HealthComponent
{
    float64 _health = 0;
    bool _destroyOnImpact = false;
};

struct DamageComponent
{
    float64 _damage = 0;
};

struct PartyComponent
{
    uint32 _partyID = 0;
};

struct VisualComponent
{
    iTexturePtr _texture;
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
    iaRectangled _viewport;
    iaVector2d _targetOffset;
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
    /*! weapon type used
     */
    WeaponType _weaponType;

    /*! base damage of this weapon
     */
    float64 _damage;

    /*! base speed of this weapon
     */
    float64 _speed;

    /*! base speed of this weapon
     */
    float64 _range;

    /*! attack speed interval
     */
    iaTime _attackInterval;

    /*! if true unit must stand still so it can fire
     */
    bool _standStillToFire;

    /*! offeset to unit position to fire from
     */
    iaVector2d _offset;

    /*! keeps track of cool down
     */
    iaTime _time;
};

#endif // __SUPREMACY_COMPONENTS__
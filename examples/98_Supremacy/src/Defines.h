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

#ifndef __SUPREMACY_DEFINES__
#define __SUPREMACY_DEFINES__

#include "Components.h"

const uint32 FRIEND = 10u;
const uint32 FOE = 20u;

const float64 PLAYFIELD_WIDTH = 1000.0;
const float64 PLAYFIELD_HEIGHT = 1000.0;
const float64 PLAYFIELD_VIEWPORT_WIDTH = 700.0;
const float64 PLAYFIELD_VIEWPORT_HEIGHT = PLAYFIELD_VIEWPORT_WIDTH / 16.0 * 9.0;
const float64 PLAYFIELD_VIEWPORT_MOVE_EDGE_WIDTH = PLAYFIELD_VIEWPORT_WIDTH * 0.3;
const float64 PLAYFIELD_VIEWPORT_MOVE_EDGE_HEIGHT = PLAYFIELD_VIEWPORT_HEIGHT * 0.3;
const float64 STANDARD_UNIT_SIZE = 30.0;

struct WeaponConfiguration
{
    iaString _texture;
    float32 _size;
    uint32 _projectileCount;
    float32 _accuracy;
    float32 _angularVelocity;
    WeaponComponent _component;
};

// weapon definitions
const WeaponConfiguration WEAPON_KNIFE{
    "knife.png", // texture
    15.0,        // size
    1,           // projectile count
    0.0,         // accuracy 0.0 == perfect
    0.2,         // angular velocity
    {
        WeaponType::Knife,               // weapon type
        10.0,                            // damage
        1.0,                             // speed
        100.0,                           // range
        iaTime::fromMilliseconds(600.0), // attack interval
        false,                           // need to stand still
    }};

const WeaponConfiguration WEAPON_SHOTGUN{
    "pellets.png", // texture
    25.0,          // size
    3,             // projectile count
    1.0,           // accuracy 0.0 == perfect
    0.0,           // angular velocity
    {
        WeaponType::Shotgun,              // weapon type
        100.0,                            // damage
        7.0,                             // speed
        120.0,                            // range
        iaTime::fromMilliseconds(1000.0), // attack interval
        true,                             // need to stand still
    }};

const WeaponConfiguration WEAPON_MINIGUN{
    "bullet.png", // texture
    20.0,         // size
    1,            // projectile count
    0.0,          // accuracy 0.0 == perfect
    0.0,          // angular velocity
    {
        WeaponType::Minigun,            // weapon type
        30.0,                           // damage
        15.0,                           // speed
        150.0,                          // range
        iaTime::fromMilliseconds(20.0), // attack interval
        true,                           // need to stand still
    }};

const WeaponConfiguration WEAPON_ROCKETLAUNCHER{
    "rocket.png", // texture
    20.0,         // size
    1,            // projectile count
    0.0,          // accuracy 0.0 == perfect
    0.0,          // angular velocity
    {
        WeaponType::RocketLauncher,       // weapon type
        2000.0,                           // damage
        5.0,                              // speed
        150.0,                            // range
        iaTime::fromMilliseconds(2000.0), // attack interval
        true,                             // need to stand still
    }};

#endif // __SUPREMACY_DEFINES__
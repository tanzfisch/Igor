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
const float64 PLAYFIELD_VIEWPORT_MOVE_EDGE_WIDTH = PLAYFIELD_VIEWPORT_WIDTH * 0.7;
const float64 PLAYFIELD_VIEWPORT_MOVE_EDGE_HEIGHT = PLAYFIELD_VIEWPORT_HEIGHT * 0.7;
const float64 STANDARD_UNIT_SIZE = 30.0;

// weapon definitions

const WeaponComponent WEAPON_ROLLINGPIN{
    WeaponType::RollingPin,           // weapon type
    20.0,                             // damage
    5.0,                              // speed
    70.0,                             // range
    iaTime::fromMilliseconds(1000.0), // attack interval
    true,                             // need to stand still
};

const WeaponComponent WEAPON_KNIFE{
    WeaponType::Knife,               // weapon type
    50.0,                            // damage
    10.0,                            // speed
    100.0,                           // range
    iaTime::fromMilliseconds(800.0), // attack interval
    true,                            // need to stand still
};

#endif // __SUPREMACY_DEFINES__
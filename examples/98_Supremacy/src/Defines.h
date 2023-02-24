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
const uint32 NEUTRAL = 30u;

const float64 PLAYFIELD_WIDTH = 1500.0;
const float64 PLAYFIELD_HEIGHT = 1500.0;
const float64 PLAYFIELD_VIEWPORT_WIDTH = 800.0;
const float64 PLAYFIELD_VIEWPORT_HEIGHT = PLAYFIELD_VIEWPORT_WIDTH / 16.0 * 9.0;
const float64 PLAYFIELD_VIEWPORT_MOVE_EDGE_WIDTH = PLAYFIELD_VIEWPORT_WIDTH * 0.3;
const float64 PLAYFIELD_VIEWPORT_MOVE_EDGE_HEIGHT = PLAYFIELD_VIEWPORT_HEIGHT * 0.2;

const float64 STANDARD_UNIT_SIZE = 30.0;

const float64 COIN_SIZE = 12.0;

#endif // __SUPREMACY_DEFINES__
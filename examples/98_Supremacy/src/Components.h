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

#ifndef __SUPREMACY_SYSTEMS__
#define __SUPREMACY_SYSTEMS__

#include <igor/igor.h>

struct PositionComponent
{
    iaVector2d _position;
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
    iaVector2d _direction;
    float64 _speed;
    bool _nonBlockable = false;
};

struct HealthComponent
{
    float64 _health = 0;
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
    iTexturePtr _character;
    bool _useDirectory = false;
};

struct OriginComponent
{
    iaVector2d _origin;
};

struct TargetComponent
{
    iEntityID _targetID;
    bool _inRange = false;
};

struct MovementControlComponent
{
    bool _up = false;
    bool _down = false;
    bool _left = false;
    bool _right = false;
};

#endif // __SUPREMACY_SYSTEMS__
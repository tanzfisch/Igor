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

struct PositionComponent : public iEntityComponent<PositionComponent>
{
    iaVector2f _position;
};

struct VelocityComponent : public iEntityComponent<VelocityComponent>
{
    iaVector2f _direction;
    float32 _speed;
};

struct HealthComponent : public iEntityComponent<HealthComponent>
{
    float32 _health = 0;
};

struct PartyComponent : public iEntityComponent<PartyComponent>
{
    uint32 _partyID = 0;
};

struct MovementControlComponent : public iEntityComponent<PartyComponent>
{
    bool _up = false;
    bool _down = false;
    bool _left = false;
    bool _right = false;
};

class PawnSystem : public iEntitySystem
{
public:
    /*! init system
     */
    PawnSystem();

    /*! updates components
    \param components the components to update
    */
    void updateComponents(iBaseComponent **components) override;
};

class DisplayEntittiesSystem : public iEntitySystem
{
public:
    /*! init system
     */
    DisplayEntittiesSystem();

    /*! updates components
    \param components the components to update
    */
    void updateComponents(iBaseComponent **components) override;
};

#endif // __SUPREMACY_SYSTEMS__
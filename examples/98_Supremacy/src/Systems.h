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

struct QuadtreeData : public iQuadtreeUserData
{
    QuadtreeData(const iaCircled &circle)
        : iQuadtreeUserData(circle)
    {
    }

    // TODO user data
};

struct PositionComponent : public iComponent<PositionComponent>
{
    iQuadtreeUserDataPtr _quadtreeUserData;
};

struct VelocityComponent : public iComponent<VelocityComponent>
{
    iaVector2d _direction;
    float32 _speed;
};

struct HealthComponent : public iComponent<HealthComponent>
{
    float32 _health = 0;
};

struct PartyComponent : public iComponent<PartyComponent>
{
    uint32 _partyID = 0;
};

struct VisualComponent : public iComponent<VisualComponent>
{
    iTexturePtr _character;
};

struct MovementControlComponent : public iComponent<MovementControlComponent>
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
    PawnSystem(iQuadtree *quadtree);

    /*! updates components
    \param components the components to update
    */
    void updateComponents(iBaseComponent **components) override;

private:

    iQuadtree *_quadtree;
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

private:
    /*! floor shadow
     */
    iTexturePtr _shadow;
};

#endif // __SUPREMACY_SYSTEMS__
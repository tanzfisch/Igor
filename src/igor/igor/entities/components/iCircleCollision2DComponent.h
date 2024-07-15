//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef IGOR_CIRCLE_COLLISION_2D_COMPONENT_H
#define IGOR_CIRCLE_COLLISION_2D_COMPONENT_H

#include <igor/entities/iEntity.h>

#include <igor/data/iOctree.h>

namespace igor
{
    /*! 2D circle collision component
     */
    class iCircleCollision2DComponent : public iEntityComponent
    {
    public:
        /*! ctor
        */
        iCircleCollision2DComponent(float64 radius, const iaVector2d &offset = iaVector2d())
            : _radius(radius), _offset(offset)
        {
        }

        /*! \returns the circles radius
         */
        float64 getRadius() const;

        /*! \returns the offset position
         */
        const iaVector2d& getOffset() const;

        /*! the circles radius
         */
        float64 _radius;

        /*! the offset position
         */
        iaVector2d _offset;
    };
}

#endif // IGOR_CIRCLE_COLLISION_2D_COMPONENT_H
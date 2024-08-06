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

#ifndef IGOR_SPHERE_COLLISION_3D_COMPONENT_H
#define IGOR_SPHERE_COLLISION_3D_COMPONENT_H

#include <igor/entities/iEntity.h>

namespace igor
{
    /*! 3D sphere collision component
     */
    class iSphereComponent : public iEntityComponent
    {
    public:
        /*! default ctor
        */
        iSphereComponent() = default;

        /*! ctor
         */
        iSphereComponent(float64 radius, const iaVector3d &offset = iaVector3d())
            : _radius(radius), _offset(offset)
        {
        }

        /*! creates instance of this component type
        */
        static iEntityComponent* createInstance();        

        /*! \returns the sphere's radius
         */
        float64 getRadius() const;

        /*! sets radius

        \param radius the radius to set
         */
        void setRadius(float64 radius);

        /*! \returns the offset position
         */
        const iaVector3d &getOffset() const;

        /*! sets offset

        \param offset the offset to set
         */
        void setOffset(const iaVector3d &offset);

    private:
        /*! the sphere's radius
         */
        float64 _radius;

        /*! the offset position
         */
        iaVector3d _offset;

        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override;
    };

}

#endif // IGOR_SPHERE_COLLISION_3D_COMPONENT_H
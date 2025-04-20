
//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef IGOR_VELOCITY_COMPONENT_H
#define IGOR_VELOCITY_COMPONENT_H

#include <igor/entities/iEntity.h>

namespace igor
{

    /*! velocity component
     */
    class iVelocityComponent : public iEntityComponent
    {
    public:
        /*! ctor

        \param velocity the velocity to set
        \param angularVelocity the angular velocity to set
         */
        iVelocityComponent(const iaVector3d &velocity = iaVector3d(), const iaVector3d &angularVelocity = iaVector3d());

        /*! creates instance of this component type
         */
        static iEntityComponent *createInstance();

        /*! \returns type name of component
         */
        static const iaString &getTypeName();

        /*! \returns velocity
         */
        const iaVector3d &getVelocity() const;

        /*! sets velocity

        \param velocity the velocity to set
        */
        void setVelocity(const iaVector3d &velocity);

        /*! \returns angular velocity
         */
        const iaVector3d &getAngularVelocity() const;

        /*! sets angular velocity

        \param angularVelocity the angular velocity to set
        */
        void setAngularVelocity(const iaVector3d &angularVelocity);

    private:
        /*! vector to describe the velocity
         */
        iaVector3d _velocity;

        /*! vector to describe angular velocity of all three axis
         */
        iaVector3d _angularVelocity;

        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override
        {
            iVelocityComponent *component = new iVelocityComponent();
            component->_velocity = _velocity;
            component->_angularVelocity = _angularVelocity;
            return component;
        }
    };

}

#endif // IGOR_VELOCITY_COMPONENT_H
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

#ifndef IGOR_TRANSFORM_COMPONENT_H
#define IGOR_TRANSFORM_COMPONENT_H

#include <igor/entities/iEntity.h>

namespace igor
{
    /*! transform component representing position, orientation and scale of given entity

    3d variant
    */
    class IGOR_API iTransformComponent : public iEntityComponent
    {
        friend class iEntityTransformTraverser;

    public:
        /*! ctor

        \param name the name of this component
        */
        iTransformComponent(const iaVector3d &position = iaVector3d(), const iaVector3d &orientation = iaVector3d(), const iaVector3d &scale = iaVector3d(1.0, 1.0, 1.0), const iaString &name = "transform");

        /*! \returns world matrix
         */
        const iaMatrixd &getWorldMatrix() const;

        /*! sets position on transform

        \param position the position to set
        */
        void setPosition(const iaVector3d &position);

        /*! translate by given amount

        \param translate the amount to translate
        */
        void translate(const iaVector3d &translate);

        /*! \returns position on transform
         */
        const iaVector3d &getPosition() const;

        /*! sets orientation on transform

        \param orientation the orientation to set
        */
        void setOrientation(const iaVector3d &orientation);

        /*! rotate by given amount

        \param rotate the amount to rotate
        */
        void rotate(const iaVector3d &rotate);

        /*! \returns orientation on transform
         */
        const iaVector3d &getOrientation() const;

        /*! sets scale on transform

        \param scale the scale to set
        */
        void setScale(const iaVector3d &scale);

        /*! \returns scale on transform
         */
        const iaVector3d &getScale() const;

        /*! \returns true if data needs update aka world matrix is invalid
         */
        bool isDirty() const;

    private:
        /*! position
         */
        iaVector3d _position;

        /*! euler angles in rad
         */
        iaVector3d _orientation;

        /*! scale
         */
        iaVector3d _scale = {1.0, 1.0, 1.0};

        /*! the world matrix of this transform
         */
        iaMatrixd _worldMatrix;

        /*! update world matrix

        \param[in/out] worldMatrix parent entity world matrix
        \returns true if matrix changed
        */
        bool updateWorldMatrix(iaMatrixd &worldMatrix);
    };
}

#endif // IGOR_TRANSFORM_COMPONENT_H
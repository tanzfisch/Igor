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

#ifndef IGOR_TRANSFORM_COMPONENT_H
#define IGOR_TRANSFORM_COMPONENT_H

#include <igor/entities/iEntity.h>

#include <iaux/math/iaTransform.h>

namespace igor
{
    /*! transform component representing translate, rotate and scale of given entity

    3d variant
    */
    class IGOR_API iTransformComponent : public iEntityComponent
    {
        friend class iEntityTransformTraverser;

    public:
        /*! default ctor
         */
        iTransformComponent() = default;

        /*! ctor
         */
        iTransformComponent(const iaVector3d &translate, const iaQuaterniond &rotate = iaQuaterniond(), const iaVector3d &scale = iaVector3d(1.0, 1.0, 1.0));

        /*! creates instance of this component type
         */
        static iEntityComponent *createInstance();

        /*! \returns type name of component
         */
        static const iaString &getTypeName();

        /*! \returns world matrix
         */
        const iaMatrixd &getWorldMatrix() const;

        /*! sets translate on transform

        \param translate the translate to set
        */
        void setPosition(const iaVector3d &translate);

        /*! translate by given amount

        \param translate the amount to translate
        */
        void translate(const iaVector3d &translate);

        /*! \returns translate on transform
         */
        const iaVector3d &getPosition() const;

        /*! \returns world translate on transform
         */
        const iaVector3d &getWorldPosition() const;

        /*! sets rotate on transform

        \param rotate the rotate to set
        */
        void setOrientation(const iaQuaterniond &rotate);

        /*! rotate by given amount

        \param rotate the amount to rotate
        */
        void rotate(const iaQuaterniond &rotate);

        /*! \returns rotate on transform
         */
        const iaQuaterniond getOrientation() const;

        /*! \returns world rotate on transform
         */
        const iaQuaterniond getWorldOrientation() const;

        /*! sets scale on transform

        \param scale the scale to set
        */
        void setScale(const iaVector3d &scale);

        /*! \returns scale on transform
         */
        const iaVector3d &getScale() const;

        /*! \returns world scale on transform
         */
        const iaVector3d &getWorldScale() const;

        /*! \returns a set of info strings
         */
        std::vector<iaString> getInfo() const override;

    private:

        /*! local transform information
        */
        iaTransformd _transform;

        /*! world transform information
        */
        iaTransformd _worldTransform;

        /*! the world matrix of this transform
         */
        iaMatrixd _worldMatrix;

        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override;

        /*! callback to activate component
         */
        void onActivate(iEntityPtr entity) override;
    };

    /*! transform component pointer definition
     */
    typedef iTransformComponent *iTransformComponentPtr;
}

#endif // IGOR_TRANSFORM_COMPONENT_H
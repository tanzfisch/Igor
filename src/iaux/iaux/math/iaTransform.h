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

#ifndef IAUX_TRANSFORM_H
#define IAUX_TRANSFORM_H

#include <iaux/math/iaMatrix.h>

namespace iaux
{

    /*! Transform based on the components position, rotate, scale
     */
    template <class T>
    class IAUX_API_EXPORT_ONLY iaTransform
    {
    public:
        /*! initializes the transform effectively with the id matrix
         */
        iaTransform();

        /*! initializes the transform with given components

        \param position translation component
        \param orientation the orientation component
        \param scale the scale component
        */
        iaTransform(const iaVector3<T> &position, const iaVector3<T> orientation, const iaVector3<T> &scale);

        /*! does nothing
         */
        ~iaTransform() = default;

        /*! set the transform with given components

        \param position translation component
        \param orientation the orientation component
        \param scale the scale component
        */
        void set(const iaVector3<T> &position, const iaVector3<T> orientation, const iaVector3<T> &scale);        

        /*! position component
         */
        iaVector3<T> _position;

        /*! orientation component with euler angles
         */
        iaVector3<T> _orientation;

        /*! scale component
         */
        iaVector3<T> _scale;

        /*! \returns matrix the resulting matrix
        */
        const iaMatrix<T> getMatrix() const;
    };

    template <class T>
    iaTransform<T> lerp(const iaTransform<T> &a, const iaTransform<T> &b, T t);

#include <iaux/math/iaTransform.inl>

    /*! float32 transform
     */
    typedef iaTransform<float64> iaTransformf;

    /*! float64 transform
     */
    typedef iaTransform<float64> iaTransformd;

}; // namespace iaux

#endif // __IAUX_TRANSFORM__

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

#include <iaux/math/iaQuaternion.h>

namespace iaux
{

    /*! Transform based on the components position, orientation, scale
     */
    template <class T>
    class IAUX_API_EXPORT_ONLY iaTransform
    {
    public:
        /*! position component
         */
        iaVector3<T> _position;

        /*! orientation component with euler angles
         */
        iaQuaternion<T> _orientation;

        /*! scale component
         */
        iaVector3<T> _scale;

        /*! comparison of two transforms

        \param other the other transform to compare with
        \returns true if transforms are equal
        */
        bool operator==(const iaTransform<T> &other) const;

        /*! negated comparison of two transforms

        \param other the other transform to compare with
        \returns true if transforms are unequal
        */
        bool operator!=(const iaTransform<T> &other) const;

        /*! transform multiplication

        equivalent of a matrix multiplication

        \param other the other transform to multiply with
        \returns product of multiplication
         */
        iaTransform<T> operator*(const iaTransform<T> &other);

        /*! transform multiplication stored in this transform

        equivalent of a matrix multiplication

        \param other the other transform to multiply with
         */
        void operator*=(const iaTransform<T> &other);

        /*! \returns true if transform has translation different from 0,0,0
         */
        bool hasTranslation() const;

        /*! \returns true if transform has rotation different from 0,0,0
         */
        bool hasRotation() const;

        /*! \returns true if transform has scale different from 1,1,1
         */
        bool hasScale() const;

        /*! set the transform with given components

        \param position translation component
        \param orientation the orientation component
        \param scale the scale component
        */
        void set(const iaVector3<T> &position, const iaQuaternion<T> orientation, const iaVector3<T> &scale);

        /*! \returns matrix the resulting matrix
         */
        const iaMatrix<T> getMatrix() const;

        /*! the equivalent of setting an identity matrix
        */
        void identity();

        /*! initializes the transform effectively with the id matrix
         */
        iaTransform();

        /*! initializes the transform with given components

        \param position translation component
        \param orientation the orientation component
        \param scale the scale component
        */
        iaTransform(const iaVector3<T> &position, const iaQuaternion<T> orientation = iaVector3<T>(0.0, 0.0, 0.0), const iaVector3<T> &scale = iaVector3<T>(1.0, 1.0, 1.0));

        /*! does nothing
         */
        ~iaTransform() = default;
    };

    template <class T>
    iaTransform<T> lerp(const iaTransform<T> &a, const iaTransform<T> &b, T t);

    /*! stream operator

    \param stream the destination
    \param m the matrix to stream
    \returns the resulting stream
    */
    template <class T>
    IAUX_API std::wostream &operator<<(std::wostream &stream, const iaTransform<T> &transform);

#include <iaux/math/iaTransform.inl>

    /*! float32 transform
     */
    typedef iaTransform<float64> iaTransformf;

    /*! float64 transform
     */
    typedef iaTransform<float64> iaTransformd;

}; // namespace iaux

#endif // __IAUX_TRANSFORM__

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

#ifndef IAUX_QUATERNION_H
#define IAUX_QUATERNION_H

#include <iaux/math/iaMatrix.h>

#include <ostream>
#include <algorithm>

namespace iaux
{

    /*! quaternion class

    x-axis -> pitch
    y-axis -> yaw
    z-axis -> roll
     */
    template <class T>
    class IAUX_API_EXPORT_ONLY iaQuaternion
    {
    public:
        /*! w component
         */
        T _w = 1.0;

        /*! x component
         */
        T _x = 0.0;

        /*! y component
         */
        T _y = 0.0;

        /*! z component
         */
        T _z = 0.0;

        // default ctor
        iaQuaternion();

        /*! ctor by direct values

        \param w the w component
        \param x the x component
        \param y the y component
        \param z the z component
        */
        iaQuaternion(T w, T x, T y, T z);

        /*! ctor by direct values

        \param x the x component
        \param y the y component
        \param z the z component
        */
        iaQuaternion(T x, T y, T z);

        /*! init with other quaternion

        \param other the other quaternion
        */
        iaQuaternion(const iaQuaternion<T> &other);

        /*! set by axis angle

        \param axis the axis to rotate around
        \param angle the angle to rotate
        */
        static const iaQuaternion<T> fromAxisAngle(const iaVector3<T> &axis, T angle);

        /*! returns axis angle from quaternion

        \param axis[out] the rotation axis
        \param angle[out] the rotation angle
        */
        void toAxisAngle(iaVector3<T> &axis, T &angle) const;

        /*! set by direct value

        \param x the x component
        \param y the y component
        \param z the z component
        \param w the w component
        */
        void set(T w, T x, T y, T z);

        /*! reset quaternion to identity
         */
        void identity();

        /*! negate quaternion
         */
        void negate();

        /*! set by euler angles

        using order XYZ or pitch,yaw,roll

        \param pitch rotation around x axis in rad
        \param yaw rotation around y axis in rad
        \param roll rotation around z axis in rad
        */
        static const iaQuaternion<T> fromEuler(T pitch, T yaw, T roll);

        /*! set by euler angles

        vector will be interpreted as pitch, yaw, roll

        using order XYZ or pitch,yaw,roll

        \param vec vector that contains euler angles
        */
        static const iaQuaternion<T> fromEuler(const iaVector3<T> &vec);

        /*! \returns euler angles

        !!! terrible percision don't use unless you have to !!!
         */
        const iaVector3<T> toEuler() const;

        /*! \returns a matrix withthe orientation of this quaternion
        */
        const iaMatrix<T> toMatrix() const;

        /*! \returns true if both quaternions are equal

        \param other the right hand side quaternion
        */
        iaQuaternion<T> &operator=(const iaQuaternion<T> &other);

        /*! \returns true if both quaternions are equal

        \param other the right hand side quaternion
        */
        bool operator==(const iaQuaternion<T> &other) const;

        /*! \returns true if both quaternions are not equal

        \param other the right hand side quaternion
        */
        bool operator!=(const iaQuaternion<T> &other) const;

        /*! add one quaternion to an other

        \returns the resulting quaternion
        \param other the right hand side quaternion
        */
        const iaQuaternion<T> operator+(const iaQuaternion<T> &other);

        /*! add one quaternion to this quaternion

        \param other the right hand side quaternion
        \returns resulting quaternion
        */
        iaQuaternion<T> &operator+=(const iaQuaternion<T> &other);

        /*! subtracts one quaternion from an other

        \returns the resulting quaternion
        \param other the right hand side quaternion
        */
        const iaQuaternion<T> operator-(const iaQuaternion<T> &other);

        /*! subtracts one quaternion from the other

        \param other the right hand side quaternion
        \returns resulting quaternion
        */
        iaQuaternion<T> &operator-=(const iaQuaternion<T> &other);

        /*! multiply one quaternion to this quaternion

        \param other the right hand side quaternion
        \returns the resulting quaternion
        */
        const iaQuaternion<T> operator*(const iaQuaternion<T> &other) const;

        /*! multiply one quaternion with an other one

        \param other the right hand side quaternion
        \returns resulting quaternion
        */
        iaQuaternion<T> &operator*=(const iaQuaternion<T> &other);

        /*! multiply quaternion with scalar

        \param scalar right hand side the scalar value
        \returns resulting quaternion
        */
        const iaQuaternion<T> operator*(T scalar);

        /*! multiplies quaternion with scalar

        \param scalar right hand side scalar value
        */
        iaQuaternion<T> &operator*=(T scalar);

        // TODO inverse
        // https://stackoverflow.com/questions/6689967/calculate-quaternion-inverse
        const iaQuaternion<T> inverse() const;

        /*! rotates a given vector

        \param vec the given vector
        \returns rotated vector
        */
        const iaVector3<T> rotate(const iaVector3<T> &vec) const;

        /*! normalize the quaternion to unit length
         */
        iaQuaternion<T> &normalize();

        /*! dot product 
        */
        T dot(const iaQuaternion<T> &other) const;
        T norm() const;
        T normSquared() const;

        /*! \returns conjugate
         */
        const iaQuaternion<T> conjugate() const;

        /*! \returns squared magnitude
        */
        T magnitude2() const;

        /*! \returns pointer to the data
         */
        const T *getData() const;

        /*! \returns pointer to the data
         */
        T *getData();
    };

    template <class T>
    iaQuaternion<T> slerp(iaQuaternion<T> a, iaQuaternion<T> b, T t);

    template <class T>
    T dotProduct(iaQuaternion<T> a, iaQuaternion<T> b);

#include <iaux/math/iaQuaternion.inl>

    /*! single precision quaternion definition
     */
    typedef iaQuaternion<float32> iaQuaternionf;

    /*! double precision quaternion definition
     */
    typedef iaQuaternion<float64> iaQuaterniond;

} // namespace iaux

#endif // IAUX_QUATERNION_H

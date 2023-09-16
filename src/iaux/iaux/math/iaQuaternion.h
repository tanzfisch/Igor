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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IAUX_QUATERNION__
#define __IAUX_QUATERNION__

#include <iaux/math/iaVector3.h>

#include <ostream>

namespace iaux
{

    /*! quaternion class
     */
    template <class T>
    class IAUX_API_EXPORT_ONLY iaQuaternion
    {
    public:
        /*! x component
         */
        T _x = 0.0;

        /*! y component
         */
        T _y = 0.0;

        /*! z component
         */
        T _z = 0.0;

        /*! w component
         */
        T _w = 1.0;

        // default ctor
        iaQuaternion();

        /*! ctor by axis angle

        \param axis the axis to rotate around
        \param angle the angle to rotate
        */
        iaQuaternion(const iaVector3<T> &axis, T angle);

        /*! ctor by direct values

        \param x the x component
        \param y the y component
        \param z the z component
        \param w the w component
        */
        iaQuaternion(T x, T y, T z, T w);

        /*! ctor by euler angles

        \param x rotation around x axis in rad
        \param y rotation around y axis in rad
        \param z rotation around z axis in rad
        */
        iaQuaternion(T x, T y, T z);

        /*! ctor by euler angles

        \param vec vector that contains euler angles
        */
        iaQuaternion(const iaVector3<T> &vec);

        /*! set by axis angle

        \param axis the axis to rotate arround
        \param angle the angle to rotate
        */
        void setAxisAngle(const iaVector3<T> &axis, T angle);

        /*! returns axis angle from quaternion

        \param axis[out] the rotation axis
        \parma angle[out] the rotation angle
        */
        void getAxisAngle(iaVector3<T> &axis, T &angle) const;

        /*! set by direct value

        \param x the x component
        \param y the y component
        \param z the z component
        \param w the w component
        */
        void set(T x, T y, T z, T w);

        /*! reset quaternion to identity
         */
        void identity();

        /*! negate quaternion
         */
        void negate();

        /*! set by euler angles

        \param x rotation arround x axis in rad
        \param y rotation arround y axis in rad
        \param z rotation arround z axis in rad
        */
        void setEuler(T x, T y, T z);

        /*! set by euler angles

        \param vec vector that contains euler angles
        */
        void setEuler(const iaVector3<T> &vec);

        /*! returns euler angles

        \param x[out] the rotation arround the x axis
        \param y[out] the rotation arround the y axis
        \param z[out] the rotation arround the z axis
        */
        void getEuler(T &x, T &y, T &z) const;

        /*! returns euler angles

        \param vec[out] euler angles in a vector
        */
        void getEuler(iaVector3<T> &vec) const;

        /*! \returns true if both quaternions are equal

        \param rhs the right hand side quaternion
        */
        bool operator==(const iaQuaternion<T> &rhs) const;

        /*! \returns true if both quaternions are not equal

        \param rhs the right hand side quaternion
        */
        bool operator!=(const iaQuaternion<T> &rhs) const;

        /*! add one quaternion to an other

        \returns the resulting quaternion
        \param rhs the right hand side quaternion
        */
        iaQuaternion<T> operator+(const iaQuaternion<T> &rhs);

        /*! add one quaternion to this quaternion

        \param rhs the right hand side quaternion
        */
        void operator+=(const iaQuaternion<T> &rhs);

        /*! subtracts one quaternion from an other

        \returns the resulting quaternion
        \param rhs the right hand side quaternion
        */
        iaQuaternion<T> operator-(const iaQuaternion<T> &rhs);

        /*! subtracts one quaternion from the other

        \param rhs the right hand side quaternion
        */
        void operator-=(const iaQuaternion<T> &rhs);

        /*! multiply one quaternion to this quaternion

        \param rhs the right hand side quaternion
        \returns the resulting quaternion
        */
        iaQuaternion<T> operator*(const iaQuaternion<T> &rhs);

        /*! multiply one quaternion with an other one

        \param rhs the right hand side quaternion
        */
        void operator*=(const iaQuaternion<T>& rhs);

        /*! multiply quaternion with scalar

        \param rhs right hand side the scalar value
        \returns resulting quaternion
        */
        iaQuaternion<T> operator*(T rhs);

        /*! multiplies quaternion with scalar

        \param rhs right hand side scalar value
        */
        void operator*=(T rhs);

        // TODO inverse
        // https://stackoverflow.com/questions/6689967/calculate-quaternion-inverse

        /*! normalize the quaternion to unit length
         */
        void normalize();

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

#endif // __IAUX_QUATERNION__
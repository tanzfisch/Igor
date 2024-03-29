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

#ifndef __IAUX_VECTOR3__
#define __IAUX_VECTOR3__

#include <iaux/iaDefines.h>

#include <cmath>
#include <ostream>
#include <iomanip>

namespace iaux
{

    /*! 3d vector class
    */
    template <typename T>
    struct IAUX_API_EXPORT_ONLY iaVector3
    {
        /*! does nothing
        */
        iaVector3() = default;

        /*! initializes member variables

        \param x x component
        \param y y component
        \param z z component
        */
        iaVector3(T x, T y, T z);


        /*! x component
        */
        T _x = static_cast<T>(0);

        /*! y component
        */
        T _y = static_cast<T>(0);

        /*! z component
        */
        T _z = static_cast<T>(0);

        /*! vector addition

        \param a the vector to add
        \returns result of vector addition
        */
        iaVector3<T> operator+(const iaVector3<T> &a) const;

        /*! vector addition and assignment

        \param a the vector to add to this vector
        */
        void operator+=(const iaVector3<T> &a);

        /*! vector subtraction

        \param a the vector to subtract
        \returns result of vector subtraction
        */
        iaVector3<T> operator-(const iaVector3<T> &a) const;

        /*! vector subtraction and assignment

        \param a the vector to subtract to this vector
        */
        void operator-=(const iaVector3<T> &a);

        /*! vector assignment

        \param a the vector to assign to this vector
        \returns resulting vector
        */
        iaVector3<T> operator=(const iaVector3<T> &a);

        /*! cross product of two vectors

        \param a the vector to calculate the cross product with
        \returns the resulting cross product
        */
        iaVector3<T> operator%(const iaVector3<T> &a) const;

        /*! vector scaling

        \param a the factor to scale the vector with
        \returns the scaled vector
        */
        iaVector3<T> operator*(T a) const;

        /*! vector scaling and assignment

        \param a the factor to scale this vector with
        */
        void operator*=(T a);

        /*! vector scaling by division

        \param a the factor to scale the vector with
        \returns the scaled vector
        */
        iaVector3<T> operator/(T a) const;

        /*! vector scaling by division and assignment

        \param a the factor to scale this vector with
        */
        void operator/=(T a);

        /*! dot product (scalar product) of two vectors

        \param a second vector to calculate the scalar product with
        \returns the resulting scalar product
        */
        T operator*(const iaVector3<T> &a) const;

        /*! comparison of two vectors

        \param a the second vector to compare this vector with
        \returns true if vectors are equal
        */
        bool operator==(const iaVector3<T> &a) const;

        /*! negated comparison of two vectors

        \param a the second vector to compare this vector with
        \returns true if vectors are not equal
        */
        bool operator!=(const iaVector3<T> &a) const;

        /*! \returns component value by index (const version)

        \param i index of component
        */
        const T &operator[](int i) const;

        /*! \returns component value by index

        \param i index of component
        */
        T &operator[](int i);

        /*! sets components of vector

        \param x x component
        \param y y component
        \param z z component
        */
        void set(T x, T y, T z);

        /*! \returns length of vector
        */
        T length() const;

        /*! \returns quadratic length of vector
        */
        T length2() const;

        /*! \returns distance between two vectors

        \param v second vector
        */
        T distance(const iaVector3<T> &v) const;

        /*! \returns quadratic distance between two vectors

        \param v second vector
        */
        T distance2(const iaVector3<T> &v) const;

        /*! project this vector on v

        \param v the vector to project on
        \returns the projected vector
        */
        iaVector3<T> project(const iaVector3<T> &v) const;

        /*! reject this vector from v

        \param v the vector to reject from
        \returns the rejected vector
        */
        iaVector3<T> reject(const iaVector3<T> &v);

        /*! \returns angle beween to vectors in radians
        */
        T angle(const iaVector3<T> &a) const;

        /*! returns angle relative to XZ plane
        */
        T angleXZ() const;

        /*! normalizes the vector
        */
        void normalize();

        /*! negates the vector
        */
        void negate();

        /*! \retruns pointer to internal data
        */
        const T *getData() const;

        /*! \returns pointer to internal data
        */
        T *getData();

        /*! converts the vector in to given type of vector

		\returns vector for given type
		*/
        template <class T2>
        iaVector3<T2> convert() const;
    };

    /*! linear interpolation of two vectors

    \param a vector a
    \param b vector b
    \param t interpolation weight
    */
    template <class T>
    iaVector3<T> lerp(const iaVector3<T> &a, const iaVector3<T> &b, T t);

#include <iaux/math/iaVector3.inl>

    /*! float32 3d vector
    */
    typedef iaVector3<float32> iaVector3f;

    /*! float64 3d vector
    */
    typedef iaVector3<float64> iaVector3d;

    /*! int32 3d vector
    */
    typedef iaVector3<int32> iaVector3i;

    /*! int64 3d vector
    */
    typedef iaVector3<int64> iaVector3I;

}; // namespace iaux

#endif // __IAUX_VECTOR3__

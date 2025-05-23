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

#ifndef __IAUX_VECTOR4__
#define __IAUX_VECTOR4__

#include <iaux/math/iaVector3.h>

namespace iaux
{

    /*! 4d vector
    */
    template <typename T>
    struct IAUX_API_EXPORT_ONLY iaVector4
    {
        /*! does nothing
        */
        iaVector4() = default;

        /*! param ctor

        \pram x x component
        \pram y y component
        \pram z z component
        \pram w w component
        */
        iaVector4(T x, T y, T z, T w);

        /*! x component
		*/
        T _x = static_cast<T>(0);

        /*! y component
		*/
        T _y = static_cast<T>(0);

        /*! z component
		*/
        T _z = static_cast<T>(0);

        /*! w component
        */
        T _w = static_cast<T>(1);

        /*! copy operator

        \param src vector to copy
        \returns this
        */
        iaVector4<T> operator=(const iaVector4<T> &src);

        /*! copy operator 3 components

        leaves w component as is

        \param src vector to copy
        \returns this
        */
        iaVector4<T> operator=(const iaVector3<T> &src);

        /*! compares two vectors

        \param a the vector to compare this vector with
        \returns true if both vectors are equal
        */
        bool operator==(const iaVector4<T> &a) const;

        /*! compares two vectors

        \param a the vector to compare this vector with
        \returns true if both vectors are not equal
        */
        bool operator!=(const iaVector4<T> &a) const;

        /*! array operator read only versione

        \param i index of field to access
        \returns value of field
        */
        const T &operator[](int i) const;

        /*! array operator read and write

        \param i index of field to access
        \returns reference to value
        */
        T &operator[](int i);

        /*! sets the components of this vector

        \pram x x component
        \pram y y component
        \pram z z component
        \pram w w component
        */
        void set(T x, T y, T z, T w);

        /*! \returns pointer to internal data
        */
        T *getData();

        /*! \returns pointer to internal data

        const variant
        */
        const T *getData() const;

        /*! converts the vector in to given type of vector

		\returns vector for given type
		*/
        template <class T2>
        iaVector4<T2> convert() const;
    };

    /*! linear interpolation of two vectors

    \param a vector a
    \param b vector b
    \param t interpolation weight
    */
    template <class T>
    iaVector4<T> lerp(const iaVector4<T> &a, const iaVector4<T> &b, T t);

#include <iaux/math/iaVector4.inl>

    /*! float32 4d vector
    */
    typedef iaVector4<float32> iaVector4f;

    /*! float64 4d vector
    */
    typedef iaVector4<float64> iaVector4d;

    /*! int32 4d vector
    */
    typedef iaVector4<int32> iaVector4i;

    /*! int64 4d vector
    */
    typedef iaVector4<int64> iaVector4I;

}; // namespace iaux

#endif // __IAUX_VECTOR4__

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
// (c) Copyright 2012-2019 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __iaVECTOR3__
#define __iaVECTOR3__

#include <iaDefines.h>

#include <cmath>
#include <ostream>
using namespace std;

namespace IgorAux
{

    /*! 3d vector class
    */
    template <class T> 
    class IgorAux_API_Template iaVector3
    {

    public:

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
        __IGOR_INLINE__ iaVector3<T> operator+(const iaVector3<T> &a) const;

        /*! vector addition and assignment

        \param a the vector to add to this vector
        */
        __IGOR_INLINE__ void operator+=(const iaVector3<T> &a);

        /*! vector subtraction

        \param a the vector to subtract
        \returns result of vector subtraction
        */
        __IGOR_INLINE__ iaVector3<T> operator-(const iaVector3<T> &a) const;

        /*! vector subtraction and assignment

        \param a the vector to subtract to this vector
        */
        __IGOR_INLINE__ void operator-=(const iaVector3<T> &a);

        /*! vector assignment

        \param a the vector to assign to this vector
        \returns resulting vector
        */
        __IGOR_INLINE__ iaVector3<T> operator=(const iaVector3<T> &a);

        /*! cross product of two vectors

        \param a the vector to calculate the cross product with
        \returns the resulting cross product
        */
        __IGOR_INLINE__ iaVector3<T> operator%(const iaVector3<T> &a) const;

        /*! vector scaling

        \param a the factor to scale the vector with
        \returns the scaled vector
        */
        __IGOR_INLINE__ iaVector3<T> operator*(T a) const;

        /*! vector scaling and assignment

        \param a the factor to scale this vector with
        */
        __IGOR_INLINE__ void operator*=(T a);

        /*! vector scaling by division

        \param a the factor to scale the vector with
        \returns the scaled vector
        */
        __IGOR_INLINE__ iaVector3<T> operator/(T a) const;

        /*! vector scaling by division and assignment

        \param a the factor to scale this vector with
        */
        __IGOR_INLINE__ void operator/=(T a);

        /*! dot product (scalar product) of two vectors

        \param a second vector to calculate the scalar product with
        \returns the resulting scalar product
        */
        __IGOR_INLINE__ T operator*(const iaVector3<T> &a) const;

        /*! comparison of two vectors

        \param a the second vector to compare this vector with
        \returns true if vectors are equal
        */
        __IGOR_INLINE__ bool operator==(const iaVector3<T> &a) const;

        /*! negated comparison of two vectors

        \param a the second vector to compare this vector with
        \returns true if vectors are not equal
        */
        __IGOR_INLINE__ bool operator!=(const iaVector3<T> &a) const;

        /*! \returns component value by index (const version)

        \param i index of component
        */
        __IGOR_INLINE__ const T& operator[](int i) const;

        /*! \returns component value by index

        \param i index of component
        */
        __IGOR_INLINE__ T& operator[](int i);

        /*! sets components of vector

        \param x x component
        \param y y component
        \param z z component
        */
        __IGOR_INLINE__ void set(T x, T y, T z);

        /*! \returns length of vector
        */
        __IGOR_INLINE__ T length(void) const;

        /*! \returns quadratic length of vector
        */
        __IGOR_INLINE__ T length2(void) const;

        /*! \returns distance between two vectors

        \param v second vector
        */
        __IGOR_INLINE__ T distance(const iaVector3<T> &v) const;

        /*! \returns quadratic distance between two vectors

        \param v second vector
        */
        __IGOR_INLINE__ T distance2(const iaVector3<T> &v) const;

        /*! project this vector on v

        \param v the vector to project on
        \returns the projected vector
        */
        __IGOR_INLINE__ iaVector3<T> project(const iaVector3<T> &v) const;

        /*! reject this vector from v

        \param v the vector to reject from
        \returns the rejected vector
        */
        __IGOR_INLINE__ iaVector3<T> reject(const iaVector3<T> &v);
        
        /*! \returns angle beween to vectors in radians
        */
        __IGOR_INLINE__ T angle(iaVector3<T> &a) const;
        __IGOR_INLINE__ T angleXZ() const;

        /*! normalizes the vector
        */
        __IGOR_INLINE__ iaVector3<T> normalize(void);

        /*! negates the vector
        */
        __IGOR_INLINE__ iaVector3<T> negate(void);

        /*! \retruns pointer to internal data
        */
        __IGOR_INLINE__ const T* getData() const;

        /*! \returns pointer to internal data
        */
        __IGOR_INLINE__ T* getData();

        /*! does nothing
        */
        iaVector3() = default;

        /*! initializes member variables

        \param x x component
        \param y y component
        \param z z component
        */
        iaVector3(T x, T y, T z);

        /*! does nothing
        */
        ~iaVector3() = default;
    };

#include <iaVector3.inl>

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

};

#endif
 
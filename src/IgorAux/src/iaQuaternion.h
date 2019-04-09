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

#ifndef __QUATERNION__
#define __QUATERNION__

#include <iaDefines.h>

namespace IgorAux
{

    /*!

    \todo not in use yet
    */
    template <class T>
    class iaQuaternion
    {
    public:

        T x, y, z, w;

        __IGOR_INLINE__ iaQuaternion<T> operator*(iaQuaternion<T> &a);
        __IGOR_INLINE__ iaQuaternion<T> operator*(T a);
        __IGOR_INLINE__ void operator*=(T a);
        __IGOR_INLINE__ iaQuaternion<T>	operator+(iaQuaternion<T> &a);
        __IGOR_INLINE__ void operator+=(iaQuaternion<T> &a);
        __IGOR_INLINE__ iaQuaternion<T>	operator-(iaQuaternion<T> &a);
        __IGOR_INLINE__ void operator-=(iaQuaternion<T> &a);

        __IGOR_INLINE__ T length();
        __IGOR_INLINE__ T length2();
        __IGOR_INLINE__ void normalize();
        __IGOR_INLINE__ void conjugate();
        __IGOR_INLINE__ T dot(iaQuaternion<T> &a);
        // TODO __IGOR_INLINE__ void slerp(iaQuaternion<T> q1, iaQuaternion<T> q2, T t);

        void set(T x, T y, T z, T w);

        iaQuaternion(void);
        iaQuaternion(T x, T y, T z, T w);
        ~iaQuaternion(void);
    };

#include <iaQuaternion.inl>

    typedef iaQuaternion<float32> iaQuaternionf;
    typedef iaQuaternion<float64> iaQuaterniond;

}

#endif
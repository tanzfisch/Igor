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
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __iVECTOR4__
#define __iVECTOR4__

#include <iaVector3.h>

namespace IgorAux
{

    template <class T> class IgorAux_API_Template iaVector4
    {
    public:
        iaVector3<T> _vec;
        T _w;

        __IGOR_INLINE__ iaVector4<T> operator=(const iaVector4<T> &a);		//!< Vektor Kopie
        __IGOR_INLINE__ iaVector4<T> operator=(const iaVector3<T> &a);		//!< Vektor Kopie

        __IGOR_INLINE__ bool operator== (const iaVector4<T> &a) const;	//!< Gleich
        __IGOR_INLINE__ bool operator!= (const iaVector4<T> &a) const;	//!< Ungleich

        __IGOR_INLINE__ const T& operator[] (int i) const;				//!< get operator
        __IGOR_INLINE__ T& operator[] (int i);						//!< set operator

        void  set(T x, T y, T z, T w);								//!< Setzt Vektor Koordinaten

        /*! \returns pointer to internal data
        */
        T* getData();

        /*! \returns pointer to internal data

        const variant
        */
        const T* getData() const;

        iaVector4();												//!< Konstruktor setzt Nullvektor
        iaVector4(T x, T y, T z, T w);								//!< Konstruktor mit startwerten
        ~iaVector4();												//!< Destruktor tut nichts
    };

#include <iaVector4.inl>

    typedef iaVector4<float32> iaVector4f;
    typedef iaVector4<float64> iaVector4d;
    typedef iaVector4<int32> iaVector4i;

};

#endif

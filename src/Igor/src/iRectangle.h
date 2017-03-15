//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/                     ) )     ((
//                                           (_(       \)
//    (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __iRECTANGLE__
#define __iRECTANGLE__

#include <iDefines.h>

namespace Igor
{
    /*! rectangle
    */
    template <class T>
    class Igor_API_Template iRectangle
    {

    public:

        iRectangle() = default;

        iRectangle(T x, T y, T width, T height)
        {
            _x = x;
            _y = y;
            _width = width;
            _height = height;
        }

        __IGOR_INLINE__ const T getX() const;
        __IGOR_INLINE__ const T getY() const;
        __IGOR_INLINE__ const T getWidth() const;
        __IGOR_INLINE__ const T getHeight() const;

        void setX(T x);
        void setY(T y);
        void setWidth(T width);
        void setHeight(T height);

        __IGOR_INLINE__ iRectangle<T> operator= (const iRectangle<T> &rectangle);

        T _x = (T)0;
        T _y = (T)0;
        T _width = (T)0;
        T _height = (T)0;

    };

#include <iRectangle.inl>

    typedef iRectangle<float32> iRectanglef;
    typedef iRectangle<float64> iRectangled;
    typedef iRectangle<int32> iRectanglei;

};

#endif

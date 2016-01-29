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
//    (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __iCOLOR3__
#define __iCOLOR3__

#include <iaDefines.h>

#include <fstream>
#include <iostream>
using namespace std;

namespace IgorAux
{

    /*! 3 component color class
    */
    template <class T>
    class IgorAux_API_Template iaColor3
    {
    public:

        /*! red component
        */
        T _r;

        /*! green component
        */
        T _g;

        /*! blue component
        */
        T _b;

        /*! ctor initializes members
        */
        iaColor3()
        {
            _r = (T)0;
            _g = (T)0;
            _b = (T)0;
        }

        /*! ctor initializes members with parameters

        \param r red component
        \param g green component
        \param b blue component
        */
        iaColor3(T r, T g, T b)
        {
            _r = r;
            _g = g;
            _b = b;
        }

        /*! set members

        \param r red component
        \param g green component
        \param b blue component
        */
        void set(T r, T g, T b)
        {
            _r = r;
            _g = g;
            _b = b;
        }

        /*! \returns pointer to data
        */
        T* iaColor3<T>::getData()
        {
            return &_r;
        }

        /*! assignment operator

        \parm color the color to copy
        */
        __IGOR_INLINE__ iaColor3<T> operator=(const iaColor3<T> &color)
        {
            iaColor3<T> result;
            result._r = _r = color._r;
            result._g = _g = color._g;
            result._b = _b = color._b;

            return result;
        }

        /*! add operator

        \parm color the color to add
        */
        __IGOR_INLINE__ void iaColor3<T>::operator+=(const iaColor3<T> &color)
        {
            _r += color._r;
            _g += color._g;
            _b += color._b;
        }

        /*! subtract operator

        \parm color the color to subtract
        */
        __IGOR_INLINE__ void iaColor3<T>::operator-=(const iaColor3<T> &color)
        {
            _r -= color._r;
            _g -= color._g;
            _b -= color._b;
        }

        /*! scale operator

        \param value the value to scale with
        */
        void iaColor3<T>::operator*=(T value)
        {
            _r = static_cast<T>(static_cast<float32>(_r) * static_cast<float32>(value));
            _g = static_cast<T>(static_cast<float32>(_g) * static_cast<float32>(value));
            _b = static_cast<T>(static_cast<float32>(_b) * static_cast<float32>(value));
        }

        /*! linear interpolation between two colors with a factor

        \param color1 first color
        \param color2 second color
        \param w factor to interpolate with
        */
        void iaColor3<T>::lerp(iaColor3<T> &color1, iaColor3<T> &color2, float32 w)
        {
            _r = static_cast<T>(static_cast<float32>(color1._r) * w + static_cast<float32>(color2._r) * (1.0f - w));
            _g = static_cast<T>(static_cast<float32>(color1._g) * w + static_cast<float32>(color2._g) * (1.0f - w));
            _b = static_cast<T>(static_cast<float32>(color1._b) * w + static_cast<float32>(color2._b) * (1.0f - w));
        }

    };

    /*! stream operator e.g. for cosole output

    \param ostr the out stream handle
    \param color the color to print in stream
    */
    template <class T>
    __IGOR_INLINE__ std::wostream& operator<<(std::wostream &ostr, const iaColor3<T> &color)
    {
        ostr << "(" << color._r << ", " << color._g << ", " << color._b << ")";
        return ostr;
    }

    /*! float32 variant of color
    */
    typedef iaColor3<float32> iaColor3f;

    /*! uint8 variant of color
    */
    typedef iaColor3<uint8> iaColor3c;

};

#endif
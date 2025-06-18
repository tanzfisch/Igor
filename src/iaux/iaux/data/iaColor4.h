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
//    (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef IAUX_COLOR4_H
#define IAUX_COLOR4_H

#include <iaux/iaDefines.h>

#include <fstream>
#include <algorithm>
#include <math.h>

namespace iaux
{

    /*! color class the represents the red, green, blue and alpha channel

    for floating point variant we expect values in range from 0.0 - 1.0
    for integer variant we expect values in range from 0.0 - 255.0
    */
    template <typename T>
    struct IAUX_API_EXPORT_ONLY iaColor4
    {
        /*! red color channel
         */
        T _r;

        /*! green color channel
         */
        T _g;

        /*! blue color channel
         */
        T _b;

        /*! alpha color channel
         */
        T _a;

        /*! ctor initialize member variables
         */
        iaColor4();

        /*! ctor initialize member variables with parameters

        \param r red component
        \param g green component
        \param b blue component
        \param a alpha component
        */
        iaColor4(T r, T g, T b, T a);

        /*! set member variables with parameters

        \param r red component
        \param g green component
        \param b blue component
        \param a alpha component
        */
        void set(T r, T g, T b, T a);

        /*! \returns pointer to data
         */
        T *getData();

        /*! assignment operator with other color

        \param color the color to assign
        */
        iaColor4<T> operator=(const iaColor4<T> &color);

        /*! add operator

        \param color the color to add
        */
        void operator+=(const iaColor4<T> &color);

        /*! subtract operator

        \param color the color to subtract
        */
        void operator-=(const iaColor4<T> &color);

        /*! scale operator

        \param factor the value to scale with
        */
        void operator*=(float32 factor);

        /*! scale operator

        \param factor the value to scale with
        */
        void operator*(float32 factor);

        /*! \returns random color (opaque)
        */
        static iaColor4<T> random();

        static IAUX_API_IMPORT_ONLY const iaColor4<T> white;
        static IAUX_API_IMPORT_ONLY const iaColor4<T> lightGray;
        static IAUX_API_IMPORT_ONLY const iaColor4<T> gray;
        static IAUX_API_IMPORT_ONLY const iaColor4<T> darkGray;
        static IAUX_API_IMPORT_ONLY const iaColor4<T> black;
        static IAUX_API_IMPORT_ONLY const iaColor4<T> red;
        static IAUX_API_IMPORT_ONLY const iaColor4<T> green;
        static IAUX_API_IMPORT_ONLY const iaColor4<T> blue;
        static IAUX_API_IMPORT_ONLY const iaColor4<T> yellow;
        static IAUX_API_IMPORT_ONLY const iaColor4<T> cyan;
        static IAUX_API_IMPORT_ONLY const iaColor4<T> magenta;
        static IAUX_API_IMPORT_ONLY const iaColor4<T> transparent;
    };

#include <iaux/data/iaColor4.inl>

    /*! stream operator e.g. for console output

    \param ostr the out stream handle
    \param color the color to print in stream
    */
    template <class T>
    std::wostream &operator<<(std::wostream &ostr, const iaColor4<T> &color);

    /*! float32 color
     */
    typedef iaColor4<float32> iaColor4f;

    /*! uint8 color
     */
    typedef iaColor4<uint8> iaColor4c;

}; // namespace iaux

#endif // IAUX_COLOR4_H
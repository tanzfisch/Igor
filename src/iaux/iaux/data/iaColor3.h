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

#ifndef IAUX_COLOR3_H
#define IAUX_COLOR3_H

#include <iaux/iaDefines.h>

#include <fstream>
#include <iostream>

namespace iaux
{

    /*! 3 component color class

    for floating point variant we expect values in range from 0.0 - 1.0
    for integer variant we expect values in range from 0.0 - 255.0    
    */
    template <class T>
    class IAUX_API_EXPORT_ONLY iaColor3
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
        iaColor3();

        /*! ctor initializes members with parameters

        \param r red component
        \param g green component
        \param b blue component
        */
        iaColor3(T r, T g, T b);

        /*! set members

        \param r red component
        \param g green component
        \param b blue component
        */
        void set(T r, T g, T b);

        /*! \returns pointer to data
        */
        T *getData();

        /*! assignment operator

        \parm color the color to copy
        */
        iaColor3<T> operator=(const iaColor3<T> &color);

        /*! add operator

        \parm color the color to add
        */
        void operator+=(const iaColor3<T> &color);

        /*! subtract operator

        \parm color the color to subtract
        */
        void operator-=(const iaColor3<T> &color);

        /*! scale operator

        \param factor the factor to scale with
        */
        void operator*=(float32 factor);

        /*! scale operator

        \param factor the factor to scale with
        */
        void operator*(float32 factor);       

        /*! \returns random color (opaque)
        */
        static iaColor3<T> random();

        static IAUX_API_IMPORT_ONLY const iaColor3<T> white;
        static IAUX_API_IMPORT_ONLY const iaColor3<T> lightGray;
        static IAUX_API_IMPORT_ONLY const iaColor3<T> gray;
        static IAUX_API_IMPORT_ONLY const iaColor3<T> darkGray;
        static IAUX_API_IMPORT_ONLY const iaColor3<T> black;
        static IAUX_API_IMPORT_ONLY const iaColor3<T> red;
        static IAUX_API_IMPORT_ONLY const iaColor3<T> green;
        static IAUX_API_IMPORT_ONLY const iaColor3<T> blue;
        static IAUX_API_IMPORT_ONLY const iaColor3<T> yellow;
        static IAUX_API_IMPORT_ONLY const iaColor3<T> cyan;
        static IAUX_API_IMPORT_ONLY const iaColor3<T> magenta;         
    };

#include <iaux/data/iaColor3.inl>

    /*! stream operator e.g. for console output

    \param ostr the out stream handle
    \param color the color to print in stream
    */
    template <class T>
    std::wostream &operator<<(std::wostream &ostr, const iaColor3<T> &color);

    /*! float32 variant of color
    */
    typedef iaColor3<float32> iaColor3f;

    /*! uint8 variant of color
    */
    typedef iaColor3<uint8> iaColor3c;

}; // namespace iaux

#endif // IAUX_COLOR3_H
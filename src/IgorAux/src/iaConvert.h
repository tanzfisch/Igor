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

#ifndef __iaConvert__
#define __iaConvert__

#include <iaColor3.h>
#include <iaVector3.h>

namespace IgorAux
{

    /*! contains some simple converters
    */
    class IgorAux_API iaConvert
    {

    public:

        /*! converts float based color to byte

        \param color float based color
        \returns byte based color
        */
        static iaColor3c convert3c(const iaColor3f& color);

        /*! converts byte based color to float

        \param color byte based color
        \returns float based color
        */
        static iaColor3f convert3f(const iaColor3c& color);

        /*! converts int64 based 3 dimensional vector in float base

        \param vector int64 based vector
        \returns float based vector 
        */
        static iaVector3f convert3f(const iaVector3I& vector);

        /*! converts float32 based 3 dimensional vector in int64 base

        \param vector float32 based vector
        \returns int64 based vector
        */
        static iaVector3I convert3I(const iaVector3f& vector);
    };
}

#endif
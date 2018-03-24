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
//    (c) Copyright 2012-2017 by Martin Loga
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

#ifndef __IGOR_AUX_CONVERT__
#define __IGOR_AUX_CONVERT__

#include <iaColor3.h>
#include <iaColor4.h>
#include <iaVector3.h>
#include <iaMatrix.h>

namespace IgorAux
{

    /*! contains some simple converters
    */
    class IgorAux_API iaConvert
    {

    public:

        /*! converts float based color to byte

        \param src src color
        \param[out] dst destination color
        */
        static void convert(const iaColor3f& src, iaColor3c& dst);

        /*! converts byte based color to float

        \param src src color
        \param[out] dst destination color
        */
        static void convert(const iaColor3c& src, iaColor3f& dst);

        /*! converts float based color to byte

        \param src src color
        \param[out] dst destination color
        */
        static void convert(const iaColor4f& src, iaColor4c& dst);

        /*! converts byte based color to float

        \param src src color
        \param[out] dst destination color
        */
        static void convert(const iaColor4c& src, iaColor4f& dst);

        /*! converts int64 based 3 dimensional vector in float base

        \param src src vector
        \param[out] dst destination vector
        */
        static void convert(const iaVector3I& src, iaVector3f& dst);

        static void convert(const iaVector3I& src, iaVector3d& dst);

        /*! converts float32 based 3 dimensional vector in int64 base

        \param src src vector
        \param[out] dst destination vector
        */
        static void convert(const iaVector3f& src, iaVector3I& dst);

        static void convert(const iaVector3d& src, iaVector3I& dst);

        /*! converts float64 based 3 dimensional vector in float32 base

        \param src src vector
        \param[out] dst destination vector
        */
        static void convert(const iaVector3d& src, iaVector3f& dst);

        static void convert(const iaVector3f& src, iaVector3d& dst);

        /*! converts float64 based 4*4 matrix in float32 base 4*4 matrix

        \param src src matrix
        \param[out] dst destination matrix
        */
        static void convert(const iaMatrixd& src, iaMatrixf& dst);

        static void convert(const iaMatrixf& src, iaMatrixd& dst);
    };
}

#endif
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
//    (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __IAUX_CONVERT__
#define __IAUX_CONVERT__

#include <iaux/data/iaColor3.h>
#include <iaux/data/iaColor4.h>
#include <iaux/math/iaVector2.h>
#include <iaux/math/iaVector3.h>
#include <iaux/math/iaMatrix.h>

namespace iaux
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
        static void convert(const iaColor3f &src, iaColor3c &dst);

        /*! converts byte based color to float

        \param src src color
        \param[out] dst destination color
        */
        static void convert(const iaColor3c &src, iaColor3f &dst);

        /*! converts float based color to byte

        \param src src color
        \param[out] dst destination color
        */
        static void convert(const iaColor4f &src, iaColor4c &dst);

        /*! converts byte based color to float

        \param src src color
        \param[out] dst destination color
        */
        static void convert(const iaColor4c &src, iaColor4f &dst);
    };
} // namespace iaux

#endif
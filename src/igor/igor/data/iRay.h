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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __iRAY__
#define __iRAY__

#include <igor/iDefines.h>

#include <iaux/math/iaVector3.h>
using namespace iaux;

namespace igor
{

    /*! a ray
    */
    template <class T>
    class IGOR_API_TEMPLATE iRay
    {

    public:
        /*! position of ray
        */
        iaVector3<T> m_pos;

        /*! direction of ray
        */
        iaVector3<T> m_dir;

        /*! constructor that basically sets member valirables

        \param pos position of ray
        \param dir direction of ray
        */
        iRay(const iaVector3<T> &pos, const iaVector3<T> &dir);

        /*! does notthing
        */
        iRay() = default;

        /*! does notthing
        */
        virtual ~iRay() = default;
    };

#include <igor/data/iRay.inl>

    /*! float32 precision ray
    */
    typedef iRay<float32> iRayf;

    /*! float64 precision ray
    */
    typedef iRay<float64> iRayd;

} // namespace igor

#endif
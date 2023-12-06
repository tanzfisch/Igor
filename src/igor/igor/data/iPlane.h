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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_PLANE__
#define __IGOR_PLANE__

#include <igor/iDefines.h>

#include <iaux/math/iaVector3.h>
using namespace iaux;

namespace igor
{

    /*! a plane
    */
    template <class T>
    class IGOR_API_EXPORT_ONLY iPlane
    {

    public:
        /*! normal vector of plane
        */
        iaVector3<T> _normal;

        /*! distance from origin of this plane along the normal vector

        works like a scalar value
        */
        T _distance = static_cast<T>(0);

        /*! normalizes the normal and calculates new distance value
        */
        IGOR_INLINE void normalize();

        /*! sets member variables

        \param normal the normal
        \param distance the distance
        */
        iPlane(iaVector3<T> normal, T distance);

        /*! does notthing
        */
        iPlane() = default;

        /*! does notthing
        */
        virtual ~iPlane() = default;
    };

#include <igor/data/iPlane.inl>

    /*! float32 precision plane
    */
    typedef iPlane<float32> iPlanef;

    /*! float64 precision plane
    */
    typedef iPlane<float64> iPlaned;

} // namespace igor

#endif
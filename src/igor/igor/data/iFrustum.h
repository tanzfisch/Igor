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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __iFRUSTUM__
#define __iFRUSTUM__

#include <igor/data/iPlane.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{

    /*! frustum defined by 8 planes

    \todo ther is something wrong with the planes
    */
    template <class T>
    class Igor_API_Template iFrustum
    {

    public:
        /*! left plane of frustum
        */
        iPlane<T> _leftPlane;

        /*! right plane of frustum
        */
        iPlane<T> _rightPlane;

        /*! top plane of frustum
        */
        iPlane<T> _topPlane;

        /*! bottom plane of frustum
        */
        iPlane<T> _bottomPlane;

        /*! near plane of frustum
        */
        iPlane<T> _nearPlane;

        /*! far plane of frustum
        */
        iPlane<T> _farPlane;

        /*! uses projection matrix to calculate all planes of the frustum

        http://www.racer.nl/reference/vfc.htm

        \param viewProjection projection matrix
        */
        void set(iaMatrix<T> &viewProjection);

        /*! does nothing
        */
        iFrustum() = default;

        /*! does nothing
        */
        virtual ~iFrustum() = default;
    };

#include <igor/data/iFrustum.inl>

    /*! float32 specialization of iFrustum
    */
    typedef iFrustum<float32> iFrustumf;

    /*! float64 specialization of iFrustum
    */
    typedef iFrustum<float64> iFrustumd;

} // namespace igor

#endif